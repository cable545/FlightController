/*
 * dshot.c
 *
 *  Created on: 04.10.2021
 *      Author: burde
 */

#include "dshot.h"

static uint32_t MotorA_Dmabuffer[DSHOT_DMA_BUFFER_SIZE];
static uint32_t MotorB_Dmabuffer[DSHOT_DMA_BUFFER_SIZE];
static uint32_t MotorC_Dmabuffer[DSHOT_DMA_BUFFER_SIZE];
static uint32_t MotorD_Dmabuffer[DSHOT_DMA_BUFFER_SIZE];

static uint16_t DSHOT_PreparePacket(uint16_t value);
static void DSHOT_PrepareDmabuffer(uint32_t *motorDmabuffer, uint16_t value);
static void DSHOT_PrepareDmabufferAll();
static void DSHOT_DmaStart();
static void DSHOT_DmaStartMotorA(void);
static void DSHOT_DmaStartMotorB(void);
static void DSHOT_DmaStartMotorC(void);
static void DSHOT_DmaStartMotorD(void);
static void DSHOT_EnableDmaRequest();

static uint32_t DSHOT_ChooseType(DshotType_e dshotType);
static void DSHOT_InitTimer(DshotType_e dshotType);
static void DSHOT_StartPwm();

void DSHOT_Init(DshotType_e dshotType)
{
	DSHOT_InitTimer(dshotType);
	DSHOT_StartPwm();
}

void DSHOT_Write(uint16_t *motorValue)
{
	DSHOT_PrepareDmabufferAll(motorValue);
	DSHOT_DmaStart();
	DSHOT_EnableDmaRequest();
}

static uint32_t DSHOT_ChooseType(DshotType_e dshotType)
{
	switch(dshotType)
	{
		case(DSHOT600):
			return DSHOT600_HZ;

		case(DSHOT300):
			return DSHOT300_HZ;

		case(DSHOT150):

		default:
			return DSHOT150_HZ;
	}
}

static void DSHOT_InitTimer(DshotType_e dshotType)
{
	uint16_t dshotPrescaler;

	GPIO_InitGpioA();
	GPIO_InitGpioC();
	TIM_InitTimer3();
	DMA_InitDma1();

	// Calculate prescaler by dshot type
	dshotPrescaler = lrintf((float) TIMER_CLK / DSHOT_ChooseType(dshotType) + 0.01f) - 1;

	LL_TIM_SetPrescaler(MOTOR_A_TIM, dshotPrescaler);
	LL_TIM_SetAutoReload(MOTOR_A_TIM, MOTOR_BITLENGTH);

//	LL_TIM_SetPrescaler(MOTOR_B_TIM, dshotPrescaler);
//	LL_TIM_SetAutoReload(MOTOR_B_TIM, MOTOR_BITLENGTH);
//
//	LL_TIM_SetPrescaler(MOTOR_C_TIM, dshotPrescaler);
//	LL_TIM_SetAutoReload(MOTOR_C_TIM, MOTOR_BITLENGTH);
//
//	LL_TIM_SetPrescaler(MOTOR_D_TIM, dshotPrescaler);
//	LL_TIM_SetAutoReload(MOTOR_D_TIM, MOTOR_BITLENGTH);
//
//	LL_TIM_EnableDMAReq_CC1(MOTOR_A_TIM);
//	LL_TIM_EnableDMAReq_CC2(MOTOR_B_TIM);
//	LL_TIM_EnableDMAReq_CC3(MOTOR_C_TIM);
//	LL_TIM_EnableDMAReq_CC4(MOTOR_D_TIM);
//
	LL_TIM_EnableCounter(MOTOR_A_TIM);
//	LL_TIM_EnableCounter(MOTOR_B_TIM);
//	LL_TIM_EnableCounter(MOTOR_C_TIM);
//	LL_TIM_EnableCounter(MOTOR_D_TIM);
}

static void DSHOT_StartPwm()
{
	/* Enable the Capture compare channel */
	LL_TIM_CC_EnableChannel(MOTOR_A_TIM, MOTOR_A_TIM_CH);
	LL_TIM_EnableIT_CC1(MOTOR_A_TIM);
	//	LL_TIM_CC_EnableChannel(MOTOR_B_TIM, MOTOR_B_TIM_CH);
	//	LL_TIM_CC_EnableChannel(MOTOR_C_TIM, MOTOR_C_TIM_CH);
	//	LL_TIM_CC_EnableChannel(MOTOR_D_TIM, MOTOR_D_TIM_CH);
}

