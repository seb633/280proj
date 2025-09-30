/**
	******************************************************************************
  * @file    PT32x0xx_adc.h
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
  *  Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  *****************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PT32x0xx_ADC_H
#define PT32x0xx_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "PT32x0xx.h"

/* Exported types ------------------------------------------------------------*/
/** @defgroup ADC_InitTypeDef
* @{
*/
typedef struct
{
	u32 ADC_Mode;												/*!< Specifies the ADC prescaler,
																	This parameter can be a value of @ref ADC_Prescaler */
			
	u32 ADC_RVSPS;												/*!< Specifies the ADC timing trigger source selection,
																	This parameter can be a value of @ref ADC_TimerTriggerSource */

	u32 ADC_Channel;											/*!< Specifies the ADC channel selection,
																	This parameter can be a value of @ref ADC_Channel */
	
	u32 ADC_Prescaler;											/*!< Specifies the ADC channel selection,
																	This parameter can be a value of @ref ADC_ChannelSetupTime */
	
	u32 ADC_ChannelSetupTime;									/*!< Specifies the ADC channel selection,
																	This parameter can be a value of @ref ADC_SampleTime */

	u32 ADC_SampleTime;											/*!< Specifies the ADC voltage reference positive selection,
																	This parameter can be a value of @ref ADC_VoltageReferencePositive */
}ADC_InitTypeDef;

/** @defgroup ADC_ALL_PERIPH
* @{
*/
#if defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x)
#define IS_ADC_ALL_PERIPH(SEL)									((SEL) == 0)
#else
#define IS_ADC_ALL_PERIPH(SEL)									((SEL) == ADC0)
#endif

/** @defgroup ADC_Mode
* @{
*/
#define ADC_Mode_Single											(ADC_CR2_MODE & 0x00000000)
#define ADC_Mode_Continuous										(ADC_CR2_MODE & 0x00000001)
#define IS_ADC_Mode(SEL)										((SEL) == ADC_Mode_Single || (SEL) == ADC_Mode_Continuous)

/** @defgroup ADC Align
* @{
*/
#define ADC_Align_Right											(ADC_CR2_ALIGN & 0x00000000)
#define ADC_Align_Left											(ADC_CR2_ALIGN & 0x00000002)
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_ADC_Align(SEL)										(0)
#else
#define IS_ADC_Align(SEL)										((SEL) == ADC_Align_Right || (SEL) == ADC_Align_Left)
#endif

/** @defgroup ADC Conversion Control
* @{
*/
#define ADC_ConversionControl_1									(ADC_CR2_CONV & 0x00000000)
#define ADC_ConversionControl_2									(ADC_CR2_CONV & 0x00000100)
#define ADC_ConversionControl_3									(ADC_CR2_CONV & 0x00000200)
#define ADC_ConversionControl_4									(ADC_CR2_CONV & 0x00000300)
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_ADC_ConversionControl(SEL)  							((SEL) == ADC_ConversionControl_1 || (SEL) == ADC_ConversionControl_4)
#else
#define IS_ADC_ConversionControl(SEL)							(((SEL) & ~ADC_CR2_CONV) == 0 && SEL <= ADC_ConversionControl_4)
#endif

/** @defgroup ADC Single Conversion Count Control
* @{
*/
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_ADC_SingleConversionCountControl(SEL)				(0)
#else
#define IS_ADC_SingleConversionCountControl(SEL)				((SEL) <= 0x4)
#endif

