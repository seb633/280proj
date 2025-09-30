/**
	****************************************************************************
  * @file    PT32x0xx_irtim.c
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the IRTIM firmware
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
  
  #include "PT32x0xx_irtim.h"
  
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
  * @brief  Initializes the IRTIM peripheral according to the specified
  *         parameters in the UART_InitStruct .
  * @param  pIRTIM: the IRTIM peripheral.
  * @param  IRTIM_InitStruct: pointer to a IRTIM_InitTypeDef structure that contains
  *         the configuration information for the specified IRTIM peripheral.
  * @retval None
  */
void IRTIM_Init(IRTIM_TypeDef *IRTIMx, IRTIM_InitTypeDef *IRTIM_InitStruct)
{
	/* Check the parameters */
	assert_param(IS_IRTIM_ALL_PERIPH(IRTIMx));
	assert_param(IS_IRTIM_DSS(IRTIM_InitStruct->IRTIM_DataSource));
	assert_param(IS_IRTIM_CSS(IRTIM_InitStruct->IRTIM_CarrierSource));
	
#if !defined(PT32x060x) && !defined(PT32x063x) && !defined(PT32x066x)	
	assert_param(IS_IRTIM_DSP(IRTIM_InitStruct->IRTIM_DataPolarity));
	IRTIMx->CFGR = IRTIM_InitStruct->IRTIM_DataSource | IRTIM_InitStruct->IRTIM_CarrierSource | IRTIM_InitStruct->IRTIM_DataPolarity;
#else
	IRTIMx->CFGR = IRTIM_InitStruct->IRTIM_DataSource | IRTIM_InitStruct->IRTIM_CarrierSource;
#endif	
}

/**
  * @brief  Fills each IRTIM_InitStruct member with its default value.
  * @param  IRTIM_InitStruct: pointer to a IRTIM_InitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void IRTIM_StructInit(IRTIM_InitTypeDef *IRTIM_InitStruct)
{
	/* IRTIM_InitStruct members default value */
	IRTIM_InitStruct->IRTIM_DataSource = IRTIM_DSS_UART0_USART0_TX;
	IRTIM_InitStruct->IRTIM_CarrierSource = IRTIM_CSS_LSI;
#if !defined(PT32x060x) && !defined(PT32x063x) && !defined(PT32x066x)
	IRTIM_InitStruct->IRTIM_DataPolarity = IRTIM_DSP_Normal;
#endif	
}

/**
  * @brief  Enable or disable the specified IRTIM peripheral.
  * @param  pIRTIM: the IRTIM peripheral.
  * @param  NewState: new state of the IRTIM peripheral.
  *       	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void IRTIM_Cmd(IRTIM_TypeDef *IRTIMx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_IRTIM_ALL_PERIPH(IRTIMx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the selected IRTIM by setting the EN bit in the CR register */
		IRTIMx->CR |= IRTIM_CR_EN;
	}
	else
	{
		/* Disable the selected IRTIM by clearing the EN bit in the CR register */
		IRTIMx->CR &= ~IRTIM_CR_EN;
	}
}

/**
 * @brief  Configure the output polarity for specified IRTIM peripheral.
 * @param  IRTIMx: the specified IRTIM peripheral.
 * @param  Polarity: IRTIM output polarity.
 *       	This parameter can be any combination of the following values:
 *        	@arg IRTIM_OutputPolarity_Normal: IRTIM output normal polarity
 *        	@arg IRTIM_OutputPolarity_Reverse: IRTIM output reverse polarity
 * @retval None
 */
void IRTIM_OutputPolarityControl(IRTIM_TypeDef *IRTIMx, u32 Polarity)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_IRTIM_ALL_PERIPH(IRTIMx));
	assert_param(IS_IRTIM_OutputPolarity(Polarity));
		
	tmpreg = IRTIMx->CR & ~IRTIM_CR_OP;
	tmpreg |= Polarity;
	IRTIMx->CR = tmpreg;
}

