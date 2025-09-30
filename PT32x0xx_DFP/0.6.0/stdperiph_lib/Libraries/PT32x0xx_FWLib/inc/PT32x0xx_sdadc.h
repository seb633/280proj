/**
  ******************************************************************************
  * @file    PT32x0xx_sdadc.h
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the SDADC firmware
  *          library
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of
  * the content or the codes within the content.
  *
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  *****************************************************************************
**/

#ifndef PT32x0xx_SDADC_H
#define PT32x0xx_SDADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PT32x0xx.h"

/** @defgroup SDADC_InitTypeDef
* @{
*/
typedef struct
{
	u32 SDADC_INP;												/*!< Specifies the ADC channel negative selection,
																	This parameter can be a value of @ref SDADC_ChannelNegative */
	
	u32 SDADC_INN;												/*!< Specifies the ADC channel positive selection,
																	This parameter can be a value of @ref SDADC_ChannelPositive */
	
	u32 SDADC_Pga0Gain;											/*!< Specifies the SDADC PGA0 gain,
																	This parameter can be a value of @ref SDADC_PgaGain */
	
	u32 SDADC_Pga1Gain;											/*!< Specifies the SDADC PGA1 gain,
																	This parameter can be a value of @ref SDADC_PgaGain */
	
	u32 SDADC_Chop;												/*!< Specifies the SDADC chopper enable or disable,
																	This parameter can be a value of @ref FunctionalState */
	
	u32 SDADC_Pga0ChopMode;										/*!< Specifies the SDADC PGA 0 chopper working mode,
																	This parameter can be a value of @ref SDADC_PgaChopperMode */

	u32 SDADC_ReferencePositive;								/*!< Specifies the SDADC reference voltage positive selection,
																	This parameter can be a value of @ref SDADC_ReferenceVoltageSourcePositiveSelection */
																	
	u32 SDADC_Prescaler;										/*!< Specifies the SDADC prescaler,
																	This parameter can be a value of @ref SDADC_Prescaler */

	u32 SDADC_ChopClockDiv;										/*!< Specifies the SDADC chopper clock divider,
																	This parameter can be a value of @ref SDADC_ChopClockDivider */
}SDADC_InitTypeDef;

/** @defgroup SDADC_ALL_PERIPH
* @{
*/
#if defined(PT32G031x) || defined(PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_SDADC_ALL_PERIPH(SEL)								((SEL) == 0)
#else
#define IS_SDADC_ALL_PERIPH(SEL)								((SEL) == SDADC0)
#endif

/** @defgroup SDADC_ALL_PGA
* @{
*/
#define SDADC_PGA0												0
#define SDADC_PGA1												1
#define IS_SDADC_PGA(SEL)										((SEL) == SDADC_PGA0 || (SEL) == SDADC_PGA1)

/** @defgroup SDADC_WorkingMode
* @{
*/
#define SDADC_WorkingMode_Normal								(SDADC_CR1_MODE & 0x00000000)
#define SDADC_WorkingMode_Calibration							(SDADC_CR1_MODE & 0x00010000)
#define IS_SDADC_WorkingMode(SEL)								((SEL) == SDADC_WorkingMode_Normal || (SEL) == SDADC_WorkingMode_Calibration)

/** @defgroup SDADC_ChopperClock
* @{
*/
#define SDADC_ChopClock_Chop									(SDADC_CR1_CCS & 0x00000000)
#define SDADC_ChopClock_IQ										(SDADC_CR1_CCS & 0x00100000)
#if defined(PT32x060x) || defined(PT32x063x)
#define IS_SDADC_ChopClock(SEL)									((SEL) == SDADC_ChopClock_Chop)
#else
#define IS_SDADC_ChopClock(SEL)									((SEL) == SDADC_ChopClock_Chop || (SEL) == SDADC_ChopClock_IQ)
#endif

/** @defgroup SDADC_FilterOSR
* @{
*/
#define IS_SDADC_FilterOSR(SEL)									((SEL) >= 32 && (SEL) <= 65536)

