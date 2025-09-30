/**
  ******************************************************************************
  * @file    PT32x0xx_pwr.h
  * @author  Application development team
  * @version V1.5.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the PWR firmware
  *          library.
  ******************************************************************************
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of
  * the content or the  codes within the content.
  *
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  *
  ******************************************************************************
**/

#ifndef PT32x0xx_PWR_H
#define PT32x0xx_PWR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PT32x0xx.h"

/** @defgroup PWR_PVD_level
  * @{
  */
#define PWR_PVDLevel_0	    									(PWR_CR1_PLS & 0x000)
#define PWR_PVDLevel_1	    									(PWR_CR1_PLS & 0x010)
#define PWR_PVDLevel_2		    								(PWR_CR1_PLS & 0x020)
#define PWR_PVDLevel_3		    								(PWR_CR1_PLS & 0x030)
#define PWR_PVDLevel_4		    								(PWR_CR1_PLS & 0x040)
#define PWR_PVDLevel_5		    								(PWR_CR1_PLS & 0x050)
#define PWR_PVDLevel_6		    								(PWR_CR1_PLS & 0x060)
#define PWR_PVDLevel_7		    								(PWR_CR1_PLS & 0x070)
#if defined (PT32G031x) || defined (PTM280x)
#define IS_PWR_PVDLevel(SEL) 									(((SEL) & ~PWR_CR1_PLS) == 0 && (SEL) <= PWR_PVDLevel_5)
#else
#define IS_PWR_PVDLevel(SEL) 									(((SEL) & ~PWR_CR1_PLS) == 0 && (SEL) <= PWR_PVDLevel_7)
#endif
										  
/** @defgroup PWR_FlashWakeUp_mode
  * @{
  */
#define PWR_FlashWakeUp_Normal       							(PWR_CR2_FWUS & 0x00000000)
#define PWR_FlashWakeUp_Fast          							(PWR_CR2_FWUS & 0x00000001)
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM160x) || defined (PTM280x)
#define IS_PWR_FlashWakeUp_Mode(SEL) 							(0)
#else
#define IS_PWR_FlashWakeUp_Mode(SEL) 							(((SEL) == PWR_FlashWakeUp_Normal) || ((SEL) == PWR_FlashWakeUp_Fast))
#endif

/** @defgroup PWR_Flag
  * @{
  */
#define PWR_FLAG_LPRDY											(PWR_SR_LPRDY & 0x00000001)
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_PWR_FLAG(SEL)										(0)
#else
#define IS_PWR_FLAG(SEL)										((SEL) == PWR_FLAG_LPRDY)
#endif

/** @defgroup PWR_VREFBUF_Mode
  * @{
  */
#define PWR_VREFBUF_Disable										(PWR_VBUFCR_MODE & 0x00000000)
#define PWR_VREFBUF_IntVREF										(PWR_VBUFCR_MODE & 0x00000001)
#define PWR_VREFBUF_ExtVREF										(PWR_VBUFCR_MODE & 0x00000002)
#define PWR_VREFBUF_IntVREF_ToPin								(PWR_VBUFCR_MODE & 0x00000003)
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_PWR_VREFBUF_Mode(SEL)								(0)
#else
#define IS_PWR_VREFBUF_Mode(SEL)								(((SEL) & ~PWR_VBUFCR_MODE) == 0 && (SEL) <= PWR_VREFBUF_IntVREF_ToPin)
#endif

/** @defgroup PWR_VREFBUF_InternalReferenceVoltage
  * @{
  */
#define PWR_VREFBUF_IntVREF_BG2v0								 (PWR_VBUFCR_IRVS & 0x00000000)
#define PWR_VREFBUF_IntVREF_BGREF								 (PWR_VBUFCR_IRVS & 0x00000010)
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_PWR_VREFBUF_IntVREF(SEL)								(0)
#else
#define IS_PWR_VREFBUF_IntVREF(SEL)								((SEL) == PWR_VREFBUF_IntVREF_BG2v0 || (SEL) == PWR_VREFBUF_IntVREF_BGREF)
#endif

