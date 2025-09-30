/**
	*******************************************************************************
  * @file    PT32x0xx_spic.c
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file provides firmware functions to manage the following
	*          functionalities of the SPI peripheral:
	*           + Initialization and Configuration
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.  
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of 
  * the content or the codes within the content. 
  * 
  *  Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  *******************************************************************************
**/

#include "PT32x0xx_spi.h"

/** @defgroup SPI
  * @brief SPI driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the SPIx peripheral according to
  *         the specified parameters in the SPI_InitTypeDef.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @param  SPI_InitStruct: pointer to a SPI_InitTypeDef
  *         structure that contains the configuration information for
  *         the specified SPI peripheral.
  * @retval None
  */
void SPI_Init(SPI_TypeDef *SPIx, SPI_InitTypeDef *SPI_InitStruct)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_SPI_MasterSlaveMode(SPI_InitStruct->SPI_MasterSlaveMode));
	assert_param(IS_SPI_DataFrameFormat(SPI_InitStruct->SPI_DataFrameFormat));
	assert_param(IS_SPI_ClockPolarity(SPI_InitStruct->SPI_ClockPolarity));
	assert_param(IS_SPI_ClockPhase(SPI_InitStruct->SPI_ClockPhase));
	assert_param(IS_SPI_CSS(SPI_InitStruct->SPI_CSS));
	assert_param(IS_SPI_SoftwareControlCSS(SPI_InitStruct->SPI_SoftwareControlCSS));
	assert_param(IS_SPI_Prescaler(SPI_InitStruct->SPI_Prescaler));
	assert_param(IS_SPI_BaudRate(SPI_InitStruct->SPI_BaudRate));
	
	SPIx->DRR = SPI_Reset_RXFR;
	SPIx->DRR = SPI_Reset_TXFR;
	
	/* Get the SPIx CR1 value */
	/* Get the SPIx CR1 value and clear CSS/DFF/CPOL/CPHA/MSM bits */
	tmpreg = SPIx->CR1 & ~(SPI_CR1_CSS | SPI_CR1_DFF | SPI_CR1_CPOL | SPI_CR1_CPHA | SPI_CR1_MSM);
	/* Configure SPIx:	master-slave mode, data frame format, clock polarity, clock phase and CSS control selection */
	/* Set MSM bit according to SPI_MasterSlaveMode value */
	/* Set DFF bits according to SPI_DataFrameFormat value */ 
	/* Set CPOL bits according to SPI_ClockPolarity value */
	/* Set CPHA bit according to SPI_ClockPhase value */
	/* Set CSS bit according to SPI_CSS value */
	tmpreg |= SPI_InitStruct->SPI_MasterSlaveMode | SPI_InitStruct->SPI_DataFrameFormat | \
				SPI_InitStruct->SPI_ClockPolarity | SPI_InitStruct->SPI_ClockPhase | SPI_InitStruct->SPI_CSS;
	/* Write to CR1 register*/
	SPIx->CR1 = tmpreg;
	/* Configure SPIx: software control CSS */
	/* Set SWCS bit according to SPI_SoftwareControlCSS value */
	SPIx->CR2 = SPI_InitStruct->SPI_SoftwareControlCSS;
	
	/* Get the SPIx BR value, Clear BR/PSC bits*/
	tmpreg = SPIx->BR & ~(SPI_BR_BR | SPI_BR_PSC);
	/* Configure SPIx:	prescaler and baud rate control */
	/* Set BR bit according to SPI_BaudRate value */
	/* Set PSC bit according to SPI_BaudRate value */
	tmpreg |= SPI_InitStruct->SPI_BaudRate | (SPI_InitStruct->SPI_Prescaler - 1) << 16;
	/* Write to BR register*/
	SPIx->BR = tmpreg;
}

/**
  * @brief  Fills each SPI_InitStruct member with its default value.
  * @param  SPI_InitStruct: pointer to a SPI_InitTypeDef structure which will be initialized.
  * @retval None
  */
