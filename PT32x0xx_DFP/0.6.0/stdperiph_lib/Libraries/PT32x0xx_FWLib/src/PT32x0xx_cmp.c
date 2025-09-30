/**
	****************************************************************************
  * @file    PT32x0xx_cmp.c
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the CMP firmware library
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


#include "PT32x0xx_cmp.h"

/** @defgroup CMP
  * @brief CMP driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the CMPx peripheral according to the specified parameters
  *         in the CMP_InitStruct.
  * @param  CMPx: where x can be 0 ,1, 2, or 3 to select the CMP peripheral.
  * @param  CMP_InitStruct: pointer to an CMP_InitTypeDef structure that contains
  *         the configuration information for the specified CMP peripheral.
  * @retval None
  */
void CMP_Init(CMP_TypeDef *CMPx, CMP_InitTypeDef *CMP_InitStruct)
{
	u32 tmpreg = 0;
	/* Check the parameters */
	assert_param(IS_CMP_ALL_PERIPH(CMPx));
	assert_param(IS_CMP_NegativeInput(CMP_InitStruct->CMP_NegativeInput));
	assert_param(IS_CMP_DigitalFilter(CMP_InitStruct->CMP_DigitalFilter));
	assert_param(IS_CMP_OutputPolarity(CMP_InitStruct->CMP_OutputPolarity));
	assert_param(IS_CMP_PositiveInput(CMP_InitStruct->CMP_PositiveInput));
	assert_param(IS_CMP_InitializationDelayTime(CMP_InitStruct->CMP_InitializationDelayTime));

	/* Get the CMPx CR1 value and Clear CNS/DFC/OPC/CPS bits */
	tmpreg = CMPx->CR1 & ~(CMP_CR1_CNS | CMP_CR1_DFC | CMP_CR1_OPC | CMP_CR1_CPS);
	/* Configure CMPx: NegativeInput, DigitalFilter, OutputPolarityand, CMP_PositiveInput */
	tmpreg |= CMP_InitStruct->CMP_NegativeInput | \
				CMP_InitStruct->CMP_DigitalFilter | \
				CMP_InitStruct->CMP_OutputPolarity | \
				CMP_InitStruct->CMP_PositiveInput;

	/* Write to CMPx CR1 */
	CMPx->CR1 = tmpreg;
	/* Write to CMPx CR2 */
	CMPx->CR2 = CMP_InitStruct->CMP_InitializationDelayTime;
}

/**
  * @brief  Fills each CMP_InitStruct member with its default value.
  * @param  CMP_InitStruct: pointer to an CMP_InitTypeDef structure which will
  *         be initialized.
  * @retval None
  */
void CMP_StructInit(CMP_InitTypeDef *CMP_InitStruct)
{
	CMP_InitStruct->CMP_NegativeInput = CMP_NegativeInput_CMPxN0;
	CMP_InitStruct->CMP_DigitalFilter = CMP_DigitalFilter_None;
	CMP_InitStruct->CMP_OutputPolarity = CMP_OutputPolarity_Normal;
	CMP_InitStruct->CMP_PositiveInput = CMP_PositiveInput_CMPxP0;
	CMP_InitStruct->CMP_InitializationDelayTime = 0x00;
}

/**
  * @brief  Enable or disable the specified CMP peripheral.
  * @param  CMPx: where x can be 0 ,1, 2, or 3 to select the CMP peripheral.
  * @param  NewState: new state of the CMPx peripheral.
  *       	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CMP_Cmd(CMP_TypeDef *CMPx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_CMP_ALL_PERIPH(CMPx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the selected CMP peripheral */
		CMPx->CR1 |= CMP_CR1_EN;
	}
	else
	{
		/* Disable the selected CMP peripheral */
		CMPx->CR1 &= ~CMP_CR1_EN;
	}
}

/**
  * @brief  Configure the power mode for the specified CMP.
  * @param 	CMPx: where x can be 0 ,1, 2, or 3 to select the CMP peripheral.
  * @param  Mode: Power mode selection of the CMP.
  *        	This parameter can be one of the following values:
  *		 	@arg CMP_PowerMode_LowPower: Low power consumption mode for optimal power consumption.
  * 	 	@arg CMP_PowerMode_HighSpeed: High speed mode for optimal performance.
  *	@retval None
  */
