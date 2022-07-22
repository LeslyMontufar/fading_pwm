/*
 * app.h
 *
 *  Created on: July 21, 2022
 *      Author: lesly
 */

#ifndef APP_H_
#define APP_H_

#define APP_DEBOUNCING_TIME_MS 		80
#define BUTTON_PRESSED_LED_OFF_TIME	3000

void app_led_fade_percent(uint16_t percent);
void app_led_off(void);
void app_button_interrupt(void);
void app_init(void);
void app_loop(void);

#endif /* APP_H_ */
