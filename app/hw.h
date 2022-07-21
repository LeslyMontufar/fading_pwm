/*
 * hw.h
 *
 *  Created on: July 21, 2022
 *      Author: lesly
 */

#ifndef HW_H_
#define HW_H_

void hw_init_debouncing_timer(void);
void hw_end_debouncing_timer(void);
void hw_pwm_start(void);
void hw_timer_start(TIM_HandleTypeDef *htim);
void hw_set_duty(uint16_t duty);
void hw_set_debouncing_timer(uint16_t time_ms);

bool hw_button_state_get(void);
void hw_delay_ms(uint32_t time_ms);
void hw_cpu_sleep();
uint32_t hw_tick_ms_get(void);

#endif /* HW_H_ */