/** @defgroup SDADC_ReferenceVoltageSourcePositiveSelection
* @{
*/
#define SDADC_RVSPS_BGREF										(SDADC_CFGR1_RVSPS & 0x00000000)
#define SDADC_RVSPS_VREFBUF										(SDADC_CFGR1_RVSPS & 0x00000001)
#define IS_SDADC_RVSPS(SEL)										((SEL) == SDADC_RVSPS_BGREF || (SEL) == SDADC_RVSPS_VREFBUF)

/** @defgroup SDADC_ConversionDiscarded
* @{
*/
#define IS_SDADC_ConversionDiscarded(SEL)						((SEL) <= 7)

/** @defgroup SDADC_ChannelPositive
* @{
*/
#define SDADC_Channel_P0										(SDADC_CFGR2_CHPS & 0x00000000)
#define SDADC_Channel_P1										(SDADC_CFGR2_CHPS & 0x00010000)
#define SDADC_Channel_P2										(SDADC_CFGR2_CHPS & 0x00020000)
#define SDADC_Channel_P3										(SDADC_CFGR2_CHPS & 0x00030000)
#define SDADC_Channel_P4										(SDADC_CFGR2_CHPS & 0x00040000)
#define SDADC_Channel_P5										(SDADC_CFGR2_CHPS & 0x00050000)
#define SDADC_Channel_P6										(SDADC_CFGR2_CHPS & 0x00060000)
#define SDADC_Channel_P7										(SDADC_CFGR2_CHPS & 0x00070000)
#define SDADC_Channel_P8										(SDADC_CFGR2_CHPS & 0x00080000)
#define SDADC_Channel_P9										(SDADC_CFGR2_CHPS & 0x00090000)
#define SDADC_Channel_P10										(SDADC_CFGR2_CHPS & 0x000A0000)
#define SDADC_Channel_P11										(SDADC_CFGR2_CHPS & 0x000B0000)
#define SDADC_Channel_P12										(SDADC_CFGR2_CHPS & 0x000C0000)
#define SDADC_Channel_P13										(SDADC_CFGR2_CHPS & 0x000D0000)
#define SDADC_Channel_P14										(SDADC_CFGR2_CHPS & 0x000E0000)
#define SDADC_Channel_P15										(SDADC_CFGR2_CHPS & 0x000F0000)
#define SDADC_Channel_P16										(SDADC_CFGR2_CHPS & 0x00100000)
#define SDADC_Channel_P17										(SDADC_CFGR2_CHPS & 0x00110000)
#define SDADC_Channel_P18										(SDADC_CFGR2_CHPS & 0x00120000)
#define SDADC_Channel_P19										(SDADC_CFGR2_CHPS & 0x00130000)
#define SDADC_Channel_P20										(SDADC_CFGR2_CHPS & 0x00140000)
#define SDADC_Channel_P21										(SDADC_CFGR2_CHPS & 0x00150000)
#define SDADC_Channel_P22										(SDADC_CFGR2_CHPS & 0x00160000)
#define SDADC_Channel_P23										(SDADC_CFGR2_CHPS & 0x00170000)
#define SDADC_Channel_P24										(SDADC_CFGR2_CHPS & 0x00180000)
#define SDADC_Channel_P25										(SDADC_CFGR2_CHPS & 0x00190000)

#define SDADC_Channel_P52										(SDADC_CFGR2_CHPS & 0x00340000)
#define SDADC_Channel_P53										(SDADC_CFGR2_CHPS & 0x00350000)
#define SDADC_Channel_P54										(SDADC_CFGR2_CHPS & 0x00360000)
#define SDADC_Channel_P58										(SDADC_CFGR2_CHPS & 0x003A0000)
#define SDADC_Channel_P59										(SDADC_CFGR2_CHPS & 0x003B0000)
#define SDADC_Channel_P60										(SDADC_CFGR2_CHPS & 0x003C0000)