/** @defgroup ADC_RegularTriggerSource
* @{
*/
#define ADC_RegularTriggerSource_Software           			(ADC_CR2_TRIGS & 0x00000000)
#define ADC_RegularTriggerSource_TIM1CH4RiseEdge				(ADC_CR2_TRIGS & 0x00010000)
#define ADC_RegularTriggerSource_TIM1CH4FallEdge				(ADC_CR2_TRIGS & 0x00020000)
#define ADC_RegularTriggerSource_TIM1CH4DoubleEdge				(ADC_CR2_TRIGS & 0x00030000)
#define ADC_RegularTriggerSource_Timer							(ADC_CR2_TRIGS & 0x00040000)
#define ADC_RegularTriggerSource_ADC_ETRRiseEdge				(ADC_CR2_TRIGS & 0x00050000)
#define ADC_RegularTriggerSource_ADC_ETRFallEdge				(ADC_CR2_TRIGS & 0x00060000)
#define ADC_RegularTriggerSource_ADC_ETRDoubleEdge				(ADC_CR2_TRIGS & 0x00070000)

#if defined (PT32G031x) || defined (PTM280x)
#define IS_ADC_RegularTriggerSource(SEL)						((SEL) == ADC_RegularTriggerSource_Software || \
																	(SEL) == ADC_RegularTriggerSource_Timer || \
																	(SEL) == ADC_RegularTriggerSource_ADC_ETRRiseEdge || \
																	(SEL) == ADC_RegularTriggerSource_ADC_ETRFallEdge || \
																	(SEL) == ADC_RegularTriggerSource_ADC_ETRDoubleEdge)
#elif defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_ADC_RegularTriggerSource(SEL)						((SEL) == ADC_RegularTriggerSource_Software || \
																	(SEL) == ADC_RegularTriggerSource_Timer)
#else
#define IS_ADC_RegularTriggerSource(SEL)						(((SEL) & ~ADC_CR2_TRIGS) == 0 && (SEL) < ADC_RegularTriggerSource_ADC_ETRDoubleEdge)
#endif

/** @defgroup ADC Regular Timer Trigger Source Selection
* @{
*/
#define ADC_RegularTimerTriggerSource_TIM1						(ADC_CR2_TIMS & 0x00100000)
#define ADC_RegularTimerTriggerSource_TIM2						(ADC_CR2_TIMS & 0x00200000)
#define ADC_RegularTimerTriggerSource_TIM3						(ADC_CR2_TIMS & 0x00300000)
#define ADC_RegularTimerTriggerSource_TIM5						(ADC_CR2_TIMS & 0x00400000)
#define ADC_RegularTimerTriggerSource_TIM6						(ADC_CR2_TIMS & 0x00500000)
#define ADC_RegularTimerTriggerSource_TIM7						(ADC_CR2_TIMS & 0x00600000)
#define ADC_RegularTimerTriggerSource_TIM8						(ADC_CR2_TIMS & 0x00700000)
#define ADC_RegularTimerTriggerSource_LPTIM						(ADC_CR2_TIMS & 0x00A00000)
#define ADC_RegularTimerTriggerSource_TIM20						(ADC_CR2_TIMS & 0x01400000)
#define ADC_RegularTimerTriggerSource_TIM21						(ADC_CR2_TIMS & 0x01500000)
#define ADC_RegularTimerTriggerSource_TIM22						(ADC_CR2_TIMS & 0x01600000)

#if defined (PT32G031x) || defined (PTM280x)
#define IS_ADC_RegularTimerTriggerSource(SEL)					((SEL) == ADC_RegularTimerTriggerSource_TIM1 || \
																	(SEL) == ADC_RegularTimerTriggerSource_TIM8)
#elif defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_ADC_RegularTimerTriggerSource(SEL)					((SEL) == ADC_RegularTimerTriggerSource_TIM20 || \
																	(SEL) == ADC_RegularTimerTriggerSource_TIM21 || \
																	(SEL) == ADC_RegularTimerTriggerSource_TIM22)
#else
#define IS_ADC_RegularTimerTriggerSource(SEL)					((SEL) == ADC_RegularTimerTriggerSource_TIM1 || \
																	(SEL) == ADC_RegularTimerTriggerSource_TIM2 || \
																	(SEL) == ADC_RegularTimerTriggerSource_TIM3 || \
																	(SEL) == ADC_RegularTimerTriggerSource_TIM5 || \
																	(SEL) == ADC_RegularTimerTriggerSource_TIM6 || \
																	(SEL) == ADC_RegularTimerTriggerSource_TIM7 || \
																	(SEL) == ADC_RegularTimerTriggerSource_TIM8 || \
																	(SEL) == ADC_RegularTimerTriggerSource_TIM20 || \
																	(SEL) == ADC_RegularTimerTriggerSource_TIM21 || \
																	(SEL) == ADC_RegularTimerTriggerSource_TIM22)
