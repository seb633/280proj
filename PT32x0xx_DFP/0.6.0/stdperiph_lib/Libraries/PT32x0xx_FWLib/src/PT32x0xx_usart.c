/**
	****************************************************************************
  * @file    PT32x0xx_USART.c
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the USART firmware
  *          library
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of
  * the content or the codes within the content.
  *
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  *****************************************************************************
**/

#include "PT32x0xx_usart.h"

/** @defgroup USART
  * @brief USART driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the USARTx peripheral according to the specified
  *         parameters in the USART_InitStruct .
  * @param  USARTx: where x can be 0 select the USART peripheral.
  * @param  USART_InitStruct: pointer to a USART_Sync_InitTypeDef structure that contains
  *         the configuration information for the specified USART peripheral.
  * @retval None
  */
void USART_Sync_Init(USART_TypeDef *USARTx, USART_Sync_InitTypeDef *USART_InitStruct)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_USART_Sync_MasterSlaveMode(USART_InitStruct->USART_MasterSlaveMode));
	assert_param(IS_USART_Sync_BaudRate(USART_InitStruct->USART_BaudRate));
	assert_param(IS_USART_Sync_FrameFormat(USART_InitStruct->USART_FrameFormat));
	assert_param(IS_USART_Sync_ClockPolarity(USART_InitStruct->USART_ClockPolarity));
	assert_param(IS_USART_Sync_ClockPhase(USART_InitStruct->USART_ClockPhase));
	assert_param(IS_USART_Receiver(USART_InitStruct->USART_Receiver));

	tmpreg = USARTx->CR1 & ~(USART_CR1_RE | USART_CR1_CPOL | USART_CR1_CPHA | USART_CR1_MODE | USART_CR1_MSM | USART_CR1_FF);
	tmpreg |= USART_InitStruct->USART_MasterSlaveMode | USART_InitStruct->USART_FrameFormat |
			   USART_InitStruct->USART_ClockPhase | USART_InitStruct->USART_ClockPolarity |
			   USART_InitStruct->USART_Receiver | USART_ModeSync;
	USARTx->CR1 = tmpreg;

	USARTx->BRR = USART_InitStruct->USART_BaudRate;
}

/**
  * @brief  Fills each USART_InitStruct member with its default value.
  * @param  USART_InitStruct: pointer to a USART_Sync_InitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void USART_Sync_InitStruct(USART_Sync_InitTypeDef *USART_InitStruct)
{
	/* USART_InitStruct members default value */
	USART_InitStruct->USART_BaudRate = 2;
	USART_InitStruct->USART_MasterSlaveMode = USART_MasterSlaveMode_Master;
	USART_InitStruct->USART_FrameFormat = USART_FrameFormat_MSB;
	USART_InitStruct->USART_ClockPolarity = USART_ClockPolarity_Low;
	USART_InitStruct->USART_ClockPhase = USART_ClockPhase_1Edge;
	USART_InitStruct->USART_Receiver = USART_Receiver_Enable;
}

/**
  * @brief  Initializes the USARTx peripheral according to the specified
  *         parameters in the USART_InitStruct .
  * @param  USARTx: where x can be 0 select the USART peripheral.
  * @param  USART_InitStruct: pointer to a USART_Async_InitTypeDef structure that contains
  *         the configuration information for the specified USART peripheral.
  * @retval None
  */