#define SDADC_INP_Dac1Out										SDADC_Channel_P52
#define SDADC_INP_Dac0Out										SDADC_Channel_P53
#define SDADC_INP_BGREF											SDADC_Channel_P54
#define SDADC_INP_VddaDiv6										SDADC_Channel_P58
#define SDADC_INP_Vssa											SDADC_Channel_P59
#define SDADC_INP_TempP											SDADC_Channel_P60
#if defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x)
#define IS_SDADC_ChannelPositive(SEL)							(((SEL) & ~SDADC_CFGR2_CHPS) == 0 && ((SEL) <= SDADC_Channel_P9 || \
																	((SEL) >= SDADC_Channel_P52 && (SEL) <= SDADC_Channel_P54) || \
																	((SEL) >= SDADC_Channel_P58 && (SEL) <= SDADC_Channel_P60)))
#else
#define IS_SDADC_ChannelPositive(SEL)							(((SEL) & ~SDADC_CFGR2_CHPS) == 0 && ((SEL) <= SDADC_Channel_P25 || \
																	((SEL) >= SDADC_Channel_P52 && (SEL) <= SDADC_Channel_P54) || \
																	((SEL) >= SDADC_Channel_P58 && (SEL) <= SDADC_Channel_P60)))
#endif

/** @defgroup SDADC_ChannelNegative
* @{
*/
#define SDADC_Channel_N0										(SDADC_CFGR2_CHNS & 0x00000000)
#define SDADC_Channel_N1										(SDADC_CFGR2_CHNS & 0x01000000)
#define SDADC_Channel_N2										(SDADC_CFGR2_CHNS & 0x02000000)
#define SDADC_Channel_N3										(SDADC_CFGR2_CHNS & 0x03000000)
#define SDADC_Channel_N4										(SDADC_CFGR2_CHNS & 0x04000000)
#define SDADC_Channel_N5										(SDADC_CFGR2_CHNS & 0x05000000)
#define SDADC_Channel_N6										(SDADC_CFGR2_CHNS & 0x06000000)
#define SDADC_Channel_N7										(SDADC_CFGR2_CHNS & 0x07000000)
#define SDADC_Channel_N8										(SDADC_CFGR2_CHNS & 0x08000000)
#define SDADC_Channel_N9										(SDADC_CFGR2_CHNS & 0x09000000)
#define SDADC_Channel_N10										(SDADC_CFGR2_CHNS & 0x0A000000)
#define SDADC_Channel_N11										(SDADC_CFGR2_CHNS & 0x0B000000)
#define SDADC_Channel_N12										(SDADC_CFGR2_CHNS & 0x0C000000)
#define SDADC_Channel_N13										(SDADC_CFGR2_CHNS & 0x0D000000)
#define SDADC_Channel_N14										(SDADC_CFGR2_CHNS & 0x0E000000)
#define SDADC_Channel_N15										(SDADC_CFGR2_CHNS & 0x0F000000)
#define SDADC_Channel_N16										(SDADC_CFGR2_CHNS & 0x10000000)
#define SDADC_Channel_N17										(SDADC_CFGR2_CHNS & 0x11000000)
#define SDADC_Channel_N18										(SDADC_CFGR2_CHNS & 0x12000000)
#define SDADC_Channel_N19										(SDADC_CFGR2_CHNS & 0x13000000)
#define SDADC_Channel_N20										(SDADC_CFGR2_CHNS & 0x14000000)
#define SDADC_Channel_N21										(SDADC_CFGR2_CHNS & 0x15000000)
#define SDADC_Channel_N22										(SDADC_CFGR2_CHNS & 0x16000000)
#define SDADC_Channel_N23										(SDADC_CFGR2_CHNS & 0x17000000)
#define SDADC_Channel_N24										(SDADC_CFGR2_CHNS & 0x18000000)
#define SDADC_Channel_N25										(SDADC_CFGR2_CHNS & 0x19000000)

