/******************************************************************************
  * @file    PT32x0xx_ldac.h
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the LDAC firmware library
  *
  ******************************************************************************
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of
  * the content or the codes within the content.
  *
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  *******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PT32x0xx_LDAC_H
#define PT32x0xx_LDAC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "PT32x0xx.h"


/** @addtogroup LDAC
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** @defgroup IS_LDAC_ALL_PERIPH
  * @{
  */
#if defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x)
#define IS_LDAC_ALL_PERIPH(SEL)					((SEL) == 0)
#else
#define IS_LDAC_ALL_PERIPH(SEL)					((SEL) == LDAC0)
#endif

/** @defgroup IS_LDAC_RVSPS
  * @{
  */
#define LDAC_RVSPS_VDDA							(LDAC_CR_RVSPS & 0x00000000)
#define LDAC_RVSPS_BGREF						(LDAC_CR_RVSPS & 0x00000010)
#if defined (PT32G031x) || defined (PTM280x)
#define IS_LDAC_ReferencePositive(SEL)			(0)
#else
#define IS_LDAC_ReferencePositive(SEL)			((SEL) == LDAC_RVSPS_VDDA || (SEL) == LDAC_RVSPS_BGREF)
#endif

/** @defgroup IS_LDAC_PowerMode
  * @{
  */
#define LDAC_PowerMode_Normal					(LDAC_CR_PM & 0x00000000)
#define LDAC_PowerMode_HighSpeed				(LDAC_CR_PM & 0x00010000)
#if defined (PT32G031x) || defined (PTM280x)
#define IS_LDAC_PowerMode(SEL)					(0)
#else
#define IS_LDAC_PowerMode(SEL)					((SEL) == LDAC_PowerMode_Normal || (SEL) == LDAC_PowerMode_HighSpeed)
#endif

/** @defgroup IS_LDAC_Value
  * @{
  */
#if defined (PT32x002x) || defined (PT32G031x)|| defined (PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_LDAC_Value(SEL)						((SEL) <= 0x1F)
#else
#define IS_LDAC_Value(SEL)						((SEL) <= 0x7F)
#endif

void LDAC_Cmd(LDAC_TypeDef *LDACx, FunctionalState NewState);
void LDAC_OutputConfig(LDAC_TypeDef *LDACx, FunctionalState NewState);
void LDAC_SetRVSPS(LDAC_TypeDef *LDACx, u32 Source);
void LDAC_SetPowerMode(LDAC_TypeDef *LDACx, u32 Mode);
void LDAC_SetData(LDAC_TypeDef *LDACx, u32 Data);

#ifdef __cplusplus
}
#endif

#endif /*__PT32x0xx_LDAC_H */
