/**
  ****************************************************************************
  * @file    PT32x0xx_dac.h
  * @author  Application development team
  * @version V1.0.0
  * @date    2024/12/20
  * @brief    This file contains all the functions prototypes for the DAC firmware library
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
#ifndef PT32x0xx_DAC_H
#define PT32x0xx_DAC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "PT32x0xx.h"


/** @addtogroup DAC
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup DAC_InitTypeDef
* @{
*/
typedef struct
{
	u32 DAC_RVSPS;								/*!< Specifies the DAC reference voltage source positive, This parameter can be a value of @ref DAC_RVSPS */
	
	u32 DAC_Align;								/*!< Specifies the DAC data align, This parameter can be a value of @ref DAC_Align */
	
	u32 DAC_PowerMode;							/*!< Specifies the DAC power mode, This parameter can be a value of @ref DAC_PowerMode */
}DAC_InitTypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** @defgroup DAC_ALL_PERIPH
  * @{
  */
#if defined (PT32G031x) || defined (PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_DAC_ALL_PERIPH(SEL)					((SEL) == 0)
#elif defined (PT32x060x) || defined (PT32x063x)
#define IS_DAC_ALL_PERIPH(SEL)					((SEL) == DAC0)
#else
#define IS_DAC_ALL_PERIPH(SEL)					((SEL) == DAC0 || (SEL) == DAC1)
#endif

/** @defgroup DAC_Align
* @{
*/
#define DAC_Align_Right							(DAC_CR1_ALIGN & 0x00000000)
#define DAC_Align_Left							(DAC_CR1_ALIGN & 0x00000010)
#define IS_DAC_Align(SEL)						((SEL) == DAC_Align_Right || (SEL) == DAC_Align_Left)

/** @defgroup DAC_RVSPS
  * @{
  */
#define DAC_RVSPS_VDDA							(DAC_CR1_RVSPS & 0x00000000)
#define DAC_RVSPS_VREFBUF						(DAC_CR1_RVSPS & 0x00000100)
#define IS_DAC_ReferencePositive(SEL)			((SEL) == DAC_RVSPS_VDDA || (SEL) == DAC_RVSPS_VREFBUF)

/** @defgroup DAC_PowerMode
  * @{
  */
#define DAC_PowerMode_Normal					(DAC_CR1_PM & 0x00000000)
#define DAC_PowerMode_HighSpeed					(DAC_CR1_PM & 0x00010000)
#define IS_DAC_PowerMode(SEL)					((SEL) == DAC_PowerMode_Normal || (SEL) == DAC_PowerMode_HighSpeed)

/** @defgroup DAC_Value
  * @{
  */
#define IS_DAC_Value(SEL)						((SEL) <= 0xFFFF)

/** @defgroup DAC_MFB_CutoffFrequency
  * @{
  */
#define DAC_MFB_5KHz							(DAC_CR2_FCFS & 0x00000000)
#define DAC_MFB_10KHz							(DAC_CR2_FCFS & 0x00000100)
#define DAC_MFB_20KHz							(DAC_CR2_FCFS & 0x00000200)
#define DAC_MFB_50KHz							(DAC_CR2_FCFS & 0x00000300)
#define DAC_MFB_100KHz							(DAC_CR2_FCFS & 0x00000400)
#define DAC_MFB_200KHz							(DAC_CR2_FCFS & 0x00000500)
#define DAC_MFB_400KHz							(DAC_CR2_FCFS & 0x00000600)
#define IS_DAC_MFB_CutoffFreq(SEL)				(((SEL) & ~DAC_CR2_FCFS) == 0 && (SEL) <= DAC_MFB_400KHz)

void DAC_Init(DAC_TypeDef *DACx, DAC_InitTypeDef *DAC_InitStruct);
void DAC_StructInit(DAC_InitTypeDef *DAC_InitStruct);
void DAC_Cmd(DAC_TypeDef *DACx, FunctionalState NewState);
void DAC_SetRVSPS(DAC_TypeDef *DACx, u32 Source);
void DAC_SetPowerMode(DAC_TypeDef *DACx, u32 Mode);
void DAC_OutputConfig(DAC_TypeDef *DACx, FunctionalState NewState);
void DAC_SetData(DAC_TypeDef *DACx, u32 Data);
void DAC_MFBCmd(DAC_TypeDef *DACx, FunctionalState NewState);
void DAC_SetMFBCutoffFreq(DAC_TypeDef *DACx, u32 Freq);

#ifdef __cplusplus
}
#endif

#endif /*__PT32x0xx_DAC_H */
