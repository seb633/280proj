/**
	******************************************************************************
  * @file    PT32x0xx_dma.c
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the DMA firmware
  *          library
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers. 
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of the content or the  codes within the content. 
  *
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  *****************************************************************************
**/

#include "PT32x0xx_dma.h"

/** @defgroup DMA
  * @brief DMA driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define DMA_CHAPCR_CH_SEL                 				((u32)0x0000003F)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief 	Initializes the DMA_Channelx peripheral according to
  *         the specified parameters in the DMA_InitStruct.
  * @param 	DMA_Channelx:where x can be 0 to 5 to select the DMA channel.
  * @param 	DMA_InitStruct: pointer to a DMA_InitTypeDef
  *         structure that contains the configuration information for
  *         the specified DMA peripheral.
  * @retval None
  */
void DMA_Init(DMA_Channel_TypeDef *DMA_Channelx, DMA_InitTypeDef *DMA_InitStruct)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_DMA_Channel(DMA_Channelx));
	assert_param(IS_DMA_NumberOfData(DMA_InitStruct->DMA_NumberOfData));
	assert_param(IS_DMA_SourceDataSize(DMA_InitStruct->DMA_SourceDataSize));
	assert_param(IS_DMA_DestinationDataSize(DMA_InitStruct->DMA_DestinationDataSize));
	assert_param(IS_DMA_SourceAddressIncrement(DMA_InitStruct->DMA_SourceAddressIncrement));
	assert_param(IS_DMA_DestinationAddressIncrement(DMA_InitStruct->DMA_DestinationAddressIncrement));
	assert_param(IS_DMA_Direction(DMA_InitStruct->DMA_Direction));
	assert_param(IS_DMA_CircularMode(DMA_InitStruct->DMA_CircularMode));
	assert_param(IS_DMA_ChannelPriority(DMA_InitStruct->DMA_ChannelPriority));
	assert_param(IS_DMA_BURST(DMA_InitStruct->DMA_Burst));

	/* Configure DMA channel x:	DMA source base address, DMA destination base address, Number of data to transfer */
	/* Set SBA bit according to DMA_SourceBaseAddress value */
	/* Set DBA bits according to DMA_DestinationBaseAddress value */ 
	/* Set NDT bits according to DMA_NumberOfData value */
	DMA_Channelx->CSBAR = DMA_InitStruct->DMA_SourceBaseAddress;
	DMA_Channelx->CDBAR = DMA_InitStruct->DMA_DestinationBaseAddress;
	DMA_Channelx->CNDTR = DMA_InitStruct->DMA_NumberOfData;

	/* Get the DMA_Channelx CCR value and Clear SSIZE/DSIZE/SINC/DINC/DIR/CIRC/CPC bits */
	/* Configure DMA channel x:	source data size, destination data size, source address increment control, 
		destination address increment control, direction, circular mode and channel priority */
	/* Set SSIZE bit according to DMA_SourceDataSize value */
	/* Set DSIZE bits according to DMA_DestinationDataSize value */ 
	/* Set SINC bits according to DMA_SourceAddressIncrement value */
	/* Set DINC bit according to DMA_DestinationAddressIncrement value */
	/* Set DIR bit according to DMA_Direction value */
	/* Set CIRC bit according to DMA_CircularMode value */
	/* Set CPC bit according to DMA_ChannelPriority value */
	tmpreg = DMA_Channelx->CCR & ~(DMA_CCR_SINC | DMA_CCR_DINC | DMA_CCR_SSIZE | DMA_CCR_BURST |\
									DMA_CCR_CPC | DMA_CCR_DSIZE | DMA_CCR_DIR | DMA_CCR_CIRC);
	tmpreg |= DMA_InitStruct->DMA_SourceDataSize | \
				DMA_InitStruct->DMA_DestinationDataSize | \
				DMA_InitStruct->DMA_SourceAddressIncrement | \
				DMA_InitStruct->DMA_DestinationAddressIncrement | \
				DMA_InitStruct->DMA_Direction | \
				DMA_InitStruct->DMA_CircularMode | \
				DMA_InitStruct->DMA_ChannelPriority | \
				DMA_InitStruct->DMA_Burst;

	/* Write to Channel x CR register */
	DMA_Channelx->CCR = tmpreg;
}