void USART_Async_Init(USART_TypeDef *USARTx, USART_Async_InitTypeDef *USART_InitStruct)
{
	u32 tmpreg, WP;
	
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_USART_Async_BaudRate(USART_InitStruct->USART_BaudRate));
	assert_param(IS_USART_Async_SampleRate(USART_InitStruct->USART_SampleRate));
	assert_param(IS_USART_Async_DataLength(USART_InitStruct->USART_DataLength));
	assert_param(IS_USART_Async_StopBitLength(USART_InitStruct->USART_StopBitLength));
	assert_param(IS_USART_Async_ParityMode(USART_InitStruct->USART_ParityMode));
	assert_param(IS_USART_Receiver(USART_InitStruct->USART_Receiver));
	assert_param(!IS_USART_Async_ErrorDataLengthAndParity(USART_InitStruct->USART_DataLength, USART_InitStruct->USART_ParityMode));

	switch(USART_InitStruct ->USART_DataLength)
	{
		case USART_DataLength_7:
		  if(USART_InitStruct->USART_ParityMode == USART_Parity_Odd)
			{
				WP = USART_WordLengthAndParity_7DO;
			}
			  else
			{
				WP = USART_WordLengthAndParity_7DE;
			}
			break;
		case USART_DataLength_8:
			if(USART_InitStruct->USART_ParityMode == USART_Parity_None)
			{
				WP = USART_WordLengthAndParity_8D;
			}
			else if(USART_InitStruct->USART_ParityMode == USART_Parity_Odd)
			{
				WP = USART_WordLengthAndParity_8DO;
			}
			else
			{
				WP = USART_WordLengthAndParity_8DE;
			}
			break;
		case USART_DataLength_9:
			WP = USART_WordLengthAndParity_9D;
			break;
		default:
			return ;
	}
	USART_Async_BaudRateConfig(USARTx, USART_InitStruct->USART_BaudRate, USART_InitStruct->USART_SampleRate);

	tmpreg = USARTx->CR1 & ~(USART_CR1_RE | USART_CR1_WP | USART_CR1_MODE | USART_CR1_STOP);
	tmpreg |= USART_InitStruct->USART_Receiver | WP | USART_ModeAsync | USART_InitStruct ->USART_StopBitLength;
	USARTx->CR1 = tmpreg;
}

/**
  * @brief  Fills each USART_InitStruct member with its default value.
  * @param  USART_InitStruct: pointer to a USART_Async_InitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void USART_Async_StructInit(USART_Async_InitTypeDef *USART_InitStruct)
{
	/* USART_InitStruct members default value */
	USART_InitStruct->USART_BaudRate = 19200;
	USART_InitStruct->USART_SampleRate = USART_SampleRate_32X;
	USART_InitStruct->USART_DataLength = USART_DataLength_8;
	USART_InitStruct->USART_StopBitLength = USART_StopBitLength_1;
	USART_InitStruct->USART_ParityMode = USART_Parity_None;
	USART_InitStruct->USART_Receiver = USART_Receiver_Enable;
}

/**
  * @brief  Enables or disables the specified USART peripheral.
  * @param  USARTx: where x can be 0 to select the USART peripheral.
  * @param  NewState: new state of the USARTx peripheral.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_Cmd(USART_TypeDef *USARTx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the selected USART by setting the EN bit in the CR1 register */
		USARTx->CR1 |= USART_CR1_EN;
	}
	else
	{
		/* Disable the selected USART by clearing the EN bit in the CR1 register */
		USARTx->CR1 &= ~USART_CR1_EN;
	}
}

/**
  * @brief  Enables or disables the specified USART Receiver enable.
  * @param  USARTx: where x can be 0 to select the USART peripheral.
  * @param  NewState: new state of the USARTx peripheral.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_ReceiverCmd(USART_TypeDef *USARTx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the selected USART by setting the RE bit in the CR1 register */
		USARTx->CR1 |= USART_CR1_RE;
	}
	else
	{
		/* Disable the selected USART by clearing the RE bit in the CR1 register */
		USARTx->CR1 &= ~USART_CR1_RE;
	}
}

/**
  * @brief  Enables or disables the USART's receive request of the specified USART peripheral.
  * @param  USARTx: where x can be 0 to select the USART peripheral.
  * @param  NewState: new state of the USARTx peripheral.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_ReceiveDMACmd(USART_TypeDef *USARTx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

#if defined(PT32x012x) || defined (PTM160x)
	return ;
#else
	if(NewState != DISABLE)
	{
		/* Enable the selected USART by setting the RXDMAE bit in the CR1 register */
		USARTx->CR1 |= USART_RXDMAE_Enable;
	}
	else
	{
		/* Disable the selected UART by clearing the RXDMAE bit in the CR1 register */
		USARTx->CR1 &= ~USART_RXDMAE_Enable;
	}
