/**
  ****************************************************************************
  * @file    PT32x0xx_cmp.h
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief    This file contains all the functions prototypes for the CMP firmware library
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PT32x0xx_CMP_H
#define PT32x0xx_CMP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "PT32x0xx.h"

/* Private define ------------------------------------------------------------*/
/** @addtogroup CMP
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/**
  * @brief  CMP Init structure definition
  */
typedef struct
{
	u32 CMP_NegativeInput;								/*!< Specifies the negative intput selection.
															This parameter can be a value of @ref CMP_negative_input_selection */

	u32 CMP_DigitalFilter;								/*!< Specifies the digital filter configuration.
															This parameter can be a value of @ref CMP_digital_filter_configuration */

	u32 CMP_OutputPolarity;								/*!< Specifies the output polarity control.
															This parameter can be a value of @ref CMP_output_polarity_selection */

	u32 CMP_PositiveInput;								/*!< Specifies the positive intput selection.
															This parameter can be a value of @ref CMP_positive_input_selection	*/

	u32 CMP_InitializationDelayTime;					/*!< Specifies the initialization delay time.
															This parameter can be a number between  0x000 and 0x3FF */
}CMP_InitTypeDef;

/** @defgroup CMP_peripheral_selection
  * @{
  */
#if defined (PT32G031x) || defined (PTM280x)
#define IS_CMP_ALL_PERIPH(SEL)							((SEL) == CMP0 || (SEL) == CMP1)
#elif defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_CMP_ALL_PERIPH(SEL)							((SEL) == CMP0)
#elif defined (PT32x060x) || defined (PT32x063x)|| defined (PT32x066x)
#define IS_CMP_ALL_PERIPH(SEL)							(0)
#else
#define IS_CMP_ALL_PERIPH(SEL)							((SEL) == CMP0 || (SEL) == CMP1 || (SEL) == CMP2 || (SEL) == CMP3)
#endif

/** @defgroup CMP_power_mode_selection
  * @{
  */
#define CMP_PowerMode_LowPower							(CMP_CR1_CPMS & 0x00000000)
#define CMP_PowerMode_HighSpeed							(CMP_CR1_CPMS & 0x00000002)
#if defined (PT32G031x) || defined (PTM280x)
#define IS_CMP_PowerMode(SEL)							(0)
#else
#define IS_CMP_PowerMode(SEL)							((SEL) == CMP_PowerMode_HighSpeed || (SEL) == CMP_PowerMode_LowPower)
#endif

/** @defgroup CMP_output_polarity_selection
  * @{
  */
#define CMP_OutputPolarity_Normal						(CMP_CR1_OPC & 0x00000000)
#define CMP_OutputPolarity_Inverted						(CMP_CR1_OPC & 0x00000004)
#define IS_CMP_OutputPolarity(SEL)						((SEL) == CMP_OutputPolarity_Normal || (SEL) == CMP_OutputPolarity_Inverted)

/** @defgroup CMP_negative_input_selection
  * @{
  */
#define CMP_NegativeInput_CMPxN0						(CMP_CR1_CNS & 0x00000000)
#define CMP_NegativeInput_LDAC							(CMP_CR1_CNS & 0x00000010)
#define CMP_NegativeInput_BG1V0							(CMP_CR1_CNS & 0x00000020)
#define CMP_NegativeInput_CMPxN1						(CMP_CR1_CNS & 0x00000030)
#if defined (PT32G031x) || defined (PTM280x)
#define IS_CMP_NegativeInput(SEL)						((SEL) == CMP_NegativeInput_CMPxN0 || \
															(SEL) == CMP_NegativeInput_LDAC || \
															(SEL) == CMP_NegativeInput_BG1V0)
#else
#define IS_CMP_NegativeInput(SEL)						((SEL) == CMP_NegativeInput_CMPxN0 || \
															(SEL) == CMP_NegativeInput_LDAC || \
															(SEL) == CMP_NegativeInput_BG1V0 || \
															(SEL) == CMP_NegativeInput_CMPxN1)
