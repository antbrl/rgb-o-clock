#ifndef __CLOCK
#define __CLOCK

#include <stdint.h>
#include "stm32l4xx_hal.h"

void display_clock(RTC_HandleTypeDef *hrtc, TIM_HandleTypeDef *htim);
void clock_ring(RTC_HandleTypeDef *hrtc, uint8_t hours, uint8_t minutes,
		uint8_t seconds, uint8_t ss);

#endif
