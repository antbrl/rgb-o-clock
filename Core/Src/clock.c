#include "clock.h"
#include "ws2812.h"

const uint8_t digits[2][10][3][9] =
	{
		{
			{
				{ 1, 1, 1, 1, 1, 1, 1, 1, 1 },
				{ 1, 0, 0, 0, 0, 0, 1 },
				{ 1, 1, 1, 1, 1 } },
			{
				{ 1, 1, 1, 1, 1, 1, 1, 1, 1 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0 } },
			{
				{ 1, 1, 1, 1, 1, 0, 0, 1, 1 },
				{ 1, 0, 0, 1, 0, 0, 1 },
				{ 0, 0, 1, 1, 1 } },
			{
				{ 1, 1, 1, 1, 1, 1, 1, 1, 1 },
				{ 1, 0, 0, 1, 0, 0, 1 },
				{ 1, 0, 0, 0, 1 } },
			{
				{ 1, 1, 1, 1, 1, 1, 1, 1, 1 },
				{ 0, 0, 0, 1, 0, 0, 0 },
				{ 1, 1, 1, 0, 0 } },
			{
				{ 1, 1, 0, 0, 1, 1, 1, 1, 1 },
				{ 1, 0, 0, 1, 0, 0, 1 },
				{ 1, 1, 1, 0, 0 } },
			{
				{ 1, 0, 0, 0, 1, 1, 1, 1, 1 },
				{ 1, 0, 0, 1, 0, 0, 1 },
				{ 1, 1, 1, 1, 1 } },
			{
				{ 1, 1, 1, 1, 1, 1, 1, 1, 1 },
				{ 1, 0, 0, 0, 0, 0, 0 },
				{ 1, 0, 0, 0, 0 } },
			{
				{ 1, 1, 1, 1, 1, 1, 1, 1, 1 },
				{ 1, 0, 0, 1, 0, 0, 1 },
				{ 1, 1, 1, 1, 1 } },
			{
				{ 1, 1, 1, 1, 1, 1, 1, 1, 1 },
				{ 1, 0, 0, 1, 0, 0, 0 },
				{ 1, 1, 1, 0, 0 } } },
		{
			{
				{ 1, 1, 1, 1, 1, 1, 1, 1, 1 },
				{ 1, 0, 0, 0, 0, 0, 1 },
				{ 1, 1, 1, 1, 1 } },
			{
				{ 1, 1, 1, 1, 1, 1, 1, 1, 1 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0 } },
			{
				{ 1, 1, 1, 1, 1, 0, 0, 1, 1 },
				{ 1, 0, 0, 1, 0, 0, 1 },
				{ 0, 0, 1, 1, 1 } },
			{
				{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 1, 0, 0, 1, 0, 0, 1 },
				{ 1, 1, 1, 1, 1 } },
			{
				{ 0, 0, 0, 1, 1, 1, 1, 1, 1 },
				{ 0, 0, 0, 1, 0, 0, 0 },
				{ 1, 1, 1, 1, 1 } },
			{
				{ 1, 1, 0, 0, 1, 1, 1, 1, 1 },
				{ 1, 0, 0, 1, 0, 0, 1 },
				{ 1, 1, 1, 0, 0 } },
			{
				{ 1, 1, 1, 1, 1, 1, 1, 1, 1 },
				{ 1, 0, 0, 1, 0, 0, 0 },
				{ 1, 1, 1, 0, 0 } },
			{
				{ 1, 1, 1, 1, 1, 1, 1, 1, 1 },
				{ 0, 0, 0, 0, 0, 0, 1 },
				{ 0, 0, 0, 0, 1 } },
			{
				{ 1, 1, 1, 1, 1, 1, 1, 1, 1 },
				{ 1, 0, 0, 1, 0, 0, 1 },
				{ 1, 1, 1, 1, 1 } },
			{
				{ 0, 0, 0, 0, 1, 1, 1, 1, 1 },
				{ 0, 0, 0, 1, 0, 0, 1 },
				{ 1, 1, 1, 1, 1 } } } };

void render_ring(RTC_HandleTypeDef *hrtc, uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t ss)
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

void render_digit(uint8_t position, uint8_t digit, uint8_t red, uint8_t green, uint8_t blue)
{
	uint32_t led;
	for (led = 0; led < 9; led++)
	{
		if (digits[position / 2][digit][0][led])
			setLEDcolor(led + 109 + 10 * position, red, green, blue);
	}
	for (led = 0; led < 7; led++)
	{
		if (digits[position / 2][digit][1][led])
			setLEDcolor(led + 149 + 8 * position, red, green, blue);
	}
	for (led = 0; led < 5; led++)
	{
		if (digits[position / 2][digit][2][led])
			setLEDcolor(led + 181 + 6 * position, red, green, blue);
	}
}

void render_digits(RTC_HandleTypeDef *hrtc, uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t ss)
{
	uint8_t r, g, b;
	r = 50;
	g = 40;
	b = 2;

	render_digit(0, hours % 10, r, g, b);
	render_digit(1, minutes % 10, r, g, b);
	render_digit(2, minutes / 10, r, g, b);
	render_digit(3, hours / 10, r, g, b);
}

void display_clock(RTC_HandleTypeDef *hrtc)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	uint8_t h, m, s, ss;

	while (1)
	{
// get time
		HAL_RTC_GetDate(hrtc, &sDate, RTC_FORMAT_BIN);
		HAL_RTC_GetTime(hrtc, &sTime, RTC_FORMAT_BIN);
		h = sTime.Hours;
		m = sTime.Minutes;
		s = sTime.Seconds;
		ss = 255 - sTime.SubSeconds;

// background
		setWHOLEcolor(0, 0, 0);

		render_ring(hrtc, h, m, s, ss);
		render_digits(hrtc, h, m, s, ss);

		saveLEDbuffer();

		HAL_Delay(10);
	}
}
