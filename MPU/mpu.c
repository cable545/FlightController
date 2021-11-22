/*
 * mpu.c
 *
 *  Created on: Nov 21, 2021
 *      Author: cable
 */

#include "mpu.h"

#define MPU_CS_PIN					LL_GPIO_PIN_2
#define MPU_CS_GPIO_PORT		GPIOD

#define CS_MPU_ACTIVATE()	(LL_GPIO_ResetOutputPin(MPU_CS_GPIO_PORT, MPU_CS_PIN))
#define CS_MPU_DEACTIVATE()	(LL_GPIO_SetOutputPin(MPU_CS_GPIO_PORT, MPU_CS_PIN))

// ICM-20948 mpu register addresses

#define WHO_AM_I_REG 				0x00
#define USER_CTRL_REG 			0x01
#define LP_CONFIG_REG				0x02

#define PWR_MGMT_1_REG			0x06

static void MPU_InitGpio(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);

	GPIO_InitStruct.Pin = MPU_CS_PIN;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(MPU_CS_GPIO_PORT, &GPIO_InitStruct);
}

void MPU_Init(void)
{
	MPU_InitGpio();
	SPI3_Init();
}

uint8_t MPU_ReadWhoAmIReg(void)
{
	uint8_t value = 0;

	CS_MPU_ACTIVATE();

	SPI3_TransferByte(WHO_AM_I_REG | 0x80);
	value = SPI3_TransferByte(0);

	CS_MPU_DEACTIVATE();

	return value;
}
