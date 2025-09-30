/*******************************************************************************
 * @file    PT32x0xx_iwdg.c
 * @author  Application development team
 * @version V1.0.0
 * @date    2025/2/13
 * @brief   This file provides firmware functions to manage the following
 *          functionalities of the Independent watchdog (IWDG) peripheral:
 *          + Prescaler and Counter configuration
 *          + IWDG activation
 *          + Flag management
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
#include "PT32x0xx_iwdg.h"

/** @defgroup IWDG
  * @brief IWDG driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
  
 /**
  * @brief  Configure the Reload value for the specified IWDG peripheral.
  * @param  IWDGx: select the IWDG peripheral.
  * @param  Reload: specifies the IWDG Reload value.
  *         This parameter must be a number between 0x00 and 0xFFFFFFFF.
  * @retval None
  */
void IWDG_SetReload(IWDG_TypeDef *IWDGx, u32 Reload)
{
	/* Check the parameters */
	assert_param(IS_IWDG_ReloadValue(Reload));
	assert_param(IS_IWDG_ALL_PERIPH(IWDGx));
	/* Write to RLR register */
	IWDGx->RLR = Reload;
}

/**
  * @brief  Enable or Disable the specified IWDG peripheral.
  * @param  IWDGx: select the IWDG peripheral.
  * @param  NewState: new state of the IWDGx peripheral.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void IWDG_Cmd(IWDG_TypeDef *IWDGx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_IWDG_ALL_PERIPH(IWDGx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the IWDG peripheral*/
		IWDGx->CR |= IWDG_CR_EN;
	}
	else
	{
		/* Disable the IWDG peripheral*/
		IWDGx->CR &= ~IWDG_CR_EN;
	}
}

/**
  * @brief  Enable or disable DebugPending for the specified IWDG peripheral. 
  * @param  IWDGx: select the IWDG peripheral.
  * @param  NewState: new state of the IWDGx peripheral.
  *			This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void IWDG_DBGPendingCmd(IWDG_TypeDef *IWDGx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_IWDG_ALL_PERIPH(IWDGx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the IWDG DebugPending */
		IWDGx->CR |= IWDG_CR_DBGE;
	}
	else
	{
		/* Disable the IWDG DebugPending*/
		IWDGx->CR &= ~IWDG_CR_DBGE;
	}
}

/**
  * @brief	Reloads the counter for the specified IWDG peripheral.
  * @param	IWDGx: select the IWDG peripheral.
  * @retval	None
  */
void IWDG_ReloadCounter(IWDG_TypeDef *IWDGx)
{
	/* Check the parameters */
	assert_param(IS_IWDG_ALL_PERIPH(IWDGx));
	/* Feed dog */
	IWDGx->KR = 1;
}

/**
  * @brief  Check whether the specified IWDG flag is set or not.
  * @param  IWDGx: select the IWDG peripheral.
  * @param  IWDG_FLAG: specifies the flag to check.
  *         This parameter can be one of the following values:
  * 		@arg IWDG_FLAG_HungryDog: hungry dog flag.
  * 		@arg IWDG_FLAG_LockStatus: Lock status.
  * @retval The new state of IWDG_FLAG (SET or RESET).
  */
FlagStatus IWDG_GetFlagStatus(IWDG_TypeDef *IWDGx, IWDG_FLAG Flag)
{
	FlagStatus bitstatus = RESET;
	/* Check the parameters */
	assert_param(IS_IWDG_FLAG(Flag));
	assert_param(IS_IWDG_ALL_PERIPH(IWDGx));
	/* Check the status of the specified IWDG flag */
	if(Flag == IWDG_FLAG_HungryDog)
	{
		if((IWDGx->SR & IWDG_SR_HDF) != (u32)RESET)
		{
			bitstatus = SET;
		}
		else
		{
			bitstatus = RESET;
		}
	}
	else if(Flag == IWDG_FLAG_LockStatus)
	{
		if((IWDGx->LR & IWDG_LR_LOCK) != (u32)RESET)
		{
			bitstatus = SET;
		}
		else
		{
			bitstatus = RESET;
		}
	}
	/* Return the flag status */
	return bitstatus;
}

/**
  * @brief  Enable or disable write access all registers for the specified 
  *			IWDG peripheral.
  * @param  IWDGx: select the IWDG peripheral.
  * @param  Key: new state of write access to all registers.
  *			This parameter can be one of the following values:
  * 		@arg IWDG_LockKey_Unlock: Enable write access to all registers
  * 		@arg IWDG_LockKey_Lock: Disable write access to all registers
  * @retval None
  */
void IWDG_LockCmd(IWDG_TypeDef *IWDGx, u32 Key)
{
	/* Check the parameters */
	assert_param(IS_IWDG_ALL_PERIPH(IWDGx));
	assert_param(IS_IWDG_LockKey(Key));
	/* Write the lock key value */
	IWDGx->LR = Key;
}

/**
  * @}
  */