#endif

/** @defgroup CMP_digital_filter_configuration
  * @{
  */
#define CMP_DigitalFilter_None							(CMP_CR1_DFC & 0x00000000)
#define CMP_DigitalFilter_4								(CMP_CR1_DFC & 0x00000100)
#define CMP_DigitalFilter_8								(CMP_CR1_DFC & 0x00000200)
#define CMP_DigitalFilter_16							(CMP_CR1_DFC & 0x00000300)
#define CMP_DigitalFilter_32							(CMP_CR1_DFC & 0x00000400)
#define CMP_DigitalFilter_64							(CMP_CR1_DFC & 0x00000500)
#define CMP_DigitalFilter_128							(CMP_CR1_DFC & 0x00000600)
#define CMP_DigitalFilter_256							(CMP_CR1_DFC & 0x00000700)
#if defined (PT32G031x) || defined (PTM280x)
#define IS_CMP_DigitalFilter(SEL)						(((SEL) & ~CMP_CR1_DFC) == 0 && SEL <= CMP_DigitalFilter_32)
#elif defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_CMP_DigitalFilter(SEL)						(((SEL) & ~CMP_CR1_DFC) == 0 && SEL <= CMP_DigitalFilter_64)
#else
#define IS_CMP_DigitalFilter(SEL)						(((SEL) & ~CMP_CR1_DFC) == 0 && SEL <= CMP_DigitalFilter_256)
#endif

/** @defgroup CMP hysteresis voltage selection
  * @{
  */
#define CMP_HysteresisVoltage_5mV						(CMP_CR1_HVS & 0x00000000)
#define CMP_HysteresisVoltage_10mV						(CMP_CR1_HVS & 0x00001000)
#define CMP_HysteresisVoltage_20mV						(CMP_CR1_HVS & 0x00002000)
#define CMP_HysteresisVoltage_100mV						(CMP_CR1_HVS & 0x00003000)
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_CMP_HysteresisVoltage(SEL)					(0)
#else
#define IS_CMP_HysteresisVoltage(SEL)					((SEL) == CMP_HysteresisVoltage_5mV || \
															(SEL) == CMP_HysteresisVoltage_10mV || \
															(SEL) == CMP_HysteresisVoltage_20mV || \
															(SEL) == CMP_HysteresisVoltage_100mV)
#endif

/** @defgroup CMP positive input selection
  * @{
  */
#define CMP_PositiveInput_CMPxP0						(CMP_CR1_CPS & 0x00000000)
#define CMP_PositiveInput_CMPxP1						(CMP_CR1_CPS & 0x00010000)
#define CMP_PositiveInput_CMPxP2						(CMP_CR1_CPS & 0x00020000)
#define CMP_PositiveInput_CMPxP3						(CMP_CR1_CPS & 0x00030000)
#define CMP_PositiveInput_CMPxP4						(CMP_CR1_CPS & 0x00040000)
#define CMP_PositiveInput_CMPxP5						(CMP_CR1_CPS & 0x00050000)
#define CMP_PositiveInput_CMPxP6						(CMP_CR1_CPS & 0x00060000)
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PTM280x)
#define IS_CMP_PositiveInput(SEL)						(((SEL) & ~CMP_CR1_CPS) == 0 && (SEL) <= CMP_PositiveInput_CMPxP3)
#elif defined (PTM160x)
#define IS_CMP_PositiveInput(SEL)						(((SEL) & ~CMP_CR1_CPS) == 0 && (SEL) <= CMP_PositiveInput_CMPxP4)
#else
#define IS_CMP_PositiveInput(SEL)						(((SEL) & ~CMP_CR1_CPS) == 0 && (SEL) <= CMP_PositiveInput_CMPxP6)
#endif

/** @defgroup CMP Output PWM Polarity control
  * @{
  */
