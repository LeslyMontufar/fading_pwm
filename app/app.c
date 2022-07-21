/*
 * Lesly Montúfar
 * app.c
 * parece um pouco o arduino esse .c
 *
 *
*/

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "app.h"
#include "hw.h"

static uint16_t percent_fade = 0;
bool app_started = false;

void app_led_fade_percent(uint16_t percent){
	hw_set_duty(100-percent); // o led fica em nível alto em PA8 = 0;
}

void app_led_off(void){
	percent_fade = 0;
	app_led_fade_percent(percent_fade);
}

void app_button_interrupt(void){
	if(!app_started)
		return;

	percent_fade += 10;
	if(percent_fade==110)
		percent_fade = 0;
	app_led_fade_percent(percent_fade);
}

void app_init(void){
	app_started = true;
	hw_set_debouncing_timer(APP_DEBOUNCING_TIME_MS);
	app_led_fade_percent(percent_fade);
	hw_pwm_start();
}

void app_loop(void){
//	hw_cpu_sleep();
}