void CMP_PowerModeConfig(CMP_TypeDef *CMPx, u32 Mode)
{
	u32 tmpreg = 0;
	assert_param(IS_CMP_ALL_PERIPH(CMPx));
	assert_param(IS_CMP_PowerMode(Mode));
	/* Get the CMPx CR1 value and Clear CPMS bits */
	tmpreg = CMPx->CR1 & ~CMP_CR1_CPMS;
	/* Set CPMS bit according to Mode value */
	tmpreg |= Mode;
	/* Write to CR1 register*/
	CMPx->CR1 = tmpreg;
}

/**
  * @brief  Configure the output polarity for the specified CMP.
  * @param  CMPx: where x can be 0 ,1, 2, or 3 to select the CMP peripheral.
  * @param  SEL: Output polarity selection of the CMP.
  *        	This parameter can be one of the following values:
  *        	@arg CMP_OutputPolarity_Normal: Normal polarity
  *       	@arg CMP_OutputPolarity_Inverted: Reverse polarity
  *	@retval None
  */
void CMP_OutputPolarityConfig(CMP_TypeDef *CMPx, u32 Polarity)
{
	u32 tmpreg = 0;
	assert_param(IS_CMP_ALL_PERIPH(CMPx));
	assert_param(IS_CMP_OutputPolarity(Polarity));
	/* Get the CMPx CR1 value and Clear OPC bits */
	tmpreg = CMPx->CR1 & ~CMP_CR1_OPC;
	/* Set OPC bit according to Polarity value */
	tmpreg |= Polarity;
	/* Write to CR1 register*/
	CMPx->CR1 = tmpreg;
}

/**
  * @brief  Enable or disable the Comparator hysteresis function.
  * @param  CMPx: where x can be 0 ,1, 2, or 3 to select the CMP peripheral.
  * @param  NewState: new state of the CMPx peripheral.
  *      	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CMP_HysteresisCmd(CMP_TypeDef *CMPx, FunctionalState NewState)
{
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
	return ;
#else
	/* Check the parameters */
	assert_param(IS_CMP_ALL_PERIPH(CMPx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the comparator hysteresis function */
		CMPx->CR1 |= CMP_CR1_HYSE;
	}
	else
	{
		/* Disable the comparator hysteresis function */
		CMPx->CR1 &= ~CMP_CR1_HYSE;
	}
#endif
}

/**
  * @brief  Configure the negative intput for the specified CMP.
  * @param  CMPx: where x can be 0 ,1, 2, or 3 to select the CMP peripheral.
  * @param  NegativeIntput: the negative intput of the CMP.
  *        	This parameter can be one of the following values:
  *         @arg CMP_NegativeInput_CMPxN: Select CMPxN as the negative input
  *         @arg CMP_NegativeInput_LDAC: Select LDAC as the negative input
  *         @arg CMP_NegativeInput_BG1V0: Select BG1V0 as the negative input
  *	@retval None
  */
void CMP_NegativeIntputConfig(CMP_TypeDef *CMPx, u32 NegativeIntput)
{
	u32 tmpreg = 0;
	assert_param(IS_CMP_ALL_PERIPH(CMPx));
	assert_param(IS_CMP_NegativeInput(NegativeIntput));
	/* Get the CMPx CR1 value and Clear CNS bits */
	tmpreg = CMPx->CR1 & ~CMP_CR1_CNS;
	/* Set CNS bit according to NegativeIntput value */
	tmpreg |= NegativeIntput;
	/* Write to CR1 register*/
	CMPx->CR1 = tmpreg;
}

/**
  * @brief  Configure the digital filter for the specified CMP.
  * @param  CMPx: where x can be 0 ,1, 2, or 3 to select the CMP peripheral.
  * @param  Filter: Digital filter level of the CMP.
  *        	This parameter can be one of the following values:
  *         @arg CMP_DigitalFilter_None: signal output without digita filtering.
  *         @arg CMP_DigitalFilter_4: digita filter level is 4.
  *         @arg CMP_DigitalFilter_8: digita filter level is 8.
  *         @arg CMP_DigitalFilter_16: digita filter level is 16.
  *         @arg CMP_DigitalFilter_32: digita filter level is 32.
  *         @arg CMP_DigitalFilter_64: digita filter level is 64.
  *         @arg CMP_DigitalFilter_128: digita filter level is 128.
  *         @arg CMP_DigitalFilter_256: digita filter level is 256.
  *	@retval None
  */
