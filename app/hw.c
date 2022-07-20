/*
 * hw.c
 * Abstrai as funções do HAL e CMSIS
 *
 *  Created on: Jun 20, 2022
 *      Author: lesly
 */

#include <stdbool.h>
#include <stdint.h>
#include "main.h"
#include "app.h"
#include "hw.h"

//#define CLKINT 2000
#define CLKINT 72000000/htim2.Instance->PSC

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

#define PWM_CHN1 TIM_CHANNEL_1

volatile uint32_t hw_time_button_pressed(void){
	while(hw_button_state_get()){}
	return __HAL_TIM_GET_COUNTER(&htim2);//*1000/CLKINT;
}

void hw_init_debouncing_timer(void){
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	hw_timer_start(&htim2);
}

void hw_end_debouncing_timer(void){
	HAL_TIM_Base_Stop_IT(&htim2);
//	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void hw_timer_start(TIM_HandleTypeDef *htim) {
	HAL_TIM_Base_Start_IT(htim);
}

void hw_pwm_start(void){
	HAL_TIM_PWM_Start(&htim1, PWM_CHN1);
}

// duty é uma porcentagem
void hw_set_duty(uint16_t duty) {
	uint16_t arr = __HAL_TIM_GET_AUTORELOAD(&htim1)+1;
	uint16_t CCR = duty*arr/100;
	__HAL_TIM_SET_COMPARE(&htim1,PWM_CHN1, CCR-1*(CCR>0));
}

void hw_set_debouncing_timer(uint16_t time_ms) {
	uint16_t arr = (CLKINT*time_ms/1000)-1;
	__HAL_TIM_SET_AUTORELOAD(&htim2, arr);
	__HAL_TIM_SET_COUNTER(&htim2, 0);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim1)	{
		__HAL_TIM_SET_COUNTER(&htim1, 0);
	}
	else if(htim == &htim2)	{
//		hw_end_debouncing_timer();

	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == BUTTON_Pin)
	{
//		HAL_NVIC_DisableIRQ(EXTI0_IRQn);
		app_button_interrupt();
	}
}

bool hw_button_state_get(void){
	GPIO_PinState button_state = HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin);
	if(button_state == GPIO_PIN_RESET)
		return true;
	else
		return false;
}

void hw_delay_ms(uint32_t time_ms){
	HAL_Delay(time_ms);
}

void hw_cpu_sleep(){
	__WFI();
}

uint32_t hw_tick_ms_get(void){
	return HAL_GetTick();
}
