/**
  ******************************************************************************
  * @file    PT32x0xx_opa.h
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the OPA firmware
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

#ifndef PT32x0xx_OPA_H
#define PT32x0xx_OPA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PT32x0xx.h"

/** @defgroup OPA_Number
* @{
*/
#if defined (PT32x002x) || defined (PT32x012x)
#define IS_OPA_ALL_PERIPH(SEL)								(0)
#elif defined (PT32G031x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM160x) || defined (PTM280x)
#define IS_OPA_ALL_PERIPH(SEL)								((SEL) == OPA0 || (SEL) == OPA1)
#else
#define IS_OPA_ALL_PERIPH(SEL)								((SEL) == OPA0 || (SEL) == OPA1 || (SEL) == OPA2 || (SEL) == OPA3)
#endif

/** @defgroup OPA PGA VCM
* @{
*/
#define PGA_VCM_VDDADiv2									(OPA_CR1_VCMS & 0x00000000)
#define PGA_VCM_BG1v0										(OPA_CR1_VCMS & 0x00000004)
#define PGA_VCM_External									(OPA_CR1_VCMS & 0x00000008)
#define PGA_VCM_Disable										(OPA_CR1_VCMS & 0x0000000C)
#if defined (PT32G031x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM280x)
#define IS_PGA_VCMConfig(SEL)								(0)
#elif defined (PTM160x)
#define IS_PGA_VCMConfig(SEL)								((SEL) == PGA_VCM_VDDADiv2 || (SEL) == PGA_VCM_BG1v0 || PGA_VCM_Disable)
#else
#define IS_PGA_VCMConfig(SEL)								((SEL) == PGA_VCM_VDDADiv2 || (SEL) == PGA_VCM_BG1v0 || (SEL) == PGA_VCM_External || PGA_VCM_Disable)
#endif

/** @defgroup OPA_Power_Mode
* @{
*/
#define OPA_PowerMode_LowerPower							(OPA_CR1_PM & 0x00000000)
#define OPA_PowerMode_Normal								(OPA_CR1_PM & 0x00000010)
#define OPA_PowerMode_HighSpeed								(OPA_CR1_PM & 0x00000020)
#if defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x)
#define IS_OPA_PowerMode(SEL)								((SEL) == OPA_PowerMode_LowerPower || (SEL) == OPA_PowerMode_Normal)
#elif defined (PT32G031x) || defined(PTM160x) || defined (PTM280x)
#define IS_OPA_PowerMode(SEL)								(0)
#else
#define IS_OPA_PowerMode(SEL)								((SEL) == OPA_PowerMode_LowerPower || \
																(SEL) == OPA_PowerMode_Normal || \
																(SEL) == OPA_PowerMode_HighSpeed)
#endif

/** @defgroup OPA_PGA_Mode
* @{
*/
#define OPA_PGA_Disable										(OPA_CR1_PGA & 0x00000000)
#define OPA_PGA_Enable										(OPA_CR1_PGA & 0x00000100)

/** @defgroup OPA_PgaGain
* @{
*/
#define OPA_PgaGain_Disable									0
#if defined (PTM160x)
#define OPA_PgaGain_x5										1
#define OPA_PgaGain_x10										2
#define IS_OPA_PgaGain(SEL)									((SEL) <= OPA_PgaGain_x10)
#elif defined (PTM280x)
#define OPA_PgaGain_x1										1
#define OPA_PgaGain_x2										2
#define OPA_PgaGain_x5										3
#define OPA_PgaGain_x10										4
#define IS_OPA_PgaGain(SEL)									((SEL) <= OPA_PgaGain_x10)
#else
#define IS_OPA_PgaGain(SEL)									((SEL) <= 0x100)
#endif

/** @defgroup OPA_PositiveInputs
* @{
*/
#define OPA_PositiveInput_0									(OPA_CR1_INP0E & 0x00010000)
#define OPA_PositiveInput_1									(OPA_CR1_INP1E & 0x00020000)
#define OPA_PositiveInput_2									(OPA_CR1_INP2E & 0x00040000)
#if defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x)
#define OPA_INP_DAC1_OUT									OPA_PositiveInput_1
#define OPA_INP_DAC0_OUT									OPA_PositiveInput_2
#endif
#if defined (PT32G031x) || defined (PTM280x)
#define OPA_INP_ALL											(0)
#else
#define OPA_INP_ALL											(OPA_PositiveInput_0 | OPA_PositiveInput_1 | OPA_PositiveInput_2)
#endif
#define IS_OPA_PositiveInput(SEL)							(((SEL) & ~OPA_INP_ALL) == 0 && (SEL) != 0)

