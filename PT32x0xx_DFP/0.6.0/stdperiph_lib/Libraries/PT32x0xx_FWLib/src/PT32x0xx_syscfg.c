/**
	******************************************************************************
	* @file    PT32x0xx_syscfg.c
	* @author  Application development team
	* @version V1.0.0
	* @date    2025/2/13
	* @brief   This file provides firmware functions to manage the following
	*          functionalities of the SYSCFG peripheral:
	*          + Initialization and Configuration
	*          + Interrupt Vector Table Move To Sram Function
	*
	*******************************************************************************
	* @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of
  * the content or the codes within the content.
  *
  *  Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
	******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "PT32x0xx_syscfg.h"

/** @defgroup SYSCFG
  * @brief SYSCFG driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/**
  * @brief  Enable or disable the SYSCFG IAP function
  * @param  NewState: new state of the SYSCFG peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SYSCFG_IAPConfig(FunctionalState NewState)
{
#if defined (PT32G031x) || defined (PTM280x)
	return ;
#else
	/* Check the parameters */
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	SYSCFG->CR = SYSCFG_CR_KEY_VALUE;
	if(NewState != DISABLE)
	{
		SYSCFG->CR = SYSCFG_CR_IAP;
	}
	else
	{
		SYSCFG->CR = SYSCFG_CR_None;
	}
#endif
}


/**
  * @brief  Enable IVTS function/Interrupt vector table move to sram.
  * @param  Address: SRAM interrupt vector table address.
  *   		This parameter can be a number between 0x00 and IVTS_SRAM_END_ADDR.
  * @retval None
  */
void SYSCFG_VTORSEnable(u32 Address)
{
	/* Check the parameters */
	assert_param(IS_SYSCFG_VTORSAddress(Address));
	
	SYSCFG->SVTOR = Address;//Set the new address of the interrupt vector table in sram
	SYSCFG->SVTOR = SYSCFG_IVTS_KEY;//Write key to activate IVTS function
	while((SYSCFG->SR1 & SYSCFG_SR1_VTOS) != SYSCFG_SR1_VTOS);//Wait for hardware to set to 1
}

/**
  * @brief  Disable IVTS function.
  * @param  None
  * @retval None
  */
void SYSCFG_VTORSDisable(void)
{
#if defined (PT32x002x) || defined(PT32x012x) || defined (PTM160x)
	return ;
#else
	SYSCFG->SR1 = SYSCFG_SR1_VTOS;	//Clear the flag in SR1 register
#endif
}
