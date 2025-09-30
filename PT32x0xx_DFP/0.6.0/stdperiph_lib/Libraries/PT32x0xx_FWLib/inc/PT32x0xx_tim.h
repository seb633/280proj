/**
	******************************************************************************
  * @file    PT32x0xx_tim.h
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the TIM firmware
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
#ifndef __PT32x0xx_TIM_H
#define __PT32x0xx_TIM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PT32x0xx.h"

/* Private define ------------------------------------------------------------*/


/** @defgroup TIM_ALL_PERIPH
* @{
*/
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)

#define IS_TIM_ALL_PERIPH(SEL) 					((SEL) == TIM2 || (SEL) == TIM4 || (SEL) == TIM20 || (SEL) == TIM21 || (SEL) == TIM22)

#define IS_TIM_LIST1_PERIPH(SEL) 				((SEL) == TIM20 || (SEL) == TIM22)

#define IS_TIM_LIST2_PERIPH(SEL) 				((SEL) == TIM20 || (SEL) == TIM22)

#define IS_TIM_LIST3_PERIPH(SEL) 				((SEL) == TIM2 || (SEL) == TIM20 || (SEL) == TIM21 || (SEL) == TIM22)

#define IS_TIM_LIST4_PERIPH(SEL) 				((SEL) == TIM21)

#define IS_TIM_LIST5_PERIPH(SEL) 				((SEL) == TIM21)

#define IS_TIM_LIST6_PERIPH(SEL) 				((SEL) == TIM20 || (SEL) == TIM21 || (SEL) == TIM22)

#define IS_TIM_LIST7_PERIPH(SEL) 				((SEL) == TIM20 || (SEL) == TIM22)

#define IS_TIM_LIST8_PERIPH(SEL)				((SEL) == TIM2 || (SEL) == TIM20 || (SEL) == TIM21 || (SEL) == TIM22)

#elif defined (PT32G031x) || defined (PTM280x)

#define IS_TIM_ALL_PERIPH(SEL) 					((SEL) == TIM1 || (SEL) == TIM2 || (SEL) == TIM3 || (SEL) == TIM4 || (SEL) == TIM8)

#define IS_TIM_LIST1_PERIPH(SEL) 				((SEL) == TIM1 || (SEL) == TIM8)

#define IS_TIM_LIST2_PERIPH(SEL) 				((SEL) == TIM1)

#define IS_TIM_LIST3_PERIPH(SEL) 				((SEL) == TIM1 || (SEL) == TIM2 || (SEL) == TIM3 || (SEL) == TIM8)

#define IS_TIM_LIST4_PERIPH(SEL) 				((SEL) == TIM8)

#define IS_TIM_LIST5_PERIPH(SEL) 				((SEL) == TIM1 || (SEL) == TIM8)

#define IS_TIM_LIST6_PERIPH(SEL) 				((SEL) == TIM1 || (SEL) == TIM8)

#define IS_TIM_LIST7_PERIPH(SEL) 				((SEL) == TIM1 || (SEL) == TIM8)

#define IS_TIM_LIST8_PERIPH(SEL)  				((SEL) == TIM1 || (SEL) == TIM2 || (SEL) == TIM3 || (SEL) == TIM8)

#elif defined (PT32x066x) || defined (PT32x063x) || defined (PT32x060x)

#define IS_TIM_ALL_PERIPH(SEL) 					((SEL) == TIM2 || (SEL) == TIM3 || (SEL) == TIM4 || (SEL) == TIM5 || (SEL) == TIM6 || (SEL) == LPTIM)

#define IS_TIM_LIST1_PERIPH(SEL) 				((SEL) == TIM5 || (SEL) == TIM6 || (SEL) == LPTIM)

#define IS_TIM_LIST2_PERIPH(SEL) 				(0)

#define IS_TIM_LIST3_PERIPH(SEL) 				((SEL) == TIM2 || (SEL) == TIM3 || (SEL) == TIM5 || (SEL) == TIM6)

#define IS_TIM_LIST4_PERIPH(SEL) 				((SEL) == LPTIM)

#define IS_TIM_LIST5_PERIPH(SEL) 				((SEL) == LPTIM)

#define IS_TIM_LIST6_PERIPH(SEL) 				((SEL) == TIM5 || (SEL) == TIM6 || (SEL) == LPTIM)

#define IS_TIM_LIST7_PERIPH(SEL) 				((SEL) == LPTIM)

#define IS_TIM_LIST8_PERIPH(SEL) 				((SEL) == TIM2 || (SEL) == TIM3 || (SEL) == TIM5 || (SEL) == TIM6 || (SEL) == LPTIM)

#endif
/** @defgroup TIM_TimeBaseInitTypeDef
* @{
*/
typedef struct
{
	u32 TIM_AutoReloadValue;       			/*!< Specifies the value to be loaded into the Auto-Reload Register at the next update event.
																		  This parameter can be a number between 0x0000 and 0xFFFF */

	u32 TIM_Prescaler; 						 /*!< Specifies the prescaler value used to divide the PCLK.
																		  This parameter can be a number between 0x0000 and 0xFFFF */

	u32 TIM_Direction;						 /*!< Specifies the counter mode.
																			This parameter can be value of @ref TIM_Direction */

	u32 TIM_CenterAlignedMode;		 		/*!< Specifies the center aligned mode.
																			This parameter can be value of @ref TIM_CenterAlignedMode */

} TIM_TimeBaseInitTypeDef;

/** @defgroup TIM_CH_Enum
* @{
*/
typedef enum
{
	TIM_Channel_1 = 0,                /*!< TIM channel 1 */
	TIM_Channel_2,                    /*!< TIM channel 2 */
	TIM_Channel_3,                    /*!< TIM channel 3 */
	TIM_Channel_4                     /*!< TIM channel 4 */
} TIM_CH_Enum;

/** @defgroup TIM_SlaveModeInitTypeDef
* @{
*/
typedef struct
{
	u32 TIM_TriggerInput;
	u32 TIM_SlaveMode;
	u32 TIM_TriggerFilter;
	u32 TIM_TriggerPrecaler;
	u32 TIM_ExternalClock;
	u32 TIM_TriggerPolarity;
} TIM_SlaveModeInitTypeDef;

/** @defgroup TIM_MasterModeInitTypeDef
* @{
*/
typedef struct
{
	u32 TIM_Synchronization;
	u32 TIM_MasterMode;
} TIM_MasterModeInitTypeDef;