#endif

/** @defgroup ADC Digital filter
* @{
*/
#define ADC_DigitalFilter_0										(ADC_CR2_DFC & 0x00000000)
#define ADC_DigitalFilter_2										(ADC_CR2_DFC & 0x10000000)
#define ADC_DigitalFilter_4										(ADC_CR2_DFC & 0x20000000)
#define ADC_DigitalFilter_8										(ADC_CR2_DFC & 0x30000000)
#define ADC_DigitalFilter_16									(ADC_CR2_DFC & 0x40000000)
#define ADC_DigitalFilter_32									(ADC_CR2_DFC & 0x50000000)
#define ADC_DigitalFilter_64									(ADC_CR2_DFC & 0x60000000)
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_ADC_DigitalFilter(SEL)								(0)
#else
#define IS_ADC_DigitalFilter(SEL)								(((SEL) & ~ADC_CR2_DFC) == 0 && (SEL) <= ADC_DigitalFilter_64)
#endif

/** @defgroup ADC Injected TriggerSource
* @{
*/
#define ADC_InjectedTriggerSource_Software           			(ADC_CR3_JTRIGS & 0x00000000)
#define ADC_InjectedTriggerSource_TIM1CH4RiseEdge				(ADC_CR3_JTRIGS & 0x00000010)
#define ADC_InjectedTriggerSource_TIM1CH4FallEdge				(ADC_CR3_JTRIGS & 0x00000020)
#define ADC_InjectedTriggerSource_TIM1CH4DoubleEdge				(ADC_CR3_JTRIGS & 0x00000030)
#define ADC_InjectedTriggerSource_Timer							(ADC_CR3_JTRIGS & 0x00000040)
#define ADC_InjectedTriggerSource_ADC_ETRRiseEdge				(ADC_CR3_JTRIGS & 0x00000050)
#define ADC_InjectedTriggerSource_ADC_ETRFallEdge				(ADC_CR3_JTRIGS & 0x00000060)
#define ADC_InjectedTriggerSource_ADC_ETRDoubleEdge				(ADC_CR3_JTRIGS & 0x00000070)

#if defined (PT32G031x) || defined (PTM280x)
#define IS_ADC_InjectedTriggerSource(SEL)						((SEL) == ADC_InjectedTriggerSource_Software ||\
																	(SEL) == ADC_InjectedTriggerSource_Timer ||\
																	(SEL) == ADC_InjectedTriggerSource_ADC_ETRRiseEdge ||\
																	(SEL) == ADC_InjectedTriggerSource_ADC_ETRFallEdge ||\
																	(SEL) == ADC_InjectedTriggerSource_ADC_ETRDoubleEdge)
#elif defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_ADC_InjectedTriggerSource(SEL)						(0)
#else
#define IS_ADC_InjectedTriggerSource(SEL)						(((SEL) & ~ADC_CR3_JTRIGS) == 0 && (SEL) < ADC_InjectedTriggerSource_ADC_ETRDoubleEdge)
#endif

/** @defgroup ADC Regular Timer Trigger Source Selection
* @{
*/
#define ADC_InjectedTimerTriggerSource_TIM1						(ADC_CR3_JTIMS & 0x00000100)
#define ADC_InjectedTimerTriggerSource_TIM2						(ADC_CR3_JTIMS & 0x00000200)
#define ADC_InjectedTimerTriggerSource_TIM3						(ADC_CR3_JTIMS & 0x00000300)
#define ADC_InjectedTimerTriggerSource_TIM5						(ADC_CR3_JTIMS & 0x00000500)
#define ADC_InjectedTimerTriggerSource_TIM6						(ADC_CR3_JTIMS & 0x00000600)
#define ADC_InjectedTimerTriggerSource_TIM7						(ADC_CR3_JTIMS & 0x00000700)
#define ADC_InjectedTimerTriggerSource_TIM8						(ADC_CR3_JTIMS & 0x00000800)
#define ADC_InjectedTimerTriggerSource_LPTIM					(ADC_CR3_JTIMS & 0x00000A00)

