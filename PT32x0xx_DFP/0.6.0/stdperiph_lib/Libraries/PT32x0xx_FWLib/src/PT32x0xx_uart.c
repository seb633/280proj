/**
	****************************************************************************
  * @file    PT32x0xx_UART.c
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

#include "PT32x0xx_uart.h"

/** @defgroup UART
  * @brief UART driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the UARTx peripheral according to the specified
  *         parameters in the UART_InitStruct .
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  UART_InitStruct: pointer to a UART_InitTypeDef structure that contains
  *         the configuration information for the specified UART peripheral.
  * @retval None
  */
void UART_Init(UART_TypeDef *UARTx, UART_InitTypeDef *UART_InitStruct)
{
	u32 tmpreg, WP;
	
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_BaudRate(UART_InitStruct->UART_BaudRate));
	assert_param(IS_UART_SampleRate(UART_InitStruct->UART_SampleRate));
	assert_param(IS_UART_DataLength(UART_InitStruct->UART_DataLength));
	assert_param(IS_UART_StopBitLength(UART_InitStruct->UART_StopBitLength));
	assert_param(IS_UART_ParityMode(UART_InitStruct->UART_ParityMode));
	assert_param(IS_UART_Receiver(UART_InitStruct->UART_Receiver));
	assert_param(!IS_UART_ErrorDataLengthAndParity(UART_InitStruct->UART_DataLength, UART_InitStruct->UART_ParityMode));

	switch(UART_InitStruct->UART_DataLength)
	{
		case UART_DataLength_7:
			if(UART_InitStruct->UART_ParityMode == UART_Parity_Odd)
			{
				WP = UART_WordLengthAndParity_7DO;
			}
		  	else
			{
				WP = UART_WordLengthAndParity_7DE;
			}
			break;
		case UART_DataLength_8:
			if(UART_InitStruct->UART_ParityMode == UART_Parity_None)
			{
				WP = UART_WordLengthAndParity_8D;
			}
			else if(UART_InitStruct->UART_ParityMode == UART_Parity_Odd)
			{
				WP = UART_WordLengthAndParity_8DO;
			}
			else
			{
				WP = UART_WordLengthAndParity_8DE;
			}
			break;
		case UART_DataLength_9:
			WP = UART_WordLengthAndParity_9D;
			break;
		default:
			return ;
	}
	
	UART_BaudRateConfig(UARTx, UART_InitStruct->UART_BaudRate, UART_InitStruct->UART_SampleRate);

	tmpreg = UARTx->CR & ~(UART_CR_RE | UART_CR_WP | UART_CR_STOP);
	tmpreg |= UART_InitStruct->UART_Receiver | UART_InitStruct ->UART_StopBitLength | WP;
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
	UARTx->CR = tmpreg;	
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}	
}

/**
  * @brief  Initializes the UARTx smart card mode according to the specified
  *         parameters in the UART_SmartCardInitStruct.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  UART_InitStruct: pointer to a UART_InitTypeDef structure that contains
  *         the configuration information for the specified UART smart card mode.
  * @retval None
  */
void UART_SmartCardInit(UART_TypeDef *UARTx, UART_SmartCardInitTypeDef *UART_SmartCardInitStruct)
{
	u32 tmpreg, WP, baud, psc;
	
	/* Check the parameters */
	assert_param(IS_UART_SmartCard_PERIPH(UARTx));
	assert_param(IS_UART_SampleRate(UART_SmartCardInitStruct->SmartCard_SampleRate));
	assert_param(IS_UART_SmartCardParityMode(UART_SmartCardInitStruct->SmartCard_ParityMode));
	assert_param(IS_UART_SmartCardDirection(UART_SmartCardInitStruct->SmartCard_Direction));
	assert_param(IS_UART_SmartCardPrescaler(UART_SmartCardInitStruct->SmartCard_Prescaler));

	if(UART_SmartCardInitStruct->SmartCard_ParityMode == UART_Parity_Even)
	{
		WP = UART_WordLengthAndParity_8DE;
	}
	else
	{
		WP = UART_WordLengthAndParity_8DO;
	}
	
	psc = 2 << ((UART_SmartCardInitStruct->SmartCard_Prescaler >> 4));
	baud = (GetClockFreq(CLKSRC_PCLK)/psc + (372/2))/372;
	
	UART_BaudRateConfig(UARTx, baud, UART_SmartCardInitStruct->SmartCard_SampleRate);

	tmpreg = UARTx->CR & ~UART_CR_WP;
	tmpreg |= UART_Receiver_Enable | WP;
	
	UARTx->CR = tmpreg;
	
	UARTx->SCCR = UART_SmartCardInitStruct->SmartCard_Direction | UART_SmartCardInitStruct->SmartCard_Prescaler;
}