#endif
}

/**
  * @brief  Enables or disables DMA transfer request of the specified USART peripheral.
  * @param  USARTx: where x can be 0 to select the USART peripheral.
  * @param  NewState: new state of the USARTx peripheral.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_TransferDMACmd(USART_TypeDef *USARTx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
#if defined(PT32x012x) || defined (PTM160x)
	return ;
#else
	if(NewState != DISABLE)
	{
		USARTx->CR1 |= USART_TXDMAE_Enable;
	}
	else
	{
		USARTx->CR1 &= ~USART_TXDMAE_Enable;
	}
#endif
}

/**
  * @brief  Clock polarity config of USARTx peripheral.
  * @param  USARTx: where x can be 0 to select the USART peripheral.
  * @param  ClockPolarity: specifies the clock polarity.
  *          This parameter can be any combination of the following values:
  *            @arg USART_ClockPolarity_Low: SCK is held low in the idle state
  *            @arg USART_ClockPolarity_High: SCK is held high in the idle state
  * @retval None
  */
void USART_Sync_ClockPolarityConfig(USART_TypeDef *USARTx, u32 ClockPolarity)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_USART_Sync_ClockPolarity(ClockPolarity));

	tmpreg = USARTx->CR1 & ~USART_CR1_CPOL;
	tmpreg |= ClockPolarity;
	USARTx->CR1 = tmpreg;
}

/**
  * @brief  Clock phase config of USARTx peripheral.
  * @param  USARTx: where x can be 0 to select the USART peripheral.
  * @param  ClockPolarity: specifies the clock phase.
  *          This parameter can be any combination of the following values:
  *            @arg USART_ClockPhase_1Edge: Data sampling from the first clock edge
  *            @arg USART_ClockPhase_2Edge: Data sampling from the second clock edge
  * @retval None
  */
void USART_Sync_ClockPhaseConfig(USART_TypeDef *USARTx, u32 ClockPhase)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_USART_Sync_ClockPhase(ClockPhase));

	tmpreg = USARTx->CR1 & ~USART_CR1_CPHA;
	tmpreg |= ClockPhase;
	USARTx->CR1 = tmpreg;
}

/**
  * @brief  Word length and parity config of USARTx peripheral.
  * @param  USARTx: where x can be 0 to select the USART peripheral.
  * @param  WordLenthAndParity: specifies the Word length and parity.
  *          This parameter can be any combination of the following values:
  *            @arg USART_WordLengthAndParity_7DE: 8-bit data and 1-bit even parity
  *            @arg USART_WordLengthAndParity_7DO: 8-bit data and 1-bit odd parity
  *            @arg USART_WordLengthAndParity_8D: 8-bit data format without checksum
  *            @arg USART_WordLengthAndParity_8W: 8-bit data format with wakeup
  *            @arg USART_WordLengthAndParity_8DE: 8-bit data and 1-bit even parity
  *            @arg USART_WordLengthAndParity_8DO: 8-bit data and 1-bit odd parity
  *            @arg USART_WordLengthAndParity_9D: 9-bit data format without checksum
  * @retval None
  */
void USART_Async_WordLengthAndParityConfig(USART_TypeDef *USARTx, u32 WordLenthAndParity)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_USART_Aync_WordLengthAndParity(WordLenthAndParity));

	tmpreg = USARTx->CR1 & ~USART_CR1_WP;
	tmpreg |= WordLenthAndParity;
	USARTx->CR1 = tmpreg;
}

