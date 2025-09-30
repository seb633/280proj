/******************************************************************************
  * @file    PT32_type.h
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief    This file contains all the functions prototypes for the TYPE firmware library.
  *
  ******************************************************************************
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers. 
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of the content or the  codes within the content.
  *
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  * 
  *****************************************************************************/

#ifndef PT32_Type_H
#define PT32_Type_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#if defined (__CC_ARM)
#include <rt_misc.h>
#endif


/** @addtogroup Exported_types **/
typedef signed long  s32;
typedef signed short s16;
typedef signed char  s8;

typedef unsigned long long  u64;
typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;
typedef unsigned char  BOOL;

#ifndef NULL
#define NULL  0
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus, RemapStatus, ProtectStatus, BitAction;
typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) 					((STATE) == DISABLE || (STATE) == ENABLE)

#ifdef __cplusplus
}
#endif
#endif
