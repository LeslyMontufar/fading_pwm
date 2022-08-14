# Fade LED com sinal PWM

Definindo intensidade do brilho do led, utilizando um sinal de saída PWM, implementado com um temporizador do STM32.

## Índice 

* [Descrição](#descrição)
* [Desenvolvimento](#desenvolvimento)

## Descrição

Para essa atividade será construída uma aplicação onde o usuário irá controlar o brilho de um LED
(0 a 100%) através de um botão. 

1. O LED deve iniciar apagado e a cada clique no botão o brilho do led
deve aumentar 10%. Caso o valor 100% seja alcançado, o próximo pressionamento do botão leva novamente o brilho para 0%, ou seja, a aplicação irá ciclar indefinidamente. 

2. Adicionalmente, se o usuário ficar com o botão pressionado por 3 segundos o LED deve apagar (0% de brilho).

3. Como sugestão, use o canal 1 do TIM1, pino PA8. Não configure o pino PC13 e coloque um jumper
entre PA8 e PC13.

4. O botão deve ser tratado por interrupção e você não deve usar a função HAL_Delay() ou qualquer
outra que bloqueie o tratamento da interrupção. O deboucing é obrigatório.

## Desenvolvimento
1. Inicia o canal 1 do timer, que é o que foi configurado como pwm.
```
#define PWM_CHN1 TIM_CHANNEL_1
```
```
void app_init(void){
	app_started = true;
	hw_set_debouncing_timer(APP_DEBOUNCING_TIME_MS);
	app_led_fade_percent(percent_fade);
	hw_pwm_start();
}
```
2. O ciclo do pwm reinicia o contador a cada fim, e o tempo estabelecido é tão pequeno que é imperceptível ao olho humano (5 a 10 ms).
```
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if(htim == &htim1) {
		__HAL_TIM_SET_COUNTER(&htim1, 0);
	}
	else if(htim == &htim2)	{
		if(hw_button_state_get()){
			if(cnt<=(BUTTON_PRESSED_LED_OFF_TIME/APP_DEBOUNCING_TIME_MS)){
//				hw_led_toggle();
				__HAL_TIM_SET_COUNTER(&htim2, 0);
				cnt++;
			} else{
				app_led_off();
				__HAL_TIM_SET_COUNTER(&htim2, 0);
			}
		}
		else{
			hw_end_debouncing_timer();
		}

	}
}
```
3. Quando o botão é pressionado, é chamada a função de callback para a interrupção de falling, já que o botão é um input do tipo pull up, e será soltado durante a interrupção de rising. Assim, a interrupção do botão é desabilitada, é chamada a função inicia `app_button_interrupt`, e se inicia o tempo de debouncing.
```
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if(GPIO_Pin == BUTTON_Pin) {
		cnt=1;
		HAL_NVIC_DisableIRQ(EXTI0_IRQn);
		app_button_interrupt();
		hw_init_debouncing_timer();
	}
}
```
4. A função `app_button_interrupt` muda a intensidade do led, mudando o duty cycle do pwm.
```
void app_button_interrupt(void){
	if(!app_started)
		return;

	percent_fade += 10;
	if(percent_fade==110)
		percent_fade = 0;
	app_led_fade_percent(percent_fade);
}
```
```
void app_led_fade_percent(uint16_t percent){
	hw_set_duty(100-percent); // o led fica em nível alto em PA8 = 0;
}

void app_led_off(void){
	percent_fade = 0;
	app_led_fade_percent(percent_fade);
}
```
```
void hw_set_duty(uint16_t duty) {
	uint16_t arr = __HAL_TIM_GET_AUTORELOAD(&htim1)+1;
	uint16_t CCR = duty*arr/100;
	__HAL_TIM_SET_COMPARE(&htim1,PWM_CHN1, CCR-1*(CCR>0));
}
```
5. Para determinar se o botão ficou pressionado por 3 segundos, reaproveitei o timer do debouncing, fazendo-o reeiniciar até que se complete 3000 ms enquanto o botão ainda estiver em nível ativo (baixo - terra), por meio da função do HAL, `HAL_TIM_PeriodElapsedCallback`. Sendo que o contador de ciclos de timer já executados é armazenado na variável `cnt`, que inicia em 1 quando o botão é pressionado.
```
else if(htim == &htim2)	{
  if(hw_button_state_get()){
    if(cnt<=(BUTTON_PRESSED_LED_OFF_TIME/APP_DEBOUNCING_TIME_MS)){
//				hw_led_toggle();
      __HAL_TIM_SET_COUNTER(&htim2, 0);
      cnt++;
    } else{
      app_led_off();
      __HAL_TIM_SET_COUNTER(&htim2, 0);
    }
  }
  else{
    hw_end_debouncing_timer();
  }

}
```