/**
  * @brief  Stop bit length config of USARTx peripheral.
  * @param  USARTx: where x can be 0 to select the USART peripheral.
  * @param  StopBitLenth: specifies the Stop bit length.
  *          This parameter can be any combination of the following values:
  *            @arg USART_StopBitLength_1: 1 bit Stop Length
  *            @arg USART_StopBitLength_2: 2 bit Stop Length
  *            @arg USART_StopBitLength_0P5: 0.5 bit Stop Length
  *            @arg USART_StopBitLength_1P5: 1.5 bit Stop Length
  * @retval None
  */
void USART_Async_StopBitLengthConfig(USART_TypeDef *USARTx, u32 StopBitLenth)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_USART_Async_StopBitLength(StopBitLenth));

	tmpreg = USARTx->CR1 & ~USART_CR1_STOP;
	tmpreg |= StopBitLenth;
	USARTx->CR1 = tmpreg;
}

/**
  * @brief  DMA trigger control config of USARTx peripheral.
  * @param  USARTx: where x can be 0 to select the USART peripheral.
  * @param  DMATiggerControl:
  *          This parameter can be any combination of the following values:
  *            @arg USART_Trigger_0: FIFO �� 1 frame of data, trigger DMA request
  *            @arg USART_Trigger_1: FIFO �� 4 frame of data, trigger DMA request
  *            @arg USART_Trigger_2: FIFO �� 8 frame of data, trigger DMA request
  * @retval None
  */
void USART_DMATriggerControlConfig(USART_TypeDef *USARTx, u32 DMATiggerControl)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_USART_DMATriggerControl(DMATiggerControl));

	tmpreg = USARTx->CR1 & ~USART_CR1_DMATC;
	tmpreg |= DMATiggerControl;
	USARTx->CR1 = tmpreg;
}

/**
  * @brief  Enables or disables the USART's Single-line mode.
  * @param  USARTx: where x can be 0 to select the USART peripheral.
  * @param  NewState: new state of the USART Communication.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_Async_SingleLineModeCmd(USART_TypeDef *USARTx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the Half-Duplex mode by setting the SLME bit in the CR1 register */
		USARTx->CR1 |= USART_CR1_SLME;
	}
	else
	{
		/* Disable the Half-Duplex mode by clearing the SLME bit in the CR1 register */
		USARTx->CR1 &= ~USART_CR1_SLME;
	}
}

/**
  * @brief  Enables or disables the USART's transmitter or receiver.
  * @param  USARTx: where x can be 0 to select the USART peripheral.
  * @param  USART_Direction: specifies the USART direction.
  *          This parameter can be any combination of the following values:
  *            @arg USARTAsync_SingleLineDirection_Tx: USART Single Line Direction Transmitter
  *            @arg USARTAsync_SingleLineDirection_Rx: USART Single Line Direction Receiver
  * @param  NewState: new state of the USART transfer direction.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_Async_SinglelineDirectionConfig(USART_TypeDef *USARTx, u32 Direction)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_USART_Async_SingleLineDirection(Direction));

	tmpreg = USARTx->CR1 & ~USART_CR1_SLDS;
	tmpreg |= Direction;
	USARTx->CR1 = tmpreg;
}

/**
  * @brief  Enables or disables the USART's Loopback mode.
  * @param  USARTx: where x can be 0 to select the USART peripheral.
  * @param  NewState: new state of the USART Communication.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_LoopbackModeCmd(USART_TypeDef *USARTx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		USARTx->CR1 |= USART_CR1_LBM;
	}
	else
	{
		USARTx->CR1 &= ~USART_CR1_LBM;
	}
}

/**
  * @brief  Sets baudrate for USART peripheral.
  * @param  USARTx: where x can be 0 to select the USART peripheral.
  * @param  BaudRate: baudrate for USART peripheral.
  * @retval None
  */
void USART_Async_BaudRateConfig(USART_TypeDef *USARTx, u32 BaudRate, u32 SampleRate)
{
	u32 quotient, clk_frequency, tmpvalue;
	
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_USART_Async_SampleRate(SampleRate));

	tmpvalue = BaudRate << (SampleRate >> 16);

	clk_frequency = GetClockFreq(CLKSRC_PCLK);
	if(clk_frequency > tmpvalue)
	{
		quotient = (clk_frequency + tmpvalue/2) / tmpvalue;
		USARTx->BRR = quotient | SampleRate;
	}
	else
	{
		USARTx->BRR = 0x01 | SampleRate;
	}
}

