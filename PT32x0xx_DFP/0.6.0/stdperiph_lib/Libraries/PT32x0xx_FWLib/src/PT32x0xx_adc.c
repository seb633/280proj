/**
	******************************************************************************
  * @file    PT32x0xx_adc.c
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the ADC firmware
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

#include "PT32x0xx_adc.h"

/** @defgroup ADC
  * @brief ADC driver modules
  * @{
  */
	
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief	Initializes the ADCx peripheral according to
  *         the specified parameters in the ADC_InitTypeDef.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	ADC_InitStruct: pointer to a ADC_InitTypeDef
  *         structure that contains the configuration information for
  *         the specified ADC peripheral.
  * @retval	None
  */
void ADC_Init(ADC_TypeDef *ADCx, ADC_InitTypeDef *ADC_InitStruct)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_Mode(ADC_InitStruct->ADC_Mode));
	assert_param(IS_ADC_RVSPS(ADC_InitStruct->ADC_RVSPS));
	assert_param(IS_ADC_Channel(ADC_InitStruct->ADC_Channel));
	assert_param(IS_ADC_Prescaler(ADC_InitStruct->ADC_Prescaler));
	assert_param(IS_ADC_SampleTime(ADC_InitStruct->ADC_SampleTime));
#if defined(PT32G031x) || defined (PTM280x)
	assert_param(IS_ADC_ChannelSetupTime(ADC_InitStruct->ADC_ChannelSetupTime));
#endif

	tmpreg = ADCx->CR2 & ~ADC_CR2_MODE;
	tmpreg |= ADC_InitStruct->ADC_Mode ;
	ADCx->CR2 = tmpreg;
	/* Get the ADCx CFGR1 value and Clear RVSPS bits */
	tmpreg = ADCx->CFGR1 & ~ADC_CFGR1_RVSPS;
	/* Configure ADCx: reference voltage source positive selection */
	/* Set ADC_RVSPS bit according to ADC_RVSPS value */
	tmpreg |= ADC_InitStruct->ADC_RVSPS;
	/* Write to CFGR1 register*/
	ADCx->CFGR1 = tmpreg;
	/* Enable BGREF*/
	if(ADC_InitStruct->ADC_RVSPS == ADC_RVSPS_BGREF)
	{
		tmpreg |= ADC_CFGR1_BGE;
	}
	/* Get the ADCx CFGR2 value and Clear CHS bits */
	tmpreg = ADCx->CFGR2 & ~ADC_CFGR2_CHS;
	/* Configure ADCx: channel selection */
	/* Set CHS bit according to ADC_Channel value */
	tmpreg |= ADC_InitStruct->ADC_Channel;
	/* Write to CFGR1 register*/
	ADCx->CFGR2 = tmpreg;
	
	tmpreg = ADCx->CFGR3 & ~(ADC_CFGR3_PSC | ADC_CFGR3_SETUP | ADC_CFGR3_SMP);
	tmpreg |= ADC_InitStruct->ADC_Prescaler | \
			   ((ADC_InitStruct->ADC_SampleTime)<<24);
#if defined(PT32G031x) || defined (PTM280x)
	tmpreg |= ((ADC_InitStruct->ADC_ChannelSetupTime)<<16);
#endif
	/* Write to CFGR3 register*/
	ADCx->CFGR3 = tmpreg;
}

/**
  * @brief	Fills each ADC_InitStruct member with its default value.
  * @param	ADC_InitStruct: pointer to a ADC_InitTypeDef structure which will be initialized.
  * @retval	None
  */
void ADC_StructInit(ADC_InitTypeDef *ADC_InitStruct)
{
	ADC_InitStruct->ADC_Mode = ADC_Mode_Single;
	ADC_InitStruct->ADC_RVSPS = ADC_RVSPS_VDDA;
	ADC_InitStruct->ADC_Channel = ADC_Channel_0;
	ADC_InitStruct->ADC_Prescaler = 2;
	ADC_InitStruct->ADC_ChannelSetupTime = 0xF;
	ADC_InitStruct->ADC_SampleTime = 0xF;
}

/**
  * @brief	Enable or disable the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	NewState: new state of the ADCx peripheral.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval	None
  */
void ADC_Cmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the selected ADC peripheral */
		ADCx->CR1 |= ADC_CR1_EN;
	}
	else
	{
		/* Disable the selected ADC peripheral */
		ADCx->CR1 &= ~ADC_CR1_EN;
	}
}

/**
  * @brief	Start of regular conversion for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @retval	None
  */
void ADC_StartOfRegularConversion(ADC_TypeDef *ADCx)
{
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	/* Enable the regular conversion */
	ADCx->CR1 |= ADC_CR1_SOC;
}

/**
  * @brief	Start of injected conversion for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @retval	None
  */
void ADC_StartOfInjectedConversion(ADC_TypeDef *ADCx)
{
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
	return ;
#else
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	/* Enable the injected conversion */
	ADCx->CR1 |= ADC_CR1_JSOC;
#endif
}

/**
  * @brief	Configure the conversion mode for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	Mode: specifies the ADC conversion mode.
  *			This parameter can be one of the following values:
  *			@arg ADC_Mode_Single: select the single conversion as conversion mode
  *			@arg ADC_Mode_Continuous: select the continuous conversion as conversion mode
  * @retval None
  */
void ADC_ModeConfig(ADC_TypeDef *ADCx, u32 Mode)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_Mode(Mode));
	/* Get the ADCx CR2 value and Clear MODE bits */
	tmpreg = ADCx->CR2 & ~ADC_CR2_MODE;
	/* Set MODE bit according to Mode value */
	tmpreg |= Mode;
	/* Write to CR2 register*/
	ADCx->CR2 = tmpreg;
}

/**
  * @brief  Configure the data align for the specified ADC peripheral.
  * @param 	ADCx: where x can be 0 to select the ADC peripheral.
  * @param  AlignFormat: specifies the ADC align format.
  *			This parameter can be one of the following values:
  * 		@arg ADC_Align_Right: Left aligned selected
  * 		@arg ADC_Align_Left: Right aligned selected
  * @retval None
  */
