/**
******************************************************************************
* @file    PT32x0xx_es.h
* @author  Application development team
* @version V1.0.0
* @date    2025/2/13
* @brief   This file contains all the functions prototypes for the ES firmware
*          library
*
******************************************************************************
* @attention
*
* The firmware provided is for guidance only. The codes are offered for the convenience of the customers.
* Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of the content or the codes within the content.
*
* Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
******************************************************************************
**/

#ifndef PT32x0xx_ES_H
#define PT32x0xx_ES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PT32x0xx.h"

/** @brief ES CID Number **/
#define IS_ES_CIDNumber(num)					((num) < 3)

/** @brief ES UDI Number **/
#if defined (PT32x002x) || defined(PT32x012x) || defined (PTM160x)
#define IS_ES_UDINumber(num)					((num) < 2)
#else
#define IS_ES_UDINumber(num)					((num) < 3)
#endif

#define ES_UDIR_KEY_VALUE						((u32)0x15EC1CCA)

/*! function declaration and definition*/
u32 ES_GetChipInfo(void);
u32 ES_GetCID(u8 CID_Number);
u32 ES_GetUserDefinedInfo(u8 UDI_Number);
void ES_SetUserDefinedInfo(u8 UDI_Number, u32 UDI);
#ifdef __cplusplus
}
#endif

#endif /*__PT32x0xx_ID_H */