#define CMP_OutputPWMPolarity_Low						(CMP_CR1_PPC & 0x00000000)
#define CMP_OutputPWMPolarity_High						(CMP_CR1_PPC & 0x00200000)
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_CMP_OutputPWMPolarity(SEL)					(0)
#else
#define IS_CMP_OutputPWMPolarity(SEL)					((SEL) == CMP_OutputPWMPolarity_Low || (SEL) == CMP_OutputPWMPolarity_High)
#endif
														
/** @defgroup CMP OUTPUT PWM selection
  * @{
  */
#define CMP_TIM1_PWM1									(CMP_CR1_PWMS & 0x01000000)
#define CMP_TIM1_PWM2									(CMP_CR1_PWMS & 0x02000000)
#define CMP_TIM1_PWM3									(CMP_CR1_PWMS & 0x03000000)
#define CMP_TIM1_PWM4									(CMP_CR1_PWMS & 0x04000000)
#define CMP_TIM5_PWM1									(CMP_CR1_PWMS & 0x05000000)
#define CMP_TIM6_PWM1									(CMP_CR1_PWMS & 0x06000000)
#define CMP_TIM7_PWM1									(CMP_CR1_PWMS & 0x07000000)
#define CMP_TIM8_PWM1									(CMP_CR1_PWMS & 0x09000000)
#define CMP_TIM8_PWM2									(CMP_CR1_PWMS & 0x0A000000)
#define CMP_TIM8_PWM3									(CMP_CR1_PWMS & 0x0B000000)
#define CMP_TIM8_PWM4									(CMP_CR1_PWMS & 0x0C000000)
#if defined (PT32G031x) || defined (PTM280x)
#define IS_CMP_OutputPWM(SEL)							(((SEL) & ~CMP_CR1_PWMS) == 0 && (SEL) != 0 && \
															(SEL) != CMP_TIM5_PWM1 && (SEL) != CMP_TIM6_PWM1 && (SEL) != CMP_TIM7_PWM1)
#elif defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_CMP_OutputPWM(SEL)							(0)
#else
#define	IS_CMP_OutputPWM(SEL)							(((SEL) & ~CMP_CR1_PWMS) == 0 && (SEL) <= CMP_TIM8_PWM4)
#endif

/** @defgroup CMP interrupt selection
  * @{
  */
#define CMP_IT_COF										CMP_IER_COF
#define CMP_IT_COR										CMP_IER_COR
#define CMP_IT_ALL										(CMP_IT_COF | CMP_IT_COR)
#define IS_CMP_IT(SEL)									(((SEL) & ~CMP_IT_ALL) == 0 && (SEL) != 0)

/** @defgroup CMP state flag selection
  * @{
  */
#define CMP_FLAG_COF									CMP_SR_COF
#define CMP_FLAG_COR									CMP_SR_COR
#define CMP_FLAG_CRS									CMP_SR_CRS
#define CMP_FLAG_ALL									(CMP_FLAG_COF | CMP_FLAG_COR | CMP_SR_CRS)
#define CMP_CLEAR_FLAG_ALL								(CMP_FLAG_COF | CMP_FLAG_COR)
#define IS_CMP_FLAG(SEL)								(((SEL) & ~CMP_FLAG_ALL) == 0 && (SEL) != 0)
#define IS_CMP_CLEAR_FLAG(SEL)							(((SEL) & ~CMP_CLEAR_FLAG_ALL) == 0 && (SEL) != 0)

/** @defgroup IS_CMP_Initialization Delay Time
  * @{
  */
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_CMP_InitializationDelayTime(SEL) 			((SEL) <= 0x3F)
#else
#define IS_CMP_InitializationDelayTime(SEL) 			((SEL) <= 0x7FF)
#endif

/** @defgroup CMP_CKD
* @{
*/
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_CMP_ClockDiv(SEL)       						((SEL) <= 63)
#elif defined (PT32G031x) || defined (PTM280x)
#define IS_CMP_ClockDiv(SEL)       						(0)
#else
#define IS_CMP_ClockDiv(SEL)       						((SEL) <= 0xFF)
#endif

