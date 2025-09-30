/**
  ******************************************************************************
  * @file    PT32x0xx_ledpwm.h
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the LEDPWM firmware
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

#ifndef __PT32x0xx_LEDPWM_H
#define __PT32x0xx_LEDPWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PT32x0xx.h"

/* Private define ------------------------------------------------------------*/

/** @defgroup LEDPWM_ALL_PERIPH
* @{
*/
#if defined (PT32G031x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM280x)
#define IS_LEDPWM_ALL_PERIPH(SEL) 					(0)
#else
#define IS_LEDPWM_ALL_PERIPH(SEL) 					((SEL) == LEDPWM)
#endif

/** @defgroup LEDPWM_InitTypeDef
* @{
*/
typedef struct
{
	u32 LEDPWM_Code0Duty;       					/*!< Specifies the code 0 positive duty. @ref LEDPWM_Code0Duty. */

	u32 LEDPWM_Code1Duty; 						 	/*!< Specifies the code 1 positive duty. @ref LEDPWM_Code1Duty. */

	u32 LEDPWM_Cycle0;						 		/*!< Specifies the code 0 cycle. @ref LEDPWM_Cycle0. */

	u32 LEDPWM_Cycle1;		 						/*!< Specifies the code 1 cycle. @ref LEDPWM_Cycle1. */
	
	u32 LEDPWM_CycleControl;						/*!< Specifies the data code cycle control function. @ref LEDPWM_DataCodeCycleControl. */
	
	u32 LEDPWM_OutputPolarity;						/*!< Specifies the output polarity. @ref LEDPWM_OutputPolarity. */
	
	u32 LEDPWM_DataCheck;							/*!< Specifies the data check function. @ref LEDPWM_DataCheck. */
} LEDPWM_InitTypeDef;

/** @defgroup LEDPWM_DataCheck
 * @{
 */
#define LEDPWM_DataCheck_0							(LEDPWM_CR1_CC & 0x00000000)
#define LEDPWM_DataCheck_1							(LEDPWM_CR1_CC & 0x00000100)
#define LEDPWM_DataCheck_Even						(LEDPWM_CR1_CC & 0x00000200)
#define LEDPWM_DataCheck_Odd						(LEDPWM_CR1_CC & 0x00000300)
#define IS_LEDPWM_DataCheck(SEL)					(((SEL) & ~LEDPWM_CR1_CC) == 0 && (SEL) <= LEDPWM_DataCheck_Odd)
	
/** @defgroup LEDPWM_OutputPolarity
 * @{
 */
#define LEDPWM_OutputPolarity_Normal				(LEDPWM_CR1_OP & 0x00000000)
#define LEDPWM_OutputPolarity_Reverse				(LEDPWM_CR1_OP & 0x00010000)
#define IS_LEDPWM_OutputPolarity(SEL)				((SEL) == LEDPWM_OutputPolarity_Normal || (SEL) == LEDPWM_OutputPolarity_Reverse)

/** @defgroup LEDPWM_DataCodeCycleControl
 * @{
 */
#define LEDPWM_DataCodeCycleControl_Share			(LEDPWM_CR1_DCC & 0x00000000)
#define LEDPWM_DataCodeCycleControl_Private			(LEDPWM_CR1_DCC & 0x00100000)
#define IS_LEDPWM_DataCodeCycleControl(SEL)			((SEL) == LEDPWM_DataCodeCycleControl_Share || (SEL) == LEDPWM_DataCodeCycleControl_Private)
	
/** @defgroup LEDPWM_Code0Duty
* @{
*/
#define IS_LEDPWM_Code0Duty(SEL) 					((SEL) <= 0xFF)

/** @defgroup LEDPWM_Code1Duty
* @{
*/
#define IS_LEDPWM_Code1Duty(SEL) 					((SEL) <= 0xFF)

/** @defgroup LEDPWM_Cycle0
* @{
*/
#define IS_LEDPWM_Cycle0(SEL) 						((SEL) <= 0xFF)

/** @defgroup LEDPWM_Cycle1
* @{
*/
#define IS_LEDPWM_Cycle1(SEL) 						((SEL) <= 0xFF)

/** @defgroup LEDPWM_Data
* @{
*/
#define IS_LEDPWM_Data(SEL)							((SEL) <= 0xFFFFFF)
	
/** @defgroup LEDPWM_DataLength
 * @{
 */
#define LEDPWM_DataLength_24						(LEDPWM_DR_LEN & 0x00000000)
#define LEDPWM_DataLength_4							(LEDPWM_DR_LEN & 0x01000000)
#define LEDPWM_DataLength_8							(LEDPWM_DR_LEN & 0x02000000)
#define LEDPWM_DataLength_12						(LEDPWM_DR_LEN & 0x03000000)
#define LEDPWM_DataLength_16						(LEDPWM_DR_LEN & 0x04000000)
#define LEDPWM_DataLength_20						(LEDPWM_DR_LEN & 0x05000000)
#define IS_LEDPWM_DataLength(SEL)					(((SEL) & ~LEDPWM_DR_LEN) == 0 && (SEL) <= LEDPWM_DataLength_20)

/** @defgroup LEDPWM_FLAG
* @{
*/
#define LEDPWM_FLAG_TXE                    			LEDPWM_SR_TXE
#define LEDPWM_FLAG_TXC                   			LEDPWM_SR_TXC
#define LEDPWM_FLAG_ALL								(LEDPWM_FLAG_TXE | LEDPWM_FLAG_TXC)
#define IS_LEDPWM_FLAG(SEL)               			(((SEL) & ~LEDPWM_FLAG_ALL) == 0 && (SEL) != 0)

/** @defgroup LEDPWM_IT
* @{
*/
#define LEDPWM_IT_TXE                    			LEDPWM_IER_TXE
#define LEDPWM_IT_TXC                   			LEDPWM_IER_TXC
#define LEDPWM_IT_ALL								(LEDPWM_IT_TXE | LEDPWM_IT_TXC)
#define IS_LEDPWM_IT(SEL)							(((SEL) & ~LEDPWM_IT_ALL) == 0 && (SEL) != 0)

void LEDPWM_Init(LEDPWM_TypeDef *LEDPWMx, LEDPWM_InitTypeDef *LEDPWMInit);
void LEDPWM_StructInit(LEDPWM_InitTypeDef *LEDPWMInit);
void LEDPWM_Cmd(LEDPWM_TypeDef *LEDPWMx, FunctionalState NewState);
void LEDPWM_SendData(LEDPWM_TypeDef *LEDPWMx, u32 Data, u8 Length, BOOL isCheck);
FlagStatus LEDPWM_GetFlagStatus(LEDPWM_TypeDef *LEDPWMx, u32 LEDPWM_FLAG);
ITStatus LEDPWM_GetITStatus(LEDPWM_TypeDef *LEDPWMx, u32 LEDPWM_IT);
void LEDPWM_ClearFlag(LEDPWM_TypeDef *LEDPWMx, u32 LEDPWM_CLEAR_FLAG);
void LEDPWM_ITConfig(LEDPWM_TypeDef *LEDPWMx, u32 LEDPWM_IT, FunctionalState NewState);


#ifdef __cplusplus
}
#endif

#endif /*__PT32x0xx_LEDPWM_H */
