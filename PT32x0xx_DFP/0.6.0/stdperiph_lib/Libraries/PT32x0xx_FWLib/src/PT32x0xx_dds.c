/**
	******************************************************************************
  * @file    PT32x0xx_dds.c
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the DDS firmware
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

#include "PT32x0xx_dds.h"

/** @defgroup DDS
  * @brief DDS driver modules
  * @{
  */
	
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief	Initializes the DDS peripheral according to
  *         the specified parameters in the DDS_InitTypeDef.
  * @param	DDSx: select the DDS peripheral.
  * @param	DDS_InitStruct: pointer to a DDS_InitTypeDef
  *         structure that contains the configuration information for
  *         the specified DDS peripheral.
  * @retval	None
  */
void DDS_Init(DDS_TypeDef *DDSx, DDS_InitTypeDef *DDS_InitStruct)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_DDS_ALL_PERIPH(DDSx));
	assert_param(IS_DDS_WaveDataMode(DDS_InitStruct->DDS_WaveMode));
	assert_param(IS_DDS_WaveDataStartAddress(DDS_InitStruct->DDS_StartAddress));
	assert_param(IS_DDS_WaveDataLength(DDS_InitStruct->DDS_DataLength));
	assert_param(IS_DDS_WaveOffset(DDS_InitStruct->DDS_Offset));
	assert_param(IS_DDS_Prescaler(DDS_InitStruct->DDS_Prescaler));
	assert_param(IS_DDS_WaveGeneratorPrescaler(DDS_InitStruct->DDS_WGPrescaler));

	tmpreg = DDSx->CR1 & ~DDS_CR1_WDMS;
	tmpreg |= DDS_InitStruct->DDS_WaveMode;
	DDSx->CR1 = tmpreg;
	
	DDSx->WDSAR = DDS_InitStruct->DDS_StartAddress;
	
	DDSx->WDLENR = DDS_InitStruct->DDS_DataLength;
	
	DDSx->WOFR = DDS_InitStruct->DDS_Offset;
	
	DDSx->PSCR = DDS_InitStruct->DDS_Prescaler;
	
	DDSx->WGPSCR = DDS_InitStruct->DDS_WGPrescaler;
}

/**
  * @brief	Fills each DDS_InitStruct member with its default value.
  * @param	DDS_InitStruct: pointer to a DDS_InitTypeDef structure which will be initialized.
  * @retval	None
  */
void DDS_StructInit(DDS_InitTypeDef *DDS_InitStruct)
{
	DDS_InitStruct->DDS_WaveMode = DDS_WaveMode_Full;
#if defined (SRAM_SIZE_8K)	
	DDS_InitStruct->DDS_StartAddress = 0x20001C00;
#else
	DDS_InitStruct->DDS_StartAddress = 0x20000C00;
#endif	
	DDS_InitStruct->DDS_DataLength = 512 - 1;
	DDS_InitStruct->DDS_Offset = 0;
	DDS_InitStruct->DDS_Prescaler = 2 - 1;		
	DDS_InitStruct->DDS_WGPrescaler = 9 - 1;
}

/**
  * @brief	Enable or disable the specified DDS peripheral.
  * @param	DDSx: select the DDS peripheral.
  * @param	NewState: new state of the DDSx peripheral.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval	None
  */
void DDS_Cmd(DDS_TypeDef *DDSx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_DDS_ALL_PERIPH(DDSx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the selected DDS peripheral */
		DDSx->CR1 |= DDS_CR1_EN;
	}
	else
	{
		/* Disable the selected DDS peripheral */
		DDSx->CR1 &= ~DDS_CR1_EN;
	}
}

/**
  * @brief	Forces or releases DDS function reset.
  * @param	DDSx: select the DDS peripheral.
  * @param	NewState: new state of the DDS function reset.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval	None
  */
void DDS_ResetCmd(DDS_TypeDef *DDSx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_DDS_ALL_PERIPH(DDSx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		DDSx->CR1 |= DDS_CR1_RST;
	}
	else
	{
		DDSx->CR1 &= ~DDS_CR1_RST;
	}
}

/**
  * @brief	Configure the I/Q signal mode for the specified DDS peripheral.
  * @param	DDSx: select the DDS peripheral.
  * @param	Mode: specifies the DDS I/Q signal mode.
  *			This parameter can be one of the following values:
  *			@arg DDS_IQMode_InPhase: the In-Phase mode is selected
  *			@arg DDS_IQMode_Quadrature: the Quadrature mode is selected
  * @retval None
  */
void DDS_IQModeConfig(DDS_TypeDef *DDSx, u32 Mode)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_DDS_ALL_PERIPH(DDSx));
	assert_param(IS_DDS_IQMode(Mode));

	tmpreg = DDSx->CR1 & ~DDS_CR1_IQMS;
	tmpreg |= Mode;
	DDSx->CR1 = tmpreg;
}