void SPI_StructInit(SPI_InitTypeDef *SPI_InitStruct)
{
	/* Initialize the SPI_MasterSlaveMode member */
	SPI_InitStruct->SPI_MasterSlaveMode = SPI_MasterSlaveMode_Master;
	/* Initialize the SPI_DataFrameFormat member */
	SPI_InitStruct->SPI_DataFrameFormat = SPI_DataFrameFormat_8b;
	/* Initialize the SPI_ClockPolarity member */
	SPI_InitStruct->SPI_ClockPolarity = SPI_ClockPolarity_Low;
	/* Initialize the SPI_ClockPhase member */
	SPI_InitStruct->SPI_ClockPhase = SPI_ClockPhase_1Edge;
	/* Initialize the SPI_ChipSelectSignal member */
	SPI_InitStruct->SPI_CSS = SPI_CSS_HardwareControl;
	/* Initialize the SPI_SWCS member */
	SPI_InitStruct->SPI_SoftwareControlCSS = SPI_SoftwareControlCSS_High;
	/* Initialize the SPI_PrescalerValue member */
	SPI_InitStruct->SPI_Prescaler = 2;
	/* Initialize the SPI_BaudRate member */
	SPI_InitStruct->SPI_BaudRate = 16;
}

/**
  * @brief  Enable or disable the specified SPI peripheral.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @param  NewState: new state of the SPIx peripheral.
  *        	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_Cmd(SPI_TypeDef *SPIx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the selected SPI peripheral */
		SPIx->CR1 |= SPI_CR1_EN;
	}
	else
	{
		/* Disable the selected SPI peripheral */
		SPIx->CR1 &= ~SPI_CR1_EN;
	}
}

/**
  * @brief  Enable or disable the specified SPI peripheral salve output in slave mode.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @param  NewState: new state of the SPIx peripheral.
  *       	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_SlaveOutputConfig(SPI_TypeDef *SPIx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the SPI slave output */
		SPIx->CR1 |= SPI_CR1_SOD;
	}
	else
	{
		/* Disable the SPI slave output */
		SPIx->CR1 &= ~SPI_CR1_SOD;
	}
}

/**
  * @brief  Configure the FrameFormat for the specified SPI peripheral.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @param  Format: new Format of the SPIx peripheral.
  *        	This parameter can be one of the following values:
  *			@arg SPI_FrameFormat_MSB: the highest bit is in the front.
  *			@arg SPI_FrameFormat_LSB: the lowest point is in the front.
  * @retval None
  */
void SPI_FrameFormatConfig(SPI_TypeDef *SPIx, u16 Format)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_SPI_FrameFormat(Format));

	tmpreg = SPIx->CR1 & ~SPI_CR1_FF;
	tmpreg |= Format;
	SPIx->CR1 = tmpreg;
}

/**
  * @brief  Enable or disable DMA receive request for the specified SPI peripheral.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @param  NewState: new state of the SPIx peripheral.
  *       	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_ReceiveDMACmd(SPI_TypeDef *SPIx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the SPI Rx DMA request */
		SPIx->CR1 |= SPI_CR1_RXDMAE;
	}
	else
	{
		/* Disable the SPI Rx DMA request */
		SPIx->CR1 &= ~SPI_CR1_RXDMAE;
	}
}

/**
  * @brief  Enable or disable DMA transfer request for the specified SPI peripheral.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @param  NewState: new state of the SPIx peripheral.
  *       	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_TransferDMACmd(SPI_TypeDef *SPIx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if(NewState != DISABLE)
	{
		/* Enable the SPI Tx DMA request */
		SPIx->CR1 |= SPI_CR1_TXDMAE;
	}
	else
	{
		/* Disable the SPI Tx DMA request */
		SPIx->CR1 &= ~SPI_CR1_TXDMAE;
	}
}

/**
  * @brief  Configure the DMA trigger condition for the specified SPI peripheral.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @param  DMA_TriggerControl: specifies the selection of DMA trigger condition.
  *        	This parameter can be one of the following values:
  *        	@arg SPI_DMA_TriggerControl_1: Set DMA trigger condition as 1 frame.
  *        	@arg SPI_DMA_TriggerControl_4: Set DMA trigger condition as 4 frame.
  * @retval None
  */
void SPI_DMATriggerControlConfig(SPI_TypeDef *SPIx, u32 DMA_TriggerControl)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_SPI_DMATrigger(DMA_TriggerControl));
	/* Get the SPIx CR1 value */
	/* Clear DMATC bits */
	tmpreg = SPIx->CR1 & ~SPI_CR1_DMATC;
	/* Set DMA trigger condition as 1 frame */
	tmpreg |= DMA_TriggerControl;
	/* Write to CR1 register*/
	SPIx->CR1 = tmpreg;
}