/** @defgroup TIM_BKICRInitTypeDef
* @{
*/
typedef struct
{
	u32 TIM_Break;
	u32 TIM_BreakPolarity;
	u32 TIM_BreakSource;
	u32 TIM_BreakInputControl;
	u32 TIM_Breakfilter;
} TIM_BKICRInitTypeDef;

/** @defgroup TIM_OCInitTypeDef
* @{
*/
typedef struct
{
	TIM_CH_Enum TIM_Channel;
	u32 TIM_OCMode;
	u32 TIM_OCIdleState;
	u32 TIM_OCNIdleState;
	u32 TIM_OCOutput;
	u32 TIM_OCNOutput;
	u16 TIM_UpOCValue;
	u16 TIM_DownOCValue;
} TIM_OCInitTypeDef;

/** @defgroup TIM_ICInitTypeDef
* @{
*/
typedef struct
{
	TIM_CH_Enum TIM_Channel;
	u32 TIM_ICRiseCapture;
	u32 TIM_ICFallCapture;
	u32 TIM_ICResetCounter;
	u32 TIM_ICSource;
	u32 TIM_ICFilter;
	u32 TIM_ICPrescaler;
} TIM_ICInitTypeDef;

/** @defgroup TIM_Channel
* @{
*/
#define IS_TIM_Channel(SEL) 					((SEL) == TIM_Channel_1 || \
													(SEL) == TIM_Channel_2 || \
													(SEL) == TIM_Channel_3 || \
													(SEL) == TIM_Channel_4)

/** @defgroup TIM_CKD
* @{
*/
#define TIM_ClockDiv_None	     				(TIM_CR_CKD & 0x000)
#define TIM_ClockDiv_2	     					(TIM_CR_CKD & 0x100)
#define TIM_ClockDiv_4	     					(TIM_CR_CKD & 0x200)
#define TIM_ClockDiv_8	     					(TIM_CR_CKD & 0x300)
#define TIM_ClockDiv_16	   						(TIM_CR_CKD & 0x400)
#define TIM_ClockDiv_32	   						(TIM_CR_CKD & 0x500)
#define IS_TIM_ClockDiv(SEL)       				(((SEL) & ~TIM_CR_CKD)== 0x0 && (SEL) <= TIM_ClockDiv_32)


/** @defgroup TIM_Direction
* @{
*/
#define TIM_Direction_Up						(TIM_CR_DIR & 0x00000000)
#define TIM_Direction_Down	    				(TIM_CR_DIR & 0x00010000)
#define IS_TIM_Direction(SEL)      				((SEL) == TIM_Direction_Up || (SEL) == TIM_Direction_Down)
													
/** @defgroup TIM_CenterAlignedMode
* @{
*/
#define TIM_CenterAlignedMode_Disable			(TIM_CR_CMS & 0x0000000)
#define TIM_CenterAlignedMode1_Enable			(TIM_CR_CMS & 0x0020000)
#define TIM_CenterAlignedMode2_Enable			(TIM_CR_CMS & 0x0040000)
#define TIM_CenterAlignedMode3_Enable			(TIM_CR_CMS & 0x0060000)
#if defined (PT32x002x) || defined(PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_TIM_CenterAlignedMode(SEL)   		((SEL) == TIM_CenterAlignedMode_Disable || \
													(SEL) == TIM_CenterAlignedMode1_Enable ||\
													(SEL) == TIM_CenterAlignedMode2_Enable ||\
													(SEL) == TIM_CenterAlignedMode3_Enable)
#elif defined (PT32G031x) || defined(PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define IS_TIM_CenterAlignedMode(SEL)   		((SEL) == TIM_CenterAlignedMode_Disable)
#endif

/** @defgroup TIM_TriggerInput
* @{
*/
#define TIM_TriggerInput_ITR0          			(TIM_MSCR_TIS & 0x0)
#define TIM_TriggerInput_ITR1          			(TIM_MSCR_TIS & 0x1)
#define TIM_TriggerInput_ITR2          			(TIM_MSCR_TIS & 0x2)
#define TIM_TriggerInput_ITR3          			(TIM_MSCR_TIS & 0x3)
#define TIM_TriggerInput_TI1FPED       			(TIM_MSCR_TIS & 0x4)
#define TIM_TriggerInput_TI2FPED       			(TIM_MSCR_TIS & 0x5)
#define TIM_TriggerInput_TI3FPED       			(TIM_MSCR_TIS & 0x6)
#define TIM_TriggerInput_TI4FPED       			(TIM_MSCR_TIS & 0x7)
#define TIM_TriggerInput_TI1FP         			(TIM_MSCR_TIS & 0x8)
#define TIM_TriggerInput_TI2FP         			(TIM_MSCR_TIS & 0x9)
#define TIM_TriggerInput_TI3FP         			(TIM_MSCR_TIS & 0xA)
#define TIM_TriggerInput_TI4FP         			(TIM_MSCR_TIS & 0xB)
#define TIM_TriggerInput_ITR4          			(TIM_MSCR_TIS & 0xC)
#define TIM_TriggerInput_ETRFP         			(TIM_MSCR_TIS & 0xD)
#if defined (PT32G031x) || defined (PTM280x)
#define IS_TIM_TriggerInput(SEL)     			(((SEL) & ~TIM_MSCR_TIS) == 0x0 && ((SEL) == TIM_TriggerInput_ETRFP || (SEL) == TIM_TriggerInput_ITR0))
#elif defined (PT32x002x) || defined(PT32x012x) || defined(PT32x060x) || defined (PT32x063x) || defined (PT32x066x) || defined (PTM160x)
#define IS_TIM_TriggerInput(SEL)    			(0)
#else
#define IS_TIM_TriggerInput(SEL)     			(((SEL) & ~TIM_MSCR_TIS) == 0x0 && (SEL) <= TIM_TriggerInput_ETRFP)
#endif

/** @defgroup TIM_SlaveMode
* @{
*/
#define	TIM_SlaveMode_Disable           		(TIM_MSCR_SMS & 0x00)
#define	TIM_SlaveMode_Encode1        			(TIM_MSCR_SMS & 0x10)
#define	TIM_SlaveMode_Encode2        			(TIM_MSCR_SMS & 0x20)
#define	TIM_SlaveMode_Encode3        			(TIM_MSCR_SMS & 0x30)
#define	TIM_SlaveMode_Reset         			(TIM_MSCR_SMS & 0x40)
#define	TIM_SlaveMode_Gated         			(TIM_MSCR_SMS & 0x50)
#define	TIM_SlaveMode_Trigger           		(TIM_MSCR_SMS & 0x60)
#define	TIM_SlaveMode_Clock             		(TIM_MSCR_SMS & 0x70)
#if defined (PT32G031x) || defined (PTM280x)
#define IS_TIM_SlaveMode(SEL)       			(((SEL) & ~TIM_MSCR_SMS) == 0x0 && (((SEL) >= TIM_SlaveMode_Reset && (SEL) <= TIM_SlaveMode_Clock) || (SEL) == TIM_SlaveMode_Disable))
#elif defined (PT32x002x) || defined(PT32x012x) || defined(PT32x060x) || defined (PT32x063x) || defined (PT32x066x) || defined (PTM160x)
#define IS_TIM_SlaveMode(SEL)       			(0)
#else
#define IS_TIM_SlaveMode(SEL)       			(((SEL) & ~TIM_MSCR_SMS) == 0x0 && (SEL) <= TIM_SlaveMode_Clock)
#endif