/**
  * @brief	Configure the wave data mode for the specified DDS peripheral.
  * @param	DDSx: select the DDS peripheral.
  * @param	Mode: specifies the wave data mode.
  *			This parameter can be one of the following values:
  *			@arg DDS_WaveMode_Full: the full wave mode selected
  *			@arg DDS_WaveMode_Half: the half wave mode selected
  *			@arg DDS_WaveMode_Quarter: the quarter wave mode selected
  * @retval None
  */
void DDS_WaveDataModeConfig(DDS_TypeDef *DDSx, u32 Mode)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_DDS_ALL_PERIPH(DDSx));
	assert_param(IS_DDS_WaveDataMode(Mode));

	tmpreg = DDSx->CR1 & ~DDS_CR1_WDMS;
	tmpreg |= Mode;
	DDSx->CR1 = tmpreg;
}

/**
  * @brief	Configure the prescaler for the specified DDS peripheral.
  * @param	DDSx: select the DDS peripheral.
  * @param	Division: specifies the DDS's prescaler.
  *			This parameter can be a number between 0 and 511.
  * @retval None
  */
void DDS_PrescalerConfig(DDS_TypeDef *DDSx, u32 Division)
{	
	/* Check the parameters */
	assert_param(IS_DDS_ALL_PERIPH(DDSx));
	assert_param(IS_DDS_Prescaler(Division));
	
	DDSx->PSCR = Division;
}

/**
  * @brief	Configure the wave generator prescaler for the specified DDS peripheral.
  * @param	DDSx: select the DDS peripheral.
  * @param	Division: specifies the wave generator's prescaler.
  *			This parameter can be a number between 0 and 511.
  * @retval None
  */
void DDS_WaveGeneratorPrescalerConfig(DDS_TypeDef *DDSx, u32 Division)
{	
	/* Check the parameters */
	assert_param(IS_DDS_ALL_PERIPH(DDSx));
	assert_param(IS_DDS_WaveGeneratorPrescaler(Division));
	
	DDSx->WGPSCR = Division;
}

/**
  * @brief	Configure the wave data start address for the specified DDS peripheral.
  * @param	DDSx: select the DDS peripheral.
  * @param	address: specifies the wave data start address
  *			This parameter can be last 1KB in SRAM.
  * @retval None
  */
void DDS_WaveDataStartAddressConfig(DDS_TypeDef *DDSx, u32 Address)
{	
	/* Check the parameters */
	assert_param(IS_DDS_ALL_PERIPH(DDSx));
	assert_param(IS_DDS_WaveDataStartAddress(Address));
	
	DDSx->WDSAR = Address;
}

/**
  * @brief	Configure the wave data length for the specified DDS peripheral.
  * @param	DDSx: select the DDS peripheral.
  * @param	length: specifies the wave data length.
  *			This parameter can be a number between 0 and 511.
  * @retval None
  */
void DDS_WaveDataLengthConfig(DDS_TypeDef *DDSx, u32 Length)
{	
	/* Check the parameters */
	assert_param(IS_DDS_ALL_PERIPH(DDSx));
	assert_param(IS_DDS_WaveDataLength(Length));
	
	DDSx->WDLENR = Length;
}

/**
  * @brief	Configure the wave offset for the specified DDS peripheral.
  * @param	DDSx: select the DDS peripheral.
  * @param	offset: specifies the wave offset.
  *			This parameter can be a number between 0 and 0xFFF.
  * @retval None
  */
void DDS_WaveOffsetConfig(DDS_TypeDef *DDSx, u32 Offset)
{	
	/* Check the parameters */
	assert_param(IS_DDS_ALL_PERIPH(DDSx));
	assert_param(IS_DDS_WaveOffset(Offset));
	
	DDSx->WOFR = Offset;
}

/**
  * @brief	Check whether the specified DDS flag is set or not.
  * @param	DDSx: select the DDS peripheral.
  * @param	DDS_FLAG: specifies the flag to check. 
  *			This parameter can be one of the following values:
  *			@arg DDS_FLAG_InPhase: Current IQ demodulation mode is In-Phase
  *			@arg DDS_FLAG_Quadrature: Current IQ demodulation mode is Quadrature
  * @retval FlagStatus: The new state of DDS FLAG (SET or RESET).
  */
FlagStatus DDS_GetFlagStatus(DDS_TypeDef *DDSx, u32 DDS_FLAG)
{
	FlagStatus bitstatus;

	/* Check the parameters */
	assert_param(IS_DDS_ALL_PERIPH(DDSx));
	assert_param(IS_DDS_FLAG(DDS_FLAG));

	if((DDSx->SR & DDS_FLAG) != (u32)RESET)
	{
		bitstatus = SET;
	}
	else
	{
		bitstatus = RESET;
	}

	return  bitstatus;
}