/**
  * @brief  Fills each UART_InitStruct member with its default value.
  * @param  UART_InitStruct: pointer to a UART_InitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void UART_StructInit(UART_InitTypeDef *UART_InitStruct)
{
	/* UART_InitStruct members default value */
	UART_InitStruct->UART_BaudRate = 19200;
	UART_InitStruct->UART_SampleRate = UART_SampleRate_16X;
	UART_InitStruct->UART_DataLength = UART_DataLength_8;
	UART_InitStruct->UART_StopBitLength = UART_StopBitLength_1;
	UART_InitStruct->UART_ParityMode = UART_Parity_None;
	UART_InitStruct->UART_Receiver = UART_Receiver_Enable;
}

/**
  * @brief  Fills each UART_SmartCardInitStruct member with its default value.
  * @param  UART_SmartCardInitStruct: pointer to a UART_SmartCardInitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void UART_SmartCardStructInit(UART_SmartCardInitTypeDef *UART_SmartCardInitStruct)
{
	/* UART_SmartCardInitStruct members default value */
	UART_SmartCardInitStruct->SmartCard_SampleRate = UART_SampleRate_16X;
	UART_SmartCardInitStruct->SmartCard_ParityMode = UART_Parity_Even;
	UART_SmartCardInitStruct->SmartCard_Direction = UART_SmartCardDir_Rx;
	UART_SmartCardInitStruct->SmartCard_Prescaler = UART_SmartCardPSC_Div32;
}

/**
  * @brief  Enable or disable the specified UART peripheral.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  NewState: new state of the UARTx peripheral.
  *       	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_Cmd(UART_TypeDef *UARTx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
	
	if(NewState != DISABLE)
	{
		/* Enable the selected UART by setting the UE bit in the CR register */
		UARTx->CR |= UART_CR_EN;
	}
	else
	{
		/* Disable the selected UART by clearing the UE bit in the CR register */
		UARTx->CR &= ~UART_CR_EN;
	}
	
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
}

/**
  * @brief  Enable or disable the specified UART Receiver enable.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  NewState: new state of the UARTx peripheral.
  *        	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_ReceiverCmd(UART_TypeDef *UARTx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
	
	if(NewState != DISABLE)
	{
		/* Enable the selected UART by setting the UE bit in the CR register */
		UARTx->CR |= UART_CR_RE;
	}
	else
	{
		/* Disable the selected UART by clearing the UE bit in the CR register */
		UARTx->CR &= ~UART_CR_RE;
	}
	
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
}

/**
  * @brief  Enable or disable the UART's receive request for the specified UART peripheral.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  NewState: new state of the UARTx peripheral.
  *        	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_ReceiveDMACmd(UART_TypeDef *UARTx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

#if defined (PT32x002x) || defined(PT32x012x) || defined (PTM160x)
	return ;
#else
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
	
	if(NewState != DISABLE)
	{
		/* Enable the selected UART by setting the UE bit in the CR register */
		UARTx->CR |= UART_RXDMAE_Enable;
	}
	else
	{
		/* Disable the selected UART by clearing the UE bit in the CR register */
		UARTx->CR &= ~UART_RXDMAE_Enable;
	}
	
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
#endif
}

