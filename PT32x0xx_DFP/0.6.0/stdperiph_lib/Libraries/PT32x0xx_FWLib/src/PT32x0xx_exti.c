/**
	*****************************************************************************
	* @file    PT32x0xx_exti.c
	* @author  Application development team
	* @version V1.0.0
	* @date    2025/2/13
	* @brief   This file contains all the functions prototypes for the EXTI firmware
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
	*****************************************************************************
**/

#include "PT32x0xx_exti.h"

/** @defgroup EXTI
  * @brief EXTI driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief	Enable or disable the specified exti interrupts.
  * @param	EXTIx: where x can be (A, B, C, D, E or F) to select the EXTI peripheral.
  * @param	EXTI_IT: specifies the EXTI interrupt sources to be enabled or disabled.
  *			This parameter can be any combination of EXTI_IT_x where x can be (0..15 / All).
  * @param  NewState: new state of the specified EXITx interrupts.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void EXTI_ITConfig(EXTI_TypeDef *EXTIx, u32 EXTI_IT, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_EXTI_ALL_PERIPH(EXTIx));
	assert_param(IS_EXTI_IT(EXTI_IT));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the specified exti interrupts */
		EXTIx->IER = EXTI_IT;
	}
	else
	{
		/* Disable the specified exti interrupts */
		EXTIx->IDR = EXTI_IT;
	}
}

/**
  * @brief 	Configure the EXTI interrupts trigger types for the specified EXTIx peripheral.
  * @param  EXTIx: where x can be (A, B, C, D, E or F) to select the EXTI peripheral.
  * @param  EXTI_Pin: specifies the port bit to be written.
  *			This parameter can be any combination of EXTI_Pin_x where x can be (0..15 / All).
  * @param  TriggerType: the source of EXTI Trigger.
  *   		This parameter can be one of the following values:
  * 		@arg EXTI_Trigger_Rising: set rising as the EXTI Trigger source
  * 		@arg EXTI_Trigger_Falling: set falling as the EXTI Trigger source
  * 		@arg EXTI_Trigger_RisingFalling: set rising and falling as the EXTI Trigger source
  * 		@arg EXTI_Trigger_HighLevel: set high level as the EXTI Trigger source
  * 		@arg EXTI_Trigger_LowLevel: set low level as the EXTI Trigger source
  * @retval None
  */
void EXTI_TriggerTypeConfig(EXTI_TypeDef *EXTIx, u32 EXTI_Pin, EXTI_Trigger TriggerType)
{
	u8 temp = 0;
	/* Check the parameters */
	assert_param(IS_EXTI_ALL_PERIPH(EXTIx));
	assert_param(IS_EXTI_Pin(EXTI_Pin));
	assert_param(IS_EXTI_Trigger(TriggerType));

	while(EXTI_Pin)
	{
		if((EXTI_Pin & 0x01) == 1)
		{
			EXTIx->CFGR[temp >> 3] &= ~(0x7 << (temp & 0x7) * 4);
			EXTIx->CFGR[temp >> 3] |= TriggerType << (temp & 0x7) * 4;
		}
		
		EXTI_Pin = EXTI_Pin >> 1;
		temp++;
	}	
}

/**
  * @brief  Clear the EXTI pending flags for the specified EXTIx peripheral.
  * @param  EXTIx: where x can be (A, B, C, D, E or F) to select the EXTI peripheral.
  * @param  EXTI_IT_FLAG: specifies the flag to clear.
  *         This parameter can be any combination of EXTI_FLAG_x where x can be (0..15 / All).
  * @retval None
  */
void EXTI_ClearFlag(EXTI_TypeDef *EXTIx, u32 EXTI_IT_FLAG)
{
	/* Check the parameters */
	assert_param(IS_EXTI_ALL_PERIPH(EXTIx));
	assert_param(IS_EXTI_FLAG(EXTI_IT_FLAG));
	
	EXTIx->SR = EXTI_IT_FLAG;
}

/**
  * @brief  Check whether the specified EXTI IT is set or not.
  * @param  EXTIx: where x can be (A, B, C, D, E or F) to select the EXTI peripheral.
  * @param  EXTI_IT: specifies the interrupt to check.
  *			This parameter can be any combination of EXTI_IT_x where x can be (0..15 / All).
  * @retval ITStatus: the new state of EXTI(SET or RESET).
  */
ITStatus EXTI_GetITStatus(EXTI_TypeDef *EXTIx, u32 EXTI_IT)
{
	ITStatus bitstatus;
	u32 enablestatus;
	
	/* Check the parameters */
	assert_param(IS_EXTI_ALL_PERIPH(EXTIx));
	assert_param(IS_EXTI_IT(EXTI_IT));

	enablestatus = (u32)(EXTIx->IER & EXTI_IT);
	if(((u32)(EXTIx->SR & EXTI_IT) != (u32)RESET) && (enablestatus != (u32)RESET))
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
  * @}
  */