/** @defgroup OPA_NegativeInputs
* @{
*/
#define OPA_NegativeInput_0									(OPA_CR1_INN0E & 0x00100000)
#define OPA_NegativeInput_1									(OPA_CR1_INN1E & 0x00200000)
#define OPA_NegativeInput_2									(OPA_CR1_INN2E & 0x00400000)
#define OPA_NegativeInput_3									(OPA_CR1_INN3E & 0x00800000)
#if defined (PT32G031x) || defined (PTM160x) || defined (PTM280x)
#define OPA_INN_ALL											(0)
#else
#define OPA_INN_ALL											(OPA_NegativeInput_0 | OPA_NegativeInput_1 | OPA_NegativeInput_2 | OPA_NegativeInput_3)
#endif
#define IS_OPA_NegativeInput(SEL)							(((SEL) & ~OPA_INN_ALL) == 0 && (SEL) != 0)

/** @defgroup OPA_Outputs
* @{
*/
#define OPA_Output_0										(OPA_CR1_OUT0E & 0x01000000)
#define OPA_Output_1										(OPA_CR1_OUT1E & 0x02000000)
#define OPA_Output_2										(OPA_CR1_OUT2E & 0x04000000)
#define OPA_Output_3										(OPA_CR1_OUT3E & 0x08000000)
#if defined (PT32G031x) || defined (PTM160x)|| defined (PTM280x)
#define OPA_OUTPUT_ALL										(0)
#else
#define OPA_OUTPUT_ALL										(OPA_Output_0 | OPA_Output_1 | OPA_Output_2 | OPA_Output_3)
#endif
#define IS_OPA_Output(SEL)									(((SEL) & ~OPA_OUTPUT_ALL) == 0 && (SEL) != 0)

/** @defgroup OPA_CalibrationMode
* @{
*/
#define	OPA_CalibrationMode_Disable							(OPA_CR2_CALE & 0x00000000)
#define OPA_CalibrationMode_Manual							(OPA_CR2_CALE & 0x00000001)
#define OPA_CalibrationMode_Auto							(OPA_CR2_CALE & 0x00000002)
#if defined (PT32G031x) || defined (PTM160x) || defined (PTM280x)
#define IS_OPA_CalibrationMode(SEL)							((SEL) == OPA_CalibrationMode_Disable || (SEL) == OPA_CalibrationMode_Manual)
#else
#define IS_OPA_CalibrationMode(SEL)							((SEL) == OPA_CalibrationMode_Disable || (SEL) == OPA_CalibrationMode_Manual || \
																(SEL) == OPA_CalibrationMode_Auto)
#endif

/** @defgroup OPA_AutoCalibrationMode
* @{
*/
#define	OPA_AutoCalibrationMode_Normal						(OPA_CR2_ACMS & 0x00000000)
#define OPA_AutoCalibrationMode_HighSpeed					(OPA_CR2_ACMS & 0x00000010)
#if defined (PT32G031x) || defined (PTM160x) || defined (PTM280x)
#define IS_OPA_AutoCalibrationMode(SEL)						(0)
#else
#define IS_OPA_AutoCalibrationMode(SEL)						((SEL) == OPA_AutoCalibrationMode_Normal || (SEL) == OPA_AutoCalibrationMode_HighSpeed)
#endif

/** @defgroup OPA_TrimmingValue
* @{
*/
#if defined (PT32G031x) || defined (PTM160x) || defined (PTM280x)
#define IS_OPA_OffsetTrimValue(VALUE)						((VALUE) <= 0x0000001F)
#else
#define IS_OPA_OffsetTrimValue(VALUE)						((VALUE) <= 0x000000FF)
#endif

/*! function declaration and definition*/
void OPA_Cmd(OPA_TypeDef *OPAx, FunctionalState NewState);
void OPA_PGAVcmConfig(OPA_TypeDef *OPAx, u32 Value);
void OPA_PowerModeConfig(OPA_TypeDef *OPAx, u32 Mode);
void OPA_PGAGainConfig(OPA_TypeDef *OPAx, u32 Gain);
void OPA_PositiveInputsConfig(OPA_TypeDef *OPAx, u32 Inputs, FunctionalState NewState);
void OPA_NegativeInputsConfig(OPA_TypeDef *OPAx, u32 Inputs, FunctionalState NewState);
void OPA_OutputsConfig(OPA_TypeDef *OPAx, u32 Outputs, FunctionalState NewState);
void OPA_VGndCmd(OPA_TypeDef *OPAx, FunctionalState NewState);
void OPA_CalibrationModeConfig(OPA_TypeDef *OPAx, u32 Mode);
void OPA_AutoCalibrationModeConfig(OPA_TypeDef *OPAx, u32 Mode);
void OPA_OffsetTrimValueConfig(OPA_TypeDef *OPAx, u32 Value);
void OPA_OffsetTrimValueReset(OPA_TypeDef *OPAx);
u8 OPA_GetCalibrationOutput(OPA_TypeDef *OPAx);
FlagStatus OPA_GetOffsetTrimSourceStatus(OPA_TypeDef *OPAx);


#ifdef __cplusplus
}
#endif

#endif /*__PT32x0xx_OPA_H */