#if defined (PT32G031x) || defined (PTM280x)
#define IS_ADC_InjectedTimerTriggerSource(SEL)					((SEL) == ADC_InjectedTimerTriggerSource_TIM1 || \
																	(SEL) == ADC_InjectedTimerTriggerSource_TIM8)
#elif defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_ADC_InjectedTimerTriggerSource(SEL)					(0)
#else
#define IS_ADC_InjectedTimerTriggerSource(SEL)					((SEL) == ADC_InjectedTimerTriggerSource_TIM1|| \
																	(SEL) == ADC_InjectedTimerTriggerSource_TIM2 || \
																	(SEL) == ADC_InjectedTimerTriggerSource_TIM3 || \
																	(SEL) == ADC_InjectedTimerTriggerSource_TIM5 || \
																	(SEL) == ADC_InjectedTimerTriggerSource_TIM6 || \
																	(SEL) == ADC_InjectedTimerTriggerSource_TIM7 || \
																	(SEL) == ADC_InjectedTimerTriggerSource_TIM8)
#endif

/** @defgroup ADC_ScanChannelNumber
* @{
*/
#if defined (PT32G031x) || defined (PTM280x)
#define IS_ADC_RegularScanChannelNumber(SEL)					((SEL) >= 2 && (SEL) <= 8)
#elif defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_ADC_RegularScanChannelNumber(SEL)					(0)
#else
#define IS_ADC_RegularScanChannelNumber(SEL)					((SEL) >= 2 && (SEL) <= 20)
#endif

/** @defgroup ADC Injected ScanChannel Number
* @{
*/
#if defined (PT32G031x) || defined (PTM280x)
#define IS_ADC_InjectedScanChannelNumber(SEL)					((SEL) >= 1 && (SEL) <= 4)
#elif defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_ADC_InjectedScanChannelNumber(SEL)					(0)
#else
#define IS_ADC_InjectedScanChannelNumber(SEL)					((SEL) >= 1 && (SEL) <= 20)
#endif

/** @defgroup ADC Reference voltage source positive selection
* @{
*/
#define ADC_RVSPS_VDDA											(ADC_CFGR1_RVSPS & 0x00000000)
#define ADC_RVSPS_BGREF											(ADC_CFGR1_RVSPS & 0x00000001)
#define ADC_RVSPS_AVREF											(ADC_CFGR1_RVSPS & 0x00000002)
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_ADC_RVSPS(SEL)										((SEL) == ADC_RVSPS_VDDA || (SEL) == ADC_RVSPS_BGREF)
#else
#define IS_ADC_RVSPS(SEL)										((SEL) == ADC_RVSPS_VDDA || (SEL) == ADC_RVSPS_BGREF || (SEL) == ADC_RVSPS_AVREF)
#endif
										
/** @defgroup ADC BGREF source selection
* @{
*/
#define ADC_BGVoltage_BG4v0										(ADC_CFGR1_BGS & 0x00000000)
#define ADC_BGVoltage_BG2v0										(ADC_CFGR1_BGS & 0x00000020)
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_ADC_BGREFSource(SEL)									(0)
#else
#define IS_ADC_BGREFSource(SEL)									((SEL)==ADC_BGVoltage_BG2v0 || (SEL)==ADC_BGVoltage_BG4v0)
#endif

/** @defgroup Key value
* @{
*/
#define ADC_KeyValue_User										(0x0000AC00)
#define ADC_KeyValue_Manufactory								(0x0000C600)
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_ADC_KeyValue(SEL)									(0)
#else
#define IS_ADC_KeyValue(SEL)									((SEL) == ADC_KeyValue_User || (SEL) == ADC_KeyValue_Manufactory)
#endif