#define SDADC_Channel_N55										(SDADC_CFGR2_CHNS & 0x37000000)
#define SDADC_Channel_N56										(SDADC_CFGR2_CHNS & 0x38000000)
#define SDADC_Channel_N59										(SDADC_CFGR2_CHNS & 0x3B000000)
#define SDADC_Channel_N60										(SDADC_CFGR2_CHNS & 0x3C000000)
#define SDADC_Channel_N61										(SDADC_CFGR2_CHNS & 0x3D000000)
#define SDADC_Channel_N62										(SDADC_CFGR2_CHNS & 0x3E000000)
#define SDADC_Channel_N63										(SDADC_CFGR2_CHNS & 0x3F000000)

#define SDADC_INN_BG1v0											SDADC_Channel_N55
#define SDADC_INN_Vssa											SDADC_Channel_N59
#define SDADC_INN_TempN											SDADC_Channel_N60
#if defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x)
#define IS_SDADC_ChannelNegative(SEL)							(((SEL) & ~SDADC_CFGR2_CHNS) == 0 && ((SEL) <= SDADC_Channel_N9 || \
																	(SEL) == SDADC_Channel_N55 || ((SEL) == SDADC_Channel_N59 || (SEL) == SDADC_Channel_N60)))
#else
#define IS_SDADC_ChannelNegative(SEL)							(((SEL) & ~SDADC_CFGR2_CHNS) == 0 && ((SEL) <= SDADC_Channel_N25 || (SEL) == SDADC_Channel_N56 || \
																	(SEL) == SDADC_Channel_N56 || ((SEL) >= SDADC_Channel_N59 && (SEL) <= SDADC_Channel_N63)))
#endif

/** @defgroup SDADC_Prescaler
* @{
*/
#define IS_SDADC_Prescaler(SEL)									((SEL) >= 1 && (SEL) <= 127)

/** @defgroup SDADC_ChopClockDivider
* @{
*/
#define SDADC_ChopClock_Div8									(SDADC_CFGR3_CDIV & 0x00000000)
#define SDADC_ChopClock_Div16									(SDADC_CFGR3_CDIV & 0x00010000)
#define SDADC_ChopClock_Div32									(SDADC_CFGR3_CDIV & 0x00020000)
#define SDADC_ChopClock_Div64									(SDADC_CFGR3_CDIV & 0x00030000)
#define SDADC_ChopClock_Div128									(SDADC_CFGR3_CDIV & 0x00040000)
#define SDADC_ChopClock_Div256									(SDADC_CFGR3_CDIV & 0x00050000)
#define SDADC_ChopClock_Div512									(SDADC_CFGR3_CDIV & 0x00060000)
#define SDADC_ChopClock_Div1024									(SDADC_CFGR3_CDIV & 0x00070000)
#define IS_SDADC_ChopClockDivider(SEL)							(((SEL) & ~SDADC_CFGR3_CDIV) == 0 && (SEL) <= SDADC_ChopClock_Div1024)

/** @defgroup SDADC_IT
* @{
*/
#define SDADC_IT_EOC											SDADC_IER_EOCIE
#define IS_SDADC_IT(SEL)										((SEL) == SDADC_IT_EOC)

/** @defgroup SDADC_FLAG
* @{
*/
#define SDADC_FLAG_EOC											SDADC_SR_EOC
#define IS_SDADC_FLAG(SEL)										((SEL) == SDADC_FLAG_EOC)

/** @defgroup SDADC_PgaGain
* @{
*/
#define SDADC_PgaGain_Disable									0
#define SDADC_PgaGain_x1										1
#define SDADC_PgaGain_x5p05										2
#define SDADC_PgaGain_x8p72										3
#define SDADC_PgaGain_x16										4
#define SDADC_PgaGain_x24										5
#define SDADC_PgaGain_x32										6
#define SDADC_PgaGain_x64										7
#define SDADC_PgaGain_x0p25										8
#define IS_SDADC_PgaGain(PGA, SEL)								((PGA) == SDADC_PGA0 ? ((SEL)) <= SDADC_PgaGain_x0p25 : ((SEL)) <= SDADC_PgaGain_x64)