/**
  * @brief  Configure the CS pin by software for the specified SPI.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @param  SWCS_Signal: specifies the selection of SPI CS mode.
  *       	This parameter can be one of the following values:
  *        	@arg SPI_SoftwareControlCSS_Low: Reset CS pin internally.
  *        	@arg SPI_SoftwareControlCSS_High: Set CS pin internally.
  * @retval None
  */
void SPI_SoftwareControlCSSConfig(SPI_TypeDef *SPIx, u32 SWCS_Signal)
{
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_SPI_SoftwareControlCSS(SWCS_Signal));

	/* Wait for status register not busy */
	while(SPIx->SR & SPI_SR_BSY);
	if(SWCS_Signal != SPI_SoftwareControlCSS_Low)
	{
		/* Set NSS pin internally by software */
		SPIx->CR2 = SPI_SoftwareControlCSS_High;
	}
	else
	{
		/* Reset NSS pin internally by software */
		SPIx->CR2 = SPI_SoftwareControlCSS_Low;
	}
}

/**
  * @brief  Configure the baudrate for the specified SPI peripheral.
  * @param  SPIx: where x can be from 0 or 1 to select the SPI peripheral.
  * @param  BaudRate: baudrate for SPI peripheral.
  * @param  Prescaler: Prescaler can be from 1 to 256 for SPI clock.
  * @retval None
  */
void SPI_BaudRateConfig(SPI_TypeDef *SPIx, u32 BaudRate, u32 Prescaler)
{
	u32 clk_source, tmpvalue;

	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_SPI_Prescaler(Prescaler));

	clk_source = GetClockFreq(CLKSRC_PCLK)/Prescaler;
	tmpvalue = (clk_source + BaudRate/2) / BaudRate;
	
	if(tmpvalue <= 2)
	{
		tmpvalue = 2;
	}
	else if(tmpvalue > 254)
	{
		tmpvalue = 254;
	}

	SPIx->BR = tmpvalue | ((Prescaler - 1) << 16);
}

/**
  * @brief  Enable or disable the specified SPI interrupts.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral
  * @param  SPI_IT: specifies the SPI interrupt source to be enabled or disabled.
  *       	This parameter can be one of the following values:
  *       	@arg SPI_IT_RXNE: Receive/Receive-FIFO not empty interrupt.
  *        	@arg SPI_IT_RXHF: Receive FIFO half full interrupt.
  *        	@arg SPI_IT_OVR: Receive/Receive-FIFO overflow interrupt.
  *        	@arg SPI_IT_RXTO: Receive/Receive-FIFO clear timeout interrupt.
  *       	@arg SPI_IT_TXE: Transmit/Transmit-FIFO empty interrupt enable.
  *        	@arg SPI_IT_TXHE: Transmit FIFO half empty interrupt enable.
  * @param  NewState: new state of the specified SPI interrupt.
  *       	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_ITConfig(SPI_TypeDef *SPIx, u8 SPI_IT, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	assert_param(IS_SPI_IT(SPI_IT));

	if(NewState != DISABLE)
	{
		/* Enable the selected SPI interrupt */
		SPIx->IER |= SPI_IT;
	}
	else
	{
		/* Disable the selected SPI interrupt */
		SPIx->IER &= ~SPI_IT;
	}
}

/**
  * @brief  Check whether the specified SPI flag is set or not.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral
  * @param  SPI_FLAG: specifies the SPI flag to check.
  *        	This parameter can be one of the following values:
  *        	@arg SPI_FLAG_RXNE: Receive/Receive-FIFO not empty flag.
  *        	@arg SPI_FLAG_RXHF: Receive FIFO half full flag.
  *         @arg SPI_FLAG_RXF: Receive FIFO full flag.
  *         @arg SPI_FLAG_OVR: Receive/Receive-FIFO overflow flag.
  *         @arg SPI_FLAG_RXTO: Receive/Receive-FIFO clear timeout flag.
  *         @arg SPI_FLAG_TXE: Transmit/Transmit-FIFO empty flag.
  *         @arg SPI_FLAG_TXHE: Transmit FIFO half empty flag.
  *         @arg SPI_FLAG_TNF: Transmit FIFO is not full flag.
  *         @arg SPI_FLAG_BSY: busy flag.
  * @retval The new state of SPI_FLAG (SET or RESET).
  */