/** @defgroup TIM_Synchronization
* @{
*/
#define TIM_Synchronization_Disable  			(TIM_MSCR_SYNC & 0x000)
#define TIM_Synchronization_Enable   			(TIM_MSCR_SYNC & 0x100)
#if defined (PT32G031x) || defined (PTM280x)
#define IS_TIM_Synchronization(SEL)  			((SEL) == TIM_Synchronization_Disable || (SEL) == TIM_Synchronization_Enable)
#elif defined (PT32x002x) || defined(PT32x012x) || defined(PT32x060x) || defined (PT32x063x) || defined (PT32x066x) || defined (PTM160x)
#define IS_TIM_Synchronization(SEL)  			(0)
#endif

/** @defgroup TIM_MasterMode
* @{
*/
#define TIM_MasterMode_Reset             		(TIM_MSCR_TOS & 0x00000000)
#define TIM_MasterMode_EN             			(TIM_MSCR_TOS & 0x00001000)
#define TIM_MasterMode_Update             		(TIM_MSCR_TOS & 0x00002000)
#define TIM_MasterMode_OC1             			(TIM_MSCR_TOS & 0x00004000)
#define TIM_MasterMode_OC2            			(TIM_MSCR_TOS & 0x00005000)
#define TIM_MasterMode_OC3             			(TIM_MSCR_TOS & 0x00006000)
#define TIM_MasterMode_OC4             			(TIM_MSCR_TOS & 0x00007000)
#define TIM_MasterMode_CH1             			(TIM_MSCR_TOS & 0x00008000)
#define TIM_MasterMode_CH2             			(TIM_MSCR_TOS & 0x00009000)
#define TIM_MasterMode_CH3             			(TIM_MSCR_TOS & 0x0000A000)
#define TIM_MasterMode_CH4            			(TIM_MSCR_TOS & 0x0000B000)
#define TIM_MasterMode_Disable       			(TIM_MSCR_TOS & 0x00003000)
#if defined (PT32G031x) || defined (PTM280x)
#define IS_TIM_MasterMode(SEL)       			(((SEL) & ~TIM_MSCR_TOS) == 0x0 && (SEL) <= TIM_MasterMode_CH4)
#elif defined (PT32x002x) || defined(PT32x012x) || defined(PT32x060x) || defined (PT32x063x) || defined (PT32x066x) || defined (PTM160x)
#define IS_TIM_MasterMode(SEL)       			(0)
#endif
/** @defgroup TIM_ExternalClock
* @{
*/
#define TIM_ExternalClock_Disable  				(TIM_MSCR_ECE & 0x00000)
#define TIM_ExternalClock_Enable    			(TIM_MSCR_ECE & 0x10000)
#if defined (PT32G031x) || defined (PTM280x)
#define IS_TIM_ExternalClock(SEL)   			((SEL) == TIM_ExternalClock_Disable || (SEL) == TIM_ExternalClock_Enable)
#elif defined (PT32x002x) || defined(PT32x012x) || defined(PT32x060x) || defined (PT32x063x) || defined (PT32x066x) || defined (PTM160x)
#define IS_TIM_ExternalClock(SEL)   			(0)
#endif

/** @defgroup TIM_ExternalTriggerPolarity
* @{
*/
#define TIM_TriggerPolarity_Disable 			(TIM_MSCR_ETP & 0x00000)
#define TIM_TriggerPolarity_Enable  			(TIM_MSCR_ETP & 0x20000)
#if defined (PT32G031x) || defined (PTM280x)
#define IS_TIM_TriggerPolarity(SEL)   			((SEL) == TIM_TriggerPolarity_Disable || (SEL) == TIM_TriggerPolarity_Enable)
#elif defined (PT32x002x) || defined(PT32x012x) || defined(PT32x060x) || defined (PT32x063x) || defined (PT32x066x) || defined (PTM160x)
#define IS_TIM_TriggerPolarity(SEL)   			(0)
#endif

/** @defgroup TIM_TriggerFilter
* @{
*/
#define TIM_TriggerFilter_None          		(TIM_MSCR_ETF & 0x00000000)
#define TIM_TriggerFilter_2          			(TIM_MSCR_ETF & 0x01000000)
#define TIM_TriggerFilter_4          			(TIM_MSCR_ETF & 0x02000000)
#define TIM_TriggerFilter_8          			(TIM_MSCR_ETF & 0x03000000)
#define TIM_TriggerFilter_16         			(TIM_MSCR_ETF & 0x04000000)
#define TIM_TriggerFilter_32         			(TIM_MSCR_ETF & 0x05000000)
#define TIM_TriggerFilter_64         			(TIM_MSCR_ETF & 0x06000000)
#if defined (PT32G031x) || defined (PTM280x)
#define IS_TIM_TriggerFilter(SEL)   			(((SEL) & ~TIM_MSCR_ETF) == 0x0 && (SEL) <= TIM_TriggerFilter_64)
#elif defined (PT32x002x) || defined(PT32x012x) || defined(PT32x060x) || defined (PT32x063x) || defined (PT32x066x) || defined (PTM160x)
#define IS_TIM_TriggerFilter(SEL)   			(0)
#endif

/** @defgroup TIM_TriggerPrecaler
* @{
*/
#define TIM_TriggerPrecaler_None       			(TIM_MSCR_ETPSC & 0x00000000)
#define TIM_TriggerPrecaler_2       			(TIM_MSCR_ETPSC & 0x10000000)
#define TIM_TriggerPrecaler_4       			(TIM_MSCR_ETPSC & 0x20000000)
#define TIM_TriggerPrecaler_8       			(TIM_MSCR_ETPSC & 0x30000000)
#if defined (PT32G031x) || defined (PTM280x)
#define IS_TIM_TriggerPrecaler(SEL)   			(((SEL) & ~TIM_MSCR_ETPSC) == 0x0 && (SEL) <= TIM_TriggerPrecaler_8)
#elif defined (PT32x002x) || defined(PT32x012x) || defined(PT32x060x) || defined (PT32x063x) || defined (PT32x066x) || defined (PTM160x)
#define IS_TIM_TriggerPrecaler(SEL)   			(0)
#endif

