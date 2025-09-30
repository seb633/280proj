/**
******************************************************************************
* @file    PT32x0xx_iwdg.h
* @author  Application development team
* @version V1.0.0
* @date    2025/2/13
* @brief   This file contains all the functions prototypes for the IWDG firmware
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
 
#ifndef PT32x0xx_IWDG_H
#define PT32x0xx_IWDG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PT32x0xx.h"


/** @defgroup IWDG_ALL_PERIPH
* @{
*/
#define IS_IWDG_ALL_PERIPH(SEL)     		((SEL) == IWDG)

/** @defgroup WDG_LockKey
* @{
*/	
#define IWDG_LockKey_Unlock        			((u32)0x1ACCE551)
#define IWDG_LockKey_Lock          			((u32)0x00000000)
#define IS_IWDG_LockKey(SEL)				((SEL) == IWDG_LockKey_Unlock || (SEL) == IWDG_LockKey_Lock)

/** @defgroup IWDG_FLAG
* @{
*/
typedef enum
{
	IWDG_FLAG_HungryDog   = 0x00,
	IWDG_FLAG_LockStatus,
} IWDG_FLAG;
#define IS_IWDG_FLAG(SEL)			 		((SEL) == IWDG_FLAG_HungryDog || (SEL) == IWDG_FLAG_LockStatus)

/** @defgroup IWDG_ReloadValue
* @{
*/
#define IS_IWDG_ReloadValue(SEL) 			((SEL) <= 0xFFFFFFFF)

/*! function declaration and definition*/
void IWDG_SetReload(IWDG_TypeDef *IWDGx, u32 Reload);
void IWDG_Cmd(IWDG_TypeDef *IWDGx, FunctionalState NewState);
void IWDG_DBGPendingCmd(IWDG_TypeDef *IWDGx, FunctionalState NewState);
void IWDG_ReloadCounter(IWDG_TypeDef *IWDGx);
FlagStatus IWDG_GetFlagStatus(IWDG_TypeDef *IWDGx, IWDG_FLAG Flag);
void IWDG_LockCmd(IWDG_TypeDef *IWDGx, u32 Key);
#ifdef __cplusplus
}
#endif

#endif /*__PT32x0xx_IWDG_H */