/**
  * @brief 	Fills each DMA_InitStruct member with its default value.
  * @param 	DMA_InitStruct: pointer to a DMA_InitTypeDef structure which will be initialized.
  * @retval None
  */
void DMA_StructInit(DMA_InitTypeDef *DMA_InitStruct)
{
	/* DMA_InitStruct members default value */
	DMA_InitStruct->DMA_NumberOfData = 0xFFFF;
	DMA_InitStruct->DMA_SourceDataSize = DMA_SourceDataSize_Half;
	DMA_InitStruct->DMA_DestinationDataSize = DMA_DestinationDataSize_Half;
	DMA_InitStruct->DMA_SourceAddressIncrement = DMA_SourceAddressIncrement_Enable;
	DMA_InitStruct->DMA_DestinationAddressIncrement = DMA_DestinationAddressIncrement_Enable;
	DMA_InitStruct->DMA_Direction = DMA_Direction_MemoryToMemory;
	DMA_InitStruct->DMA_CircularMode = DMA_CircularMode_Disable;
	DMA_InitStruct->DMA_ChannelPriority = DMA_ChannelPriority_0;
	DMA_InitStruct->DMA_Burst = DMA_Burst_1Unit;
}

/**
  * @brief 	Enable or Disable DMA peripheral.
  * @param 	DMA_Channelx: where x can be 0 to 5 to select the DMA channel.
  * @param  NewState: new state of the DMA channel.
  *        	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DMA_Cmd(DMA_Channel_TypeDef *DMA_Channelx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_DMA_Channel(DMA_Channelx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the selected DMA peripheral */
		DMA_Channelx->CCR |= DMA_CCR_EN;
	}
	else
	{
		/* Disable the selected DMA peripheral */
		DMA_Channelx->CCR &= ~DMA_CCR_EN;
	}
}

/**
  * @brief 	Get DMA's channel current source address.
  * @param 	DMA_Channelx: where x can be 0 to 5 to select the DMA channel.
  * @retval DMA channel current source address.
  */
u32 DMA_GetCurrentSourceAddress(DMA_Channel_TypeDef *DMA_Channelx)
{
	/* Check the parameters */
	assert_param(IS_DMA_Channel(DMA_Channelx));
	/*Get DMA_CHx Current Source Address */
	return DMA_Channelx->CCSAR;
}

/**
  * @brief 	Get DMA channel current destination address.
  * @param 	DMA_Channelx:where x can be 0 to 5 to select the DMA channel.
  * @retval DMA channel current destination address.
  */
u32 DMA_GetCurrentDestinationAddress(DMA_Channel_TypeDef *DMA_Channelx)
{
	/* Check the parameters */
	assert_param(IS_DMA_Channel(DMA_Channelx));
	/*Get DMA_CHx Current Destination Address */
	return DMA_Channelx->CCDAR;
}

/**
  * @brief 	Get DMA channel number of data.
  * @param 	DMA_Channelx:where x can be 0 to 5 to select the DMA channel.
  * @retval DMA channel number of data.
  */
u16 DMA_GetNumberOfData(DMA_Channel_TypeDef *DMA_Channelx)
{
	/* Check the parameters */
	assert_param(IS_DMA_Channel(DMA_Channelx));

	/*Get DMA_CHx Current Destination Address */
	return DMA_Channelx->CCNTR;
}