/** @defgroup Offset trimming value
* @{
*/
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_ADC_TrimValue(SEL)									(0)
#else
#define IS_ADC_TrimValue(SEL)									((SEL) <= 0x3F)
#endif

/** @defgroup ADC Input negative selection
* @{
*/
#define ADC_InputNegative_VSSA									(ADC_CFGR2_INNS & 0x00000000)
#define ADC_InputNegative_INN									(ADC_CFGR2_INNS & 0x00000001)
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_ADC_InputNegative(SEL)								(0)
#else
#define IS_ADC_InputNegative(SEL)								((SEL) == ADC_InputNegative_INN || (SEL) == ADC_InputNegative_VSSA)
#endif

/** @defgroup ADC_Channel
* @{
*/
#define ADC_Channel_0											(ADC_CFGR2_CHS & 0x00000000)
#define ADC_Channel_1											(ADC_CFGR2_CHS & 0x00010000)
#define ADC_Channel_2											(ADC_CFGR2_CHS & 0x00020000)
#define ADC_Channel_3											(ADC_CFGR2_CHS & 0x00030000)
#define ADC_Channel_4											(ADC_CFGR2_CHS & 0x00040000)
#define ADC_Channel_5											(ADC_CFGR2_CHS & 0x00050000)
#define ADC_Channel_6											(ADC_CFGR2_CHS & 0x00060000)
#define ADC_Channel_7											(ADC_CFGR2_CHS & 0x00070000)
#define ADC_Channel_8											(ADC_CFGR2_CHS & 0x00080000)
#define ADC_Channel_9											(ADC_CFGR2_CHS & 0x00090000)
#define ADC_Channel_10											(ADC_CFGR2_CHS & 0x000A0000)
#define ADC_Channel_11											(ADC_CFGR2_CHS & 0x000B0000)
#define ADC_Channel_12											(ADC_CFGR2_CHS & 0x000C0000)
#define ADC_Channel_13											(ADC_CFGR2_CHS & 0x000D0000)
#define ADC_Channel_14											(ADC_CFGR2_CHS & 0x000E0000)
#define ADC_Channel_15											(ADC_CFGR2_CHS & 0x000F0000)
#define ADC_Channel_16											(ADC_CFGR2_CHS & 0x00100000)
#define ADC_Channel_17											(ADC_CFGR2_CHS & 0x00110000)
#define ADC_Channel_18											(ADC_CFGR2_CHS & 0x00120000)
#define ADC_Channel_19											(ADC_CFGR2_CHS & 0x00130000)
#define ADC_Channel_20											(ADC_CFGR2_CHS & 0x00140000)
#define ADC_Channel_21											(ADC_CFGR2_CHS & 0x00150000)
#define ADC_Channel_22											(ADC_CFGR2_CHS & 0x00160000)
#define ADC_Channel_23											(ADC_CFGR2_CHS & 0x00170000)
#define ADC_Channel_24											(ADC_CFGR2_CHS & 0x00180000)
#define ADC_Channel_25											(ADC_CFGR2_CHS & 0x00190000)

#define ADC_Channel_55											(ADC_CFGR2_CHS & 0x00370000)
#define ADC_Channel_56											(ADC_CFGR2_CHS & 0x00380000)
#define ADC_Channel_57											(ADC_CFGR2_CHS & 0x00390000)
#define ADC_Channel_58											(ADC_CFGR2_CHS & 0x003A0000)
#define ADC_Channel_59											(ADC_CFGR2_CHS & 0x003B0000)
#define ADC_Channel_60											(ADC_CFGR2_CHS & 0x003C0000)
#define ADC_Channel_61											(ADC_CFGR2_CHS & 0x003D0000)
#define ADC_Channel_62											(ADC_CFGR2_CHS & 0x003E0000)
#define ADC_Channel_63											(ADC_CFGR2_CHS & 0x003F0000)

