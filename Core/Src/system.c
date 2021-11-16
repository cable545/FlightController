/*
 * system.h
 *
 *  Created on: 26.10.2021
 *      Author: burde
 */

#include "system.h"

void SysTick_Handler(void)
{
	STIMER_PollHandler();
}

void delayInit(void)
{
	CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk;
	CoreDebug->DEMCR |=  CoreDebug_DEMCR_TRCENA_Msk;

	DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;
	DWT->CTRL |=  DWT_CTRL_CYCCNTENA_Msk;

	DWT->CYCCNT = 0;
}

void SYSTEM_InitSystick(uint32_t microseconds)
{
	SysTick_Config(SystemCoreClock / (1000000 / microseconds));
	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),3, 1));
}
