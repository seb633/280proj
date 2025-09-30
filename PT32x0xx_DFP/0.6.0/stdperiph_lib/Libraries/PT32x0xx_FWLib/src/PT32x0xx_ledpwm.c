/**
	****************************************************************************
  * @file    PT32x0xx_ledpwm.c
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the PWM firmware library.
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
#include "PT32x0xx_ledpwm.h"

/** @defgroup LEDPWM
  * @brief LEDPWM driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the LEDPWMx  peripheral according to
  *         the specified parameters in the LEDPWM_Init.
  * @param  LEDPWMx: where x can be 0 to select the LEDPWM peripheral.
  * @param  LEDPWMInit: pointer to a LEDPWM_InitTypeDef
  *         structure that contains the configuration information for
  *         the specified LEDPWM peripheral.
  * @retval None
  */
void LEDPWM_Init(LEDPWM_TypeDef *LEDPWMx, LEDPWM_InitTypeDef *LEDPWMInit)
{
	u32  tmpreg;
	/* Check the parameters */
	assert_param(IS_LEDPWM_ALL_PERIPH(LEDPWMx));
	assert_param(IS_LEDPWM_Code0Duty(LEDPWMInit->LEDPWM_Code0Duty));
	assert_param(IS_LEDPWM_Code1Duty(LEDPWMInit->LEDPWM_Code1Duty));
	assert_param(IS_LEDPWM_Cycle0(LEDPWMInit->LEDPWM_Cycle0));
	assert_param(IS_LEDPWM_Cycle1(LEDPWMInit->LEDPWM_Cycle1));
	assert_param(IS_LEDPWM_DataCodeCycleControl(LEDPWMInit->LEDPWM_CycleControl));
	assert_param(IS_LEDPWM_OutputPolarity(LEDPWMInit->LEDPWM_OutputPolarity));
	assert_param(IS_LEDPWM_DataCheck(LEDPWMInit->LEDPWM_DataCheck));
	
	tmpreg = LEDPWMx->CR1 & ~(LEDPWM_CR1_CC | LEDPWM_CR1_OP | LEDPWM_CR1_DCC);
	tmpreg |= LEDPWMInit->LEDPWM_DataCheck | LEDPWMInit->LEDPWM_OutputPolarity | LEDPWMInit->LEDPWM_CycleControl;
	LEDPWMx->CR1 = tmpreg;
	
	/* Configure LEDPWMx: code0duty, code1duty, cycle0 and cycle1 */
	LEDPWMx->CR2 = LEDPWMInit->LEDPWM_Code0Duty | (LEDPWMInit->LEDPWM_Code1Duty << 8) | (LEDPWMInit->LEDPWM_Cycle0 << 16) | (LEDPWMInit->LEDPWM_Cycle1 << 24);
}

/**
  * @brief  Fills each LEDPWMInit member with its default value.
  * @param  LEDPWMInit: pointer to a LEDPWM_InitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void LEDPWM_StructInit(LEDPWM_InitTypeDef *LEDPWMInit)
{
	/* Set the default configuration */
	LEDPWMInit->LEDPWM_Code0Duty = 0x0D;
	LEDPWMInit->LEDPWM_Code1Duty = 0x2E;
	LEDPWMInit->LEDPWM_Cycle0 = 0x3B;
	LEDPWMInit->LEDPWM_Cycle1 = 0x3B;
	LEDPWMInit->LEDPWM_CycleControl = LEDPWM_DataCodeCycleControl_Share;
	LEDPWMInit->LEDPWM_OutputPolarity = LEDPWM_OutputPolarity_Normal;
	LEDPWMInit->LEDPWM_DataCheck = LEDPWM_DataCheck_0;
}

/**
  * @brief  Enable or Disable the specified LEDPWM peripheral.
  * @param  LEDPWMx: where x can be 0 to select the LEDPWM peripheral.
  * @param  NewState: new state of the LEDPWMx peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void LEDPWM_Cmd(LEDPWM_TypeDef *LEDPWMx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_LEDPWM_ALL_PERIPH(LEDPWMx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if(NewState == ENABLE)
	{
		/* Set the enable bit */
		LEDPWMx->CR1 |= LEDPWM_CR1_EN;
	}
	else
	{
		/* Reset the enable bit */
		LEDPWMx->CR1 &= ~LEDPWM_CR1_EN;
	}
}

/**
 * @brief  Transmits data through the UARTx LEDPWMx.
 * @param  LEDPWMx: where x can be 0 to select the LEDPWM peripheral
 * @param  Data: the data to transmit.
 * @param  Length: the valid data bits of Data.
 * @param  isCheck: the valid data needs to be checked or not
 * @retval None
 */
