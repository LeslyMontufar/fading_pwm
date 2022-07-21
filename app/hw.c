/*
 * hw.c
 * Abstrai as funções do HAL e CMSIS
 *
 *  Created on: July 21, 2022
 *      Author: lesly
 */

#include <stdbool.h>
#include <stdint.h>
#include "main.h"
#include "app.h"
#include "hw.h"

#define CLKINT 			(72000000/htim2.Instance->PSC) //2000

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

#define PWM_CHN1 TIM_CHANNEL_1

typedef enum{
	MENOR_TIME_DEBOUNCING = 0,
	MAIOR_TIME_DEBOUCING,
	MAIOR_BUTTON_LED_OFF,
	RISING_DEBOUCING
} Mode_Time;

static Mode_Time flag = MENOR_TIME_DEBOUNCING;

void hw_init_debouncing_timer(void){
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	hw_timer_start(&htim2);
}

void hw_end_debouncing_timer(void){
	HAL_TIM_Base_Stop_IT(&htim2);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void hw_set_debouncing_timer(uint16_t time_ms) {
	uint16_t arr = (CLKINT*time_ms/1000)-1;
	__HAL_TIM_SET_AUTORELOAD(&htim2, arr);
	__HAL_TIM_SET_COUNTER(&htim2, 0);
}

void hw_timer_start(TIM_HandleTypeDef *htim) {
	HAL_TIM_Base_Start_IT(htim);
}

void hw_pwm_start(void){
	HAL_TIM_PWM_Start(&htim1, PWM_CHN1);
}

void hw_set_duty(uint16_t duty) {
	uint16_t arr = __HAL_TIM_GET_AUTORELOAD(&htim1)+1;
	uint16_t CCR = duty*arr/100;
	__HAL_TIM_SET_COMPARE(&htim1,PWM_CHN1, CCR-1*(CCR>0));
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if(htim == &htim1) {
		__HAL_TIM_SET_COUNTER(&htim1, 0);
	}
	else if(htim == &htim2)	{
		if(flag == RISING_DEBOUCING){
			hw_end_debouncing_timer();
			flag = MENOR_TIME_DEBOUNCING;
		}
		else if(flag==MENOR_TIME_DEBOUNCING){
			if(hw_button_state_get()){
				flag = MAIOR_TIME_DEBOUCING;
				hw_end_debouncing_timer();
				hw_set_debouncing_timer(BUTTON_PRESSED_LED_OFF_TIME-APP_DEBOUNCING_TIME_MS);
				hw_init_debouncing_timer();
			}else{
				hw_end_debouncing_timer(); // chega aqui sim
			}
		}
		else if(flag==MAIOR_TIME_DEBOUCING){ // chegou no elapsed do 3segundos
			app_led_off();
			if(hw_button_state_get()){ //desnecessario
				flag = MAIOR_BUTTON_LED_OFF;
				HAL_TIM_Base_Stop_IT(&htim2);
			}else{
				hw_end_debouncing_timer();
			}
		}
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if(GPIO_Pin == BUTTON_Pin) {
		if(hw_button_state_get()){ // rising
			hw_led_state_set(true);
			if(flag == MENOR_TIME_DEBOUNCING){
				hw_init_debouncing_timer();
				flag = RISING_DEBOUCING;
			} else if(flag==MAIOR_TIME_DEBOUCING){
				hw_set_debouncing_timer(APP_DEBOUNCING_TIME_MS);
				hw_init_debouncing_timer();
				flag = RISING_DEBOUCING;
			}else if(flag==MAIOR_BUTTON_LED_OFF){
				hw_set_debouncing_timer(APP_DEBOUNCING_TIME_MS);
				hw_init_debouncing_timer();
				flag = RISING_DEBOUCING;
			}
		} else { //falling
			hw_led_state_set(false);
			hw_init_debouncing_timer();
			app_button_interrupt();
			HAL_NVIC_DisableIRQ(EXTI0_IRQn);
		}
	}
}

void hw_led_state_set(bool state){
	GPIO_PinState led_state = state ? GPIO_PIN_RESET : GPIO_PIN_SET;
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, led_state);
}

bool hw_button_state_get(void){
	GPIO_PinState button_state = HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin);
	if(button_state == GPIO_PIN_RESET)
		return false;
	else
		return true;
}

void hw_cpu_sleep(){
	__WFI();
}

