/**
	****************************************************************************
  * @file    PT32x0xx_beep.c
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the BEEP firmware library
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
#include "PT32x0xx_beep.h"

/** @defgroup BEEP
  * @brief BEEP driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief	Initializes the BEEP peripheral according to
  *         the specified parameters in the Beep_InitTypeDef.
  * @param	Beep_InitStruct: pointer to a Beep_InitTypeDef
  *         structure that contains the configuration information for
  *         the specified BEEP peripheral.
  * @retval	None
  */
void Beep_Init(BEEP_TypeDef *BEEPx, Beep_InitTypeDef *Beep_InitStruct)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_BEEP_ALL_PERIPH(BEEPx));
	assert_param(IS_BEEP_Prescaler(Beep_InitStruct->BEEP_PrescalerValue));
	assert_param(IS_BEEP_HighSink(Beep_InitStruct->BEEP_HinghSink));

	tmpreg = BEEPx->CR & ~(BEEP_CR_DIV | BEEP_CR_HSCE);
	tmpreg |= Beep_InitStruct->BEEP_HinghSink | ((Beep_InitStruct->BEEP_PrescalerValue - 1) << 0x08);
	BEEPx->CR = tmpreg;
}

/**
  * @brief  Enable or disable the specified BEEP peripheral.
  * @param  NewState: new state of the BEEP peripheral.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void Beep_Cmd(BEEP_TypeDef *BEEPx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_BEEP_ALL_PERIPH(BEEPx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the BEEP peripheral */
		BEEP->CR |= BEEP_CR_EN;
	}
	else
	{
		/* Disable the BEEP peripheral */
		BEEP->CR &= ~BEEP_CR_EN;
	}
}

/**
  * @brief  Enable or disable HighSink current config for the specified BEEP peripheral.
  * @param  BEEPx: select the BEEP peripheral.
  * @param  NewState: new state of the BEEPx peripheral.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void Beep_HighSinkConfig(BEEP_TypeDef *BEEPx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_BEEP_ALL_PERIPH(BEEPx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the HighSink current */
		BEEP->CR |= BEEP_CR_HSCE;
	}
	else
	{
		/* Ddisable the HighSink current */
		BEEP->CR &= ~BEEP_CR_HSCE;
	}
}
