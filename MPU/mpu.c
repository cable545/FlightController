/*
 * mpu.c
 *
 *  Created on: Nov 21, 2021
 *      Author: cable
 */

#include "mpu.h"

#define MPU_CS_PIN					LL_GPIO_PIN_2
#define MPU_CS_GPIO_PORT		GPIOD

static void MPU_InitGpio(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	LL_AHB1_GRP1_EnableClockLowPower(LL_AHB1_GRP1_PERIPH_GPIOD);

	GPIO_InitStruct.Pin = MPU_CS_PIN;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(MPU_CS_GPIO_PORT, &GPIO_InitStruct);

//	#define SPI3_SCK_PIN		LL_GPIO_PIN_10
//	#define SPI3_MISO_PIN		LL_GPIO_PIN_11
//	#define SPI3_MOSI_PIN		LL_GPIO_PIN_12
//	#define SPI3_GPIO_PORT	GPIOC
//
//	LL_AHB1_GRP1_EnableClockLowPower(LL_AHB1_GRP1_PERIPH_GPIOC);
//
//	GPIO_InitStruct.Pin = SPI3_SCK_PIN | SPI3_MISO_PIN | SPI3_MOSI_PIN;
//	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
//	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
//	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
//	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
//	GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
//	LL_GPIO_Init(SPI3_GPIO_PORT, &GPIO_InitStruct);
}

void MPU_Init(void)
{
	SPI3_Init();
}