static uint16_t DSHOT_PreparePacket(uint16_t value)
{
	uint16_t packet;
	uint8_t dshot_telemetry = 0;

	packet = (value << 1) | (dshot_telemetry ? 1 : 0);

	// compute checksum
	unsigned csum = 0;
	unsigned csumData = packet;

	for(int i = 0; i < 3; i++)
	{
		csum ^=  csumData; // xor data by nibbles
		csumData >>= 4;
	}

	csum &= 0xf;
	packet = (packet << 4) | csum;

	return packet;
}

static void DSHOT_PrepareDmabuffer(uint32_t *motorDmabuffer, uint16_t value)
{
	uint16_t packet;
	packet = DSHOT_PreparePacket(value);

	for(int i = 0; i < 16; i++)
	{
		motorDmabuffer[i] = (packet & 0x8000) ? MOTOR_BIT_1 : MOTOR_BIT_0;
		packet <<= 1;
	}

	motorDmabuffer[16] = 0;
	motorDmabuffer[17] = 0;
}

static void DSHOT_PrepareDmabufferAll(uint16_t *motorValue)
{
	DSHOT_PrepareDmabuffer(MotorA_Dmabuffer, motorValue[0]);
//	DSHOT_PrepareDmabuffer(MotorB_Dmabuffer, motorValue[1]);
//	DSHOT_PrepareDmabuffer(MotorC_Dmabuffer, motorValue[2]);
//	DSHOT_PrepareDmabuffer(MotorD_Dmabuffer, motorValue[3]);
}

static void DSHOT_DmaStart()
{
	DSHOT_DmaStartMotorA();
//	DSHOT_DmaStartMotorB();
//	DSHOT_DmaStartMotorC();
//	DSHOT_DmaStartMotorD();
}

static void DSHOT_EnableDmaRequest()
{
	LL_TIM_EnableDMAReq_CC1(MOTOR_A_TIM);
//	LL_TIM_EnableDMAReq_CC2(MOTOR_B_TIM);
//	LL_TIM_EnableDMAReq_CC3(MOTOR_C_TIM);
//	LL_TIM_EnableDMAReq_CC4(MOTOR_D_TIM);
}

static void DSHOT_DmaStartMotorA(void)
{
	LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_4);
	LL_DMA_ClearFlag_TC4(DMA1);
	LL_DMA_ClearFlag_HT4(DMA1);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_4, DSHOT_DMA_BUFFER_SIZE);
	LL_DMA_SetChannelSelection(DMA1, LL_DMA_STREAM_4, LL_DMA_CHANNEL_5);
	LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_4, (uint32_t)MotorA_Dmabuffer, (uint32_t)&MOTOR_A_TIM->CCR1, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_4);
}

//static void DSHOT_DmaStartMotorB(void)
//{
//	LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_5);
//	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_5, DSHOT_DMA_BUFFER_SIZE);
//	LL_DMA_SetChannelSelection(DMA1, LL_DMA_STREAM_5, LL_DMA_CHANNEL_5);
//	LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_5, (uint32_t)MotorB_Dmabuffer, (uint32_t)&MOTOR_B_TIM->CCR2, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
//	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_5);
//}
//
//static void DSHOT_DmaStartMotorC(void)
//{
//	LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_7);
//	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_7, DSHOT_DMA_BUFFER_SIZE);
//	LL_DMA_SetChannelSelection(DMA1, LL_DMA_STREAM_7, LL_DMA_CHANNEL_5);
//	LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_7, (uint32_t)MotorC_Dmabuffer, (uint32_t)&MOTOR_C_TIM->CCR3, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
//	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_7);
//}
//
//static void DSHOT_DmaStartMotorD(void)
//{
//	LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_2);
//	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_2, DSHOT_DMA_BUFFER_SIZE);
//	LL_DMA_SetChannelSelection(DMA1, LL_DMA_STREAM_2, LL_DMA_CHANNEL_5);
//	LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_2, (uint32_t)MotorD_Dmabuffer, (uint32_t)&MOTOR_D_TIM->CCR4, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
//	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_2);
//}
