/**
	******************************************************************************
  * @file    PT32x0xx_irtim.h
  * @author  Application development team
  * @version V1.0.0
  * @date    2024/11/20
  * @brief   This file contains all the functions prototypes for the IRTIM firmware
  *          library
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

#ifndef PT32x0xx_IRTIM_H
#define PT32x0xx_IRTIM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PT32x0xx.h"

/** @defgroup IRTIM_InitTypeDef
* @{
*/
typedef struct
{
	u32 IRTIM_DataSource;					/*!< This member Configure the IRTIM data source. @ref IRTTM_DSS. */

	u32 IRTIM_CarrierSource;				/*!< This member Configure the IRTIM carrier source. @ref IRTTM_CSS.*/

#if !defined(PT32x060x) && !defined(PT32x063x) && !defined(PT32x066x)
	u32 IRTIM_DataPolarity;					/*!< This member Configure the IRTIM data polarity. @ref IRTTM_DSP. */
#endif
} IRTIM_InitTypeDef;

/** @defgroup IRTIM_ALL_PERIPH
* @{
*/
#if defined(PT32G031x) || defined (PTM280x)
#define IS_IRTIM_ALL_PERIPH(SEL)      						((SEL) == 0)
#else
#define	IS_IRTIM_ALL_PERIPH(SEL)      						((SEL) == IRTIM)
#endif

/** @defgroup IRTIM_OutputPolarity
 * @{
 */
#define IRTIM_OutputPolarity_Normal							(IRTIM_CR_OP & 0x00000000)
#define IRTIM_OutputPolarity_Reverse						(IRTIM_CR_OP & 0x00000002)
#if defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x)
#define IS_IRTIM_OutputPolarity(SEL)						(0)
#else
#define IS_IRTIM_OutputPolarity(SEL)						((SEL) == IRTIM_OutputPolarity_Normal || (SEL) == IRTIM_OutputPolarity_Reverse)
#endif
	
/** @defgroup IRTTM_DSS
* @{
*/
#define IRTIM_DSS_TIM1_OC4                					(IRTIM_CFGR_DSS & 0x00000001)
#define IRTIM_DSS_TIM5_OC1                					(IRTIM_CFGR_DSS & 0x00000005)
#define IRTIM_DSS_TIM6_OC1                					(IRTIM_CFGR_DSS & 0x00000006)
#define IRTIM_DSS_TIM7_OC1                					(IRTIM_CFGR_DSS & 0x00000007)
#define IRTIM_DSS_TIM8_OC1                					(IRTIM_CFGR_DSS & 0x00000008)
#define IRTIM_DSS_LPTIM_OC1                					(IRTIM_CFGR_DSS & 0x0000000A)
#define IRTIM_DSS_TIM20_OC1                					(IRTIM_CFGR_DSS & 0x00000014)
#define IRTIM_DSS_TIM22_OC1                					(IRTIM_CFGR_DSS & 0x00000016)
#define IRTIM_DSS_UART0_USART0_TX                			(IRTIM_CFGR_DSS & 0x0000001A)
#define IRTIM_DSS_UART1_USART1_TX                			(IRTIM_CFGR_DSS & 0x0000001B)
#define IRTIM_DSS_UART2_TX                					(IRTIM_CFGR_DSS & 0x0000001C)
#define IRTIM_DSS_UART3_TX                					(IRTIM_CFGR_DSS & 0x0000001D)
#define IRTIM_DSS_LPUART0_TX                				(IRTIM_CFGR_DSS & 0x0000001E)
#define IRTIM_DSS_LPUART1_TX                				(IRTIM_CFGR_DSS & 0x0000001F)
#if defined(PT32x012x) || defined (PTM160x)
#define IS_IRTIM_DSS(SEL)	               					((SEL) == IRTIM_DSS_TIM20_OC1 || (SEL) == IRTIM_DSS_TIM22_OC1  || \
																(SEL) == IRTIM_DSS_UART0_USART0_TX || (SEL) == IRTIM_DSS_LPUART0_TX)
#elif defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x)
#define IS_IRTIM_DSS(SEL)	               					((SEL) == IRTIM_DSS_TIM5_OC1 || (SEL) == IRTIM_DSS_TIM6_OC1  || \
																(SEL) == IRTIM_DSS_UART0_USART0_TX || (SEL) == IRTIM_DSS_UART1_USART1_TX || \
																(SEL) == IRTIM_DSS_LPUART0_TX || (SEL) == IRTIM_DSS_LPUART1_TX)
