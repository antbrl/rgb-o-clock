#include "ws2812.h"
#include <string.h>

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

/* Variables -----------------------------------------------*/
static uint8_t LEDbuffer[LED_BUFFER_SIZE];
static uint8_t LEDbuffer_temp[LED_BUFFER_SIZE];

/* Functions -----------------------------------------------*/

void ws2812_start(TIM_HandleTypeDef *htim)
{
	fillBufferBlack();
	saveLEDbuffer();
	HAL_TIM_PWM_Start_DMA(htim, TIM_CHANNEL_1, (uint32_t*) LEDbuffer, LED_BUFFER_SIZE);
}

void ws2812_update(TIM_HandleTypeDef *htim)
{
	HAL_TIM_PWM_Start_DMA(htim, TIM_CHANNEL_1, (uint32_t*) LEDbuffer, LED_BUFFER_SIZE);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
}

void saveLEDbuffer(void)
{
	memcpy(LEDbuffer, LEDbuffer_temp, LED_BUFFER_SIZE);
}

void getLEDcolor(uint32_t LEDnumber, uint8_t *ro, uint8_t *go, uint8_t *bo)
{
	uint32_t i;
	uint32_t LEDindex = LEDnumber % LED_NUMBER;
	*ro = *go = *bo = 0;

	for (i = 0; i < 8; i++)
	{
		*go |= ((LEDbuffer[RESET_SLOTS_BEGIN + LEDindex * 24 + 7 - i]) >= WS2812_1) << i;
		*ro |= ((LEDbuffer[RESET_SLOTS_BEGIN + LEDindex * 24 + 15 - i]) >= WS2812_1) << i;
		*bo |= ((LEDbuffer[RESET_SLOTS_BEGIN + LEDindex * 24 + 23 - i]) >= WS2812_1) << i;
	}
}

void setLEDcolor(uint32_t LEDnumber, uint8_t RED, uint8_t GREEN, uint8_t BLUE)
{
	uint8_t tempBuffer[24];
	uint32_t i;
	uint32_t LEDindex;
	LEDindex = LEDnumber % LED_NUMBER;

	for (i = 0; i < 8; i++) // GREEN data
		tempBuffer[i] = ((GREEN << i) & 0x80) ? WS2812_1 : WS2812_0;
	for (i = 0; i < 8; i++) // RED
		tempBuffer[8 + i] = ((RED << i) & 0x80) ? WS2812_1 : WS2812_0;
	for (i = 0; i < 8; i++) // BLUE
		tempBuffer[16 + i] = ((BLUE << i) & 0x80) ? WS2812_1 : WS2812_0;

	for (i = 0; i < 24; i++)
		LEDbuffer[RESET_SLOTS_BEGIN + LEDindex * 24 + i] = tempBuffer[i];
}

void mixLEDcolor(uint32_t LEDnumber, uint8_t RED, uint8_t GREEN, uint8_t BLUE)
{
	uint8_t r, g, b;
	getLEDcolor(LEDnumber, &r, &g, &b);
	setLEDcolor(LEDnumber, min(r + RED, 255), min(g + GREEN, 255), min(b + BLUE, 255));
}

void setWHOLEcolor(uint8_t RED, uint8_t GREEN, uint8_t BLUE)
{
	uint32_t index;

	for (index = 0; index < LED_NUMBER; index++)
		setLEDcolor(index, RED, GREEN, BLUE);
}

void dimBuffer(uint8_t amount)
{
	uint32_t index;
	uint8_t r, g, b;

	if (amount > 100)
		amount = 100;

	for (index = 0; index < LED_NUMBER; index++)
	{
		getLEDcolor(index, &r, &g, &b);
		setLEDcolor(index, r * (100 - amount) / 100, g * (100 - amount) / 100, b * (100 - amount) / 100);
	}
}

void fillBufferBlack(void)
{
	/*Fill LED buffer - ALL OFF*/
	uint32_t index, buffIndex;
	buffIndex = 0;

	for (index = 0; index < RESET_SLOTS_BEGIN; index++)
	{
		LEDbuffer_temp[buffIndex] = WS2812_RESET;
		buffIndex++;
	}
	for (index = 0; index < LED_DATA_SIZE; index++)
	{
		LEDbuffer_temp[buffIndex] = WS2812_0;
		buffIndex++;
	}
	LEDbuffer_temp[buffIndex] = WS2812_0;
	buffIndex++;
	for (index = 0; index < RESET_SLOTS_END; index++)
	{
		LEDbuffer_temp[buffIndex] = 0;
		buffIndex++;
	}
}

void fillBufferWhite(void)
{
	/*Fill LED buffer - ALL OFF*/
	uint32_t index, buffIndex;
	buffIndex = 0;

	for (index = 0; index < RESET_SLOTS_BEGIN; index++)
	{
		LEDbuffer_temp[buffIndex] = WS2812_RESET;
		buffIndex++;
	}
	for (index = 0; index < LED_DATA_SIZE; index++)
	{
		LEDbuffer_temp[buffIndex] = WS2812_1;
		buffIndex++;
	}
	LEDbuffer_temp[buffIndex] = WS2812_0;
	buffIndex++;
	for (index = 0; index < RESET_SLOTS_END; index++)
	{
		LEDbuffer_temp[buffIndex] = 0;
		buffIndex++;
	}
}