/** @defgroup TIM_ChannelPolarity
* @{
*/
#define TIM_Channel1Polarity 					(TIM_PCR_CC1P & 0x0001)
#define TIM_Channel2Polarity  					(TIM_PCR_CC2P & 0x0010)
#define TIM_Channel3Polarity  					(TIM_PCR_CC3P & 0x0100)
#define TIM_Channel4Polarity  					(TIM_PCR_CC4P & 0x1000)
#define TIM_Channel1NPolarity 					(TIM_PCR_C1NP & 0x0002)
#define TIM_Channel2NPolarity					(TIM_PCR_C2NP & 0x0020)
#define TIM_Channel3NPolarity					(TIM_PCR_C3NP & 0x0200)
#define TIM_Channel4NPolarity					(TIM_PCR_C4NP & 0x2000)
#define TIM_ChannelPolarity_ALL      			(TIM_Channel1Polarity | TIM_Channel2Polarity | \
													TIM_Channel3Polarity | TIM_Channel4Polarity | \
													TIM_Channel1NPolarity | TIM_Channel2NPolarity | \
													TIM_Channel3NPolarity | TIM_Channel4NPolarity)
#define IS_TIM_ChannelPolarity(SEL)  			(((SEL) & ~TIM_ChannelPolarity_ALL) == 0x0)

/** @defgroup TIM_ICRiseCapture
* @{
*/
#define  TIM_ICRiseCapture_Disable				(TIM_CAPR1_IC1R & 0x00)
#define  TIM_ICRiseCapture_Enable				(TIM_CAPR1_IC1R & 0x01)
#define  IS_TIM_ICRiseCapture(SEL)				((SEL) == TIM_ICRiseCapture_Disable || (SEL) == TIM_ICRiseCapture_Enable)

/** @defgroup TIM_ICFallCapture
* @{
*/
#define  TIM_ICFallCapture_Disable				(TIM_CAPR1_IC1F & 0x00)
#define  TIM_ICFallCapture_Enable				(TIM_CAPR1_IC1F & 0x02)
#define  IS_TIM_ICFallCapture(SEL)				((SEL) == TIM_ICFallCapture_Disable || (SEL) == TIM_ICFallCapture_Enable)

/** @defgroup TIM_ICResetCounter
* @{
*/
#define  TIM_ICResetCounter_Disable				(TIM_CAPR1_IC1RC & 0x00)
#define  TIM_ICResetCounter_Enable				(TIM_CAPR1_IC1RC & 0x04)
#define  IS_TIM_ICResetCounter(SEL)				((SEL) == TIM_ICResetCounter_Disable || (SEL) == TIM_ICResetCounter_Enable)

/** @defgroup TIM_ICPrescaler
* @{
*/
#define TIM_ICPrescaler_None	          		(TIM_CAPR1_IC1PSC & 0x00000000)
#define TIM_ICPrescaler_2	         			(TIM_CAPR1_IC1PSC & 0x00000010)
#define TIM_ICPrescaler_4	         			(TIM_CAPR1_IC1PSC & 0x00000020)
#define TIM_ICPrescaler_8	         			(TIM_CAPR1_IC1PSC & 0x00000030)
#define IS_TIM_ICPrescaler(SEL)     			(((SEL) & ~TIM_CAPR1_IC1PSC) == 0x0 && (SEL) <= TIM_ICPrescaler_8)

/** @defgroup TIM_ICSource
* @{
*/
#define TIM_ICSource_ICS1            			(TIM_CAPR1_IC1S & 0x00000100)
#define TIM_ICSource_ICS2            			(TIM_CAPR1_IC1S & 0x00000200)
#define TIM_ICSource_ICS3            			(TIM_CAPR1_IC1S & 0x00000300)
#define TIM_ICSource_ICS4            			(TIM_CAPR1_IC1S & 0x00000400)
#define TIM_ICSource_TRC             			(TIM_CAPR1_IC1S & 0x00000500)
#define IS_TIM_ICSource(SEL)        			(((SEL) & ~TIM_CAPR1_IC1S) == 0x0 && (SEL) != 0x0000 && (SEL) <= TIM_ICSource_TRC)

/** @defgroup TIM_ICFilter
* @{
*/
#define TIM_ICFilter_None	     				(TIM_CAPR1_IC1FR & 0x00000000)
#define TIM_ICFilter_2	     					(TIM_CAPR1_IC1FR & 0x00001000)
#define TIM_ICFilter_4	     					(TIM_CAPR1_IC1FR & 0x00002000)
#define TIM_ICFilter_8	     					(TIM_CAPR1_IC1FR & 0x00003000)
#define TIM_ICFilter_16	     					(TIM_CAPR1_IC1FR & 0x00004000)
#define TIM_ICFilter_32	     					(TIM_CAPR1_IC1FR & 0x00005000)
#define TIM_ICFilter_64	     					(TIM_CAPR1_IC1FR & 0x00006000)
#define IS_TIM_ICFilter(SEL)       				(((SEL) & ~TIM_CAPR1_IC1FR) == 0x0 && (SEL) <= TIM_ICFilter_64)

/** @defgroup TIM_OCMode
* @{
*/
#define TIM_OCMode_Timing						(TIM_OCMR_OC1M & 0x0000)
#define TIM_OCMode_PWM1							(TIM_OCMR_OC1M & 0x0001)
#define TIM_OCMode_PWM2							(TIM_OCMR_OC1M & 0x0002)
#define TIM_OCMode_Toggle						(TIM_OCMR_OC1M & 0x0003)
#define TIM_OCMode_Active						(TIM_OCMR_OC1M & 0x0004)
#define TIM_OCMode_Inactive				    	(TIM_OCMR_OC1M & 0x0005)
#define TIM_OCMode_CompelInactive	        	(TIM_OCMR_OC1M & 0x0006)
#define TIM_OCMode_CompelActive		        	(TIM_OCMR_OC1M & 0x0007)
#define IS_TIM_OCMode(SEL)         				(((SEL) & ~TIM_OCMR_OC1M) == 0x0 && (SEL) <= TIM_OCMode_CompelActive)
													