void ADC_AlignConfig(ADC_TypeDef *ADCx, u32 AlignFormat)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_Align(AlignFormat));
	/* Get the ADCx CR2 value and Clear ALIGN bits */
	tmpreg = ADCx->CR2 & ~ADC_CR2_ALIGN;
	/* Set ALIGN bit according to AlignFormat value */
	tmpreg |= AlignFormat;
	/* Write to CR2 register*/
	ADCx->CR2 = tmpreg;
}

/**
  * @brief  Enable or disable DMA tansfer for the specified ADC peripheral.
  * @param  ADCx: where x can be 0 to select the ADC peripheral.
  * @param  NewState: new state of the ADCx peripheral.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_DMACmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
	return ;
#else
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Set the DMAE bit to Enable the ADC DMA transition*/
		ADCx->CR2 |= ADC_CR2_DMAE;
	}
	else
	{
		/* Clear the DMAE bit to Disable the ADC DMA transition*/
		ADCx->CR2 &= ~ADC_CR2_DMAE;
	}
#endif
}

/**
  * @brief  Configure the conversion control for the specified ADC peripheral.
  * @param  ADCx: where x can be 0 to select the ADC peripheral.
  * @param 	ConversionControl: specifies the conversion control.
  *			This parameter can be one of the following values:
  * 		@arg ADC_ConversionControl_1: Convert once
  * 		@arg ADC_ConversionControl_2: Convert 2^SCCNT times,The result is the average of 2^SCCNT conversion results
  * 		@arg ADC_ConversionControl_3: Convert 2^SCCNT times,The result is the last conversion result
  * 		@arg ADC_ConversionControl_4: Convert 2^SCCNT+1 times,Discard the first conversion result and convert it to the average of the next 2 SCCNT conversion results
  * @retval None
  */
void ADC_ConversionControlConfig(ADC_TypeDef *ADCx, u32 ConversionControl)
{
	u32 tmpreg;
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_ConversionControl(ConversionControl));
	/* Get the ADCx CR2 value and Clear CONV bits */
	tmpreg = ADCx->CR2 & ~ADC_CR2_CONV;
	/* Set CONV bit according to ConversionControl value */
	tmpreg |= ConversionControl;
	/* Write to CR2 register*/
	ADCx->CR2 = tmpreg;
}

/**
  * @brief  Configure the single conversion count control for the specified ADC peripheral.
  * @param  ADCx: where x can be 0 to select the ADC peripheral.
  * @param  ConversionCount: specifies the single conversion count control.
  *			This parameter must be a number between 0x00 to 0x04.
  * @retval None
  */
void ADC_SingleConversionCountControl(ADC_TypeDef *ADCx, u32 ConversionCount)
{
	u32 tmpreg;
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_SingleConversionCountControl(ConversionCount));
	/* Get the ADCx CR2 value and Clear SCCNT bits */
	tmpreg = ADCx->CR2 & ~ADC_CR2_SCCNT;
	/* Set SCCNT bit according to ConversionCount value */
	tmpreg |= ConversionCount<<12;
	/* Write to CR2 register*/
	ADCx->CR2 = tmpreg;
}

/**
  * @brief	Configure the regular trigger source for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	TriggerSource: specifies the regular trigger source.
  *			This parameter can be one of the following values:
	*			@arg ADC_RegularTriggerSource_Software: software trigger selected
	*			@arg ADC_RegularTriggerSource_TIM1CH4RiseEdge: TIM1CH4RiseEdge trigger selected
	*			@arg ADC_RegularTriggerSource_TIM1CH4FallEdge: TIM1CH4FallEdge trigger selected
	*			@arg ADC_RegularTriggerSource_TIM1CH4DoubleEdge: TIM1CH4DoubleEdge trigger selected
	*			@arg ADC_RegularTriggerSource_Timer: Timer trigger selected
	*			@arg ADC_RegularTriggerSource_ADC_ETRRiseEdge: ADC ETRRiseEdge trigger selected
	*			@arg ADC_RegularTriggerSource_ADC_ETRFallEdge: ADC ETRFallEdge trigger selected
	*			@arg ADC_RegularTriggerSource_ADC_ETRDoubleEdge: ADC ETRDoubleEdge trigger selected
  * @retval None
  */
void ADC_RegularTriggerSource(ADC_TypeDef *ADCx, u32 TriggerSource)
{
	/* Check the parameters */
	u32 tmpreg;
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_RegularTriggerSource(TriggerSource));
	/* Get the ADCx CR2 value and Clear TRIGS bits */
	tmpreg = ADCx->CR2 & ~ADC_CR2_TRIGS;
	/* Set TRIGS bit according to TriggerSource value */
	tmpreg |= TriggerSource;
	/* Write to CR2 register*/
	ADCx->CR2 = tmpreg;
}

/**
  * @brief	Configure the regular timer trigger source for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	TimerTrigger: specifies the regular timer trigger source.
  *			This parameter can be one of the following values:
	* 		@arg ADC_RegularTimerTriggerSource_TIM1: TIM1 ADC trigger selected
	* 		@arg ADC_RegularTimerTriggerSource_TIM2: TIM2 ADC trigger selected
	* 		@arg ADC_RegularTimerTriggerSource_TIM3: TIM3 ADC trigger selected
	* 		@arg ADC_RegularTimerTriggerSource_TIM5: TIM5 ADC trigger selected
	* 		@arg ADC_RegularTimerTriggerSource_TIM6: TIM6 ADC trigger selected
	* 		@arg ADC_RegularTimerTriggerSource_TIM7: TIM7 ADC trigger selected
	* 		@arg ADC_RegularTimerTriggerSource_TIM8: TIM8 ADC trigger selected
	* 		@arg ADC_RegularTimerTriggerSource_LPTIM: LPTIM ADC trigger selected
	* 		@arg ADC_RegularTimerTriggerSource_TIM20: TIM20 ADC trigger selected
	* 		@arg ADC_RegularTimerTriggerSource_TIM21: TIM21 ADC trigger selected
	* 		@arg ADC_RegularTimerTriggerSource_TIM22: TIM22 ADC trigger selected
  * @retval None
  */
