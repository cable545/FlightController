/*
 * status_led.h
 *
 *  Created on: Feb 20, 2020
 *      Author: burde
 */

#ifndef _STATUS_LEDS_H
#define _STATUS_LEDS_H

#include "gpio.h"

#define STATUS_LED_GPIO_PORT	GPIOB
#define STATUS_LED_R_PIN			LL_GPIO_PIN_8
#define STATUS_LED_G_PIN			LL_GPIO_PIN_9

#define LED_R_TOGGLE	(LL_GPIO_TogglePin(STATUS_LED_GPIO_PORT, STATUS_LED_R_PIN))
#define LED_G_TOGGLE	(LL_GPIO_TogglePin(STATUS_LED_GPIO_PORT, STATUS_LED_G_PIN))

#define LED_R_ON (LL_GPIO_SetOutputPin(STATUS_LED_GPIO_PORT, STATUS_LED_R_PIN))
#define LED_R_OFF (LL_GPIO_ResetOutputPin(STATUS_LED_GPIO_PORT, STATUS_LED_R_PIN))

void STATUS_LEDS_Init(void);
void STATUS_LEDS_ToggleLeds(void);

#endif
