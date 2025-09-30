/**
	****************************************************************************
  * @file    PT32x0xx_dac.c
  * @author  Application development team
  * @version V1.0.0
  * @date    2024/12/20
  * @brief   This file contains all the functions prototypes for the DAC firmware library
  *
  ******************************************************************************
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.  
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of
  * the content or the codes within the content.  
  * 
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved 
  *****************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "PT32x0xx_dac.h"

/** @defgroup DAC
  * @brief DAC driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief	Initializes the DACx peripheral according to
  *         the specified parameters in the DAC_InitTypeDef.
  * @param	ADCx: where x can be 0~1 to select the DAC peripheral.
  * @param	DAC_InitStruct: pointer to a DAC_InitTypeDef
  *         structure that contains the configuration information for
  *         the specified DAC peripheral.
  * @retval	None
  */
void DAC_Init(DAC_TypeDef *DACx, DAC_InitTypeDef *DAC_InitStruct)
{
	u32 tmpreg = 0;
	
	/* Check the parameters */
	assert_param(IS_DAC_ALL_PERIPH(DACx));
	assert_param(IS_DAC_ReferencePositive(DAC_InitStruct->DAC_RVSPS));
	assert_param(IS_DAC_Align(DAC_InitStruct->DAC_Align));
	assert_param(IS_DAC_PowerMode(DAC_InitStruct->DAC_PowerMode));
	
	tmpreg = DACx->CR1 & ~(DAC_CR1_RVSPS | DAC_CR1_ALIGN | DAC_CR1_PM);
	tmpreg |= DAC_InitStruct->DAC_RVSPS | \
			   DAC_InitStruct->DAC_Align | \
			   DAC_InitStruct->DAC_PowerMode;
	DACx->CR1 = tmpreg;
}

/**
  * @brief	Fills each DAC_InitStruct member with its default value.
  * @param	DAC_InitStruct: pointer to a DAC_InitTypeDef structure which will be initialized.
  * @retval	None
  */
void DAC_StructInit(DAC_InitTypeDef *DAC_InitStruct)
{
	DAC_InitStruct->DAC_RVSPS = DAC_RVSPS_VDDA;
	DAC_InitStruct->DAC_Align = DAC_Align_Right;
	DAC_InitStruct->DAC_PowerMode = DAC_PowerMode_Normal;
}

/**
  * @brief  Enable or disable the specified DAC peripheral.
  * @param  DACx: select the DAC peripheral.
  * @param  NewState: new state of the DACx peripheral.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DAC_Cmd(DAC_TypeDef *DACx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_DAC_ALL_PERIPH(DACx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the selected DACx peripheral */
		DACx->CR1 |= DAC_CR1_EN;
	}
	else
	{
		/* Disable the selected DACx peripheral */
		DACx->CR1 &= ~DAC_CR1_EN;
	}
}

/**
  * @brief	Configure the reference voltage source positive for the specified 
  *			DAC peripheral.
  * @param	DACx: select the DAC peripheral.
  * @param	Source: specifies reference voltage source positive selection.
  *			This parameter can be one of the following values:
  * 		@arg DAC_RVSPS_VDDA: select the VDDA as reference voltage source positive.
  * 		@arg DAC_RVSPS_VREFBUF: select the VREFBUF as reference voltage source positive.
  * @retval None
  */
void DAC_SetRVSPS(DAC_TypeDef *DACx, u32 Source)
{
	u32 tmpreg = 0;
	
	/* Check the parameters */
	assert_param(IS_DAC_ALL_PERIPH(DACx));
	assert_param(IS_DAC_ReferencePositive(Source));
	/* Get the DACx CR1 value and Clear RVSPS bits */
	tmpreg = DACx->CR1 & ~DAC_CR1_RVSPS;
	/* Set RVSPS bit according to Source value */
	tmpreg |= Source;
	/* Write to CR1 register*/
	DACx->CR1 = tmpreg;
}