void ADC_RegularTimerTriggerSource(ADC_TypeDef *ADCx, u32 TimerTrigger)
{
	/* Check the parameters */
	u32 tmpreg;
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_RegularTimerTriggerSource(TimerTrigger));
	/* Get the ADCx CR2 value and Clear TIMS bits */
	tmpreg = ADCx->CR2 & ~ADC_CR2_TIMS;
	/* Set TIMS bit according to TimerTrigger value */
	tmpreg |= TimerTrigger;
	/* Write to CR2 register*/
	ADCx->CR2 = tmpreg;
}

/**
  * @brief	Configure the digitalfilter for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	Filter: specifies the digital filter's signal counters configuration.
  *			This parameter can be one of the following values:
	* 		@arg ADC_DigitalFilter_0: the comparator signal is output directly
	* 		@arg ADC_DigitalFilter_2: when the comparator signal counters is equal to 2, it can output
	* 		@arg ADC_DigitalFilter_4: when the comparator signal counters is equal to 4, it can output
	* 		@arg ADC_DigitalFilter_8: when the comparator signal counters is equal to 8, it can output
	* 		@arg ADC_DigitalFilter_16: when the comparator signal counters is equal to 16, it can output
	* 		@arg ADC_DigitalFilter_32: when the comparator signal counters is equal to 32, it can output
	* 		@arg ADC_DigitalFilter_64: when the comparator signal counters is equal to 64, it can output
  * @retval None
  */
void ADC_DigitalFilterConfig(ADC_TypeDef *ADCx, u32 Filter)
{
	/* Check the parameters */
	u32 tmpreg;
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_DigitalFilter(Filter));
	/* Get the ADCx CR2 value and Clear DFC bits */
	tmpreg = ADCx->CR2 & ~ADC_CR2_DFC;
	/* Set DFC bit according to Filter value */
	tmpreg |= Filter;
	/* Write to CR2 register*/
	ADCx->CR2 = tmpreg;
}

/**
  * @brief  Enable or disable regular scan for the specified ADC peripheral.
  * @param  ADCx: where x can be 0 to select the ADC peripheral.
  * @param  NewState: new state of the ADCx peripheral.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_RegularScanCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
	return;
#else
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the regular scan */
		ADCx->CR3 |= ADC_CR3_SCANE;
	}
	else
	{
		/* Disable the regular scan */
		ADCx->CR3 &= ~ADC_CR3_SCANE;
	}
#endif
}

/**
  * @brief  Enable or disable Injected scan for the specified ADC peripheral.
  * @param  ADCx: where x can be 0 to select the ADC peripheral.
  * @param  NewState: new state of the ADCx peripheral.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_InjectedScanCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
	return;
#else
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the injected scan */
		ADCx->CR3 |= ADC_CR3_JSCANE;
	}
	else
	{
		/* Disable the injected scan */
		ADCx->CR3 &= ~ADC_CR3_JSCANE;
	}
#endif
}

/**
  * @brief	Configure the injected trigger source for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	TriggerSource: specifies the injected trigger source.
  *			This parameter can be one of the following values:
	*			@arg ADC_InjectedTriggerSource_Software: software trigger selected
	*			@arg ADC_InjectedTriggerSource_TIM1CH4RiseEdge: TIM1CH4RiseEdge trigger selected
	*			@arg ADC_InjectedTriggerSource_TIM1CH4FallEdge: TIM1CH4FallEdge trigger selected
	*			@arg ADC_InjectedTriggerSource_TIM1CH4DoubleEdge: TIM1CH4DoubleEdge trigger selected
	*			@arg ADC_InjectedTriggerSource_Timer: Timer trigger selected
	*			@arg ADC_InjectedTriggerSource_ADC_ETRRiseEdge: ADC ETRRiseEdge trigger selected
	*			@arg ADC_InjectedTriggerSource_ADC_ETRFallEdge: ADC ETRFallEdge trigger selected
	*			@arg ADC_InjectedTriggerSource_ADC_ETRDoubleEdge: ADC ETRDoubleEdge trigger selected
  * @retval None
  */
void ADC_InjectedTriggerSource(ADC_TypeDef *ADCx, u32 TriggerSource)
{
	/* Check the parameters */
	u32 tmpreg;
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_InjectedTriggerSource(TriggerSource));
	/* Get the ADCx CR3 value and Clear JTRIGS bits */
	tmpreg = ADCx->CR3 & ~ADC_CR3_JTRIGS;
	/* Set JTRIGS bit according to TriggerSource value */
	tmpreg |= TriggerSource;
	/* Write to CR3 register*/
	ADCx->CR3 = tmpreg;
}

/**
  * @brief	Configure the injected timer trigger source for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	TimerTrigger: specifies the injected timer trigger source.
  *			This parameter can be one of the following values:
	*			@arg ADC_InjectedTimerTriggerSource_TIM1: TIM1 ADC trigger selected
	*			@arg ADC_InjectedTimerTriggerSource_TIM2: TIM2 ADC trigger selected
	*			@arg ADC_InjectedTimerTriggerSource_TIM3: TIM3 ADC trigger selected
	*			@arg ADC_InjectedTimerTriggerSource_TIM5: TIM5 ADC trigger selected
	*			@arg ADC_InjectedTimerTriggerSource_TIM6: TIM6 ADC trigger selected
	*			@arg ADC_InjectedTimerTriggerSource_TIM7: TIM7 ADC trigger selected
	*			@arg ADC_InjectedTimerTriggerSource_TIM8: TIM8 ADC trigger selected
	*			@arg ADC_InjectedTimerTriggerSource_LPTIM: LPTIM ADC trigger selected
  * @retval None
  */
void ADC_InjectedTimerTriggerSource(ADC_TypeDef *ADCx, u32 TimerTrigger)
{
	/* Check the parameters */
	u32 tmpreg;
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_InjectedTimerTriggerSource(TimerTrigger));
	/* Get the ADCx CR3 value and Clear JTIMS bits */
	tmpreg = ADCx->CR3 & ~ADC_CR3_JTIMS;
	/* Set JTIMS bit according to TimerTrigger value */
	tmpreg |= TimerTrigger;
	/* Write to CR3 register*/
	ADCx->CR3 = tmpreg;
}

/**
  * @brief	Configure the regular scan channel number for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	ScanNumber: specifies the regular scan channel number.
  *			This parameter must be a number between 2 to 20.
  * @retval None
  */
