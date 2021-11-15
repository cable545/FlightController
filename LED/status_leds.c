/*
 * status_led.c
 *
 *  Created on: Feb 20, 2020
 *      Author: burde
 */

#include "status_leds.h"

void STATUS_LEDS_Init(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

	GPIO_InitStruct.Pin = STATUS_LED_R_PIN | STATUS_LED_G_PIN;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(STATUS_LED_GPIO_PORT, &GPIO_InitStruct);
}

void STATUS_LEDS_ToggleLeds(void)
{
	LED_R_TOGGLE;
	LED_G_TOGGLE;
}