/**
  * @brief	Configure the Power Mode for the specified DAC peripheral.
  * @param	DACx: select the DAC peripheral.
  * @param	Mode: specifies the DAC Power Mode.
  *			This parameter can be one of the following values:
  * 		@arg DAC_PowerMode_Normal: Normal mode selected.
  * 		@arg DAC_PowerMode_HighSpeed: HighSpeed mode selected.
  * @retval	None
  */
void DAC_SetPowerMode(DAC_TypeDef *DACx, u32 Mode)
{
	u32 tmpreg = 0;
	
	/* Check the parameters */
	assert_param(IS_DAC_ALL_PERIPH(DACx));
	assert_param(IS_DAC_PowerMode(Mode));
	/* Get the DACx CR1 value and Clear PM bits */
	tmpreg = DACx->CR1 & ~DAC_CR1_PM;
	/* Set PM bit according to Mode value */
	tmpreg |= Mode;
	/* Write to CR1 register*/
	DACx->CR1 = tmpreg;		
}

/**
  * @brief  Enable or disable output config for the specified DAC peripheral.
  * @param  DACx: select the DAC peripheral.
  * @param  NewState: new state of the DACx peripheral.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DAC_OutputConfig(DAC_TypeDef *DACx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_DAC_ALL_PERIPH(DACx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable DAC output */
		DACx->CR1 |= DAC_CR1_OE;
	}
	else
	{
		/* Disable DAC output */
		DACx->CR1 &= ~DAC_CR1_OE;
	}
}

/**
  * @brief  Set a conversion value for the specified DAC peripheral.
  * @param  DACx: select the DACx peripheral.
  * @param  Data: DAC Value.
  * @retval None.
  */
void DAC_SetData(DAC_TypeDef *DACx, u32 Data)
{
	/* Check the parameters */
	assert_param(IS_DAC_ALL_PERIPH(DACx));
	assert_param(IS_DAC_Value(Data));

	/* Write to DR register*/
	DACx->DR = Data;
}

/**
  * @brief  Enable or disable MFB filter of the specified DAC peripheral.
  * @param  DACx: select the DAC peripheral.
  * @param  NewState: new state of MFB filter.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DAC_MFBCmd(DAC_TypeDef *DACx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_DAC_ALL_PERIPH(DACx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		DACx->CR2 |= DAC_CR2_OFE;
	}
	else
	{
		DACx->CR2 &= ~DAC_CR2_OFE;
	}
}

/**
  * @brief	Configure the Cutoff Frequency for the MFB filter.
  * @param	DACx: select the DAC peripheral.
  * @param	Freq: specifies the Cutoff Frequency for the DAC's MFB filter.
  *			This parameter can be one of the following values:
  * 		@arg DAC_MFB_5KHz: the Cutoff Frequency for the DAC's MFB filter is 5KHz.
  * 		@arg DAC_MFB_10KHz: the Cutoff Frequency for the DAC's MFB filter is 10KHz.
  * 		@arg DAC_MFB_20KHz: the Cutoff Frequency for the DAC's MFB filter is 20KHz.
  * 		@arg DAC_MFB_50KHz: the Cutoff Frequency for the DAC's MFB filter is 50KHz.
  * 		@arg DAC_MFB_100KHz: the Cutoff Frequency for the DAC's MFB filter is 100KHz.
  * 		@arg DAC_MFB_200KHz: the Cutoff Frequency for the DAC's MFB filter is 200KHz.
  * 		@arg DAC_MFB_400KHz: the Cutoff Frequency for the DAC's MFB filter is 400KHz.
  * @retval	None
  */
void DAC_SetMFBCutoffFreq(DAC_TypeDef *DACx, u32 Freq)
{
	u32 tmpreg = 0;
	
	/* Check the parameters */
	assert_param(IS_DAC_ALL_PERIPH(DACx));
	assert_param(IS_DAC_MFB_CutoffFreq(Freq));

	tmpreg = DACx->CR2 & ~DAC_CR2_FCFS;
	tmpreg |= Freq;
	DACx->CR2 = tmpreg;		
}