/** @defgroup TIM_OCIdleState
* @{
*/
#define TIM_OCIdleState_Low						(TIM_OCMR_OIS1 & 0x00000)
#define TIM_OCIdleState_High					(TIM_OCMR_OIS1 & 0x10000)
#define IS_TIM_OCIdleState(SEL)					((SEL) == TIM_OCIdleState_Low || (SEL) == TIM_OCIdleState_High)

/** @defgroup TIM_OCNIdleState
* @{
*/
#define TIM_OCNIdleState_Low					(TIM_OCMR_OIS1N & 0x000000)
#define TIM_OCNIdleState_High					(TIM_OCMR_OIS1N & 0x100000)
#define IS_TIM_OCNIdleState(SEL)				((SEL) == TIM_OCNIdleState_Low || (SEL) == TIM_OCNIdleState_High)

/** @defgroup TIM_OCNOutput
* @{
*/
#define TIM_OCNOutput_Disable					(TIM_OCMR_OC1NE & 0x0000000)
#define TIM_OCNOutput_Enable					(TIM_OCMR_OC1NE & 0x1000000)
#define IS_TIM_OCNOutput(SEL)					((SEL) == TIM_OCNOutput_Disable || (SEL) == TIM_OCNOutput_Enable)

/** @defgroup TIM_OCOutput
* @{
*/
#define TIM_OCOutput_Disable					(TIM_OCMR_OC1E & 0x00000000)
#define TIM_OCOutput_Enable						(TIM_OCMR_OC1E & 0x10000000)
#define IS_TIM_OCOutput(SEL)					((SEL) == TIM_OCOutput_Disable || (SEL) == TIM_OCOutput_Enable)

/** @defgroup TIM_Break
* @{
*/
#define TIM_Break_Disable			   			(TIM_BKICR_BKE & 0x00)
#define TIM_Break_Enable			  			(TIM_BKICR_BKE & 0x01)
#define IS_TIM_Break(SEL)						((SEL) == TIM_Break_Disable || (SEL) == TIM_Break_Enable)

/** @defgroup TIM_BreakPolarity
* @{
*/
#define TIM_BreakPolarity_Low					(TIM_BKICR_BKP & 0x00)
#define TIM_BreakPolarity_High					(TIM_BKICR_BKP & 0x02)
#define IS_TIM_BreakPolarity(SEL)				((SEL) == TIM_BreakPolarity_Low || (SEL) == TIM_BreakPolarity_High)

/** @defgroup TIM_BreakInputControl
* @{
*/
#define TIM_BreakInput_OutputIdle	    		(TIM_BKICR_BKIC & 0x00)
#define TIM_BreakInput_TIMOFF	        		(TIM_BKICR_BKIC & 0x04)
#define IS_TIM_BreakInputControl(SEL)	     	((SEL) == TIM_BreakInput_OutputIdle || (SEL) == TIM_BreakInput_TIMOFF)
													
/** @defgroup TIM_BreakSoftwareControl
* @{
*/
#define TIM_BreakSoftwareControl_Disable		(TIM_BKICR_BKSC & 0x00)
#define TIM_BreakSoftwareControl_Enable			(TIM_BKICR_BKSC & 0x10)
#define IS_TIM_BreakSoftwareControl(SEL)		((SEL) == TIM_BreakSoftwareControl_Disable || (SEL) == TIM_BreakSoftwareControl_Enable)

/** @defgroup TIM_Breakfilter
* @{
*/
#define TIM_Breakfilter_None    				(TIM_BKICR_BKINFR & 0x000)
#define TIM_Breakfilter_2    					(TIM_BKICR_BKINFR & 0x100)
#define TIM_Breakfilter_4    					(TIM_BKICR_BKINFR & 0x200)
#define TIM_Breakfilter_8    					(TIM_BKICR_BKINFR & 0x300)
#define TIM_Breakfilter_16    					(TIM_BKICR_BKINFR & 0x400)
#define TIM_Breakfilter_32    					(TIM_BKICR_BKINFR & 0x500)
#define TIM_Breakfilter_64    					(TIM_BKICR_BKINFR & 0x600)
#define IS_TIM_Breakfilter(SEL)       			(((SEL) & ~TIM_BKICR_BKINFR) == 0x0 && (SEL) <= TIM_Breakfilter_64)

/** @defgroup TIM_BreakSource
* @{
*/
#define TIM_BreakSource_LOCKUP	    			TIM_BKICR_LOCKUPE
#define TIM_BreakSource_PVD				    	TIM_BKICR_PVDE
#define TIM_BreakSource_Software	    		TIM_BKICR_SWE
#define TIM_BreakSource_TIMxBKIN	    		TIM_BKICR_BKINE
#define TIM_BreakSource_CMP0			    	TIM_BKICR_CMP0E
#define TIM_BreakSource_CMP1			    	TIM_BKICR_CMP1E
#define TIM_BreakSource_CMP2		    		TIM_BKICR_CMP2E
#define TIM_BreakSource_CMP3			    	TIM_BKICR_CMP3E
#if defined (PT32G031x)	|| defined (PTM280x)
#define TIM_BreakSource_ALL			        	(TIM_BreakSource_LOCKUP | TIM_BreakSource_PVD | TIM_BreakSource_Software | TIM_BreakSource_TIMxBKIN | \
													TIM_BreakSource_CMP0 | TIM_BreakSource_CMP1)
#elif defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define TIM_BreakSource_ALL			        	(TIM_BreakSource_LOCKUP | TIM_BreakSource_Software | TIM_BreakSource_TIMxBKIN | TIM_BreakSource_CMP0)
#else
#define TIM_BreakSource_ALL			        	(TIM_BreakSource_LOCKUP | TIM_BreakSource_PVD | TIM_BreakSource_Software | TIM_BreakSource_TIMxBKIN | \
													TIM_BreakSource_CMP0 | TIM_BreakSource_CMP1 | TIM_BreakSource_CMP2 | TIM_BreakSource_CMP3)
#endif
#define IS_TIM_BreakSource(SEL)	       			(((SEL) & ~TIM_BreakSource_ALL) == 0x0 && (SEL) != 0x0000)

/** @defgroup TIM_DeadTime
* @{
*/
#define IS_TIM_DeadTime(SEL)					((SEL) <4096)