/** @defgroup CMP_ScanQueue
* @{
*/
#define CMP_Scan_P0E									CMP_CR3_P0E
#define CMP_Scan_P1E									CMP_CR3_P1E
#define CMP_Scan_P2E									CMP_CR3_P2E
#define CMP_Scan_P3E									CMP_CR3_P3E
#define CMP_Scan_P4E									CMP_CR3_P4E
#define CMP_Scan_P5E									CMP_CR3_P5E
#define CMP_Scan_P6E									CMP_CR3_P6E
#if defined (PT32G031x) || defined (PTM280x)
#define CMP_Scan_ALL									(0)
#elif defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define CMP_Scan_ALL									(CMP_Scan_P0E | CMP_Scan_P1E | CMP_Scan_P2E | CMP_Scan_P3E | CMP_Scan_P4E)
#else
#define CMP_Scan_ALL									(CMP_Scan_P0E | CMP_Scan_P1E | CMP_Scan_P2E | CMP_Scan_P3E | \
															CMP_Scan_P4E | CMP_Scan_P5E | CMP_Scan_P6E)
#endif
#define IS_CMP_ScanQueue(SEL)							(((SEL) & ~CMP_Scan_ALL) == 0 && (SEL) != 0)

/** @defgroup CMP_ScanTime
* @{
*/
#if defined(PT32G031x) || defined (PTM280x)
#define IS_CMP_ScanTime(SEL)       						(0)
#else
#define IS_CMP_ScanTime(SEL)       						((SEL) <= 0xFF)
#endif


void CMP_Init(CMP_TypeDef *CMPx, CMP_InitTypeDef *CMP_InitStruct);
void CMP_StructInit(CMP_InitTypeDef *CMP_InitStruct);
void CMP_Cmd(CMP_TypeDef *CMPx, FunctionalState NewState);
void CMP_PowerModeConfig(CMP_TypeDef *CMPx, u32 Mode);
void CMP_OutputPolarityConfig(CMP_TypeDef *CMPx, u32 Polarity);
void CMP_HysteresisCmd(CMP_TypeDef *CMPx, FunctionalState NewState);
void CMP_NegativeIntputConfig(CMP_TypeDef *CMPx, u32 NegativeIntput);
void CMP_DigitalFilterConfig(CMP_TypeDef *CMPx, u32 Filter);
void CMP_HysteresisVoltageConfig(CMP_TypeDef *CMPx, u32 Voltage);
void CMP_PositiveIntputConfig(CMP_TypeDef *CMPx, u32 PositiveIntput);
void CMP_OutputSynchronization(CMP_TypeDef *CMPx, FunctionalState NewState);
void CMP_OutputPWMPolarityConfig(CMP_TypeDef *CMPx, u32 Polarity);
void CMP_OutputPWMConfig(CMP_TypeDef *CMPx, u32 TIMx_PWMx);
void CMP_ITConfig(CMP_TypeDef *CMPx, u32 CMP_IT, FunctionalState NewState);
FlagStatus CMP_GetFlagStatus(CMP_TypeDef *CMPx, u32 CMP_FLAG);
void CMP_ClearFlag(CMP_TypeDef *CMPx, u32 CMP_FLAG);
ITStatus CMP_GetITStatus(CMP_TypeDef *CMPx, u32 CMP_IT);
u8 CMP_GetPositiveIntput(CMP_TypeDef *CMPx);
void CMP_InitializationDelayTime(CMP_TypeDef *CMPx, u32 Time);
void CMP_SetClockDivision(CMP_TypeDef *CMPx, u8 CMP_ClockDiv);
void CMP_ScanQueueConfig(CMP_TypeDef *CMPx,u8 ScanQueue);
void CMP_ScanTimeConfig(CMP_TypeDef *CMPx,u8 Time);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__PT32x0xx_CMP_H */

