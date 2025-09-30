/**
******************************************************************************
* @file    PT32x0xx_exti.h
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
******************************************************************************
**/

#ifndef PT32x0xx_EXTI_H
#define PT32x0xx_EXTI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PT32x0xx.h"

/** @defgroup EXTI_ALL_PERIPH
* @{
*/
#if defined (PT32x002x) || defined(PT32G031x) || defined(PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_EXTI_ALL_PERIPH(SEL)			((SEL) == EXTIA || (SEL) == EXTIB)
#elif defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x)
#define IS_EXTI_ALL_PERIPH(SEL)			((SEL) == EXTIA || (SEL) == EXTIB || (SEL) == EXTIC || (SEL) == EXTID || (SEL) == EXTIE)
#else
#define IS_EXTI_ALL_PERIPH(SEL)			((SEL) == EXTIA || (SEL) == EXTIB || (SEL) == EXTIC || (SEL) == EXTID || (SEL) == EXTIE || (SEL) == EXTIF)
#endif

/** @defgroup EXTI_Trigger
* @{
*/
typedef enum
{
	EXTI_Trigger_LowLevel = 0,
	EXTI_Trigger_Falling,
	EXTI_Trigger_HighLevel,
	EXTI_Trigger_Rising,
	EXTI_Trigger_RisingFalling
} EXTI_Trigger;
#define IS_EXTI_Trigger(SEL)			((SEL) <= EXTI_Trigger_RisingFalling)

/** @defgroup EXTI_Pin
* @{
*/
#define EXTI_Pin_0						((u16)0x0001)		//!< Pin 0 selected
#define EXTI_Pin_1						((u16)0x0002)		//!< Pin 1 selected
#define EXTI_Pin_2						((u16)0x0004)		//!< Pin 2 selected
#define EXTI_Pin_3						((u16)0x0008)		//!< Pin 3 selected
#define EXTI_Pin_4						((u16)0x0010)		//!< Pin 4 selected
#define EXTI_Pin_5						((u16)0x0020)		//!< Pin 5 selected
#define EXTI_Pin_6						((u16)0x0040)		//!< Pin 6 selected
#define EXTI_Pin_7						((u16)0x0080)		//!< Pin 7 selected
#define EXTI_Pin_8						((u16)0x0100)		//!< Pin 8 selected
#define EXTI_Pin_9						((u16)0x0200)		//!< Pin 9 selected
#define EXTI_Pin_10						((u16)0x0400)		//!< Pin 10 selected
#define EXTI_Pin_11						((u16)0x0800)		//!< Pin 11 selected
#define EXTI_Pin_12						((u16)0x1000)		//!< Pin 12 selected
#define EXTI_Pin_13						((u16)0x2000)		//!< Pin 13 selected
#define EXTI_Pin_14						((u16)0x4000)		//!< Pin 14 selected
#define EXTI_Pin_15						((u16)0x8000)		//!< Pin 15 selected
#define EXTI_Pin_All					((u16)0xFFFF)		//!< All pins selected
#define IS_EXTI_Pin(SEL)        		(((SEL) & ~EXTI_Pin_All) == 0 && (SEL) != 0)

/** @defgroup EXTI_IT
* @{
*/
#define EXTI_IT_0						((u16)0x0001)
#define EXTI_IT_1						((u16)0x0002)
#define EXTI_IT_2						((u16)0x0004)
#define EXTI_IT_3						((u16)0x0008)
#define EXTI_IT_4						((u16)0x0010)
#define EXTI_IT_5						((u16)0x0020)
#define EXTI_IT_6						((u16)0x0040)
#define EXTI_IT_7						((u16)0x0080)
#define EXTI_IT_8						((u16)0x0100)
#define EXTI_IT_9						((u16)0x0200)
#define EXTI_IT_10						((u16)0x0400)
#define EXTI_IT_11						((u16)0x0800)
#define EXTI_IT_12						((u16)0x1000)
#define EXTI_IT_13						((u16)0x2000)
#define EXTI_IT_14						((u16)0x4000)
#define EXTI_IT_15						((u16)0x8000)
#define EXTI_IT_All						((u16)0xFFFF)
#define IS_EXTI_IT(SEL)        			(((SEL) & ~EXTI_IT_All) == 0 && (SEL) != 0)

/** @defgroup EXTI_FLAG
* @{
*/
#define EXTI_FLAG_0						((u16)0x0001)
#define EXTI_FLAG_1						((u16)0x0002)
#define EXTI_FLAG_2						((u16)0x0004)
#define EXTI_FLAG_3						((u16)0x0008)
#define EXTI_FLAG_4						((u16)0x0010)
#define EXTI_FLAG_5						((u16)0x0020)
#define EXTI_FLAG_6						((u16)0x0040)
#define EXTI_FLAG_7						((u16)0x0080)
#define EXTI_FLAG_8						((u16)0x0100)
#define EXTI_FLAG_9						((u16)0x0200)
#define EXTI_FLAG_10					((u16)0x0400)
#define EXTI_FLAG_11					((u16)0x0800)
#define EXTI_FLAG_12					((u16)0x1000)
#define EXTI_FLAG_13					((u16)0x2000)
#define EXTI_FLAG_14					((u16)0x4000)
#define EXTI_FLAG_15					((u16)0x8000)
#define EXTI_FLAG_All					((u16)0xFFFF)
#define IS_EXTI_FLAG(SEL)        		(((SEL) & ~EXTI_FLAG_All) == 0 && (SEL) != 0)

/*! function declaration and definition*/
void EXTI_ITConfig(EXTI_TypeDef *EXTIx, u32 EXTI_IT, FunctionalState NewState);
void EXTI_TriggerTypeConfig(EXTI_TypeDef *EXTIx, u32 EXTI_Pin, EXTI_Trigger TriggerType);
void EXTI_ClearFlag(EXTI_TypeDef *EXTIx, u32 EXTI_IT_FLAG);
ITStatus EXTI_GetITStatus(EXTI_TypeDef *EXTIx, u32 EXTI_IT);

#ifdef __cplusplus
}
#endif

#endif /*__PT32x0xx_EXTI_H */