/**
  * @brief  Enable or disable DMA transfer request for the specified UART peripheral.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  NewState: new state of the UARTx peripheral.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_TransferDMACmd(UART_TypeDef *UARTx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
#if defined (PT32x002x) || defined(PT32x012x) || defined (PTM160x)
	return ;
#else
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
	
	if(NewState != DISABLE)
	{
		/* Enable the UART slave output */
		UARTx->CR |= UART_TXDMAE_Enable;
	}
	else
	{
		/* Disable the UART slave output */
		UARTx->CR &= ~UART_TXDMAE_Enable;
	}
	
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
#endif
}

/**
  * @brief  Configure the word length and parity for the specified UART peripheral.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  WordLenthAndParity: specifies the Word length and parity.
  *        	This parameter can be any combination of the following values:
  *        	@arg UART_WordLengthAndParity_8D: 8-bit data format without checksum
  *        	@arg UART_WordLengthAndParity_8DE: 8-bit data and 1-bit even parity
  *        	@arg UART_WordLengthAndParity_8DO: 8-bit data and 1-bit odd parity
  *        	@arg UART_WordLengthAndParity_9D: 9-bit data format without checksum
  * @retval None
  */

void UART_WordLengthAndParityConfig(UART_TypeDef *UARTx, u32 WordLenthAndParity)
{
    u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_WordLengthAndParity(WordLenthAndParity));

	tmpreg = UARTx->CR & ~UART_CR_WP;
	tmpreg |= WordLenthAndParity;
	
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
	
	UARTx->CR = tmpreg;
	
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
}

/**
  * @brief  Configure the stop bit length for the specified UART peripheral.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  StopBitLenth: specifies the Stop bit length.
  *       	This parameter can be any combination of the following values:
  *        	@arg UART_StopBitLength_1: 1 bit Stop Length.
  *        	@arg UART_StopBitLength_2: 2 bit Stop Length.
  *       	@arg UART_StopBitLength_0P5: 0.5 bit Stop Length.
  *       	@arg UART_StopBitLength_1P5: 1.5 bit Stop Length.
  * @retval None
  */
void UART_StopBitLengthConfig(UART_TypeDef *UARTx, u32 StopBitLenth)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_StopBitLength(StopBitLenth));

	tmpreg = UARTx->CR & ~UART_CR_STOP;
	tmpreg |= StopBitLenth;
	
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
	
	UARTx->CR = tmpreg;
	
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
}

/**
  * @brief  Configure the the DMA trigger control for the specified UART peripheral.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  DMATiggerControl: specifies the stop bit length.
  *       	This parameter can be any combination of the following values:
  *        	@arg UART_Trigger_0: Receive FIFO �� 1 frame of data, trigger DMA request
  *        	@arg UART_Trigger_1: Receive FIFO �� 4 frame of data, trigger DMA request
  *        	@arg UART_Trigger_2: Receive FIFO �� 8 frame of data, trigger DMA request
  *        	@arg UART_Trigger_3: reserve
  * @retval None
  */
void UART_DMATriggerControlConfig(UART_TypeDef *UARTx, u32 DMATiggerControl)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_DMATriggerControl(DMATiggerControl));

	tmpreg = UARTx->CR & ~UART_CR_DMATC;
	tmpreg |= DMATiggerControl;
	
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
	
	UARTx->CR = tmpreg;
	
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
}

