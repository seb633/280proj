/**
******************************************************************************
* @file    PT32x0xx_ifmc.h
* @author  Application development team
* @version V1.0.0
* @date    2025/2/13
* @brief   This file contains all the functions prototypes for the IFMC firmware library.
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
#ifndef __PT32x0xx_IFMC_H
#define __PT32x0xx_IFMC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "PT32x0xx.h"



/** @addtogroup IFMC
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup IFMC_flash_size_selction
  * @{
  */
#if defined(FLASH_SIZE_16K)
	#define MAINCODE_END_ADDR    					((u32)0x00003FFF)
#elif defined(FLASH_SIZE_32K)
	#define MAINCODE_END_ADDR    					((u32)0x00007FFF)
#elif defined(FLASH_SIZE_48K)
	#define MAINCODE_END_ADDR    					((u32)0x0000BFFF)
#elif defined(FLASH_SIZE_64K)
	#define MAINCODE_END_ADDR    					((u32)0x0000FFFF)
#elif defined(FLASH_SIZE_120K)
	#define MAINCODE_END_ADDR    					((u32)0x0001DFFF)
#endif

/**  @brief  IFMC Status
  * @{
  */
typedef enum
{
	IFMC_WriteEnd = 1,
	IFMC_PageEraseEnd,
	IFMC_ChipEraseEnd,
	IFMC_CommandError,
	IFMC_KeyError,
	IFMC_AddrError,
	IFMC_Complete,
} IFMC_Status;

/** @defgroup IFMC_Latency
  * @{
  */
#if defined (PT32G031x) || defined (PTM280x)
#define IS_IFMC_Latency(SEL)					((SEL) < 3)
#elif defined (PT32x002x) || defined (PT32x012x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM160x)
#define IS_IFMC_Latency(SEL)					((SEL) < 2)
#else
#define IS_IFMC_Latency(SEL)					((SEL) < 8)
#endif
/**
  * @}
  */

/** @defgroup IS_FLASH_PSC_Value
  * @{
  */
#define IS_IFMC_Prescaler(SEL) 	 	 			((SEL) <= 255)

/**
  * @}
  */

/** @brief  IFMC EncryptedArea addr **/
#define IS_IFMC_PageAddress(SEL)   				((SEL) < MAINCODE_END_ADDR)

/** @defgroup IFMC_interrupt_selection
  * @{
  */
#define IFMC_IT_WCIE							IFMC_IER_WCIE			/*!< Write operation complete interrupt enable */
#define IFMC_IT_PECIE							IFMC_IER_PECIE   		/*!< Page erase operation complete interrupt enable */
#define IFMC_IT_MECIE							IFMC_IER_MECIE  		/*!< Mass erase operation complete interrupt enable */
#define IFMC_IT_CERIE							IFMC_IER_CERIE			/*!< Operation command error interrupt enable */
#define IFMC_IT_AERIE							IFMC_IER_AERIE			/*!< Key error interrupt enable */
#define IFMC_IT_KERIE							IFMC_IER_KERIE			/*!< Address error interrupt enable */
#define IFMC_IT_ALL								(IFMC_IT_WCIE | IFMC_IT_PECIE | IFMC_IT_MECIE | IFMC_IT_CERIE | IFMC_IT_AERIE | IFMC_IT_KERIE)
#define IS_IFMC_IT(SEL)							((SEL) != 0 && ((SEL) & ~IFMC_IT_ALL) == 0)
/**
  * @}
  */

/** @defgroup IFMC_flag_selection
  * @{
  */
#define IFMC_FLAG_WC							IFMC_SR1_WC				/*!< Write operation complete */
#define IFMC_FLAG_PEC							IFMC_SR1_PEC   			/*!< Page erase operation complete */
#define IFMC_FLAG_MEC							IFMC_SR1_MEC  			/*!< Mass erase operation complete */
#define IFMC_FLAG_CERR							IFMC_SR1_CERR			/*!< Operation command error */
#define IFMC_FLAG_AERR							IFMC_SR1_AERR			/*!< Address error */
#define IFMC_FLAG_KERR							IFMC_SR1_KERR			/*!< Key error */
#define IFMC_FLAG_ALL							(IFMC_FLAG_WC | IFMC_FLAG_PEC | IFMC_FLAG_MEC | IFMC_FLAG_CERR | IFMC_FLAG_AERR | IFMC_FLAG_KERR)
#define IS_IFMC_FLAG(SEL) 						((SEL) != 0 && ((SEL) & ~IFMC_FLAG_ALL) == 0)
/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

IFMC_Status IFMC_GetStatus(void);
void IFMC_SetLatency(u32 Latency);
IFMC_Status IFMC_ProgramWord(u32 Address, u32 Data);
IFMC_Status IFMC_ProgramWords(u32 Address, u32 Data[], u32 Size);
IFMC_Status IFMC_ProgramDWord(u32 Address, u64 Data);
IFMC_Status IFMC_ProgramDWords(u32 Address, u64 Data[], u32 Size);
IFMC_Status IFMC_ErasePage(u32 Address);
IFMC_Status IFMC_EraseChip(void);
void IFMC_ITConfig(u32 IFMC_IT, FunctionalState NewState);
FlagStatus IFMC_GetFlagStatus(u32 IFMC_FLAG);
void IFMC_ClearFlag(u32 IFMC_FLAG);
ProtectStatus IFMC_GetReadProtectStatus(void);
void IFMC_SetReadProtect(FunctionalState NewState);
u32 IFMC_ReadWord(u32 Address);
u16 IFMC_ReadHalfWord(u32 Address);
u8 IFMC_ReadByte(u32 Address);

/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /*__PT32x0xx_IFMC_H */