/**
  * @brief 	Enable or disable the specified DMA interrupts.
  * @param 	DMAx: select the DMA peripheral.
  * @param 	DMA_IT: Specify the DMA interrupts sources to be enabled or disabled.
  *       	This parameter can be one of the following values:
  *       	@arg DMA_IT_TC0E: DMAx channel 0 transfer complete interrupt.
  *       	@arg DMA_IT_TC1E: DMAx channel 1 transfer complete interrupt.
  *       	@arg DMA_IT_TC2E: DMAx channel 2 transfer complete interrupt.
  *       	@arg DMA_IT_TC3E: DMAx channel 3 transfer complete interrupt.
  *       	@arg DMA_IT_TC4E: DMAx channel 4 transfer complete interrupt.
  *       	@arg DMA_IT_TC5E: DMAx channel 5 transfer complete interrupt.
  *       	@arg DMA_IT_TE0E: DMAx channel 0 transfer error interrupt flag.
  *       	@arg DMA_IT_TE1E: DMAx channel 1 transfer error interrupt flag.
  *       	@arg DMA_IT_TE2E: DMAx channel 2 transfer error interrupt flag.
  *       	@arg DMA_IT_TE3E: DMAx channel 3 transfer error interrupt flag.
  *       	@arg DMA_IT_TE4E: DMAx channel 4 transfer error interrupt flag.
  *       	@arg DMA_IT_TE5E: DMAx channel 5 transfer error interrupt flag.
  *       	@arg DMA_IT_TH0E: DMAx channel 0 transfer over half interrupt.
  *       	@arg DMA_IT_TH1E: DMAx channel 1 transfer over half interrupt.
  *       	@arg DMA_IT_TH2E: DMAx channel 2 transfer over half interrupt.
  *       	@arg DMA_IT_TH3E: DMAx channel 3 transfer over half interrupt.
  *       	@arg DMA_IT_TH4E: DMAx channel 4 transfer over half interrupt.
  *       	@arg DMA_IT_TH5E: DMAx channel 5 transfer over half interrupt.
  * @param  NewState: new state of the specified DMAx interrupts.
  *        	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DMA_ITConfig(DMA_TypeDef *DMAx, u32 DMA_IT, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	assert_param(IS_DMA_IT(DMA_IT));

	if(NewState != DISABLE)
	{
		/* Enable the selected DMA interrupt */
		DMAx->IER |= DMA_IT;
	}
	else
	{
		/* Disable the selected DMA interrupt */
		DMAx->IER &= ~DMA_IT;
	}
}

/**
  * @brief 	Clear DMA interrupt flag.
  * @param 	DMA_FLAG: Specify the DMA interrupts flag to be clear.
  *        	This parameter can be one of the following values:
  *        	@arg DMA_FLAG_TC0F: DMAx channel 0 transfer complete interrupt flag.
  *        	@arg DMA_FLAG_TC1F: DMAx channel 1 transfer complete interrupt flag.
  *        	@arg DMA_FLAG_TC2F: DMAx channel 2 transfer complete interrupt flag.
  *        	@arg DMA_FLAG_TC3F: DMAx channel 3 transfer complete interrupt flag.
  *        	@arg DMA_FLAG_TC4F: DMAx channel 4 transfer complete interrupt flag.
  *        	@arg DMA_FLAG_TC5F: DMAx channel 5 transfer complete interrupt flag.
  *        	@arg DMA_FLAG_TE0F: DMAx channel 0 transfer error interrupt flag.
  *        	@arg DMA_FLAG_TE1F: DMAx channel 1 transfer error interrupt flag.
  *        	@arg DMA_FLAG_TE2F: DMAx channel 2 transfer error interrupt flag.
  *        	@arg DMA_FLAG_TE3F: DMAx channel 3 transfer error interrupt flag.
  *        	@arg DMA_FLAG_TE4F: DMAx channel 4 transfer error interrupt flag.
  *        	@arg DMA_FLAG_TE5F: DMAx channel 5 transfer error interrupt flag.
  *        	@arg DMA_FLAG_C0THF: DMAx channel 0 transfer over half interrupt flag.
  *        	@arg DMA_FLAG_C1THF: DMAx channel 1 transfer over half interrupt flag.
  *        	@arg DMA_FLAG_C2THF: DMAx channel 2 transfer over half interrupt flag.
  *        	@arg DMA_FLAG_C3THF: DMAx channel 3 transfer over half interrupt flag.
  *        	@arg DMA_FLAG_C4THF: DMAx channel 4 transfer over half interrupt flag.
  *        	@arg DMA_FLAG_C5THF: DMAx channel 5 transfer over half interrupt flag.
  *        	@arg DMA_FLAG_CEF: DMAx channel configuration error flag.   
  * @retval None
  */
void DMA_ClearITFlag(DMA_TypeDef *DMAx, u32 DMA_FLAG)
{
	/* Check the parameters */
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	assert_param(IS_DMA_FLAG(DMA_FLAG));

	DMAx->SR = DMA_FLAG;
}