/**
  * @brief  Configure the UART data polarity for the specified UART peripheral.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  Polarity: UART polarity.
  *       	This parameter can be any combination of the following values:
  *        	@arg UART_PolarityControl_RXP: UART data receiving polarity
  *        	@arg UART_PolarityControl_TXP: UART data sending polarity
  * @param  NewState: new state of the UART data polarity.
  *       	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_PolarityControl(UART_TypeDef *UARTx, u32 Polarity, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_PolarityControl(Polarity));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
	
	if(NewState != DISABLE)
	{
		 UARTx->CR |= Polarity;
	}
	else
	{
		UARTx->CR &= ~Polarity;
	}
	
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
}

/**
  * @brief  Enable or disable the Single-line mode for the specified UART peripheral.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  NewState: new state of the UART Communication.
  *       	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_SingleLineModeCmd(UART_TypeDef *UARTx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
	
	if(NewState != DISABLE)
	{
		UARTx->CR |= UART_CR_SLME;
	}
	else
	{
		UARTx->CR &= ~UART_CR_SLME;
	}
	
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
}

/**
  * @brief  Enable or disable the transmitter or receiver direction for the
  *  		specified UART peripheral.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  UART_Direction: specifies the UART direction.
  *       	This parameter can be any combination of the following values:
  *        	@arg UART_SingleLineDirection_Tx: UART Single Line Direction Transmitter
  *        	@arg UART_SingleLineDirection_Rx: UART Single Line Direction Receiver
  * @param  NewState: new state of the UART transfer direction.
  *        	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_SinglelineDirectionConfig(UART_TypeDef *UARTx, u32 Direction)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_SingleLineDirection(Direction));

	tmpreg = UARTx->CR & ~UART_CR_SLDS;
	tmpreg |= Direction;
	
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
	
	UARTx->CR = tmpreg;
	
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
}

/**
  * @brief  Configure the mute mode wakeup method for the specified UART peripheral.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  MuteModeWakeUpMethod: specifies the UART mute mode wake-up method.
  *       	This parameter can be any combination of the following values:
  *        	@arg UART_MuteModeWakeupMethod_Idle: Line idle
  *        	@arg UART_MuteModeWakeupMethod_Match: Address matching
  * @retval None
  */
void UART_MuteModeWakeupMethodConfig(UART_TypeDef *UARTx, u32 MuteModeWakeUpMethod)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_MuteModeWakeupMethod(MuteModeWakeUpMethod));

	tmpreg = UARTx->CR & ~UART_CR_MMWU;
	tmpreg |= MuteModeWakeUpMethod;
	
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
	
	UARTx->CR = tmpreg;
	
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
}

/**
  * @brief  Enable or disable the Mute mode for the specified UART peripheral.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  NewState: new state of the UART Communication.
  *        	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_MuteModeCmd(UART_TypeDef *UARTx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
#if defined (PT32x002x) || defined(PT32G031x) || defined(PT32x012x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM160x) || defined (PTM280x)
	return ;
#else
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
	
	if(NewState != DISABLE)
	{
		/* Enable the Half-Duplex mode by setting the OWE bit in the CR register */
		UARTx->CR |= UART_CR_MME;
	}
	else
	{
		/* Disable the Half-Duplex mode by clearing the OWE bit in the CR register */
		UARTx->CR &= ~UART_CR_MME;
	}
	
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
#endif
}

/**
  * @brief  Sets low power mode wakeup method for UART peripheral.
  * @param  UARTx: where x can be from 0 to 3 to select the UART peripheral.
  *                where x can be from 0 to 1 to select the LPUART peripheral.
  * @param  UART_WakeupMode: specifies the wakeupmode for UART peripheral.
  *          This parameter can be one of the following values:
  *          @arg UART_WakeupMethod_RXNE: Detects that a data frame has been received
  *          @arg UART_WakeupMethod_StartBit: Start bit detected
  * @retval None
  */
void UART_LowPowerWakeupConfig(UART_TypeDef *UARTx, u32 Method)
{
	u32 tmpreg = 0;
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_WakeupMode(Method));
	tmpreg = UARTx->CR;
	tmpreg &= ~(UART_CR_LPWU);
	tmpreg |= Method;

	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}

	UARTx->CR = tmpreg;

	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
}