#define ADC_Channel_VSSA										ADC_Channel_59
#define ADC_Channel_BG1v0										ADC_Channel_62
#if defined (PT32G031x) || defined (PTM280x)
#define IS_ADC_Channel(SEL)										(((SEL) & ~ADC_CFGR2_CHS) == 0 && ((SEL) <= ADC_Channel_10 || \
																	(SEL) == ADC_Channel_58 || (SEL) == ADC_Channel_59 || \
																	(SEL) == ADC_Channel_61 || (SEL) == ADC_Channel_62))
#elif defined (PT32x002x) || defined (PT32x012x)
#define ADC_Channel_VddaDiv5									ADC_Channel_58
#define IS_ADC_Channel(SEL)										(((SEL) & ~ADC_CFGR2_CHS) == 0 && ((SEL) <= ADC_Channel_12 || \
																	(SEL) == ADC_Channel_58 || (SEL) == ADC_Channel_59 || (SEL) == ADC_Channel_62))
#elif defined (PTM160x)
#define ADC_Channel_OPA0_OUT0									ADC_Channel_55
#define ADC_Channel_OPA0_OUT1									ADC_Channel_56
#define ADC_Channel_VddaDiv5									ADC_Channel_58
#define IS_ADC_Channel(SEL)										(((SEL) & ~ADC_CFGR2_CHS) == 0 && ((SEL) <= ADC_Channel_12 || \
																	(SEL) == ADC_Channel_55 || (SEL) == ADC_Channel_56 || (SEL) == ADC_Channel_58 || \
																	(SEL) == ADC_Channel_59 || (SEL) == ADC_Channel_62))
#else
#define IS_ADC_Channel(SEL)										(((SEL) & ~ADC_CFGR2_CHS) == 0 && ((SEL) <= ADC_Channel_25 || \
																	((SEL) >= ADC_Channel_55 && (SEL) <= ADC_Channel_63)))
#endif

/** @defgroup ADC_Prescaler
* @{
*/
#define IS_ADC_Prescaler(SEL)									((SEL) <= 255)

/** @defgroup ADC ready time control
* @{
*/
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_ADC_ReadyTime(SEL)									(0)
#else
#define IS_ADC_ReadyTime(SEL)									((SEL) <= 255)
#endif
/** @defgroup ADC_Channel Setup Time
* @{
*/
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_ADC_ChannelSetupTime(SEL)							(0)
#else
#define IS_ADC_ChannelSetupTime(SEL)							((SEL) <= 255)
#endif

/** @defgroup ADC_SampleTime
* @{
*/
#if defined (PT32G031x) || defined (PTM280x)
#define IS_ADC_SampleTime(SEL)									((SEL) <= 34 && (SEL) >= 3)
#elif defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_ADC_SampleTime(SEL)									((SEL) <= 127)
#else
#define IS_ADC_SampleTime(SEL)									((SEL) <= 255)
#endif

/** @defgroup ADC_IT
* @{
*/
#define ADC_IT_EOC												ADC_IER_EOCIE
#define ADC_IT_JEOC												ADC_IER_JEOCIE
#define ADC_IT_EOS												ADC_IER_EOSIE
#define ADC_IT_JEOS												ADC_IER_JEOSIE
#define ADC_IT_AWD												ADC_IER_AWDIE
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define ADC_IT_ALL												(ADC_IT_EOC)
#else
#define ADC_IT_ALL												(ADC_IT_EOC | ADC_IT_JEOC | ADC_IT_EOS | ADC_IT_JEOS | ADC_IT_AWD)
#endif
#define IS_ADC_IT(SEL)											(((SEL) & ~ADC_IT_ALL) == 0 && (SEL) != 0)