void CMP_DigitalFilterConfig(CMP_TypeDef *CMPx, u32 Filter)
{
	u32 tmpreg = 0;
	assert_param(IS_CMP_ALL_PERIPH(CMPx));
	assert_param(IS_CMP_DigitalFilter(Filter));
	/* Get the CMPx CR1 value and Clear DFC bits */
	tmpreg = CMPx->CR1 & ~CMP_CR1_DFC;
	/* Set DFC bit according to Filter value */
	tmpreg |= Filter;
	/* Write to CR1 register*/
	CMPx->CR1 = tmpreg;
}

/**
  * @brief  Configure the Comparator hysteresis voltage for the specified CMP.
  * @param 	CMPx: where x can be 0 ,1, 2, or 3 to select the CMP peripheral.
  * @param  Voltage: Hysteresis Voltage of the CMP.
  *         @arg CMP_HysteresisVoltage_5mV: hysteresis voltage 5mV.
  *         @arg CMP_HysteresisVoltage_10mV: hysteresis voltage 10mV.
  *         @arg CMP_HysteresisVoltage_20mV: hysteresis voltage 20mV.
  *         @arg CMP_HysteresisVoltage_100mV: hysteresis voltage 100mV.
  *	@retval None
  */
void CMP_HysteresisVoltageConfig(CMP_TypeDef *CMPx, u32 Voltage)
{
	u32 tmpreg = 0;
	assert_param(IS_CMP_ALL_PERIPH(CMPx));
	assert_param(IS_CMP_HysteresisVoltage(Voltage));
	/* Get the CMPx CR1 value and Clear HVS bits */
	tmpreg = CMPx->CR1 & ~CMP_CR1_HVS;
	/* Set HVS bit according to Voltage value */
	tmpreg |= Voltage;
	/* Write to CR1 register*/
	CMPx->CR1 = tmpreg;
}

/**
  * @brief  Configure the positive intput for the specified CMP peripheral.
  * @param  CMPx: where x can be 0 ,1, 2, or 3 to select the CMP peripheral.
  * @param  SEL: Positive intput selection of the CMP.
  *       	This parameter can be one of the following values:
  *       	@arg IS_CMP_PositiveInput(SEL): The value of SEL that satisfies the condition
  *	@retval None
  */
void CMP_PositiveIntputConfig(CMP_TypeDef *CMPx, u32 PositiveIntput)
{
	u32 tmpreg = 0;
	assert_param(IS_CMP_ALL_PERIPH(CMPx));
	assert_param(IS_CMP_PositiveInput(PositiveIntput));

	tmpreg = CMPx->CR1 & ~CMP_CR1_CPS;
	tmpreg |= PositiveIntput;
	CMPx->CR1 = tmpreg;
}

/**
  * @brief  Enable or disable the output synchronization for the specified CMP.
  * @param  CMPx: where x can be 0 ,1, 2, or 3 to select the CMP peripheral.
  * @param  NewState: new state of the CMPx peripheral.
  *        	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CMP_OutputSynchronization(CMP_TypeDef *CMPx, FunctionalState NewState)
{
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
	return ;
#else
	/* Check the parameters */
	assert_param(IS_CMP_ALL_PERIPH(CMPx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		CMPx->CR1 |= CMP_CR1_SYNC;
	}
	else
	{
		CMPx->CR1 &= ~CMP_CR1_SYNC;
	}
#endif
}

/**
  * @brief  Configure the comparator output PWM polarity control for the specified CMP.
  * @param  CMPx: where x can be 0 ,1, 2, or 3 to select the CMP peripheral.
  * @param  Polarity: Output PWM polarity selection of the CMP.
  *       	This parameter can be one of the following values:
  *         @arg CMP_OutputPWMPolarity_Low: Normal polarity
  *         @arg CMP_OutputPWMPolarity_High: Reverse polarity
  *	@retval None
  */
void CMP_OutputPWMPolarityConfig(CMP_TypeDef *CMPx, u32 Polarity)
{
	u32 tmpreg = 0;
	assert_param(IS_CMP_ALL_PERIPH(CMPx));
	assert_param(IS_CMP_OutputPWMPolarity(Polarity));

	tmpreg = CMPx->CR1 & ~CMP_CR1_PPC;
	tmpreg |= Polarity;
	CMPx->CR1 = tmpreg;
}

/**
  * @brief  Enable or disable output PWM function for the specified CMP peripheral.
  * @param  CMPx: where x can be 0 ,1, 2, or 3 to select the CMP peripheral.
  * @param  TIMx_PWMx: select the CMP output PWM.
  *        	This parameter can be one of the following values:
  *         @arg The value of TIMx_PWMx that satisfies the condition
  * @param  NewState: new state of the specified CMP function.
  *        	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */

void CMP_OutputPWMConfig(CMP_TypeDef *CMPx, u32 TIMx_PWMx)
{
	u32 tmpreg = 0;
	/* Check the parameters */
	assert_param(IS_CMP_ALL_PERIPH(CMPx));
	assert_param(IS_CMP_OutputPWM(TIMx_PWMx));

	tmpreg = CMPx->CR1 & ~CMP_CR1_PWMS;
	tmpreg |= TIMx_PWMx;
	CMPx->CR1 = tmpreg;
}

/**
  * @brief  Enable or disable the specified CMP interrupts for the specified CMP peripheral.
  * @param 	CMPx: where x can be 0 ,1, 2, or 3 to select the CMP peripheral.
  * @param  CMP_IT: specifies the OPA interrupt source to be enabled or disabled.
  *        	This parameter can be one of the following values:
  *         @arg CMP_IT_COF: Comparator output falling edge trigger interrupt enable control
  *         @arg CMP_IT_COR: Comparator output rising edge trigger interrupt enable control
  *         @arg CMP_IT_COFW: Comparator output falling edge trigger interrupt enable control(Sleep Mode)
  *         @arg CMP_IT_CORW: Comparator output rising edge trigger interrupt enable control(Sleep Mode)
  * @param  NewState: new state of the specified CMP interrupt.
  *        	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CMP_ITConfig(CMP_TypeDef *CMPx, u32 CMP_IT, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_CMP_ALL_PERIPH(CMPx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	assert_param(IS_CMP_IT(CMP_IT));

	if(NewState != DISABLE)
	{
		CMPx->IER |= CMP_IT;
	}
	else
	{
		CMPx->IER &= ~CMP_IT;
	}
}

/**
  * @brief  Check whether the specified CMP flag is set or not.
  * @param 	CMPx: where x can be 0 ,1, 2, or 3 to select the CMP peripheral.
  * @param  CMP_FLAG: specifies the flag to check.
  *       	This parameter can be one of the following values:
  *         @arg CMP_FLAG_COF: Comparator output falling edge trigger interrupt enable control
  *         @arg CMP_FLAG_COR: Comparator output rising edge trigger interrupt enable control
  *         @arg CMP_FLAG_COFW: Comparator output falling edge trigger interrupt enable control(Sleep Mode)
  *         @arg CMP_FLAG_CORW: Comparator output rising edge trigger interrupt enable control(Sleep Mode)
  * @retval The new state of CMP_FLAG (SET or RESET).
  */