/**
  * @brief  Enable or disable the Loopback mode for the specified UART peripheral.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  NewState: new state of the UART Communication.
  *        	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_LoopbackModeCmd(UART_TypeDef *UARTx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
	
	if(NewState != DISABLE)
	{
		/* Enable the Half-Duplex mode by setting the OWE bit in the CR register */
		UARTx->CR |= UART_CR_LBM;
	}
	else
	{
		/* Disable the Half-Duplex mode by clearing the OWE bit in the CR register */
		UARTx->CR &= ~UART_CR_LBM;
	}
	
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		while(UARTx->CR & UART_CR_BSY);
	}
}

/**
  * @brief  Configure the baudrate for the specified UART peripheral.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  BaudRate: baudrate for UART peripheral.
  * @retval None
  */
void UART_BaudRateConfig(UART_TypeDef *UARTx, u32 BaudRate, u32 SampleRate)
{
	u32 quotient, clk_frequency, tmpvalue;
	u8 clk_src = CLKSRC_PCLK;
	
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_SampleRate(SampleRate));

	tmpvalue = BaudRate << (SampleRate >> 16);
	if(UARTx == LPUART0 || UARTx == LPUART1)
	{
		u8 lpuart_clk[5] = {CLKSRC_PCLK, CLKSRC_HSI, CLKSRC_LSI, CLKSRC_LSE, CLKSRC_HCLK};
		u8 src;

		if(UARTx == LPUART0)
		{
			src = RCC->PCSCR2 & RCC_PCSCR2_LPUART0;
		}
		else
		{
			src = (RCC->PCSCR2 & RCC_PCSCR2_LPUART1)>>4;
		}

		if(src < 5)
		{
			clk_src = lpuart_clk[src];
		}
	}
	clk_frequency = GetClockFreq(clk_src);

	if(clk_frequency > tmpvalue)
	{
		quotient = (clk_frequency + tmpvalue/2) / tmpvalue;
		UARTx->BRR = quotient | SampleRate;
	}
	else
	{
		UARTx->BRR = 0x01 | SampleRate;
	}
}

/**
  * @brief  Configure the device address of the slave for the specified
  *			UART peripheral.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  Address: address of the slave.
  *        	This parameter can be a number between 0 and 0xFF.
  * @retval None
  */
void UART_SlaveDeviceAddressConfig(UART_TypeDef *UARTx, u32 Address)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_SlaveDeviceAddress(Address));

	UARTx->AR = Address;
}

/**
  * @brief  Enable or disable the specified UART interrupts.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  UART_IT: specifies the UART interrupt sources to be enabled or disabled.
  *			This parameter can be one of the following values:
  *        	@arg UART_IT_RXNE: Receive/Receive-FIFO not empty interrupt.
  *        	@arg UART_IT_RXHF: Receive-FIFO half full interrupt.
  *        	@arg UART_IT_RXF: Receive-FIFO full interrupt.
  *        	@arg UART_IT_RXTO: Receive/Receive-FIFO clear timeout interrupt.
  *        	@arg UART_IT_IDLE: Idle frame detected interrupt.
  *        	@arg UART_IT_PE: parity error interrupt.
  *        	@arg UART_IT_FE: frame error interrupt.
  *        	@arg UART_IT_OVR: Receive/Receive-FIFO overflow interrupt.
  *        	@arg UART_IT_TXE: Transmit/Transmit-FIFO empty interrupt.
  *        	@arg UART_IT_TXHE: Transmit FIFO half empty interrupt.
  *        	@arg UART_IT_TXF: Transmit FIFO full interrupt.
  *        	@arg UART_IT_TXC: Transmit complete interrupt.
  * @param  NewState: new state of the specified UARTx interrupts.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_ITConfig(UART_TypeDef *UARTx, u32 UART_IT, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_IT(UART_IT));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the selected UART interrupts */
		UARTx->IER |= UART_IT;
	}
	else
	{
		/* Disable the selected UART interrupts */
		UARTx->IER &= ~UART_IT;
	}
}