/** @defgroup ADC_FLAG
* @{
*/
#define ADC_FLAG_RDY											ADC_SR_RDY
#define ADC_FLAG_EOC											ADC_SR_EOC
#define ADC_FLAG_JEOC											ADC_SR_JEOC
#define ADC_FLAG_EOS											ADC_SR_EOS
#define ADC_FLAG_JEOS											ADC_SR_JEOS
#define ADC_FLAG_AWD											ADC_SR_AWD
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define ADC_FLAG_ALL											(ADC_FLAG_RDY | ADC_FLAG_EOC)
#define ADC_CLEAR_FLAG_ALL										(0)
#else
#define ADC_FLAG_ALL											(ADC_FLAG_RDY | ADC_FLAG_EOC | ADC_FLAG_JEOC | ADC_FLAG_EOS | ADC_FLAG_JEOS | ADC_FLAG_AWD)
#define ADC_CLEAR_FLAG_ALL										(ADC_FLAG_AWD)
#endif
#define IS_ADC_FLAG(SEL)										(((SEL) & ~ADC_FLAG_ALL) == 0 && (SEL) != 0)
#define IS_ADC_CLEAR_FLAG(SEL)									(((SEL) & ~ADC_CLEAR_FLAG_ALL) == 0 && (SEL) != 0)

/** @defgroup ADC Analog Watchdog Channel Control
* @{
*/
#define ADC_Channel_All											(ADC_AWDCR_CHC & 0x00000000)
#define ADC_Channel_Single										(ADC_AWDCR_CHC & 0x00000002)
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_ADC_AnalogWatchdogChannelControl(SEL)				(0)
#else
#define IS_ADC_AnalogWatchdogChannelControl(SEL)				((SEL) == ADC_Channel_All || (SEL) == ADC_Channel_Single)
#endif

/** @defgroup ADC Analog Watchdog Threshold Value
* @{
*/
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_ADC_AnalogWatchdogThreshold(VALUE)					(0)
#else
#define IS_ADC_AnalogWatchdogThreshold(VALUE)					((VALUE) <= 0xFFFF)
#endif

/** @defgroup ADC_ScanChannel
* @{
*/
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_ADC_ScanChannel(SEL)									(0)
#elif defined (PT32G031x) || defined (PTM280x)
#define IS_ADC_ScanChannel(SEL)									((SEL) < 8)
#else
#define IS_ADC_ScanChannel(SEL)									((SEL) < 20)
#endif

/** @defgroup ADC_InjectedScanChannel
* @{
*/
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_ADC_InjectedScanChannel(SEL)							(0)
#elif defined (PT32G031x) || defined (PTM280x)
#define IS_ADC_InjectedScanChannel(SEL)							((SEL) < 4)
#else
#define IS_ADC_InjectedScanChannel(SEL)							((SEL) < 20)
#endif


