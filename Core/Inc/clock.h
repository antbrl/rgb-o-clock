#ifndef __CLOCK
#define __CLOCK

#include <stdint.h>
#include "stm32l4xx_hal.h"

void display_clock(RTC_HandleTypeDef *hrtc, TIM_HandleTypeDef *htim);

#endif