void ADC_RSCNTConfig(ADC_TypeDef *ADCx, u32 ScanNumber)
{
	u32 tmpreg;
//	assert_param(IS_ADC_ALL_PERIPH(ADCx));
//	assert_param(IS_ADC_RegularScanChannelNumber(ScanNumber));
	/* Get the ADCx CR3 value and Clear SCNT bits */
	tmpreg = ADCx->CR3 & ~ADC_CR3_SCNT;
	/* Set SCNT bit according to ScanNumber value */
	tmpreg |= ((ScanNumber - 1) << 16);
	/* Write to CR3 register*/
	ADCx->CR3 = tmpreg;
}

/**
  * @brief	Configure the injected scan channel number for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	ScanNumber: specifies the injected scan channel number.
  *			This parameter must be a number between 2 to 20.
  * @retval None
  */
void ADC_JSCNTConfig(ADC_TypeDef *ADCx, u32 ScanNumber)
{
	u32 tmpreg;
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_InjectedScanChannelNumber(ScanNumber));
	/* Get the ADCx CR3 value and Clear JSCNT bits */
	tmpreg = ADCx->CR3 & ~ADC_CR3_JSCNT;
	/* Set JSCNT bit according to ScanNumber value */
	tmpreg |= ((ScanNumber-1) << 24);
	/* Write to CR3 register*/
	ADCx->CR3 = tmpreg;
}

/**
  * @brief	Configure the reference voltage source positive selection for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	Source: specifies the reference voltage source positive selection.
  *			This parameter can be one of the following values:
  * 		@arg ADC_RVSPS_VDDA: set VDDA as the reference voltage source positive selection
  *			@arg ADC_RVSPS_BG2v0: set BG2V0 as the reference voltage source positive selection
  *			@arg ADC_RVSPS_VREF: set VREF as the reference voltage source positive selection
  * @retval None
  */
void ADC_RVSPSConfig(ADC_TypeDef *ADCx, u32 Source)
{
	u32 tmpreg;
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_RVSPS(Source));
	/* Get the ADCx CFGR1 value and Clear RVSPS bits */
	tmpreg = ADCx->CFGR1 & ~ADC_CFGR1_RVSPS;
	/* Set RVSPS bit according to Source value */
	tmpreg |= Source;
	/* Write to CFGR1 register*/
	ADCx->CFGR1 = tmpreg;
}

/**
  * @brief  Enable or disable BGREF for the specified ADC peripheral.
  * @param  ADCx: where x can be 0 to select the ADC peripheral.
  * @param  NewState: new state of the ADCx peripheral.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_BGREFCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		ADCx->CFGR1 |= ADC_CFGR1_BGE;
	}
	else
	{
		ADCx->CFGR1 &= ~ADC_CFGR1_BGE;
	}
}

/**
  * @brief	Configure the BGREF source selection for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	Source: specifies the BGREF source selection.
  *			This parameter can be one of the following values:
  * 		@arg ADC_BGVoltage_BG2v0: set BG2v0 as BGREF source selection
  * 		@arg ADC_BGVoltage_BG4v0: set BG4v0 as BGREF source selection
  * @retval None
  */
void ADC_BGREFSourceConfig(ADC_TypeDef *ADCx, u32 Source)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_BGREFSource(Source));
	/* Get the ADCx CFGR1 value and Clear BGS bits */
	tmpreg = ADCx->CFGR1 & ~ADC_CFGR1_BGS;
	/* Set BGS bit according to Source value */
	tmpreg |= Source;
	/* Write to CFGR1 register*/
	ADCx->CFGR1 = tmpreg;
}

/**
  * @brief  Enable or disable BGREF output for the specified ADC peripheral.
  * @param  ADCx: where x can be 0 to select the ADC peripheral.
  * @param  NewState: new state of the ADCx peripheral.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_BGREFOutputCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		ADCx->CFGR1 |= ADC_CFGR1_BGOE;
	}
	else
	{
		ADCx->CFGR1 &= ~ADC_CFGR1_BGOE;
	}
}

/**
  * @brief	Configure the key value for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	Key: specifies the key value.
  *			This parameter can be one of the following values:
  * 		@arg ADC_KeyValue_User: write the password, calibration mode to write the offset trim value to the TRIM2[4:0] or TRIM4[4:0] bit
  * 		@arg ADC_KeyValue_Manufactory: write password, switch back to factory mode, and restore factory defaults to TRIM2[4:0] and TRIM4[4:0] bits
  * @retval None
  */
void ADC_KeyValueConfig(ADC_TypeDef *ADCx, u32 Key)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_KeyValue(Key));
	
	tmpreg = ADCx->CFGR1 & ~ADC_CFGR1_KEY;
	tmpreg |= Key;
	ADCx->CFGR1 = tmpreg;
}

/**
  * @brief	Configure the BG2v0 offset trimming value for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	Value: specifies the BG2v0 offset trimming value.
	*			This parameter must be a number between 0x00 to 0x3F.
  * @retval None
  */
void ADC_BG2v0OffsetTrimValueConfig(ADC_TypeDef *ADCx, u32 Value)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_TrimValue(Value));
	
	tmpreg = ADCx->CFGR1 & ~ADC_CFGR1_TRIM2;
	tmpreg |= (Value<<16) | ADC_KeyValue_User;
	ADCx->CFGR1 = tmpreg;
}

/**
  * @brief get BG2v0 offset trimming value
  * @param ADCx:ADC Peripheral
  * @retval trimming value
  */
u8 ADC_GetBG2v0TrimValue(ADC_TypeDef *ADCx)
{
	assert_param(IS_ADC_ALL_PERIPH(ADCx));

	return ((ADCx->CFGR1 & ADC_CFGR1_TRIM2)>>16);
}

#if defined (PT32G031x) || defined (PTM280x)
/**
  * @brief	Configure the BG4v0 offset trimming value for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	Value: specifies the BG4v0 offset trimming value.
	*			This parameter must be a number between 0x00 to 0x3F.
  * @retval None
  */