/**
  * @brief 	Check whether the specified DMA flag is set or not.
  * @param 	DMA_FLAG: Specify the flag to be checked.
  *       	This parameter can be one of the following values:
  *        	@arg DMA_FLAG_TC0F: DMAx channel 0 transfer complete interrupt flag.
  *        	@arg DMA_FLAG_TC1F: DMAx channel 1 transfer complete interrupt flag.
  *        	@arg DMA_FLAG_TC2F: DMAx channel 2 transfer complete interrupt flag.
  *        	@arg DMA_FLAG_TC3F: DMAx channel 3 transfer complete interrupt flag.
  *        	@arg DMA_FLAG_TC4F: DMAx channel 4 transfer complete interrupt flag.
  *        	@arg DMA_FLAG_TC5F: DMAx channel 5 transfer complete interrupt flag.
  *        	@arg DMA_FLAG_TE0F: DMAx channel 0 transfer error interrupt flag.
  *        	@arg DMA_FLAG_TE1F: DMAx channel 1 transfer error interrupt flag.
  *        	@arg DMA_FLAG_TE2F: DMAx channel 2 transfer error interrupt flag.
  *        	@arg DMA_FLAG_TE3F: DMAx channel 3 transfer error interrupt flag.
  *        	@arg DMA_FLAG_TE4F: DMAx channel 4 transfer error interrupt flag.
  *        	@arg DMA_FLAG_TE5F: DMAx channel 5 transfer error interrupt flag.
  *        	@arg DMA_FLAG_C0THF: DMAx channel 0 transfer over half interrupt flag.
  *        	@arg DMA_FLAG_C1THF: DMAx channel 1 transfer over half interrupt flag.
  *        	@arg DMA_FLAG_C2THF: DMAx channel 2 transfer over half interrupt flag.
  *        	@arg DMA_FLAG_C3THF: DMAx channel 3 transfer over half interrupt flag.
  *        	@arg DMA_FLAG_C4THF: DMAx channel 4 transfer over half interrupt flag.
  *        	@arg DMA_FLAG_C5THF: DMAx channel 5 transfer over half interrupt flag.
  *        	@arg DMA_FLAG_CEF: DMAx channel configuration error flag. 
  * @retval The new state of DMA_FLAG (SET or RESET).
  */
FlagStatus DMA_GetFlagStatus(DMA_TypeDef *DMAx, u32 DMA_FLAG)
{
	/* Check the parameters */
	assert_param(IS_DMA_ALL_PERIPH(DMAx));
	assert_param(IS_DMA_FLAG(DMA_FLAG));

	if((DMAx->SR & DMA_FLAG) != 0)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/**
  * @brief 	Config DMA Peripheral.
  * @param 	DMAx: where x can be 0 to 5 to select the DMA peripheral.
  * @param 	ch: select channel number.
  *         This parameter can be one of the following values:
  *       	@arg DMA_CHNUM_0: DMA channel 0
  *       	@arg DMA_CHNUM_1: DMA channel 1
  *       	@arg DMA_CHNUM_2: DMA channel 2
  *       	@arg DMA_CHNUM_3: DMA channel 3
  *       	@arg DMA_CHNUM_4: DMA channel 4
  *       	@arg DMA_CHNUM_5: DMA channel 5
  * @param 	Peripheral: DMA CH Relevance Peripheral.
  *        	This parameter can be one of the following values:
  *        	@arg IS_DMA_ChannelPERIPH(x): The value of x that satisfies the condition.
  * @retval None
  */
void DMA_PeripheralConfig(DMA_TypeDef *DMAx, DMA_ChannelNum ch, u32 Peripheral)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_DMA_ALL_PERIPH(DMAx));	
	assert_param(IS_DMA_ChannelNum(ch));
	assert_param(IS_DMA_ChannelPERIPH(Peripheral));
	/* Clear the selected channelx bits */	
	tmpreg = DMAx->CHAPCR[ch >> 2] & ~(DMA_CHAPCR_CH_SEL << ((ch & 0x03) * 8));
	/* Write to CHAPCRx register*/
	tmpreg |= Peripheral << ((ch & 0x03) * 8);
	DMAx->CHAPCR[ch >> 2] = tmpreg;
}
/**
  * @}
  */
