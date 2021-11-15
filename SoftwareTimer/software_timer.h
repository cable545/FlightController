/*
 * software_timer.h
 *
 *  Created on: 05.11.2021
 *      Author: burde
 */

#ifndef _SOFTWARE_TIMER_H
#define _SOFTWARE_TIMER_H

#include "main.h"

enum
{
	STIMER1 = 0,
	STIMER_MAX
};

#define ONESECTIMER			STIMER1

void STIMER_Init(void (*funcptr)(void));
int8_t STIMER_StartTimer (uint8_t soft_id, uint16_t ticks, void (*funcptr) (void));
uint8_t STIMER_TestTimer(uint8_t soft_id);
uint16_t STIMER_GetTimer (uint8_t soft_id);
void STIMER_KillTimer (uint8_t soft_id);
void STIMER_PollHandler(void);

void STIMER_OneSectimerExpired(void);

#endif
