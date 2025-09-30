/**
	****************************************************************************
  * @file    PT32x0xx_nvic.c
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the NVIC firmware
  *          library
  *
  ******************************************************************************
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.  
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of
  * the content or the codes within the content.  
  * 
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved 
  ***************************************************************************
**/


/* Includes ------------------------------------------------------------------*/
#include "PT32x0xx_nvic.h"

/** @defgroup NVIC
  * @brief NVIC driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the NVIC peripheral according to the specified
  *         parameters in the NVIC_InitStruct.
  * @param  NVIC_InitStruct: pointer to a NVIC_InitTypeDef structure that contains
  *         the configuration information for the specified NVIC peripheral.
  * @retval None
  */
void NVIC_Init(NVIC_InitTypeDef *NVIC_InitStruct)
{
	u32 tmppriority;
	/* Check the parameters */
	assert_param(IS_FUNCTIONAL_STATE(NVIC_InitStruct->NVIC_IRQChannelCmd));
	assert_param(IS_NVIC_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelPriority));

	if(NVIC_InitStruct->NVIC_IRQChannelCmd != DISABLE)
	{
		/* Compute the Corresponding IRQ Priority --------------------------------*/
		tmppriority = NVIC->IP[NVIC_InitStruct->NVIC_IRQChannel >> 0x02];
		tmppriority &= (u32)(~(((u32)0xFF) << ((NVIC_InitStruct->NVIC_IRQChannel & 0x03) * 8)));
		tmppriority |= (u32)((((u32)NVIC_InitStruct->NVIC_IRQChannelPriority << 6) & 0xFF) << ((NVIC_InitStruct->NVIC_IRQChannel & 0x03) * 8));
		NVIC->IP[NVIC_InitStruct->NVIC_IRQChannel >> 0x02] = tmppriority;
		/* Enable the Selected IRQ Channels --------------------------------------*/
		NVIC->ISER[0] = (u32)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F);
	}
	else
	{
		/* Disable the Selected IRQ Channels -------------------------------------*/
		NVIC->ICER[0] = (u32)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F);
	}
}


/**
  * @brief  Selects the condition for the system to enter low power mode.
  * @param  LowPowerMode: Specifies the new mode for the system to enter low power mode.
  *          This parameter can be one of the following values:
  *            @arg NVIC_LP_SEVONPEND: Low Power SEV on Pend.
  *            @arg NVIC_LP_SLEEPDEEP: Low Power DEEPSLEEP request.
  *            @arg NVIC_LP_SLEEPONEXIT: Low Power Sleep on Exit.
  * @param  NewState: new state of LP condition.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void NVIC_SystemLPConfig(uint8_t LowPowerMode, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_NVIC_LP(LowPowerMode));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		SCB->SCR |= LowPowerMode;
	}
	else
	{
		SCB->SCR &= (u32)(~(u32)LowPowerMode);
	}
}

/**
  * @}
  */

