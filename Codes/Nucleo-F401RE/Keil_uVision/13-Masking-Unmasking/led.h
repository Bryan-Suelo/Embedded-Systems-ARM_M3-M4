#ifndef __LED_H_
#define __LED_H_

#include "stdint.h"

void led_init(void);	
void led_on(uint8_t led_no);
void led_off(uint8_t led_no);
void led_toggle(uint8_t led_no);

#endif 