void LEDPWM_SendData(LEDPWM_TypeDef *LEDPWMx, u32 Data, u8 Length, BOOL isCheck)
{
	/* Check the parameters */
	assert_param(IS_LEDPWM_ALL_PERIPH(LEDPWMx));
	assert_param(IS_LEDPWM_Data(Data));
	assert_param(IS_LEDPWM_DataLength(Length));
	
	if(isCheck)
	{
		LEDPWMx->DR = Data | Length | LEDPWM_DR_CE;
	}
	else
	{
		LEDPWMx->DR = Data | Length;
	}
	
	while((LEDPWMx->SR & LEDPWM_FLAG_TXC) == 0);
}

/**
  * @brief  Check whether the specified LEDPWM flag is set or not.
  * @param  LEDPWMx: where x can be 0 to select the LEDPWM peripheral.
  * @param  LEDPWM_FLAG: specifies the flag to check.
  *       	This parameter can be one of the following values:
  *        	@arg LEDPWM_FLAG_TXE: Data code Transmit empty flag.
  *       	@arg LEDPWM_FLAG_TXC: Data code Transmit completed flag.
  * @retval The new state of LEDPWM_FLAG (SET or RESET).
  */
FlagStatus LEDPWM_GetFlagStatus(LEDPWM_TypeDef *LEDPWMx, u32 LEDPWM_FLAG)
{
	FlagStatus bitstatus = RESET;
	/* Check the parameters */
	assert_param(IS_LEDPWM_ALL_PERIPH(LEDPWMx));
	assert_param(IS_LEDPWM_FLAG(LEDPWM_FLAG));
	/* Check the status of the specified LEDPWM flag */
	if((LEDPWMx->SR & LEDPWM_FLAG) != 0)
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
  * @brief  Check whether the specified LEDPWM intterrupt status is set or not.
  * @param  LEDPWMx: where x can be 0 to select the LEDPWM peripheral.
  * @param  LEDPWM_IT: specifies the LEDPWM interrupt sources to be enabled or disabled.
  *       	This parameter can be one of the following values:
  *        	@arg LEDPWM_IT_TXE: Data code Transmit empty interrupt.
  *        	@arg LEDPWM_IT_TXC: Data code Transmit completed interrupt.
  * @return ITStatus of LEDPWM_IT (SET or RESET).
  */
ITStatus LEDPWM_GetITStatus(LEDPWM_TypeDef *LEDPWMx, u32 LEDPWM_IT)
{
	ITStatus bitstatus;

	/* Check the parameters */
	assert_param(IS_LEDPWM_ALL_PERIPH(LEDPWMx));
	assert_param(IS_LEDPWM_IT(LEDPWM_IT));

	/* Check the status of the specified LEDPWM flag */
	if((LEDPWMx->IER & LEDPWM_IT) != 0 && (LEDPWMx->SR & LEDPWM_IT) != RESET)
	{
		/* LEDPWM_FLAG is set */
		bitstatus = SET;
	}
	else
	{
		/* LEDPWM_FLAG is reset */
		bitstatus = RESET;
	}
	
	/* Return the LEDPWM_FLAG status */
	return bitstatus;
}

/**
  * @brief  Clear the LEDPWMx's pending flags.
  * @param  LEDPWMx: where x can be 0 to select the LEDPWM peripheral.
  * @param  LEDPWM_CLEAR_FLAG: specifies the flag to clear.
  *       	This parameter can be one of the following values:
  *        	@arg LEDPWM_FLAG_DTXE: Data code Transmit empty flag.
  *       	@arg LEDPWM_FLAG_RTXC: Reset code Transmit completed flag.
  * @retval None
  */
void LEDPWM_ClearFlag(LEDPWM_TypeDef *LEDPWMx, u32 LEDPWM_CLEAR_FLAG)
{
	/* Check the parameters */
	assert_param(IS_LEDPWM_ALL_PERIPH(LEDPWMx));
	assert_param(IS_LEDPWM_FLAG(LEDPWM_CLEAR_FLAG));

	LEDPWMx->SR = LEDPWM_CLEAR_FLAG;
}

/**
  * @brief  Enable or disable the specified LEDPWM interrupts.
  * @param  LEDPWMx: where x can be 0 to select the LEDPWM peripheral.
  * @param  LEDPWM_IT: specifies the LEDPWM interrupt sources to be enabled or disabled.
  *       	This parameter can be one of the following values:
  *        	@arg LEDPWM_IT_DTXE: Data code Transmit empty interrupt.
  *        	@arg LEDPWM_IT_RTXC: Reset code Transmit completed interrupt.
  * @param  NewState: new state of the specified LEDPWMx interrupts.
  *        	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void LEDPWM_ITConfig(LEDPWM_TypeDef *LEDPWMx, u32 LEDPWM_IT, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_LEDPWM_ALL_PERIPH(LEDPWMx));
	assert_param(IS_LEDPWM_IT(LEDPWM_IT));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the selected LEDPWM interrupts */
		LEDPWMx->IER |= LEDPWM_IT;
	}
	else
	{
		/* Disable the selected LEDPWM interrupts */
		LEDPWMx->IER &= ~LEDPWM_IT;
	}
}