/**
  * @brief  Enables or disables the specified USART interrupts.
  * @param  USARTx: where x can be 0 to select the USART peripheral.
  * @param  USART_IT: specifies the USART interrupt sources to be enabled or disabled.
  *        @arg IS_USART_IT(x)  The value of x that satisfies the condition
  * @param  NewState: new state of the specified USARTx interrupts.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_ITConfig(USART_TypeDef *USARTx, u32 USART_IT, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_USART_IT(USART_IT));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		USARTx->IER |= USART_IT;
	}
	else
	{
		USARTx->IER &= ~USART_IT;
	}
}

/**
  * @brief  Checks whether the specified USART flag is set or not.
  * @param  USARTx: where x can be 0 to select the USART peripheral.
  * @param  USART_FLAG: specifies the flag to check.
  *        @arg IS_USART_FLAG(x)  The value of x that satisfies the condition
  * @retval The new state of USART_FLAG (SET or RESET).
  */
FlagStatus USART_GetFlagStatus(USART_TypeDef *USARTx, u32 USART_FLAG)
{
	FlagStatus bitstatus;
	
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_USART_FLAG(USART_FLAG));
	
	if((USARTx->SR & USART_FLAG) != 0)
	{
		bitstatus = SET;
	}
	else
	{
		bitstatus = RESET;
	}
	
	return bitstatus;
}

/**
  * @brief  Clears the USARTx's pending flags.
  * @param  USARTx: where x can be 0 to select the USART peripheral.
  * @param  USART_FLAG: specifies the flag to clear.
  *          This parameter can be any combination of the following values:
  *            @arg USART_FLAG_RXNE:  Receive/Receive-FIFO not empty flag.
  *            @arg USART_FLAG_IDLE:  Idle frame detected flag.
  *            @arg USART_FLAG_PE: Parity error flag.
  *            @arg USART_FLAG_FE: Frame error flag.
  *            @arg USART_FLAG_TXE: Transmit/Transmit-FIFO empty flag.
  *            @arg USART_FLAG_TXC: Transmit complete flag.
  * @retval None
  */
void USART_ClearFlag(USART_TypeDef *USARTx, u32 USART_FLAG)
{
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_USART_FLAG(USART_FLAG));

	USARTx->SR = USART_FLAG;
}

/**
  * @brief  Checks whether the specified USART intterrupt status is set or not.
  * @param  USARTx: where x can be 0 to select the USART peripheral.
  * @param  USART_IT: specifies the USART interrupt sources to be enabled or disabled.
  *        @arg IS_USART_IT(x)  The value of x that satisfies the condition
  * @return ITStatus of USART_IT (SET or RESET).
  */
ITStatus USART_GetITStatus(USART_TypeDef *USARTx, u32 USART_IT)
{
	ITStatus bitstatus;
	u32 enablestatus;
	
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_USART_IT(USART_IT));

	enablestatus = USARTx->IER & USART_IT;

	/* Check the status of the specified USART flag */
	if((USARTx->SR & USART_IT) != 0 && enablestatus != 0)
	{
		/* USART_FLAG is set */
		bitstatus = SET;
	}
	else
	{
		/* USART_FLAG is reset */
		bitstatus = RESET;
	}
	/* Return the USART_FLAG status */
	return  bitstatus;
}

/**
  * @brief  Transmits single data through the USARTx peripheral in synchronous mode.
  * @param  USARTx: where x can be 0 to select the USART peripheral.
  * @param  Data: the data to transmit.
  * @retval None
  */