FlagStatus SPI_GetFlagStatus(SPI_TypeDef *SPIx, u16 SPI_FLAG)
{
	FlagStatus bitstatus;
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_SPI_FLAG(SPI_FLAG));

	/* Check the status of the specified SPI flag */
	if((SPIx->SR & SPI_FLAG) != 0)
	{
		bitstatus = SET;
	}
	else
	{
		bitstatus = RESET;
	}
	
	return  bitstatus;
}

/**
  * @brief  Clear the SPIx status flag.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral
  * @param  SPI_FLAG: Specify the status flag to be cleared.
  *         This parameter can be one of the following values:
  *         @arg SPI_FLAG_RXNE: Receive/Receive-FIFO not empty flag.
  *         @arg SPI_FLAG_RXHF: Receive FIFO half full flag.
  *         @arg SPI_FLAG_RXF: Receive FIFO full flag.
  *         @arg SPI_FLAG_OVR: Receive/Receive-FIFO overflow flag.
  *         @arg SPI_FLAG_RXTO: Receive/Receive-FIFO clear timeout flag.
  *         @arg SPI_FLAG_TXE: Transmit/Transmit-FIFO empty flag.
  *         @arg SPI_FLAG_TXHE: Transmit FIFO half empty flag.
  *         @arg SPI_FLAG_TNF: Transmit FIFO is not full flag.
  *         @arg SPI_FLAG_BSY: busy flag.
  * @retval None
  */
void SPI_ClearFlag(SPI_TypeDef *SPIx, u16 SPI_FLAG)
{
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_SPI_FLAG(SPI_FLAG));
		
	if(SPI_FLAG & SPI_FLAG_OVR)
	{
		SPIx->SR = SPI_IT_OVR;
	}

	if(SPI_FLAG & SPI_FLAG_RXTO)
	{
		SPIx->SR = SPI_IT_RXTO;
	}
}

/**
  * @brief  Get the number of datas in SPI receive and transmit FIFOs.
  * @param  SPIx:  where x can be 0 or 1 to select the SPI peripheral.
  * @param  FIFO: Transmit FIFO or receive FIFO for the SPI.
  *        	This parameter can be:
  *        	@arg SPI_FIFO_RX: select SPI receive FIFO.
  *        	@arg SPI_FIFO_TX: select SPI transmit FIFO.
  * @retval	num: the number of datas in SPI FIFOs.
  */
u8 SPI_GetFifoSize(SPI_TypeDef *SPIx, u8 SPI_FIFO)
{
	u8 num;
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_SPI_FIFO(SPI_FIFO));

	if(SPI_FIFO == SPI_FIFO_RX)
	{
		num = (SPIx->SR & SPI_SR_RXFS) >> 16;
	}
	else
	{
		num = (SPIx->SR & SPI_SR_TXFS) >> 24;
	}

	return num;
}

/**
  * @brief  Transmit a Data through the SPIx peripheral.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @param  Data: the data to be transmitted.
  * @retval None
  */
void SPI_SendData(SPI_TypeDef *SPIx, u16 Data)
{
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	/* Write in the DR register the data to be sent */
	SPIx->DR = Data;
}

/**
  * @brief  Return the most recent received data by the SPIx peripheral.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @retval The value of the received data.
  */
u16 SPI_ReceiveData(SPI_TypeDef *SPIx)
{
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	/* Return the data in the DR register */
	return (u16)SPIx->DR;
}

/**
  * @brief  Reset the receive or transmit FIFOs for the specified SPI peripheral.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @param  FIFO: Transmit FIFO or receive FIFO for the SPI.
  *        	This parameter can be:
  *        	@arg SPI_FIFO_RX: select SPI receive FIFO.
  *        	@arg SPI_FIFO_TX: select SPI transmit FIFO.
  * @retval None.
  */
void SPI_FifoReset(SPI_TypeDef *SPIx, u8 SPI_FIFO)
{
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_SPI_FIFO(SPI_FIFO));

	if(SPI_FIFO == SPI_FIFO_RX)
	{
		SPIx->DRR = SPI_Reset_RXFR;
	}
	else
	{
		SPIx->DRR = SPI_Reset_TXFR;
	}
}

/**
  * @}
  */