/**
  * @brief  Check whether the specified UART flag is set or not.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  UART_FLAG: specifies the flag to check.
  *        	This parameter can be any combination of the following values:
  *        	@arg UART_FLAG_RXNE: Receive/Receive-FIFO not empty flag.
  *        	@arg UART_FLAG_RXHF: Receive-FIFO half full flag.
  *        	@arg UART_FLAG_RXF: Receive-FIFO full flag.
  *        	@arg UART_FLAG_RXTO: Receive/Receive-FIFO clear timeout flag.
  *        	@arg UART_FLAG_IDLE: Idle frame detected flag.
  *        	@arg UART_FLAG_PE: parity error flag.
  *        	@arg UART_FLAG_FE: frame error flag.
  *        	@arg UART_FLAG_OVR: Receive/Receive-FIFO overflow flag.
  *        	@arg UART_FLAG_TXE: Transmit/Transmit-FIFO empty flag.
  *        	@arg UART_FLAG_TXHE: Transmit FIFO half empty flag.
  *        	@arg UART_FLAG_TXF: Transmit FIFO full flag.
  *        	@arg UART_FLAG_TXC: Transmit complete flag.
  *        	@arg UART_FLAG_MUTE: Mute mode flag.
  * @retval The new state of UART_FLAG (SET or RESET).
  */
FlagStatus UART_GetFlagStatus(UART_TypeDef *UARTx, u32 UART_FLAG)
{
	FlagStatus bitstatus = RESET;
	
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_FLAG(UART_FLAG));
	/* Check the status of the specified UART flag */
	if((UARTx->SR & UART_FLAG) != (u16)RESET)
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
  * @brief  Clear the UARTx's pending flags.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  UART_CLEAR_FLAG: specifies the flag to clear.
  *        	This parameter can be any combination of the following values:
  *        	@arg UART_FLAG_PE: Parity error.
  *        	@arg UART_FLAG_FE: Frame error.
  *        	@arg UART_FLAG_OVR: Receiver FIFO buffer overflow.
  * @retval None
  */
void UART_ClearFlag(UART_TypeDef *UARTx, u32 UART_CLEAR_FLAG)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_FLAG(UART_CLEAR_FLAG));

	UARTx->SR = UART_CLEAR_FLAG;
}

/**
  * @brief  Check whether the specified UART interrupt status is set or not.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  UART_IT: specifies the UART interrupt sources to be enabled or disabled.
  *			This parameter can be one of the following values:
  *        	@arg UART_IT_RXNE: Receive/Receive-FIFO not empty interrupt.
  *        	@arg UART_IT_RXHF: Receive-FIFO half full interrupt.
  *        	@arg UART_IT_RXF: Receive-FIFO full interrupt.
  *        	@arg UART_IT_RXTO: Receive/Receive-FIFO clear timeout interrupt.
  *        	@arg UART_IT_IDLE: Idle frame detected interrupt.
  *        	@arg UART_IT_PE: parity error interrupt.
  *        	@arg UART_IT_FE: frame error interrupt.
  *        	@arg UART_IT_OVR: Receive/Receive-FIFO overflow interrupt.
  *        	@arg UART_IT_TXE: Transmit/Transmit-FIFO empty interrupt.
  *        	@arg UART_IT_TXHE: Transmit FIFO half empty interrupt.
  *        	@arg UART_IT_TXF: Transmit FIFO full interrupt.
  *        	@arg UART_IT_TXC: Transmit complete interrupt.
  * @return ITStatus of UART_IT (SET or RESET).
  */
ITStatus UART_GetITStatus(UART_TypeDef *UARTx, u32 UART_IT)
{
	ITStatus bitstatus = RESET;

	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_IT(UART_IT));

	/* Check the status of the specified UART flag */
	if((UARTx->SR & UART_IT) != 0 && (UARTx->IER & UART_IT) != 0)
	{
		/* UART_FLAG is set */
		bitstatus = SET;
	}
	else
	{
		/* UART_FLAG is reset */
		bitstatus = RESET;
	}
	/* Return the UART_FLAG status */
	return  bitstatus;
}

/**
  * @brief  Transmits single data through the UARTx peripheral.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  Data: the data to transmit.
  * @retval None
  */