/** @defgroup SDADC_PgaChopperMode
* @{
*/
#define SDADC_PgaChopMode_Disable								(SDADC_PGACR_PCM0 & 0x00000000)
#define SDADC_PgaChopMode_ACImpedance							(SDADC_PGACR_PCM0 & 0x00000020)
#define SDADC_PgaChopMode_Enable								(SDADC_PGACR_PCM0 & 0x00000030)
#if defined(PT32x060x) || defined(PT32x063x)
#define IS_SDADC_PgaChopperMode(PGA, SEL)						((PGA) == SDADC_PGA0 && ((SEL) == SDADC_PgaChopMode_Disable || (SEL) == SDADC_PgaChopMode_Enable))
#else
#define IS_SDADC_PgaChopperMode(PGA, SEL)						((PGA) == SDADC_PGA0 && ((SEL) == SDADC_PgaChopMode_Disable || \
																	(SEL) == SDADC_PgaChopMode_ACImpedance || (SEL) == SDADC_PgaChopMode_Enable))
#endif

/** @defgroup SDADC_DataOffsetValue
* @{
*/
#define IS_SDADC_DataOffsetValue(SEL)							(((SEL) & ~SDADC_DOFR_OFFSET) == 0)

void SDADC_Init(SDADC_TypeDef *SDADCx, SDADC_InitTypeDef *SDADC_InitStruct);
void SDADC_StructInit(SDADC_InitTypeDef *SDADC_InitStruct);
void SDADC_Cmd(SDADC_TypeDef *SDADCx, FunctionalState NewState);
void SDADC_WorkModeConfig(SDADC_TypeDef *SDADCx, u32 Mode);
void SDADC_ChopperClockConfig(SDADC_TypeDef *SDADCx, u32 Source);
void SDADC_DMACmd(SDADC_TypeDef *SDADCx, FunctionalState NewState);
void SDADC_FilterOSRConfig(SDADC_TypeDef *SDADCx, u32 OSR);
void SDADC_RVSPSConfig(SDADC_TypeDef *SDADCx, u32 Source);
void SDADC_BGREFCmd(SDADC_TypeDef *SDADCx, FunctionalState NewState);
void SDADC_TempSensorCmd(SDADC_TypeDef *SDADCx, FunctionalState NewState);
void SDADC_ConversionDiscardedConfig(SDADC_TypeDef *SDADCx, u32 value);
void SDADC_ChannelConfig(SDADC_TypeDef *SDADCx, u32 INP, u32 INN);
void SDADC_PrescalerConfig(SDADC_TypeDef *SDADCx, u32 Division);
void SDADC_ChopClockDividerConfig(SDADC_TypeDef *SDADCx, u32 Division);
void SDADC_ITConfig(SDADC_TypeDef *SDADCx, u32 SDADC_IT, FunctionalState NewState);
FlagStatus SDADC_GetFlagStatus(SDADC_TypeDef *SDADCx, u32 SDADC_FLAG);
void SDADC_PgaGainConfig(SDADC_TypeDef *SDADCx, u32 PGAx, u32 gain);
void SDADC_PgaChopperWorkModeConfig(SDADC_TypeDef *SDADCx, u32 PGAx, u32 Mode);
s32 SDADC_GetConversionValue(SDADC_TypeDef *SDADCx);
void SDADC_DataOffsetConfig(SDADC_TypeDef *SDADCx, u32 offset);
void SDADC_ChopperCmd(SDADC_TypeDef *SDADCx, FunctionalState NewState);
double SDADC_GetDataMaxResolution(SDADC_TypeDef *SDADCx);

#ifdef __cplusplus
}
#endif
#endif /*__PT32x0xx_SDADC_H */
