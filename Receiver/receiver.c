/*
 * receiver.c
 *
 *  Created on: 03.11.2021
 *      Author: burde
 */

#include "receiver.h"

#define ENABLE_DMA_TRANSFER_COMPLETE_ISR	LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_0)
#define DISABLE_DMA_TRANSFER_COMPLETE_ISR	LL_DMA_DisableIT_TC(DMA1, LL_DMA_STREAM_0)

#define ENABLE_TIM_CC1_ISR		LL_TIM_EnableIT_CC1(RECEIVER_TIM)
#define DISABLE_TIM_CC1_ISR		LL_TIM_DisableIT_CC1(RECEIVER_TIM)

static uint16_t DmaBuffer[RECEIVER_DMA_BUFFER_SIZE];
static uint8_t BufferFull;

static uint16_t ProcessedValues[RECEIVER_CHANNEL_CNT];

#define TIMER_RESET_VALUE 0x00000000

void DMA1_Stream0_IRQHandler(void)
{
	if(LL_DMA_IsActiveFlag_TC0(DMA1))
	{
		LL_DMA_ClearFlag_TC0(DMA1);
		BufferFull = TRUE;
	}

	if(LL_DMA_IsActiveFlag_HT0(DMA1))
	{
		LL_DMA_ClearFlag_HT0(DMA1);
	}

	RECEIVER_TIM->CNT = TIMER_RESET_VALUE;
}

void TIM4_IRQHandler(void)
{
	if(LL_TIM_IsActiveFlag_CC1(RECEIVER_TIM))
	{
		LL_TIM_ClearFlag_CC1(RECEIVER_TIM);

	}
}

static void RECEIVER_InitGpio(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

	GPIO_InitStruct.Pin = RECEIVER_PIN;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
	LL_GPIO_Init(RECEIVER_GPIO_PORT, &GPIO_InitStruct);
}

static void RECEIVER_InitTimer(void)
{
	LL_TIM_InitTypeDef TIM_InitStruct = {0};
//	LL_TIM_IC_InitTypeDef TIM_IC_InitStruct = {0};

	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);

	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	TIM_InitStruct.Prescaler = 84;
	TIM_InitStruct.Autoreload = 0xFFFF;
	LL_TIM_Init(RECEIVER_TIM, &TIM_InitStruct);
	LL_TIM_SetClockSource(RECEIVER_TIM, LL_TIM_CLOCKSOURCE_INTERNAL);

//	TIM_IC_InitStruct.ICPolarity = LL_TIM_IC_POLARITY_FALLING;
//	TIM_IC_InitStruct.ICPrescaler = LL_TIM_ICPSC_DIV1;
//	LL_TIM_IC_Init(RECEIVER_TIM, LL_TIM_CHANNEL_CH1, &TIM_IC_InitStruct);

	RECEIVER_TIM->CCMR1 = 0x01;
	LL_TIM_IC_SetPolarity(RECEIVER_TIM, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_FALLING);
	LL_TIM_CC_EnableChannel(RECEIVER_TIM, RECEIVER_TIM_CH);
	LL_TIM_CC_SetDMAReqTrigger(RECEIVER_TIM, LL_TIM_CCDMAREQUEST_CC);
}

static void RECEIVER_InitDma(void)
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

	LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_STREAM_0, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
	LL_DMA_SetStreamPriorityLevel(DMA1, LL_DMA_STREAM_0, LL_DMA_PRIORITY_LOW);
	LL_DMA_SetMode(DMA1, LL_DMA_STREAM_0, LL_DMA_MODE_CIRCULAR);
	LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_STREAM_0, LL_DMA_PERIPH_NOINCREMENT);
	LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_0, LL_DMA_MEMORY_INCREMENT);
	LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_0, LL_DMA_PDATAALIGN_HALFWORD);
	LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_0, LL_DMA_PDATAALIGN_HALFWORD);
	LL_DMA_SetChannelSelection(DMA1, LL_DMA_STREAM_0, LL_DMA_CHANNEL_2);
	LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_0, (uint32_t)&RECEIVER_TIM->CCR1, (uint32_t)DmaBuffer, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, RECEIVER_DMA_BUFFER_SIZE);

	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0);

	ENABLE_DMA_TRANSFER_COMPLETE_ISR;
	NVIC_SetPriority(DMA1_Stream0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),3, 2));
	NVIC_EnableIRQ(DMA1_Stream0_IRQn);
}

static uint8_t RECEIVER_IsSyncSignalValue(uint32_t value)
{
	return value > RECEIVER_CHANNEL_VALUE_COUNT_MAX;
}

static void RECEIVER_PrepareCapturedValues(void)
{
	for(uint32_t i = 0; i < RECEIVER_CHANNEL_CNT; i++)
	{
		if(ProcessedValues[i] < 1000)
			ProcessedValues[i] = 1000;
		else if(ProcessedValues[i] > 2000)
			ProcessedValues[i] = 2000;
	}
}

uint16_t* RECEIVER_Init(void)
{
	BufferFull = FALSE;

	RECEIVER_InitGpio();
	RECEIVER_InitTimer();
	RECEIVER_InitDma();

	LL_TIM_EnableDMAReq_CC1(RECEIVER_TIM);

	LL_TIM_EnableCounter(RECEIVER_TIM);

	return ProcessedValues;
}

uint8_t RECEIVER_ProcessCapturedValues(void)
{
	uint16_t tmpValues[RECEIVER_DMA_BUFFER_SIZE] = {0};
	uint32_t syncSignalIndex = 0;
	uint32_t offset;

	if(BufferFull)
	{
		for(uint32_t i = 0; i < RECEIVER_DMA_BUFFER_SIZE; i++)
		{
			if(i == 0)
				tmpValues[i] = DmaBuffer[i];
			else
				tmpValues[i] = DmaBuffer[i] - DmaBuffer[i - 1];

			if(RECEIVER_IsSyncSignalValue(tmpValues[i]))
				syncSignalIndex = i;
		}

		if(syncSignalIndex == 0 || syncSignalIndex == RECEIVER_CHANNEL_CNT)
		{
			offset = (syncSignalIndex == 0) ? 1 : 0;

			for(uint32_t i = 0; i < RECEIVER_CHANNEL_CNT; i++)
				ProcessedValues[i] = tmpValues[i + offset];
		}
		else
		{
			for(uint32_t i = 0, j = syncSignalIndex + 1; i < RECEIVER_CHANNEL_CNT; j++)
			{
				if(j >= RECEIVER_DMA_BUFFER_SIZE) j = 0;
				ProcessedValues[i++] = tmpValues[j];
			}
		}

		RECEIVER_PrepareCapturedValues();

		BufferFull = FALSE;
	}

	return TRUE;
}