FlagStatus CMP_GetFlagStatus(CMP_TypeDef *CMPx, u32 CMP_FLAG)
{
	FlagStatus bitstatus = RESET;
	/* Check the parameters */
	assert_param(IS_CMP_ALL_PERIPH(CMPx));
	assert_param(IS_CMP_FLAG(CMP_FLAG));

	/* Check the status of the specified ADC flag */
	if((CMPx->SR & CMP_FLAG) != (u32)RESET)
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
  * @brief  Clear the CMPx's pending flags.
  * @param 	CMPx: where x can be 0 ,1, 2, or 3 to select the CMP peripheral.
  * @param  CMP_FLAG: specifies the flag to clear.
  *       	This parameter can be any combination of the following values:
  *         @arg CMP_FLAG_COF: Comparator output falling edge trigger interrupt enable control
  *         @arg CMP_FLAG_COR: Comparator output rising edge trigger interrupt enable control
  *         @arg CMP_FLAG_COFW: Comparator output falling edge trigger interrupt enable control(Sleep Mode)
  *         @arg CMP_FLAG_CORW: Comparator output rising edge trigger interrupt enable control(Sleep Mode)
  * @retval None
  */
void CMP_ClearFlag(CMP_TypeDef *CMPx, u32 CMP_FLAG)
{
	/* Check the parameters */
	assert_param(IS_CMP_ALL_PERIPH(CMPx));
	assert_param(IS_CMP_CLEAR_FLAG(CMP_FLAG));
	
	CMPx->SR = CMP_FLAG;
}

/**
  * @brief  Check whether the specified CMPx's interrupt flag is set or not.
  * @param 	CMPx: where x can be 0 ,1, 2, or 3 to select the CMP peripheral.
  * @param  CMP_IT: specifies the IT to check.
  *       	This parameter can be one of the following values:
  *         @arg CMP_IT_COF: Comparator output falling edge trigger interrupt enable control
  *         @arg CMP_IT_COR: Comparator output rising edge trigger interrupt enable control
  *         @arg CMP_IT_COFW: Comparator output falling edge trigger interrupt enable control(Sleep Mode)
  *         @arg CMP_IT_CORW: Comparator output rising edge trigger interrupt enable control(Sleep Mode)
  * @retval ITStatus: The new state of CMP_IT (SET or RESET).
  */
ITStatus CMP_GetITStatus(CMP_TypeDef *CMPx, u32 CMP_IT)
{
	ITStatus bitstatus;
	/* Check the parameters */
	assert_param(IS_CMP_ALL_PERIPH(CMPx));
	assert_param(IS_CMP_IT(CMP_IT));
	
	if((CMPx->SR & CMP_IT) != 0 && ( CMPx->IER & CMP_IT) != 0)
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
  * @brief  Get current comparator positive intput.
  * @param  CMPx: where x can be 0 ,1, 2, or 3 to select the CMP peripheral.
  * @retval Status: The new state of CMP_CPS (SET or RESET).
  */
u8 CMP_GetPositiveIntput(CMP_TypeDef *CMPx)
{
	/* Check the parameters */
	assert_param(IS_CMP_ALL_PERIPH(CMPx));
#if defined (PT32G031x)
	return 0;
#else
	return ((CMPx->SR & CMP_SR_CPS)>>24);
#endif
}

/**
  * @brief  Configure the initialization delay time for the specified CMP peripheral.
  * @param  CMPx: where x can be 0 ,1, 2, or 3 to select the CMP peripheral.
  * @param  Time: This parameter can be a number between 0 and 0x7FF.
  *	@retval None
  */
void CMP_InitializationDelayTime(CMP_TypeDef *CMPx, u32 Time)
{
	u32 tmpreg = 0;
	assert_param(IS_CMP_ALL_PERIPH(CMPx));
	assert_param(IS_CMP_InitializationDelayTime(Time));

	tmpreg = CMPx->CR2 & ~CMP_CR2_DELAY;
	tmpreg |= Time;
	CMPx->CR2 = tmpreg;
}

/**
  * @brief  Configure the sampling clock division for the specified CMP peripheral.
  * @param  CMPx: where x can be 0 ,1, 2, or 3 to select the CMP peripheral.
  * @param  CMP_ClockDiv: sampling clock division.
  * @retval None
  */
void CMP_SetClockDivision(CMP_TypeDef *CMPx, u8 CMP_ClockDiv)
{
	u32 tmpreg = 0;
	/* Check the parameters */
	assert_param(IS_CMP_ALL_PERIPH(CMPx));
	assert_param(IS_CMP_ClockDiv(CMP_ClockDiv));

	tmpreg = CMPx->CR2 & ~CMP_CR2_CKD;
	tmpreg |= (CMP_ClockDiv << 24);
	CMPx->CR2 = tmpreg;
}

/**
  * @brief  Enable or Disable the scan queue for the specified CMP peripheral.
  * @param  CMPx: where x can be 0 ,1, 2, or 3 to select the CMP peripheral.
  * @param  ScanQueue: Select scan queue.
  * @retval None
  */
void CMP_ScanQueueConfig(CMP_TypeDef *CMPx,u8 ScanQueue)
{
	u32 tmpreg = 0;
	/* Check the parameters */
	assert_param(IS_CMP_ALL_PERIPH(CMPx));
	assert_param(IS_CMP_ScanQueue(ScanQueue));
	
	tmpreg = CMPx->CR3 & ~(CMP_Scan_P0E | CMP_Scan_P1E | CMP_Scan_P2E | CMP_Scan_P3E | CMP_Scan_P4E | CMP_Scan_P5E | CMP_Scan_P6E);
	tmpreg |= ScanQueue;
	CMPx->CR3 = tmpreg;
}

/**
  * @brief  Configure the scan time for the specified CMP peripheral.
  * @param  CMPx: where x can be 0 ,1, 2, or 3 to select the CMP peripheral.
  * @param  Time: scan time.
  * @retval None
  */
void CMP_ScanTimeConfig(CMP_TypeDef *CMPx,u8 Time)
{
	u32 tmpreg = 0;
	/* Check the parameters */
	assert_param(IS_CMP_ALL_PERIPH(CMPx));
	assert_param(IS_CMP_ScanTime(Time));

	tmpreg = CMPx->CR3 & ~CMP_CR3_SCAN;
	tmpreg |= (Time << 8);
	CMPx->CR3 = tmpreg;
}
