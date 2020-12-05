#include "clock.h"
#include "ws2812.h"
#include <stdbool.h>

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
				{ 0, 0, 0, 0, 1, 1, 1, 1, 1 },
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
	uint32_t hours_led = (hours % 12) * 5 + minutes / 12;
	bool animating_minutes = seconds == 0;
	bool animating_hours = seconds == 0 && minutes % 12 == 0;

	for (led = 0; led < 60; led++)
		setLEDcolor(led, 0, 5, 5);
	for (led = 0; led < 12; led++)
			setLEDcolor(60 + led * 4, 0, 5, 5);
	for (led = 0; led < 4; led++)
			setLEDcolor(108 + led * 10, 0, 5, 5);

	// current second
	mixLEDcolor(seconds, 0, ss, ss);
	// last second animation
	mixLEDcolor((seconds + 59) % 60, 0, 255 - ss, 255 - ss);
	// turn around
	//mixLEDcolor((ss * 60 / 255 + seconds) % 60, 0, 2, 2);
	// minute
	mixLEDcolor(minutes, 0, animating_minutes ? ss : 255, 0);
	// last minute animation
	if (animating_minutes)
		mixLEDcolor((minutes + 59) % 60, 0, 255 - ss, 0);
	// hour
	mixLEDcolor(hours_led, animating_hours ? ss : 255, 0, 0);
	// last hour led animation
	if (animating_hours)
		mixLEDcolor((hours_led + 59) % 60, 255 - ss, 0, 0);
}

void render_digit(uint8_t position, uint8_t digit, uint8_t red, uint8_t green, uint8_t blue)
{
	uint32_t led;
	for (led = 0; led < 9; led++)
	{
		if (digits[position / 2][digit][0][led])
			mixLEDcolor(led + 109 + 10 * position, red, green, blue);
	}
	for (led = 0; led < 7; led++)
	{
		if (digits[position / 2][digit][1][led])
			mixLEDcolor(led + 149 + 8 * position, red, green, blue);
	}
	for (led = 0; led < 5; led++)
	{
		if (digits[position / 2][digit][2][led])
			mixLEDcolor(led + 181 + 6 * position, red, green, blue);
	}
}

void render_digits(RTC_HandleTypeDef *hrtc, uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t ss)
{
	uint8_t r, g, b, r0, g0, b0, r1, g1, b1;
	r = 50;
	g = 40;
	b = 2;
	r0 = r * ss / 255;
	g0 = g * ss / 255;
	b0 = b * ss / 255;
	r1 = r * (255 - ss) / 255;
	g1 = g * (255 - ss) / 255;
	b1 = b * (255 - ss) / 255;
	uint8_t new_digits[4] =
		{ hours % 10, minutes % 10, minutes / 10, hours / 10 };
	uint8_t old_digits[4] =
		{ (hours + 23 * (minutes == 0)) % 24 % 10, (minutes + 9) % 10, (minutes + 59 * (minutes % 10 == 0)) % 60 / 10,
				(hours + 23 * (minutes == 0)) % 24 / 10 };
	bool anim[4] =
		{ seconds == 0 && new_digits[0] != old_digits[0], seconds == 0 && new_digits[1] != old_digits[1], seconds == 0
				&& new_digits[2] != old_digits[2], seconds == 0 && new_digits[3] != old_digits[3] };

	// current digits
	render_digit(0, new_digits[0], anim[0] ? r0 : r, anim[0] ? g0 : g, anim[0] ? b0 : b);
	render_digit(1, new_digits[1], anim[1] ? r0 : r, anim[1] ? g0 : g, anim[1] ? b0 : b);
	render_digit(2, new_digits[2], anim[2] ? r0 : r, anim[2] ? g0 : g, anim[2] ? b0 : b);
	render_digit(3, new_digits[3], anim[3] ? r0 : r, anim[3] ? g0 : g, anim[3] ? b0 : b);

	// last digits
	if (anim[0])
		render_digit(0, old_digits[0], r1, g1, b1);
	if (anim[1])
		render_digit(1, old_digits[1], r1, g1, b1);
	if (anim[2])
		render_digit(2, old_digits[2], r1, g1, b1);
	if (anim[3])
		render_digit(3, old_digits[3], r1, g1, b1);
}

void display_clock(RTC_HandleTypeDef *hrtc, TIM_HandleTypeDef *htim)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	uint8_t h, m, s, ss;

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

	dimBuffer(50);

	ws2812_update(htim);

	HAL_Delay(10);
	//saveLEDbuffer();

}