/*! function declaration and definition*/
void ADC_Init(ADC_TypeDef *ADCx, ADC_InitTypeDef *ADC_InitStruct);
void ADC_StructInit(ADC_InitTypeDef *ADC_InitStruct);
void ADC_Cmd(ADC_TypeDef *ADCx, FunctionalState NewState);
void ADC_StartOfRegularConversion(ADC_TypeDef *ADCx);
void ADC_StartOfInjectedConversion(ADC_TypeDef *ADCx);
void ADC_ModeConfig(ADC_TypeDef *ADCx, u32 Mode);
void ADC_AlignConfig(ADC_TypeDef *ADCx, u32 AlignFormat);
void ADC_DMACmd(ADC_TypeDef *ADCx, FunctionalState NewState);
void ADC_ConversionControlConfig(ADC_TypeDef *ADCx, u32 ConversionControl);
void ADC_SingleConversionCountControl(ADC_TypeDef *ADCx, u32 ConversionCount);
void ADC_RegularTriggerSource(ADC_TypeDef *ADCx, u32 TriggerSource);
void ADC_RegularTimerTriggerSource(ADC_TypeDef *ADCx, u32 TimerTrigger);
void ADC_DigitalFilterConfig(ADC_TypeDef *ADCx, u32 Filter);
void ADC_RegularScanCmd(ADC_TypeDef *ADCx, FunctionalState NewState);
void ADC_InjectedScanCmd(ADC_TypeDef *ADCx, FunctionalState NewState);
void ADC_InjectedTriggerSource(ADC_TypeDef *ADCx, u32 TriggerSource);
void ADC_InjectedTimerTriggerSource(ADC_TypeDef *ADCx, u32 TimerTrigger);
void ADC_RSCNTConfig(ADC_TypeDef *ADCx, u32 ScanNumber);
void ADC_JSCNTConfig(ADC_TypeDef *ADCx, u32 ScanNumber);
void ADC_RVSPSConfig(ADC_TypeDef *ADCx, u32 Source);
void ADC_BGREFCmd(ADC_TypeDef *ADCx, FunctionalState NewState);
void ADC_BGREFSourceConfig(ADC_TypeDef *ADCx, u32 Source);
void ADC_BGREFOutputCmd(ADC_TypeDef *ADCx, FunctionalState NewState);
void ADC_KeyValueConfig(ADC_TypeDef *ADCx,u32 Key);
void ADC_BG2v0OffsetTrimValueConfig(ADC_TypeDef *ADCx, u32 Value);
u8 ADC_GetBG2v0TrimValue(ADC_TypeDef *ADCx);
#if defined (PT32G031x) || defined (PTM280x)
void ADC_BG4v0OffsetTrimValueConfig(ADC_TypeDef *ADCx, u32 Value);
u8 ADC_GetBG4v0TrimValue(ADC_TypeDef *ADCx);
FlagStatus ADC_GetBGTFFlagStatus(ADC_TypeDef *ADCx);
#endif
void ADC_InputNegative(ADC_TypeDef *ADCx, u32 NegativeInput);
void ADC_ChannelConfig(ADC_TypeDef *ADCx, u32 ADC_Channel);
void ADC_Prescaler(ADC_TypeDef *ADCx, u32 Division);
void ADC_ReadyTimeConfig(ADC_TypeDef *ADCx, u32 ReadyTime);
void ADC_ChannelSetupTimeConfig(ADC_TypeDef *ADCx, u32 Time);
void ADC_SampleTimeConfig(ADC_TypeDef *ADCx, u32 SampleTime);
void ADC_ITConfig(ADC_TypeDef *ADCx, u32 ADC_IT, FunctionalState NewState);
FlagStatus ADC_GetFlagStatus(ADC_TypeDef *ADCx, u32 ADC_FLAG);
void ADC_ClearFlag(ADC_TypeDef *ADCx, u32 ADC_FLAG);
#if defined (PT32G031x) || defined (PTM280x)
u8 ADC_GetAnalogWatchdogChannel(ADC_TypeDef *ADCx);
#endif
void ADC_AnalogWatchdogCmd(ADC_TypeDef *ADCx, FunctionalState NewState);
void ADC_AnalogWatchdogChannelControl(ADC_TypeDef *ADCx, u32 ChannelControl);
void ADC_AnalogWatchdogChannelSelect(ADC_TypeDef *ADCx, u32 ADC_Channel);
void ADC_AnalogWatchdogLowerThreshold(ADC_TypeDef *ADCx, u32 Value);
void ADC_AnalogWatchdogHigherThreshold(ADC_TypeDef *ADCx, u32 Value);
u16 ADC_GetConversionValue(ADC_TypeDef *ADCx);
void ADC_ScanChannelConfig(ADC_TypeDef *ADCx, u32 ScanChannel, u32 ADC_Channel);
void ADC_InjectScanChannelConfig(ADC_TypeDef *ADCx, u32 ScanChannel, u32 ADC_Channel);
u16 ADC_GetRegularScanConversionValue(ADC_TypeDef *ADCx, u32 ScanChannel);
u16 ADC_GetInjectScanConversionValue(ADC_TypeDef *ADCx, u32 ScanChannel);

#ifdef __cplusplus
}
#endif

#endif /*__PT32x0xx_ADC_H */
