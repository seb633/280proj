/**
	******************************************************************************
	* @file    PT32x0xx_syscfg.h
	* @author  Application development team
	* @version V1.0.0
	* @date    2025/2/13
	* @brief   This file contains all the functions prototypes for the SYCFG firmware library.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PT32x0xx_SYSCFG_H
#define PT32x0xx_SYSCFG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "PT32x0xx.h"

/** @defgroup SYSCFG_RAM_SIZE_selction
  * @{
  */

/*----------------------------------------------------------*/
/* 		          User Please Select Ram Size        		*/
/*----------------------------------------------------------*/
#if defined (SRAM_SIZE_3K)
#define VTORS_SRAM_END_ADDR   				((u32)0x20000BFF)
#elif defined (SRAM_SIZE_4K)
#define VTORS_SRAM_END_ADDR   				((u32)0x20000FFF)
#elif defined (SRAM_SIZE_8K)
#define VTORS_SRAM_END_ADDR   				((u32)0x20001FFF)
#endif

/**
  * @}
  */
#if defined (PT32x002x) || defined(PT32x012x) || defined (PTM160x)
#define IS_SYSCFG_VTORSAddress(SEL)			(0)
#else
#define IS_SYSCFG_VTORSAddress(SEL)			(((SEL)&0x3) == 0 && (SEL) < VTORS_SRAM_END_ADDR)
#endif

/** @defgroup SYSCFG_Code_Key
  * @{
  */
#define SYSCFG_CR_KEY_VALUE					((u32)0x15EC1CCA)
#define SYSCFG_CR_IAP						((u32)0x000000D3)
#define SYSCFG_CR_None						((u32)0x00000000)
#define SYSCFG_IVTS_KEY						((u32)0x584D5049)
/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SYSCFG_IAPConfig(FunctionalState NewState);
void SYSCFG_VTORSEnable(u32 Address);
void SYSCFG_VTORSDisable(void);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__PT32x0xx_SYSCFG_H */
