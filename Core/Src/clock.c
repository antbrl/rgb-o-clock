#include "clock.h"
#include "ws2812.h"

void clock_ring(RTC_HandleTypeDef *hrtc, uint8_t hours, uint8_t minutes,
		uint8_t seconds, uint8_t ss)
{
	uint32_t led;
	uint32_t led_hours = (hours % 12) * 5 + minutes / 12;

	for (led = 0; led < 60; led++)
	{
		// current second
		if (led == seconds)
		{
			setLEDcolor(led, 0, 255 - ss, 255 - ss);
		}
		// last second
		else if ((led + 59) % 60 == seconds)
		{
			setLEDcolor(led, 0, ss, ss);
		}
		// turn around
		else if (led == (ss * 60 / 255 + seconds) % 60)
		{
			setLEDcolor(led, 0, 2, 2);
		}
		// ring
		else
		{
			setLEDcolor(led, 0, 1, 1);
		}

		if (led == minutes)
		{
			if (led == led_hours)
				setLEDcolor(led, 255, 255, 0);
			else
				setLEDcolor(led, 0, 255, 0);
		}
		else if (led == led_hours)
		{
			setLEDcolor(led, 255, 0, 0);
		}
	}
}

void display_clock(RTC_HandleTypeDef *hrtc, TIM_HandleTypeDef *htim)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;

		// get time
		HAL_RTC_GetDate(hrtc, &sDate, RTC_FORMAT_BIN);
		HAL_RTC_GetTime(hrtc, &sTime, RTC_FORMAT_BIN);

		// background
		setWHOLEcolor(0, 0, 0);

		clock_ring(hrtc, sTime.Hours, sTime.Minutes, sTime.Seconds,
				255 - sTime.SubSeconds);

		ws2812_update(htim);

		HAL_Delay(10);
		//saveLEDbuffer();


}