void ADC_BG4v0OffsetTrimValueConfig(ADC_TypeDef *ADCx, u32 Value)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_TrimValue(Value));
	
	tmpreg = ADCx->CFGR1 & ~ADC_CFGR1_TRIM4;
	tmpreg |= (Value<<24) | ADC_KeyValue_User;
	ADCx->CFGR1 = tmpreg;
}

/**
  * @brief get BG4v0 offset trimming value
  * @param ADCx:ADC Peripheral
  * @retval trimming value
  */
u8 ADC_GetBG4v0TrimValue(ADC_TypeDef *ADCx)
{
	assert_param(IS_ADC_ALL_PERIPH(ADCx));

	return ((ADCx->CFGR1 & ADC_CFGR1_TRIM4)>>24);
}
/**
  * @brief  Check whether the specified BGTF flag is set or not.
  * @param  ADCx: where x can be 0 to select the ADC peripheral.
	* @retval FlagStatus: The new state of BGTF FLAG (SET or RESET).
  */
FlagStatus ADC_GetBGTFFlagStatus(ADC_TypeDef *ADCx)
{
	FlagStatus bitstatus = RESET;

	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));

	/* Check the status of the specified BGREF offset trimming value source flag */
	if((ADCx->CFGR1 & ADC_CFGR1_BGTF) != 0)
	{
		bitstatus = SET;
	}
	else
	{
		bitstatus = RESET;
	}
	
	/* Return the ADC_FLAG status */
	return  bitstatus;
}
#endif

/**
  * @brief  Configure the input negative for the specified ADC.
  * @param 	ADCx: ADC Peripheral.
  * @param  SEL: the Input Negative of the ADC.
  *          This parameter can be one of the following values:
  * 		@arg ADC_InputNegative_VSS: select the VSS as Input Negative
  * 		@arg ADC_InputNegative_INN: select the INN as Input Negative
  * @retval None
  */
void ADC_InputNegative(ADC_TypeDef *ADCx, u32 NegativeInput)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_InputNegative(NegativeInput));
	/* Get the ADCx CFGR2 value and Clear INNS bits */
	tmpreg = ADCx->CFGR2 & ~ADC_CFGR2_INNS;
	/* Set INNS bit according to NegativeInput value */
	tmpreg |= NegativeInput;
	/* Write to CFGR2 register*/
	ADCx->CFGR2 = tmpreg;
}

/**
	* @brief	Configure the channel selection for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	ADC_Channel: specifies the channel selection.
  *			This parameter can be one of the following values:
  *			@arg ADC_Channel_0: ADC channel 0 selected
  *			@arg ADC_Channel_1: ADC channel 1 selected
  *			@arg ADC_Channel_2: ADC channel 2 selected
  *			@arg ADC_Channel_3: ADC channel 3 selected
  *			@arg ADC_Channel_4: ADC channel 4 selected
  *			@arg ADC_Channel_5: ADC channel 5 selected
  *			@arg ADC_Channel_6: ADC channel 6 selected
  *			@arg ADC_Channel_7: ADC channel 7 selected
  *			@arg ADC_Channel_8: ADC channel 8 selected
  *			@arg ADC_Channel_9: ADC channel 9 selected
  *			@arg ADC_Channel_10: ADC channel 10 selected
  *			@arg ADC_Channel_11: ADC channel 11 selected
  *			@arg ADC_Channel_12: ADC channel 12 selected
  *			@arg ADC_Channel_13: ADC channel 13 selected
  *			@arg ADC_Channel_14: ADC channel 14 selected
  *			@arg ADC_Channel_15: ADC channel 15 selected
  *			@arg ADC_Channel_16: ADC channel 16 selected
  *			@arg ADC_Channel_17: ADC channel 17 selected
  *			@arg ADC_Channel_18: ADC channel 18 selected
  *			@arg ADC_Channel_19: ADC channel 19 selected
  *			@arg ADC_Channel_20: ADC channel 20 selected
  *			@arg ADC_Channel_21: ADC channel 21 selected
  *			@arg ADC_Channel_22: ADC channel 22 selected
  *			@arg ADC_Channel_23: ADC channel 23 selected
  *			@arg ADC_Channel_24: ADC channel 24 selected
  *			@arg ADC_Channel_25: ADC channel 25 selected
  *			@arg ADC_Channel_55: ADC channel 55 selected
  *			@arg ADC_Channel_56: ADC channel 56 selected
  *			@arg ADC_Channel_57: ADC channel 57 selected
  *			@arg ADC_Channel_58: ADC channel 58 selected
  *			@arg ADC_Channel_59: ADC channel 59(VSSA) selected
  *			@arg ADC_Channel_60: ADC channel 60(Temp Sensor) selected
  *			@arg ADC_Channel_61: ADC channel 61 selected
  *			@arg ADC_Channel_62: ADC channel 62(BG1v0) selected
  *			@arg ADC_Channel_63: ADC channel 63 selected
  * @retval None
  */
void ADC_ChannelConfig(ADC_TypeDef *ADCx, u32 ADC_Channel)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_Channel(ADC_Channel));
	
	/* Clear the ADC Channel Selection bits */
	tmpreg = ADCx->CFGR2 & ~ADC_CFGR2_CHS;
	tmpreg |= ADC_Channel;
	/* Configure the ADC Channel */
	ADCx->CFGR2 = tmpreg;
}

/**
  * @brief	Configure the prescaler for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	Division: specifies the prescaler.
  *			This parameter can be a number between 0 and 255.
  * @retval None
  */
void ADC_Prescaler(ADC_TypeDef *ADCx, u32 Division)
{
	/* Check the parameters */
	u32 tmpreg;
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_Prescaler(Division));
	
	/* Get the ADCx CFGR3 value and Clear PSC bits */
	tmpreg = ADCx->CFGR3 & ~ADC_CFGR3_PSC;
	/* Set PSC bit according to Division value */
	tmpreg |= Division;
	/* Write to CFGR3 register*/
	ADCx->CFGR3 = tmpreg;
}

/**
  * @brief	Configure the ready time for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	ReadyTime: specifies the ready time.
  *			This parameter can be a number between 0 and 255.
  * @retval None
  */