#else
#define IS_IRTIM_DSS(SEL)	               					((SEL) == IRTIM_DSS_TIM1_OC4 || ((SEL) >= IRTIM_DSS_TIM5_OC1 && (SEL) <= IRTIM_DSS_TIM8_OC1) || \
																(SEL) == IRTIM_DSS_LPTIM_OC1 || (SEL) == IRTIM_DSS_TIM20_OC1 || (SEL) == IRTIM_DSS_TIM22_OC1 || \
																((SEL) >= IRTIM_DSS_UART0_USART0_TX && (SEL) <= IRTIM_DSS_LPUART1_TX))
#endif

/** @defgroup IRTTM_CSS
* @{
*/
#define IRTIM_CSS_LSI                						(IRTIM_CFGR_CSS & 0x00000000)
#define IRTIM_CSS_TIM1_OC4                					(IRTIM_CFGR_CSS & 0x00000100)
#define IRTIM_CSS_LSE                						(IRTIM_CFGR_CSS & 0x00000200)
#define IRTIM_CSS_TIM5_OC1                					(IRTIM_CFGR_CSS & 0x00000500)
#define IRTIM_CSS_TIM6_OC1                					(IRTIM_CFGR_CSS & 0x00000600)
#define IRTIM_CSS_TIM7_OC1                					(IRTIM_CFGR_CSS & 0x00000700)
#define IRTIM_CSS_TIM8_OC1                					(IRTIM_CFGR_CSS & 0x00000800)
#define IRTIM_CSS_LPTIM_OC1                					(IRTIM_CFGR_CSS & 0x00000A00)
#define IRTIM_CSS_TIM20_OC1                					(IRTIM_CFGR_CSS & 0x00001400)
#define IRTIM_CSS_TIM22_OC1                					(IRTIM_CFGR_CSS & 0x00001600)
#if defined(PT32x012x) || defined (PTM160x)
#define IS_IRTIM_CSS(SEL)	               					(((SEL) & ~IRTIM_CFGR_CSS) == 0 && ((SEL) == IRTIM_CSS_LSI || (SEL) == IRTIM_CSS_LSE || \
																(SEL) == IRTIM_CSS_TIM20_OC1 || (SEL) == IRTIM_CSS_TIM22_OC1))
#elif defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x)
#define IS_IRTIM_CSS(SEL)	               					(((SEL) & ~IRTIM_CFGR_CSS) == 0 && ((SEL) == IRTIM_CSS_LSI || (SEL) == IRTIM_CSS_TIM5_OC1 || \
																(SEL) == IRTIM_CSS_TIM6_OC1))
#else
#define IS_IRTIM_CSS(SEL)	               					(((SEL) & ~IRTIM_CFGR_CSS) == 0 && ((SEL) <= IRTIM_CSS_LSE || \
																((SEL) >= IRTIM_CSS_TIM5_OC1 && (SEL) <= IRTIM_CSS_TIM8_OC1) || \
																(SEL) == IRTIM_CSS_LPTIM_OC1 || (SEL) == IRTIM_CSS_TIM20_OC1 || (SEL) == IRTIM_CSS_TIM22_OC1))
#endif
	
/** @defgroup IRTTM_DSP
 * @{
 */
#define IRTIM_DSP_Normal                					(IRTIM_CFGR_DSP & 0x00000000)
#define IRTIM_DSP_Reverse                 					(IRTIM_CFGR_DSP & 0x00010000)
#if defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x)
#define IS_IRTIM_DSP(SEL)	               					(0)
#else
#define IS_IRTIM_DSP(SEL)	               					((SEL) == IRTIM_DSP_Normal || (SEL) == IRTIM_DSP_Reverse)
#endif

void IRTIM_Init(IRTIM_TypeDef *IRTIMx, IRTIM_InitTypeDef *IRTIM_InitStruct);
void IRTIM_StructInit(IRTIM_InitTypeDef *IRTIM_InitStruct);
void IRTIM_Cmd(IRTIM_TypeDef *IRTIMx, FunctionalState NewState);
void IRTIM_OutputPolarityControl(IRTIM_TypeDef *IRTIMx, u32 Polarity);
	
#ifdef __cplusplus
}
#endif

#endif /*__PT32x0xx_UART_H */