void USART_Sync_SendData(USART_TypeDef *USARTx, u16 Data)
{
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_USART_Sync_Data(Data));

	while(!(USARTx->SR & USART_SR_TXE));
	/* Transmit Data */
	USARTx->DR = Data & 0xFF;
}

/**
  * @brief  Returns the most recent received data by the USARTx peripheral in synchronous mode.
  * @param  USARTx: where x can be 0 to select the USART peripheral.
  * @retval The received data.
  */
u16 USART_Sync_ReceiveData(USART_TypeDef *USARTx)
{
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	/* Receive Data */
	return (USARTx->DR & 0xFF);
}

/**
  * @brief  Transmits single data through the USARTx peripheral in asynchronous mode.
  * @param  USARTx: where x can be 0 to select the USART peripheral.
  * @param  Data: the data to transmit.
  * @retval None
  */
void USART_Async_SendData(USART_TypeDef *USARTx, u16 Data)
{
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_USART_Async_Data(Data));

	while(!(USARTx->SR & USART_SR_TXE));
	/* Transmit Data */
	USARTx->DR = Data & 0x1FF;
}

/**
  * @brief  Returns the most recent received data by the USARTx peripheral in asynchronous mode.
  * @param  USARTx: where x can be 0 to select the USART peripheral.
  * @retval The received data.
  */
u16 USART_Async_ReceiveData(USART_TypeDef *USARTx)
{
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	/* Receive Data */
	return (USARTx->DR & 0x1FF);
}

/**
  * @brief  Transmits SCK clocks through the USARTx peripheral in synchronous mode.
  * @param  USARTx: where x can be 0 to select the USART peripheral.
  * @param  Count: the count of SCK clocks.
  * @retval None
  */
void USART_Sync_SendSCKClock(USART_TypeDef *USARTx, u32 Count)
{
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_USART_Sync_SCKCount(Count));

	USARTx->CR3 = (Count - 1) & USART_CR3_CNT;
}

/**
  * @brief  Sets the frame interval time value in asynchronous mode.
  * @param  USARTx: where x can be 0 to select the USART peripheral.
  * @param  FrameIntervalTime: specifies the frame interval time.
  * @retval None
  */
void USART_Async_SetFrameIntervalTime(USART_TypeDef *USARTx, u32 FrameIntervalTime)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_USART_Async_FrameIntervalTime(FrameIntervalTime));

	tmpreg = USARTx->FITR & ~USART_FITR_FIT;
	tmpreg |= FrameIntervalTime;
	USARTx->FITR = tmpreg;
}

/**
  * @brief  Sets the receiver Timeout value in asynchronous mode.
  * @param  USARTx: where x can be 0 to select the USART peripheral.
  * @param  Timeout: specifies the Receiver Time Out value(0x00~0xFF).
  * @retval None
  */
void USART_Async_SetTimeout(USART_TypeDef *USARTx, u32 Timeout)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_USART_Async_Timeout(Timeout));

	/* Clear the receiver Timeout value by clearing the TO[7:0] bits in the TCR register  */
	tmpreg = USARTx->TCR & ~USART_TCR_TO;
	tmpreg |= Timeout;
	/* Set the receiver Timeout value by setting the TO[7:0] bits in the TCR register  */
	USARTx->TCR = Timeout;
}

/**
  * @brief  This function reset the USART receive and transmit FIFOs.
  * @param  USARTx: where x can be 0 or 1 to select the USART peripheral.
  * @param  FIFO: Transmit FIFO or receive FIFO for the USART.
  *          This parameter can be:
  *            @arg USART_FIFO_RX: select USART receive FIFO.
  *            @arg USART_FIFO_TX: select USART transmit FIFO.
  * @retval None.
  */
void USART_FifoReset(USART_TypeDef *USARTx, u8 FIFO)
{
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_USART_FIFO(FIFO));

	if(FIFO == USART_FIFO_RX)
	{
		USARTx->FRR = USART_Reset_RXFR;
	}
	else
	{
		USARTx->FRR = USART_Reset_TXFR;
	}
}

/**
  * @}
  */