void ADC_ReadyTimeConfig(ADC_TypeDef *ADCx, u32 ReadyTime)
{
	u32 tmpreg;
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_ReadyTime(ReadyTime));
	/* Get the ADCx CFGR3 value and Clear RDTC bits */
	tmpreg = ADCx->CFGR3 & ~ADC_CFGR3_RDTC;
	/* Set RDTC bit according to ReadyTime value */
	tmpreg |= (ReadyTime << 8);
	/* Write to CFGR3 register*/
	ADCx->CFGR3 = tmpreg;
}

/**
  * @brief	Configure the channel setup time for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	Time: specifies the channel setup time.
  *			This parameter can be a number between 0 and 255.
  * @retval None
  */
void ADC_ChannelSetupTimeConfig(ADC_TypeDef *ADCx, u32 Time)
{
	u32 tmpreg;
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_ChannelSetupTime(Time));
	/* Get the ADCx CFGR3 value and Clear SETUP bits */
	tmpreg = ADCx->CFGR3 & ~ADC_CFGR3_SETUP;
	/* Set SETUP bit according to Time value */
	tmpreg |= (Time << 16);
	/* Write to CFGR3 register*/
	ADCx->CFGR3 = tmpreg;
}

/**
  * @brief	Configure the sample time for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	SampleTime: specifies the sample time.
  *			This parameter can be a number between 0 and 127.
  * @retval	None
  */
void ADC_SampleTimeConfig(ADC_TypeDef *ADCx, u32 SampleTime)
{
	u32 tmpreg;
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_SampleTime(SampleTime));
	/* Get the ADCx CFGR3 value and Clear SMP bits */
	tmpreg = ADCx->CFGR3 & ~ADC_CFGR3_SMP;
	/* Set SMP bit according to SampleTime value */
	tmpreg |= (SampleTime << 24);
	/* Write to CFGR3 register*/
	ADCx->CFGR3 = tmpreg;
}

/**
  * @brief	Enable or disable the specified ADC interrupts.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	ADC_IT: specifies the ADC interrupt sources to be enabled or disabled.
  *			This parameter can be one of the following values:
  *			@arg ADC_IT_EOC: regular channel end of conversion interrupt
  *			@arg ADC_IT_JEOC: injected channel end of conversion interrupt
  *			@arg ADC_IT_EOS: regular channel end of scan interrupt
  *			@arg ADC_IT_JEOS: injected channel end of scan interrupt
  *			@arg ADC_IT_AWD: analog watchdog interrupt
  *			@arg ADC_IT_ALL: All of the above interrupt
  * @param	NewState: new state of the specified ADC interrupts.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval	None
  */
void ADC_ITConfig(ADC_TypeDef *ADCx, u32 ADC_IT, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_IT(ADC_IT));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if(NewState != DISABLE)
	{
		/* Enable the selected ADC interrupts */
		ADCx->IER |= ADC_IT;
	}
	else
	{
		/* Disable the selected ADC interrupts */
		ADCx->IER &= ~ADC_IT;
	}
}

/**
  * @brief	Check whether the specified ADC flag is set or not.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	ADC_FLAG: specifies the flag to check.
  *			This parameter can be one of the following values:
  *			@arg ADC_FLAG_RDY: ready flag
  *			@arg ADC_FLAG_EOC: regular channel end of conversion flag
  *			@arg ADC_FLAG_JEOC: injected channel end of conversion flag
  *			@arg ADC_FLAG_EOS: regular channel end of scan flag
  *			@arg ADC_FLAG_JEOS: injected channel end of scan flag
  *			@arg ADC_FLAG_AWD: analog watchdog flag
  *			@arg ADC_FLAG_ALL: All of the above flag
  * @retval FlagStatus: The new state of ADC FLAG (SET or RESET).
  */
FlagStatus ADC_GetFlagStatus(ADC_TypeDef *ADCx, u32 ADC_FLAG)
{
	FlagStatus bitstatus;

	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_FLAG(ADC_FLAG));

	/* Check the status of the specified ADC flag */
	if((ADCx->SR & ADC_FLAG) != (u32)RESET)
	{
		/* ADC_FLAG is set */
		bitstatus = SET;
	}
	else
	{
		/* ADC_FLAG is reset */
		bitstatus = RESET;
	}

	/* Return the ADC_FLAG status */
	return  bitstatus;
}

/**
  * @brief  Clears the ADCx's pending flags.
  * @param 	ADCx:ADC Peripheral.
  * @param  ADC_FLAG: specifies the flag to clear.
  *          This parameter can be any combination of the following values:
  *         @arg ADC_FLAG_AWD: ADC Analog watchdog interrupt enable control
  * @retval None
  */
void ADC_ClearFlag(ADC_TypeDef *ADCx, u32 ADC_FLAG)
{
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_CLEAR_FLAG(ADC_FLAG));
	
	ADCx->SR = ADC_FLAG;
}

#if defined (PT32G031x) || defined (PTM280x)
/**
  * @brief	Gets the analog watchdog channel in the event of a simulated watchdog event.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @retval	Analog watchdog channel selection in the event of a simulated watchdog event.
  */
u8 ADC_GetAnalogWatchdogChannel(ADC_TypeDef *ADCx)
{
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	if(ADCx->SR & ADC_FLAG_AWD)
	{
		return ((ADCx->SR & ADC_SR_AWDCH)>>16);
	}
	else
	{
		return 0;
	}
}
#endif

/**
  * @brief	Enable or disable analog watchdog for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	NewState: new state of the ADCx peripheral.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval	None
  */
void ADC_AnalogWatchdogCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
	return ;
#else
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Set the AWDE bit to Enable the ADC Analog watchdog */
		ADCx->AWDCR |= ADC_AWDCR_AWDE;
	}
	else
	{
		ADCx->AWDCR &= ~ADC_AWDCR_AWDE;
	}
#endif
}

/**
  * @brief	Configure the analog watchdog channel control for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	ChannelControl: specifies the channel control.
  *			This parameter can be one of the following values:
  * 		@arg ADC_Channel_All: select all channel as analog watchdog channel
  * 		@arg ADC_Channel_Single: select single channel as analog watchdog channel
  * @retval	None
  */
