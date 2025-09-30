/******************************************************************************
  * @file    PT32x0xx_ldac.c
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the LDAC firmware library
  *
  ******************************************************************************
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of
  * the content or the codes within the content.
  *
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  *******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "PT32x0xx_ldac.h"

/** @defgroup LDAC
  * @brief LDAC driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Enable or disable the specified LDAC peripheral.
  * @param  LDACx: select the LDAC peripheral.
  * @param  NewState: new state of the LDACx peripheral.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void LDAC_Cmd(LDAC_TypeDef *LDACx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_LDAC_ALL_PERIPH(LDACx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the selected LDACx peripheral */
		LDACx->CR |= LDAC_CR_EN;
	}
	else
	{
		/* Disable the selected LDACx peripheral */
		LDACx->CR &= ~LDAC_CR_EN;
	}
}

/**
  * @brief  Enable or disable output config for the specified LDAC peripheral.
  * @param  LDACx: select the LDAC peripheral.
  * @param  NewState: new state of the LDACx peripheral.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void LDAC_OutputConfig(LDAC_TypeDef *LDACx, FunctionalState NewState)
{
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PTM160x) || defined (PTM280x)
	return ;
#else
	/* Check the parameters */
	assert_param(IS_LDAC_ALL_PERIPH(LDACx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable LDAC output */
		LDACx->CR |= LDAC_CR_OE;
	}
	else
	{
		/* Disable LDAC output */
		LDACx->CR &= ~LDAC_CR_OE;
	}
#endif
}

/**
  * @brief	Configure the reference voltage source positive for the specified
  *			LDAC peripheral.
  * @param	LDACx: select the LDAC peripheral.
  * @param	Source: specifies reference voltage source positive selection.
  *			This parameter can be one of the following values:
  * 		@arg LDAC_RVSPS_VDDA: select the VDDA as reference voltage source positive.
  * 		@arg LDAC_RVSPS_BGREF: select the BGREF as reference voltage source positive.
  * @retval None
  */
void LDAC_SetRVSPS(LDAC_TypeDef *LDACx,u32 Source)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_LDAC_ALL_PERIPH(LDACx));
	assert_param(IS_LDAC_ReferencePositive(Source));
	/* Get the LDACx CR value and Clear RVSPS bits */
	tmpreg = LDACx->CR & ~LDAC_CR_RVSPS;
	/* Set RVSPS bit according to Source value */
	tmpreg |= Source;
	/* Write to CR register*/
	LDACx->CR = tmpreg;
}

/**
  * @brief	Configure the Power Mode for the specified LDAC peripheral.
  * @param	LDACx: select the LDAC peripheral.
  * @param	Mode: specifies the LDAC Power Mode.
  *			This parameter can be one of the following values:
  * 		@arg LDAC_PowerMode_Normal: Normal mode selected.
  * 		@arg LDAC_PowerMode_HighSpeed: HighSpeed mode selected.
  * @retval	None
  */
void LDAC_SetPowerMode(LDAC_TypeDef *LDACx,u32 Mode)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_LDAC_ALL_PERIPH(LDACx));
	assert_param(IS_LDAC_PowerMode(Mode));
	/* Get the LDACx CR value and Clear PM bits */
	tmpreg = LDACx->CR & ~LDAC_CR_PM;
	/* Set PM bit according to Mode value */
	tmpreg |= Mode;
	/* Write to CR register*/
	LDACx->CR = tmpreg;
}

/**
  * @brief  Set a conversion value for the specified LDAC peripheral.
  * @param  LDACx: select the LDACx peripheral.
  * @param  Data: LDAC_DAC Value can be a number between 0x00 and 0x1F.
  * @retval None.
  */
void LDAC_SetData(LDAC_TypeDef *LDACx, u32 Data)
{
	/* Check the parameters */
	assert_param(IS_LDAC_ALL_PERIPH(LDACx));
	assert_param(IS_LDAC_Value(Data));

	/* Write to DR register*/
	LDACx->DR = Data;
}
