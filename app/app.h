/*
 * app.h
 *
 *  Created on: Jun 20, 2022
 *      Author: lesly
 */

#ifndef APP_H_
#define APP_H_

void app_led_fade_percent(uint16_t percent);
void app_button_interrupt(void);
void app_init(void);
void app_loop(void);

#endif /* APP_H_ */
