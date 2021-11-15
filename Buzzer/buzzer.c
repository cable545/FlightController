/*
 * buzzer.c
 *
 *  Created on: 25.10.2021
 *      Author: burde
 */

#include "buzzer.h"

void BUZZER_Init(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);

	GPIO_InitStruct.Pin = BUZZER_GPIO_PIN_A | BUZZER_GPIO_PIN_B;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(BUZZER_GPIO_PORT, &GPIO_InitStruct);
}

void BUZZER_Beep(void)
{
	for(int j = 1000; j > 0; j--)
	{
		LL_GPIO_SetOutputPin(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN_A);
		LL_GPIO_ResetOutputPin(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN_B);

		delayMicros(BUZZER_DELAY_MICROS);

		LL_GPIO_SetOutputPin(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN_B);
		LL_GPIO_ResetOutputPin(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN_A);

		delayMicros(BUZZER_DELAY_MICROS);
	}
}

void BUZZER_BeepXTimers(uint32_t cntBeeps)
{
	for(int i = cntBeeps; i > 0; i--)
	{
		BUZZER_Beep();
		delayMicros(500000);
	}
}
