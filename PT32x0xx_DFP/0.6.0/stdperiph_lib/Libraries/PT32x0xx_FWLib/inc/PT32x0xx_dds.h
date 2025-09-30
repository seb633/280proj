/**
  ******************************************************************************
  * @file    PT32x0xx_dds.h
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the DDS firmware
  *          library
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of
  * the content or the codes within the content.
  *
  *  Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  *****************************************************************************
**/

#ifndef PT32x0xx_DDS_H
#define PT32x0xx_DDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PT32x0xx.h"

/** @defgroup DDS_InitTypeDef
* @{
*/
typedef struct
{
	u32 DDS_WaveMode;											/*!< Specifies the wave data mode selection,
																	This parameter can be a value of @ref DDS_WaveDataMode */
	
	u32 DDS_StartAddress;										/*!< Specifies the wave data start address,
																	This parameter can be a value of @ref DDS_WaveDataStartAddress */
	
	u32 DDS_DataLength;											/*!< Specifies the wave data length,
																	This parameter can be a value of @ref DDS_WaveDataLength */
	
	u32 DDS_Offset;												/*!< Specifies the wave offset,
																	This parameter can be a value of @ref DDS_WaveOffset */
																	
	u32 DDS_Prescaler;											/*!< Specifies the DDS prescaler,
																	This parameter can be a value of @ref DDS_Prescaler */

	u32 DDS_WGPrescaler;										/*!< Specifies the wave generator prescaler,
																	This parameter can be a value of @ref DDS_WaveGeneratorPrescaler */
}DDS_InitTypeDef;

/** @defgroupDDS_ALL_PERIPH
* @{
*/
#if defined(PT32G031x) || defined(PT32x012x) || defined(PT32x060x) || defined(PT32x063x) || defined (PTM160x) || defined (PTM280x)
#define IS_DDS_ALL_PERIPH(SEL)									((SEL) == 0)
#else
#define IS_DDS_ALL_PERIPH(SEL)									((SEL) == DDS)
#endif

/** @defgroup DDS_WaveDataMode
* @{
*/
#define DDS_WaveMode_Full										(DDS_CR1_WDMS & 0x00000000)
#define DDS_WaveMode_Half										(DDS_CR1_WDMS & 0x00000004)
#define DDS_WaveMode_Quarter									(DDS_CR1_WDMS & 0x00000008)
#define IS_DDS_WaveDataMode(SEL)								((SEL) == DDS_WaveMode_Full || (SEL) == DDS_WaveMode_Half || \
																	(SEL) == DDS_WaveMode_Quarter)

/** @defgroup DDS_IQMode
* @{
*/
#define DDS_IQMode_InPhase										(DDS_CR1_IQMS & 0x00000010)
#define DDS_IQMode_Quadrature									(DDS_CR1_IQMS & 0x00000020)
#define IS_DDS_IQMode(SEL)							((SEL) == DDS_IQMode_InPhase || (SEL) == DDS_IQMode_Quadrature)

/** @defgroup DDS_Prescaler
* @{
*/
#define IS_DDS_Prescaler(SEL)									((SEL) <= 511)

/** @defgroup DDS_WaveGeneratorPrescaler
* @{
*/
#define IS_DDS_WaveGeneratorPrescaler(SEL)						((SEL) <= 511)

/** @defgroup DDS_WaveDataStartAddress
* @{
*/
#if defined(SRAM_SIZE_8K)
#define IS_DDS_WaveDataStartAddress(ADDR)						((ADDR) >= 0x20001C00 && (ADDR) < 0x20002000 && ((ADDR) & 0x1) == 0)
#else
#define IS_DDS_WaveDataStartAddress(ADDR)						((ADDR) >= 0x20000C00 && (ADDR) < 0x20001000 && ((ADDR) & 0x1) == 0)
#endif

/** @defgroup DDS_WaveDataLength
* @{
*/
#define IS_DDS_WaveDataLength(LEN)								((LEN) <= 511)

/** @defgroup DDS_WaveOffset
* @{
*/
#define IS_DDS_WaveOffset(OFFSET)								((OFFSET) <= 0x0FFF)

/** @defgroup DDS_FLAG
* @{
*/
#define DDS_FLAG_InPhase										DDS_SR_I
#define DDS_FLAG_Quadrature										DDS_SR_Q
#define IS_DDS_FLAG(SEL)										((SEL)  == DDS_FLAG_InPhase || (SEL) == DDS_FLAG_Quadrature)

void DDS_Init(DDS_TypeDef *DDSx, DDS_InitTypeDef *DDS_InitStruct);
void DDS_StructInit(DDS_InitTypeDef *DDS_InitStruct);
void DDS_Cmd(DDS_TypeDef *DDSx, FunctionalState NewState);
void DDS_ResetCmd(DDS_TypeDef *DDSx, FunctionalState NewState);
void DDS_IQModeConfig(DDS_TypeDef *DDSx, u32 Mode);
void DDS_WaveDataModeConfig(DDS_TypeDef *DDSx, u32 Mode);
void DDS_PrescalerConfig(DDS_TypeDef *DDSx, u32 Division);
void DDS_WaveGeneratorPrescalerConfig(DDS_TypeDef *DDSx, u32 Division);
void DDS_WaveDataStartAddressConfig(DDS_TypeDef *DDSx, u32 Address);
void DDS_WaveDataLengthConfig(DDS_TypeDef *DDSx, u32 Length);
void DDS_WaveOffsetConfig(DDS_TypeDef *DDSx, u32 Offset);
FlagStatus DDS_GetFlagStatus(DDS_TypeDef *DDSx, u32 DDS_FLAG);

#ifdef __cplusplus
}
#endif
#endif /*__PT32x0xx_DDS_H */