void UART_SendData(UART_TypeDef *UARTx, u16 Data)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_Data(Data));

	while(!(UARTx->SR & UART_SR_TXE));
	/* Transmit Data */
	UARTx->DR = (Data & (u16)0x01FF);
}

/**
  * @brief  Returns the most recent received data for the UARTx peripheral.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @retval The received data.
  */
u16 UART_ReceiveData(UART_TypeDef *UARTx)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	/* Receive Data */
	return (u16)(UARTx->DR & (u16)0x01FF);
}

/**
  * @brief  Sets the frame interval time value.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  UART_ReceiverTimeOut: specifies the Receiver Time Out value.
  *			This parameter can be a number between 0 and 0xFF.
  * @retval None
  */
void UART_SetFrameIntervalTime(UART_TypeDef *UARTx, u32 FrameIntervalTime)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_FrameIntervalTime(FrameIntervalTime));

	tmpreg = UARTx->FITR & ~UART_FITR_FIT;
	tmpreg |= FrameIntervalTime;
	UARTx->FITR = tmpreg;
}

/**
  * @brief  Sets the receiver Time Out value.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  UART_ReceiverTimeOut: specifies the Receiver Time Out value.
  *			This parameter can be a number between 0 and 0xFF.
  * @retval None
  */
void UART_SetTimeout(UART_TypeDef *UARTx, u32 Timeout)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_Timeout(Timeout));

	/* Clear the receiver Timeout value by clearing the TO[7:0] bits in the TCR register  */
	tmpreg = UARTx->TCR & ~UART_TCR_TO;
	tmpreg |= Timeout;
	/* Set the receiver Timeout value by setting the TO[7:0] bits in the TCR register  */
	UARTx->TCR = Timeout;
}

/**
  * @brief  This function reset the UART receive and transmit FIFOs.
  * @param  UARTx: where x can be 0 or 1 to select the UART peripheral.
  * @param  FIFO: Transmit FIFO or receive FIFO for the UART.
  *          This parameter can be:
  *            @arg UART_FIFO_RX: select UART receive FIFO.
  *            @arg UART_FIFO_TX: select UART transmit FIFO.
  * @retval None.
  */
void UART_FifoReset(UART_TypeDef *UARTx, u8 FIFO)
{
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_FIFO(FIFO));

	if(FIFO == UART_FIFO_RX)
	{
		UARTx->FRR = UART_Reset_RXFR;
	}
	else
	{
		UARTx->FRR = UART_Reset_TXFR;
	}
}

/**
  * @brief  Enable or disable the UART's smart card interface.
  * @param  UARTx: where x can be from 0 to 3 to select the UART peripheral.
  *                where x can be from 0 to 1 to select the LPUART peripheral.
  * @param  NewState: new state of the smart card mode.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_SmartCardCmd(UART_TypeDef *UARTx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_UART_SmartCard_PERIPH(UARTx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		UARTx->SCCR |= UART_SCCR_SCEN;
	}
	else
	{
		UARTx->SCCR &= ~UART_SCCR_SCEN;
	}
}

/**
  * @brief  Enable or disable the transmitter or receiver direction for the
  *  		specified UART peripheral.
  * @param  UARTx: where x can be 0 to 3 to select the UART peripheral.
  * @param  UART_Direction: specifies the UART direction.
  *       	This parameter can be any combination of the following values:
  *        	@arg UART_SmartCardDir_Rx: UART Smart Card Direction Transmitter
  *        	@arg UART_SmartCardDir_Tx: UART Smart Card Direction Receiver
  * @param  NewState: new state of the UART transfer direction.
  *        	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_SmartCardDirectionConfig(UART_TypeDef *UARTx, u32 Direction)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_UART_SmartCard_PERIPH(UARTx));
	assert_param(IS_UART_SmartCardDirection(Direction));

	tmpreg = UARTx->SCCR & ~UART_SCCR_DIR;
	tmpreg |= Direction;
	UARTx->SCCR = tmpreg;
}

/**
  * @}
  */

