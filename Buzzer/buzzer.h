/*
 * buzzer.h
 *
 *  Created on: 25.10.2021
 *      Author: burde
 */

#ifndef _BUZZER_H
#define _BUZZER_H

#include "gpio.h"

#define BUZZER_GPIO_PORT 		GPIOC
#define BUZZER_GPIO_PIN_A 	LL_GPIO_PIN_0
#define BUZZER_GPIO_PIN_B 	LL_GPIO_PIN_1

#define BUZZER_DELAY_MICROS 125

void BUZZER_Init(void);
void BUZZER_Beep(void);
void BUZZER_BeepXTimers(uint32_t cntBeeps);

#endif