/** @defgroup TIM_DMARequest
* @{
*/
#define TIM_DMARequest_ARDE                   	(u64)TIM_TDCR1_ARDE
#define TIM_DMARequest_OC1DE                  	(u64)TIM_TDCR1_OC1DE
#define TIM_DMARequest_OC2DE                  	(u64)TIM_TDCR1_OC2DE
#define TIM_DMARequest_OC3DE                  	(u64)TIM_TDCR1_OC3DE
#define TIM_DMARequest_OC4DE                  	(u64)TIM_TDCR1_OC4DE
#define TIM_DMARequest_IC1RDE                 	(u64)TIM_TDCR1_IC1RDE
#define TIM_DMARequest_IC1FDE                 	(u64)TIM_TDCR1_IC1FDE
#define TIM_DMARequest_IC2RDE                 	(u64)TIM_TDCR1_IC2RDE
#define TIM_DMARequest_IC2FDE                 	(u64)TIM_TDCR1_IC2FDE
#define TIM_DMARequest_IC3RDE                 	(u64)TIM_TDCR1_IC3RDE
#define TIM_DMARequest_IC3FDE                 	(u64)TIM_TDCR1_IC3FDE
#define TIM_DMARequest_IC4RDE                 	(u64)TIM_TDCR1_IC4RDE
#define TIM_DMARequest_IC4FDE                 	(u64)TIM_TDCR1_IC4FDE

#define TIM_DMARequest_TDE                    	((u64)TIM_TDCR2_TDE << 32)
#define TIM_DMARequest_UDE                    	((u64)TIM_TDCR2_UDE << 32)
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define TIM_DMARequest_ALL    					(0)
#elif defined (PT32G031x) || defined (PTM280x)
#define TIM_DMARequest_ALL 						(TIM_DMARequest_ARDE | TIM_DMARequest_OC1DE | TIM_DMARequest_OC2DE | TIM_DMARequest_OC3DE | \
													TIM_DMARequest_OC4DE | TIM_DMARequest_IC1RDE | TIM_DMARequest_IC1FDE | TIM_DMARequest_IC2RDE | \
													TIM_DMARequest_IC2FDE | TIM_DMARequest_IC3RDE | TIM_DMARequest_IC3FDE | TIM_DMARequest_IC4RDE | \
													TIM_DMARequest_IC4FDE | TIM_DMARequest_TDE | TIM_DMARequest_UDE)
#elif defined (PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define TIM_DMARequest_ALL 						(TIM_DMARequest_ARDE | TIM_DMARequest_OC1DE | TIM_DMARequest_IC1RDE | TIM_DMARequest_IC1FDE | \
													TIM_DMARequest_UDE)
#else
#define TIM_DMARequest_ALL 						(TIM_DMARequest_ARDE | TIM_DMARequest_OC1DE | TIM_DMARequest_OC2DE | TIM_DMARequest_OC3DE | \
													TIM_DMARequest_OC4DE | TIM_DMARequest_IC1RDE | TIM_DMARequest_IC1FDE | TIM_DMARequest_IC2RDE | \
													TIM_DMARequest_IC2FDE | TIM_DMARequest_IC3RDE | TIM_DMARequest_IC3FDE | TIM_DMARequest_IC4RDE | \
													TIM_DMARequest_IC4FDE | TIM_DMARequest_TDE | TIM_DMARequest_UDE)
#endif
#define IS_TIM_DMARequest(SEL)               	(((SEL) & ~TIM_DMARequest_ALL) == 0x0 && (SEL) != 0x0000)

/** @defgroup TIM_ADCTrigger
* @{
*/
#define TIM_ADCTrigger_UOAE                   	TIM_TACR_UOAE
#define TIM_ADCTrigger_DOAE                   	TIM_TACR_DOAE
#define TIM_ADCTrigger_OC1UE                 	TIM_TACR_OC1UE
#define TIM_ADCTrigger_OC1DE                  	TIM_TACR_OC1DE
#define TIM_ADCTrigger_OC2UE                  	TIM_TACR_OC2UE
#define TIM_ADCTrigger_OC2DE                  	TIM_TACR_OC2DE
#define TIM_ADCTrigger_OC3UE                  	TIM_TACR_OC3UE
#define TIM_ADCTrigger_OC3DE                  	TIM_TACR_OC3DE
#define TIM_ADCTrigger_OC4UE                  	TIM_TACR_OC4UE
#define TIM_ADCTrigger_OC4DE                  	TIM_TACR_OC4DE
#define TIM_ADCTrigger_TAE                    	TIM_TACR_TAE
#define TIM_ADCTrigger_IC1RAE                 	TIM_TACR_IC1RAE
#define TIM_ADCTrigger_IC1FAE                 	TIM_TACR_IC1FAE
#define TIM_ADCTrigger_IC2RAE                 	TIM_TACR_IC2RAE
#define TIM_ADCTrigger_IC2FAE                 	TIM_TACR_IC2FAE
#define TIM_ADCTrigger_IC3RAE                 	TIM_TACR_IC3RAE
#define TIM_ADCTrigger_IC3FAE                 	TIM_TACR_IC3FAE
#define TIM_ADCTrigger_IC4RAE                 	TIM_TACR_IC4RAE
#define TIM_ADCTrigger_IC4FAE                 	TIM_TACR_IC4FAE

#if defined(PT32G031x) || defined (PTM280x)
#define TIM_ADCTrigger_ALL                    	(TIM_ADCTrigger_UOAE | TIM_ADCTrigger_DOAE | TIM_ADCTrigger_OC1UE | TIM_ADCTrigger_OC1DE | \
													TIM_ADCTrigger_OC2UE | TIM_ADCTrigger_OC2DE | TIM_ADCTrigger_OC3UE | TIM_ADCTrigger_OC3DE | \
													TIM_ADCTrigger_OC4UE | TIM_ADCTrigger_OC4DE | TIM_ADCTrigger_TAE | TIM_ADCTrigger_IC1RAE | \
													TIM_ADCTrigger_IC1FAE | TIM_ADCTrigger_IC2RAE | TIM_ADCTrigger_IC2FAE | TIM_ADCTrigger_IC3RAE | \
													TIM_ADCTrigger_IC3FAE | TIM_ADCTrigger_IC4RAE | TIM_ADCTrigger_IC4FAE)
#elif defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define TIM_ADCTrigger_ALL                    	(TIM_ADCTrigger_UOAE | TIM_ADCTrigger_DOAE | TIM_ADCTrigger_OC1UE | TIM_ADCTrigger_OC1DE | \
													TIM_ADCTrigger_OC2UE | TIM_ADCTrigger_OC2DE | TIM_ADCTrigger_OC3UE | TIM_ADCTrigger_OC3DE)
