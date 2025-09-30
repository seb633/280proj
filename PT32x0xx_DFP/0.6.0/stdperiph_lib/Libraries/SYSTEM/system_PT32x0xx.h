 /*
 *******************************************************************************
 * @file    system_PT32x0xx.h
 * @author  Application development team
 * @version V1.0.0
 * @date    2025/2/13
 * @brief   
 *******************************************************************************
 * @attention
 *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers. 
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of the content or the  codes within the content.
  *
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  * 
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYSTEM_PT32x0xx_H
#define __SYSTEM_PT32x0xx_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PT32_Type.h"

/** @defgroup ClockSource_Frequency_Selection
  * @{
  */
#define CLKSRC_SYSCLK									((u8)0x01)
#define CLKSRC_HCLK										((u8)0x02)
#define CLKSRC_PCLK										((u8)0x03)
#define CLKSRC_HSI  									((u8)0x04)
#define CLKSRC_LSI  									((u8)0x05)
#define	CLKSRC_HSE  									((u8)0x06)
#define CLKSRC_LSE  									((u8)0x07)
#if defined (PT32G031x) || defined (PTM280x)
#define IS_ClockSource(SEL)   				           	((SEL) <= CLKSRC_LSI && (SEL) != 0x0)
#elif defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_ClockSource(SEL)   				           	((SEL) <= CLKSRC_LSE && (SEL) != CLKSRC_HSE && (SEL) != 0x0)
#else
#define IS_ClockSource(SEL)   				           	((SEL) <= CLKSRC_LSE && (SEL) != 0x0)
#endif

/**
  * @}
  */
u32 GetSystemClock(void);
void CLOCK_Cmd(void);
void CLOCK_SystemClockConfig(void);
void CLOCK_MCOConfig(void);
u32 GetClockFreq(u8 clock);
void SystemInit(void);

#ifdef __cplusplus
}
#endif

#endif /*system_PT32x0xx_h*/

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */


/******************* Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved *****END OF FILE****/

