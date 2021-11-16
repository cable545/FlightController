/*
 * system.h
 *
 *  Created on: 26.10.2021
 *      Author: burde
 */

#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "main.h"

uint32_t multiplier;

void delayInit(void);
void SYSTEM_InitSystick(uint32_t microseconds);

static inline void delayMicros(uint32_t micros)
{
	uint32_t clk_cycle_start = DWT->CYCCNT;

	micros = (micros * 168.0f) - 40;

	while((DWT->CYCCNT - clk_cycle_start) < micros);
}

static inline void delayMillis(uint32_t millis)
{
	uint32_t clk_cycle_start = DWT->CYCCNT;

	millis = (millis * 168000.0f) - 40;

	while((DWT->CYCCNT - clk_cycle_start) < millis);
}

#endif
