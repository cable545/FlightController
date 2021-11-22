/*
 * receiver.h
 *
 *  Created on: 03.11.2021
 *      Author: burde
 */

#ifndef _RECEIVER_H
#define _RECEIVER_H

#include "main.h"

#define RECEIVER_GPIO_PORT	GPIOB
#define RECEIVER_PIN 				LL_GPIO_PIN_6

#define RECEIVER_TIM				TIM4
#define RECEIVER_TIM_CH			LL_TIM_CHANNEL_CH1

#define RECEIVER_CHANNEL_CNT			4
#define RECEIVER_DMA_BUFFER_SIZE	RECEIVER_CHANNEL_CNT + 1

#define RECEIVER_CHANNEL_VALUE_COUNT_MIN			1000
#define RECEIVER_CHANNEL_VALUE_COUNT_MIDDLE		2000
#define RECEIVER_CHANNEL_VALUE_COUNT_MAX			2100

#define RECEIVER_THROTTLE_CHANNEL 0

uint16_t* RECEIVER_Init(void);
uint8_t RECEIVER_ProcessCapturedValues(void);

#endif
