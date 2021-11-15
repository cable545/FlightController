/*
 * dshot.h
 *
 *  Created on: 04.10.2021
 *      Author: burde
 */

#ifndef _DSHOT_H
#define _DSHOT_H

#include <math.h>

#include "gpio.h"
#include "tim.h"
#include "dma.h"

#define TIMER_CLK				84000000			//84MHz

#define MOTOR_A_TIM			TIM3
#define MOTOR_A_TIM_CH	LL_TIM_CHANNEL_CH1

#define MOTOR_B_TIM			TIM3
#define MOTOR_B_TIM_CH	LL_TIM_CHANNEL_CH2

#define MOTOR_C_TIM			TIM3
#define MOTOR_C_TIM_CH	LL_TIM_CHANNEL_CH3

#define MOTOR_D_TIM			TIM3
#define MOTOR_D_TIM_CH	LL_TIM_CHANNEL_CH4

#define MHZ_TO_HZ(x)		((x) * 1000000)

#define DSHOT600_HZ			MHZ_TO_HZ(12)
#define DSHOT300_HZ			MHZ_TO_HZ(6)
#define DSHOT150_HZ			MHZ_TO_HZ(3)

#define MOTOR_BIT_0            	7
#define MOTOR_BIT_1            	14
#define MOTOR_BITLENGTH        	19

#define DSHOT_FRAME_SIZE       	16
#define DSHOT_DMA_BUFFER_SIZE   18 /* resolution + frame reset (2us) */

#define DSHOT_MIN_THROTTLE      48
#define DSHOT_MAX_THROTTLE     	2047
#define DSHOT_RANGE 						(DSHOT_MAX_THROTTLE - DSHOT_MIN_THROTTLE)

typedef enum
{
	DSHOT150,
	DSHOT300,
	DSHOT600
} DshotType_e;

void DSHOT_Init(DshotType_e dshotType);
void DSHOT_Write(uint16_t *motorValue);

#endif