void ADC_AnalogWatchdogChannelControl(ADC_TypeDef *ADCx, u32 ChannelControl)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_AnalogWatchdogChannelControl(ChannelControl));
	/* Get the ADCx AWDCR value and Clear CHC bits */
	tmpreg = ADCx->AWDCR & ~ADC_AWDCR_CHC;
	/* Set CHC bit according to ChannelControl value */
	tmpreg |= ChannelControl;
	/* Write to AWDCR register*/
	ADCx->AWDCR = tmpreg;
}

/**
  * @brief	Configure the analog watchdog channel for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripheral.
  * @param	ADC_Channel: specifies the analog watchdog channel.
  *			This parameter can be one of the following values:
  *			@arg ADC_Channel_0: ADC Channel0 selected
  *			@arg ADC_Channel_1: ADC Channel1 selected
  *			@arg ADC_Channel_2: ADC Channel2 selected
  *			@arg ADC_Channel_3: ADC Channel3 selected
  *			@arg ADC_Channel_4: ADC Channel4 selected
  *			@arg ADC_Channel_5: ADC Channel5 selected
  *			@arg ADC_Channel_6: ADC Channel6 selected
  *			@arg ADC_Channel_7: ADC Channel7 selected
  *			@arg ADC_Channel_8: ADC Channel8 selected
  *			@arg ADC_Channel_9: ADC Channel9 selected
  *			@arg ADC_Channel_10: ADC Channel10 selected
  *			@arg ADC_Channel_11: ADC Channel11 selected
  *			@arg ADC_Channel_12: ADC Channel12 selected
  *			@arg ADC_Channel_13: ADC Channel13 selected
  *			@arg ADC_Channel_14: ADC Channel14 selected
  *			@arg ADC_Channel_15: ADC Channel15 selected
  *			@arg ADC_Channel_16: ADC Channel16 selected
  *			@arg ADC_Channel_17: ADC Channel17 selected
  *			@arg ADC_Channel_18: ADC Channel18 selected
  *			@arg ADC_Channel_19: ADC Channel19 selected
  *			@arg ADC_Channel_20: ADC Channel20 selected
  *			@arg ADC_Channel_21: ADC Channel21 selected
  *			@arg ADC_Channel_22: ADC Channel22 selected
  *			@arg ADC_Channel_23: ADC Channel23 selected
  *			@arg ADC_Channel_24: ADC Channel24 selected
  *			@arg ADC_Channel_25: ADC Channel25 selected
  *			@arg ADC_Channel_57: ADC Channel57 selected
  *			@arg ADC_Channel_58: ADC Channel58 selected
  *			@arg ADC_Channel_59: ADC Channel59 selected
  *			@arg ADC_Channel_60: ADC Channel60 selected
  *			@arg ADC_Channel_61: ADC Channel61 selected
  *			@arg ADC_Channel_62: ADC Channel62 selected
  *			@arg ADC_Channel_63: ADC Channel63 selected
  * @retval	None
  */
void ADC_AnalogWatchdogChannelSelect(ADC_TypeDef *ADCx, u32 ADC_Channel)
{
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
	return;
#else
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_Channel(ADC_Channel));
	/* Get the ADCx AWDCR value and Clear CHS bits */
	tmpreg = ADCx->AWDCR & ~ADC_AWDCR_CHS;
	/* Set CHS bit according to ADC_Channel value */
	tmpreg |= ADC_Channel;
	/* Write to AWDCR register*/
	ADCx->AWDCR = tmpreg;
	#endif
}

/**
  * @brief	Configure the analog watchdog lower threshold for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripher.
  * @param	Value: specifies the analog watchdog lower threshold.
	*			This parameter can be a number between 0 and 4095.
  * @retval None
  */
void ADC_AnalogWatchdogLowerThreshold(ADC_TypeDef *ADCx, u32 Value)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_AnalogWatchdogThreshold(Value));
	
	tmpreg = ADCx->AWDTR & ~ADC_AWDTR_LT;
	tmpreg |= Value;
	ADCx->AWDTR = tmpreg;
}

/**
  * @brief	Configure the analog watchdog higher threshold for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripher.
  * @param	Value: specifies the analog watchdog higher threshold.
  *			This parameter can be a number between 0 and 4095.
  * @retval None
  */
void ADC_AnalogWatchdogHigherThreshold(ADC_TypeDef *ADCx, u32 Value)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_AnalogWatchdogThreshold(Value));
	
	tmpreg = ADCx->AWDTR & ~ADC_AWDTR_HT;
	tmpreg |= (Value << 16);
	ADCx->AWDTR = tmpreg;
}

/**
  * @brief	Returns the last ADCx conversion result data for channel.
  * @param	ADCx: where x can be 0 to select the ADC peripher.
  * @retval (u16)ADCx->DR
  */
u16 ADC_GetConversionValue(ADC_TypeDef *ADCx)
{
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));

	/* Return the selected ADC conversion value */
	return ADCx->DR;
}

/**
  * @brief	Configure the scan channel for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripher.
  * @param	ScanChannel: the ADCx scan channel to configure.
  *			This parameter can be a number between 0 and 19.
  * @param	ADC_Channel: the ADCx channel to configure.
	*			This parameter can be one of the following values:
  *			@arg ADC_Channel_0: ADC Channel0 selected
  *			@arg ADC_Channel_1: ADC Channel1 selected
  *			@arg ADC_Channel_2: ADC Channel2 selected
  *			@arg ADC_Channel_3: ADC Channel3 selected
  *			@arg ADC_Channel_4: ADC Channel4 selected
  *			@arg ADC_Channel_5: ADC Channel5 selected
  *			@arg ADC_Channel_6: ADC Channel6 selected
  *			@arg ADC_Channel_7: ADC Channel7 selected
  *			@arg ADC_Channel_8: ADC Channel8 selected
  *			@arg ADC_Channel_9: ADC Channel9 selected
  *			@arg ADC_Channel_10: ADC Channel10 selected
  *			@arg ADC_Channel_11: ADC Channel11 selected
  *			@arg ADC_Channel_12: ADC Channel12 selected
  *			@arg ADC_Channel_13: ADC Channel13 selected
  *			@arg ADC_Channel_14: ADC Channel14 selected
  *			@arg ADC_Channel_15: ADC Channel15 selected
  *			@arg ADC_Channel_16: ADC Channel16 selected
  *			@arg ADC_Channel_17: ADC Channel17 selected
  *			@arg ADC_Channel_18: ADC Channel18 selected
  *			@arg ADC_Channel_19: ADC Channel19 selected
  *			@arg ADC_Channel_20: ADC Channel20 selected
  *			@arg ADC_Channel_21: ADC Channel21 selected
  *			@arg ADC_Channel_22: ADC Channel22 selected
  *			@arg ADC_Channel_23: ADC Channel23 selected
  *			@arg ADC_Channel_24: ADC Channel24 selected
  *			@arg ADC_Channel_25: ADC Channel25 selected
  *			@arg ADC_Channel_57: ADC Channel57 selected
  *			@arg ADC_Channel_58: ADC Channel58 selected
  *			@arg ADC_Channel_59: ADC Channel59 selected
  *			@arg ADC_Channel_60: ADC Channel60 selected
  *			@arg ADC_Channel_61: ADC Channel61 selected
  *			@arg ADC_Channel_62: ADC Channel62 selected
  *			@arg ADC_Channel_63: ADC Channel63 selected
  * @retval	None
  */