#elif defined(PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define TIM_ADCTrigger_ALL                    	(0)
#else
#define TIM_ADCTrigger_ALL                    	(TIM_ADCTrigger_UOAE | TIM_ADCTrigger_DOAE | TIM_ADCTrigger_OC1UE | TIM_ADCTrigger_OC1DE | \
													TIM_ADCTrigger_OC2UE | TIM_ADCTrigger_OC2DE | TIM_ADCTrigger_OC3UE | TIM_ADCTrigger_OC3DE | \
													TIM_ADCTrigger_OC4UE | TIM_ADCTrigger_OC4DE | TIM_ADCTrigger_TAE | TIM_ADCTrigger_IC1RAE | \
													TIM_ADCTrigger_IC1FAE | TIM_ADCTrigger_IC2RAE | TIM_ADCTrigger_IC2FAE | TIM_ADCTrigger_IC3RAE | \
													TIM_ADCTrigger_IC3FAE | TIM_ADCTrigger_IC4RAE | TIM_ADCTrigger_IC4FAE)
#endif
#define IS_TIM_ADCTrigger(SEL)                	(((SEL) & ~TIM_ADCTrigger_ALL) == 0x0 && (SEL) != 0x0000)

/** @defgroup TIM_ScanMode
* @{
*/
typedef enum
{
	TIM_ScanMode_Regular = 0,
	TIM_ScanMode_Inject
} TIM_ScanMode;

#if defined (PT32G031x) || defined (PTM280x)
#define IS_TIM_ScanMode(SEL)                	((SEL) == TIM_ScanMode_Regular || (SEL) == TIM_ScanMode_Inject)
#elif defined (PT32x002x) || defined(PT32x012x) || defined(PT32x060x) || defined (PT32x063x) || defined (PT32x066x) || defined (PTM160x)
#define IS_TIM_ScanMode(SEL)                	((SEL) == TIM_ScanMode_Regular)
#endif

/** @defgroup TIM_FLAG
* @{
*/
#define TIM_FLAG_ARF                    		(u64)TIM_SR1_ARF
#define TIM_FLAG_OC1F                   		(u64)TIM_SR1_OC1F
#define TIM_FLAG_OC2F                   		(u64)TIM_SR1_OC2F
#define TIM_FLAG_OC3F                   		(u64)TIM_SR1_OC3F
#define TIM_FLAG_OC4F                   		(u64)TIM_SR1_OC4F
#define TIM_FLAG_IC1R                			(u64)TIM_SR1_IC1R
#define TIM_FLAG_IC1F               			(u64)TIM_SR1_IC1F
#define TIM_FLAG_IC2R               			(u64)TIM_SR1_IC2R
#define TIM_FLAG_IC2F                   		(u64)TIM_SR1_IC2F
#define TIM_FLAG_IC3R               			(u64)TIM_SR1_IC3R
#define TIM_FLAG_IC3F               			(u64)TIM_SR1_IC3F
#define TIM_FLAG_IC4R                			(u64)TIM_SR1_IC4R
#define TIM_FLAG_IC4F               			(u64)TIM_SR1_IC4F

#define TIM_FLAG_BIF                    		((u64)TIM_SR2_BIF << 32)
#define TIM_FLAG_TIF                    		((u64)TIM_SR2_TIF << 32)
#define TIM_FLAG_UF                    			((u64)TIM_SR2_UF << 32)
#define TIM_FLAG_ARRUF                     		((u64)TIM_SR2_ARRUF << 32)
#define TIM_FLAG_OCR1UF                    		((u64)TIM_SR2_OCR1UF << 32)
#define TIM_FLAG_DOWN                     		((u64)TIM_SR2_DOWN << 32)
#define TIM_FLAG_UP                     		((u64)TIM_SR2_UP << 32)
#define TIM_FLAG_BSY                     		((u64)TIM_SR2_BSY << 32)

#if defined(PT32G031x) || defined (PTM280x)
#define TIM_FLAG_ALL                    		(TIM_FLAG_ARF | TIM_FLAG_OC1F | TIM_FLAG_OC2F | TIM_FLAG_OC3F | TIM_FLAG_OC4F | \
													TIM_FLAG_IC1R | TIM_FLAG_IC1F | TIM_FLAG_IC2R | TIM_FLAG_IC2F | TIM_FLAG_IC3R | \
													TIM_FLAG_IC3F | TIM_FLAG_IC4R | TIM_FLAG_IC4F | TIM_FLAG_BIF | TIM_FLAG_TIF | TIM_FLAG_UF)
#elif defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define TIM_FLAG_ALL                    		(TIM_FLAG_ARF | TIM_FLAG_OC1F | TIM_FLAG_OC2F | TIM_FLAG_OC3F | TIM_FLAG_IC1R | \
													TIM_FLAG_IC1F | TIM_FLAG_IC2R | TIM_FLAG_IC2F | TIM_FLAG_BIF | TIM_FLAG_UF)
#elif defined(PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define TIM_FLAG_ALL                    		(TIM_FLAG_ARF | TIM_FLAG_OC1F | TIM_FLAG_IC1R | TIM_FLAG_IC1F | TIM_FLAG_UF | TIM_FLAG_ARRUF | \
													TIM_FLAG_OCR1UF | TIM_FLAG_BSY)
#endif
#define IS_TIM_FLAG(SEL)               			(((SEL) & ~TIM_FLAG_ALL) == 0 && (SEL) != 0x0000)

/** @defgroup TIM_IT
* @{
*/
#define TIM_IT_ARI                   			(u64)TIM_IER1_ARIE
#define TIM_IT_OC1I                  			(u64)TIM_IER1_OC1IE
#define TIM_IT_OC2I                  			(u64)TIM_IER1_OC2IE
#define TIM_IT_OC3I                  			(u64)TIM_IER1_OC3IE
#define TIM_IT_OC4I                  			(u64)TIM_IER1_OC4IE
#define TIM_IT_IC1I                  			(u64)TIM_IER1_IC1IE
#define TIM_IT_IC2I                  			(u64)TIM_IER1_IC2IE
#define TIM_IT_IC3I                  			(u64)TIM_IER1_IC3IE
#define TIM_IT_IC4I                  			(u64)TIM_IER1_IC4IE

#define TIM_IT_BKI                   			((u64)TIM_IER2_BKIE << 32)
#define TIM_IT_TIE                   			((u64)TIM_IER2_TIE << 32)
#define TIM_IT_UIE                   			((u64)TIM_IER2_UIE << 32)
#define TIM_IT_ARRUIE                   		((u64)TIM_IER2_ARRUIE << 32)
#define TIM_IT_OCR1UIE                   		((u64)TIM_IER2_OCR1UIE << 32)
#define TIM_IT_DOWNIE                   		((u64)TIM_IER2_DOWNIE << 32)
#define TIM_IT_UPIE                   			((u64)TIM_IER2_UPIE << 32)