/** @defgroup PWR_VREFBUF_BG2v0TrimmingValue
* @{
*/
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_PWR_VREFBUF_TrimValue(SEL)							(0)
#else
#define IS_PWR_VREFBUF_TrimValue(SEL)							((SEL) <= 0x3F)
#endif

/** @defgroup PWR_SDADC0_PowerVoltage
* @{
*/
#define PWR_SDADC0_PowerVoltage_2p2V							(PWR_SDADC0PCR_PVS & 0x00000000)
#define PWR_SDADC0_PowerVoltage_2p4V							(PWR_SDADC0PCR_PVS & 0x00000010)
#define PWR_SDADC0_PowerVoltage_2p7V							(PWR_SDADC0PCR_PVS & 0x00000020)
#define PWR_SDADC0_PowerVoltage_3p0V							(PWR_SDADC0PCR_PVS & 0x00000030)
#define PWR_SDADC0_PowerVoltage_3p3V							(PWR_SDADC0PCR_PVS & 0x00000040)
#define PWR_SDADC0_PowerVoltage_3p6V							(PWR_SDADC0PCR_PVS & 0x00000050)
#define PWR_SDADC0_PowerVoltage_4p2V							(PWR_SDADC0PCR_PVS & 0x00000060)
#define PWR_SDADC0_PowerVoltage_4p5V							(PWR_SDADC0PCR_PVS & 0x00000070)
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_PWR_SDADC0_PowerVoltage(SEL)							(0)
#else
#define IS_PWR_SDADC0_PowerVoltage(SEL)							(((SEL) & ~PWR_SDADC0PCR_PVS) == 0 && (SEL) <= PWR_SDADC0_PowerVoltage_4p5V)
#endif

/** @defgroup PWR_Sleep_mode_entry
  * @{
  */
#define PWR_SleepEntry_WFI         								((u8)0x01)
#define PWR_SleepEntry_WFE              						((u8)0x02)
#define IS_PWR_SleepEntry(SEL) 									((SEL) == PWR_SleepEntry_WFI || (SEL) == PWR_SleepEntry_WFE)

/** @defgroup PWR_DeepSleep_mode_entry
  * @{
  */
#define PWR_DeepSleepEntry_WFI        							((u8)0x01)
#define PWR_DeepSleepEntry_WFE         							((u8)0x02)
#define IS_PWR_DeepSleepEntry(SEL) 								((SEL) == PWR_DeepSleepEntry_WFI || (SEL) == PWR_DeepSleepEntry_WFE)


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void PWR_PVDCmd(FunctionalState NewState);
void PWR_PVDLevelConfig(u32 PWR_PVDLevel);
void PWR_PVDLevelReset(void);
void PWR_FlashWakeUpMode(u32 FlashWakeUpMode);
FlagStatus PWR_GetFlagStatus(u32 PWR_FLAG);
void PWR_VREFBUF_Config(u32 Mode);
void PWR_VREFBUF_InternalRefVoltageConfig(u32 Sel);
void PWR_VREFBUF_BG2v0TrimValueConfig(u32 Value);
#if defined (PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
u8 PWR_VREFBUF_GetBG2v0TrimValue(void);
FlagStatus PWR_VREFBUF_GetBGTFFlagStatus(void);
void PWR_SDADC0_PowerCmd(FunctionalState NewState);
#endif
void PWR_SDADC0_PowerCapModeConfig(FunctionalState NewState);
void PWR_SDADC0_PowerVoltageConfig(u32 Voltage);
void PWR_EnterSleepMode(u8 PWR_SleepEntry);
void PWR_EnterDeepSleepMode(u8 PWR_DeepSleepEntry);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif
#endif /* __PT32x0xx_PWR_H */