void ADC_ScanChannelConfig(ADC_TypeDef *ADCx, u32 ScanChannel,u32 ADC_Channel)
{
	u32 tmpreg,group,pos;
//	assert_param(IS_ADC_ALL_PERIPH(ADCx));
//	assert_param(IS_ADC_ScanChannel(ScanChannel));
//	assert_param(IS_ADC_Channel(ADC_Channel));

	group = ScanChannel / 4;
	pos = ScanChannel % 4;
	tmpreg = ADCx->SCHR[group];
	tmpreg &= ~(ADC_SCHR1_CH0 << (pos * 8));
	tmpreg |= ((ADC_Channel >> 16) << (pos * 8));
	/* Write to SCHRx register*/
	ADCx->SCHR[group] = tmpreg;
}

/**
  * @brief	Configure the inject scan channel for the specified ADC peripheral.
  * @param	ADCx: where x can be 0 to select the ADC peripher.
  * @param	ScanChannel: the ADCx scan channel to configure.
  *			This parameter can be a number between 0 and 19.
  * @param	ADC_Channel: the ADCx channel to configure.
  *			This parameter can be one of the following values:
  *			@arg ADC_Channel_0: ADC Channel0 selected
  *			@arg ADC_Channel_1: ADC Channel1 selected
  *			@arg ADC_Channel_2: ADC Channel2 selected
  *			@arg ADC_Channel_3: ADC Channel3 selected
  *			@arg ADC_Channel_4: ADC Channel4 selected
  *			@arg ADC_Channel_5: ADC Channel5 selected
  *			@arg ADC_Channel_6: ADC Channel6 selected
  *			@arg ADC_Channel_7: ADC Channel7 selected
  *			@arg ADC_Channel_8: ADC Channel8 selected
  *			@arg ADC_Channel_9: ADC Channel9 selected
  *			@arg ADC_Channel_10: ADC Channel10 selected
  *			@arg ADC_Channel_11: ADC Channel11 selected
  *			@arg ADC_Channel_12: ADC Channel12 selected
  *			@arg ADC_Channel_13: ADC Channel13 selected
  *			@arg ADC_Channel_14: ADC Channel14 selected
  *			@arg ADC_Channel_15: ADC Channel15 selected
  *			@arg ADC_Channel_16: ADC Channel16 selected
  *			@arg ADC_Channel_17: ADC Channel17 selected
  *			@arg ADC_Channel_18: ADC Channel18 selected
  *			@arg ADC_Channel_19: ADC Channel19 selected
  *			@arg ADC_Channel_20: ADC Channel20 selected
  *			@arg ADC_Channel_21: ADC Channel21 selected
  *			@arg ADC_Channel_22: ADC Channel22 selected
  *			@arg ADC_Channel_23: ADC Channel23 selected
  *			@arg ADC_Channel_24: ADC Channel24 selected
  *			@arg ADC_Channel_25: ADC Channel25 selected
  *			@arg ADC_Channel_57: ADC Channel57 selected
  *			@arg ADC_Channel_58: ADC Channel58 selected
  *			@arg ADC_Channel_59: ADC Channel59 selected
  *			@arg ADC_Channel_60: ADC Channel60 selected
  *			@arg ADC_Channel_61: ADC Channel61 selected
  *			@arg ADC_Channel_62: ADC Channel62 selected
  *			@arg ADC_Channel_63: ADC Channel63 selected
  * @retval	None
  */
void ADC_InjectScanChannelConfig(ADC_TypeDef *ADCx, u32 ScanChannel, u32 ADC_Channel)
{
	u32 tmpreg,group,pos;
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_InjectedScanChannel(ScanChannel));
	assert_param(IS_ADC_Channel(ADC_Channel));

	group = ScanChannel / 4;
	pos = ScanChannel % 4;
	tmpreg = ADCx->JSCHR[group];
	tmpreg &= ~(ADC_JSCHR1_CH0 << (pos * 8));
	tmpreg |= ((ADC_Channel >> 16) << (pos * 8));
	/* Write to JSCHRx register*/
	ADCx->JSCHR[group] = tmpreg;
}

/**
  * @brief	Returns the last ADCx conversion result data for regular sacan channel.
  * @param	ADCx: where x can be 0 to select the ADC peripher.
  * @retval	(u16)ADCx->SCHDR
  */
u16 ADC_GetRegularScanConversionValue(ADC_TypeDef *ADCx,u32 ScanChannel)
{
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_ScanChannel(ScanChannel));
	
	/* Return the selected ADC conversion value */
	return ADCx->SCHDR[ScanChannel];
}

/**
  * @brief	Returns the last ADCx conversion result data for inject sacan channel.
  * @param	ADCx: where x can be 0 to select the ADC peripher.
  * @retval	(u16)ADCx->JSCHDR
  */
u16 ADC_GetInjectScanConversionValue(ADC_TypeDef *ADCx,u32 ScanChannel)
{
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_InjectedScanChannel(ScanChannel));
	
	/* Return the selected ADC conversion value */
	return ADCx->JSCHDR[ScanChannel];
}