#if defined (PT32G031x) || defined (PTM280x)
#define TIM_IT_ALL                   			(TIM_IT_ARI | TIM_IT_OC1I | TIM_IT_OC2I | TIM_IT_OC3I | TIM_IT_OC4I | TIM_IT_IC1I | TIM_IT_IC2I | TIM_IT_IC3I | TIM_IT_IC4I | \
													TIM_IT_BKI | TIM_IT_TIE | TIM_IT_UIE)
#elif defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define TIM_IT_ALL                   			(TIM_IT_ARI | TIM_IT_OC1I | TIM_IT_OC2I | TIM_IT_OC3I | TIM_IT_IC1I | TIM_IT_IC2I | TIM_IT_BKI | TIM_IT_UIE)
#elif defined(PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define TIM_IT_ALL								(TIM_IT_ARI | TIM_IT_OC1I | TIM_IT_IC1I | TIM_IT_UIE | TIM_IT_ARRUIE | TIM_IT_OCR1UIE )
#endif
#define IS_TIM_IT(SEL)							(((SEL) & ~TIM_IT_ALL) == 0x0 && (SEL) != 0x0000)

/** @defgroup TIM_RepeatTimes
* @{
*/
#if defined(PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define IS_TIM_RepeatTimes(TIMx, SEL)			((TIMx) == LPTIM && (SEL) <= 0xFF)
#else
#define IS_TIM_RepeatTimes(TIMx, SEL)			((SEL) <= 0xF)
#endif

/** @defgroup TIM_CounterValue
* @{
*/
#if defined (PT32G031x) || defined (PTM280x)
#define IS_TIM_CounterValue(TIMx, VALUE)		((TIMx) == TIM3 || ((TIMx) !=TIM3 && (VALUE) <= 0xFFFF))
#else
#define IS_TIM_CounterValue(TIMx, VALUE)		((VALUE) <= 0xFFFF)
#endif

/** @defgroup TIM_AutoReloadValue
* @{
*/
#if defined (PT32G031x) || defined (PTM280x)
#define IS_TIM_AutoReloadValue(TIMx, VALUE)		((TIMx) == TIM3 || ((TIMx) !=TIM3 && (VALUE) <= 0xFFFF))
#else
#define IS_TIM_AutoReloadValue(TIMx, VALUE)		((VALUE) <= 0xFFFF)
#endif



/*! function declaration and definition*/
void TIM_TimeBaseInit(TIM_TypeDef *TIMx, TIM_TimeBaseInitTypeDef *TimeBaseInit);
void TIM_TimeBaseStructInit(TIM_TimeBaseInitTypeDef *TimeBaseInit);
void TIM_Cmd(TIM_TypeDef *TIMx, FunctionalState NewState);
void TIM_UpdateEvent(TIM_TypeDef *TIMx);
void TIM_SetOPM(TIM_TypeDef *TIMx);
void TIM_SetClockDivision(TIM_TypeDef *TIMx, u32 TIM_ClockDiv);
void TIM_DirectionConfig(TIM_TypeDef *TIMx, u32 Direction);
FlagStatus TIM_GetDirection(TIM_TypeDef *TIMx);
void TIM_SlaveModeInit(TIM_TypeDef *TIMx, TIM_SlaveModeInitTypeDef *TIM_SlaveModeInit);
void TIM_SlaveModeStructInit(TIM_SlaveModeInitTypeDef *TIM_SlaveModeInit);
void TIM_MasterModeInit(TIM_TypeDef *TIMx, TIM_MasterModeInitTypeDef *TIM_MasterModeInit);
void TIM_MasterModeStructInit(TIM_MasterModeInitTypeDef *TIM_MasterModeInit);
void TIM_ChannelPolarityEnable(TIM_TypeDef *TIMx,u32 ChannelPolarity);
void TIM_ChannelPolarityDisable(TIM_TypeDef *TIMx,u32 ChannelPolarity);
void TIM_ICInit(TIM_TypeDef *TIMx, TIM_ICInitTypeDef *CapInit);
void TIM_ICStructInit(TIM_ICInitTypeDef *CapInit);
void TIM_OCInit(TIM_TypeDef *TIMx, TIM_OCInitTypeDef *OutInit);
void TIM_OCStructInit(TIM_OCInitTypeDef *OutInit);
void TIM_BKICRInit(TIM_TypeDef *TIMx, TIM_BKICRInitTypeDef *BKICRInit);
void TIM_BKICRStructInit(TIM_BKICRInitTypeDef *BKICRInit);
void TIM_SoftwareBreakCMD(TIM_TypeDef *TIMx, FunctionalState NewState);
void TIM_BreakSourceConfig(TIM_TypeDef *TIMx,u32 BreakSource,FunctionalState NewState);
void TIM_SetDeadTime(TIM_TypeDef *TIMx, u16 Value);
void TIM_DMAConfig(TIM_TypeDef *TIMx, u64 DMA_Event, FunctionalState NewState);
void TIM_ADCTrigger(TIM_TypeDef *TIMx, u32 ADCTrigger, TIM_ScanMode ScanMode, FunctionalState NewState);
FlagStatus TIM_GetFlagStatus(TIM_TypeDef *TIMx, u64 TIM_FLAG);
void TIM_ClearFlag(TIM_TypeDef *TIMx, u64 TIM_FLAG);
void TIM_ITConfig(TIM_TypeDef *TIMx, u64 TIM_IT, FunctionalState NewState);
void TIM_SetInterruptAutoreload(TIM_TypeDef *TIMx, u8 Value);
void TIM_SetPrescaler(TIM_TypeDef *TIMx, u32 Value);
u32 TIM_GetPrescaler(TIM_TypeDef *TIMx);
void TIM_SetCounter(TIM_TypeDef *TIMx, u32 Value);
u32 TIM_GetCounter(TIM_TypeDef *TIMx);
void TIM_SetAutoreload(TIM_TypeDef *TIMx, u32 Value);
u32 TIM_GetAutoreload(TIM_TypeDef *TIMx);
void TIM_SetOCxValue(TIM_TypeDef *TIMx, u8 TIM_Channel, u16 Value);
void TIM_SetUpDownOCxValue(TIM_TypeDef *TIMx, u8 TIM_Channel, u16 UpValue, u16 DownValue);
u16 TIM_GetICxValue(TIM_TypeDef *TIMx, u8 TIM_Channel);

#ifdef __cplusplus
}
#endif

#endif /*__PT32x0xx_TIM_H */
