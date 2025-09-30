/**
	****************************************************************************
  * @file    PT32x0xx_tim.c
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the TIM firmware library.
  *
  ******************************************************************************
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.  
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of 
  * the content or the codes within the content. 
  * 
  *  Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  ***************************************************************************
**/

#include "PT32x0xx_tim.h"
					
/** @defgroup TIM
  * @brief TIM driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define TIM_Init_CARPCH_MASK(x)         		((TIM_CAPR1_IC1R | TIM_CAPR1_IC1F | TIM_CAPR1_IC1RC | TIM_CAPR1_IC1PSC | TIM_CAPR1_IC1S | TIM_CAPR1_IC1FR) << ((x) << 4))
#define TIM_Init_OCMRCH_MASK(x)	         		(((TIM_OCMR_OIS1 | TIM_OCMR_OIS1N | TIM_OCMR_OC1NE | TIM_OCMR_OC1E) << (x)) | (TIM_OCMR_OC1M << ((x) << 2)))

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the TIMx Time Base Unit peripheral according to
  *         the specified parameters in the TimeBaseInit.
  * @param  TIMx: ALL Tim periph.
  * @param  TimeBaseInit: pointer to a TIM_TimeBaseInitTypeDef
  *         structure that contains the configuration information for
  *         the specified TIM peripheral.
  * @retval None
  */
void TIM_TimeBaseInit(TIM_TypeDef *TIMx, TIM_TimeBaseInitTypeDef *TimeBaseInit)
{
	u32  tmpreg;
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	assert_param(IS_TIM_AutoReloadValue(TIMx,TimeBaseInit->TIM_AutoReloadValue));
	/* Get the TIMx CR value and Clear DIR/CMS bits */
	tmpreg = TIMx->CR & ~(TIM_CR_DIR | TIM_CR_CMS);
	/* Configure TIMx: direction and CenterAlignedMode */
	if(TIMx == TIM2 || TIMx == TIM3)
	{
		assert_param(IS_TIM_Direction(TimeBaseInit->TIM_Direction));	
		tmpreg |= TimeBaseInit->TIM_Direction;
	}
	else if((TIMx != TIM4) && (TIMx != LPTIM))
	{
		assert_param(IS_TIM_Direction(TimeBaseInit->TIM_Direction));	
		assert_param(IS_TIM_CenterAlignedMode(TimeBaseInit->TIM_CenterAlignedMode));
		tmpreg |= (TimeBaseInit->TIM_Direction | TimeBaseInit->TIM_CenterAlignedMode);
	}
#if defined (PT32G031x) || defined (PT32x060x) || defined (PT32x063x)|| defined (PT32x066x) || defined (PTM280x)
	/* When writing TIM4_CR, must wait for the BSY bit to be 0 */
	while((TIMx == TIM4 || TIMx == LPTIM) && (TIMx->CR & TIM_CR_BSY));
	/* Write to CR register*/
	TIMx->CR = tmpreg;
	while((TIMx == TIM4 || TIMx == LPTIM) && (TIMx->CR & TIM_CR_BSY));	
#else
	/* Write to CR register*/
	TIMx->CR = tmpreg;
#endif
	/* Set the counter reload value */
	TIMx->ARR = TimeBaseInit->TIM_AutoReloadValue;
	/* Set the Prescaler value */
	TIMx->PSCR = TimeBaseInit->TIM_Prescaler;
}

/**
  * @brief  Fills each TimeBaseInit member with its default value.
  * @param  TimeBaseInit: pointer to a TIM_TimeBaseInitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void TIM_TimeBaseStructInit(TIM_TimeBaseInitTypeDef *TimeBaseInit)
{
	/* Set the default configuration */
	TimeBaseInit->TIM_AutoReloadValue = 0xFFFF;
	TimeBaseInit->TIM_Prescaler = 0x0000;
	TimeBaseInit->TIM_Direction = TIM_Direction_Up;
	TimeBaseInit->TIM_CenterAlignedMode = TIM_CenterAlignedMode_Disable;
}

/**
  * @brief  Enable or Disable the specified TIMx peripheral.
  * @param  TIMx: ALL Tim periph.
  * @param  NewState: new state of the TIMx peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TIM_Cmd(TIM_TypeDef *TIMx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
#if defined (PT32G031x) || defined (PT32x060x) || defined (PT32x063x)|| defined (PT32x066x) || defined (PTM280x)
	/* When writing TIM4_CR, must wait for the BSY bit to be 0 */
	while((TIMx == TIM4 || TIMx == LPTIM) && (TIMx->CR & TIM_CR_BSY));
#endif
	if(NewState == ENABLE)
	{
		/* Set the enable bit */
		TIMx->CR |= TIM_CR_EN;
	}
	else
	{
		/* Reset the enable bit */
		TIMx->CR &= (~TIM_CR_EN);
	}
#if defined (PT32G031x) || defined (PT32x060x) || defined (PT32x063x)|| defined (PT32x066x) || defined (PTM280x)
	while((TIMx == TIM4 || TIMx == LPTIM) && (TIMx->CR & TIM_CR_BSY));	
#endif
}

/**
  * @brief  Configure the TIMx update event to be generate by software.
  * @param  TIMx: ALL Tim periph.
  * @retval None
  */
void TIM_UpdateEvent(TIM_TypeDef *TIMx)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));

#if defined (PT32G031x) || defined (PT32x060x) || defined (PT32x063x)|| defined (PT32x066x) || defined (PTM280x)
	while((TIMx == TIM4 || TIMx == LPTIM) && (TIMx->CR & TIM_CR_BSY));	
	/* Set the update bit */		
	TIMx->CR |= TIM_CR_UG;
	while((TIMx == TIM4 || TIMx == LPTIM) && (TIMx->CR & TIM_CR_BSY));
#else
	TIMx->CR |= TIM_CR_UG;
#endif
}

/**
  * @brief  Configure the OPM mode for the specified TIMx peripheral..
  * @param  TIMx: ALL Tim periph.
  * @retval None
  */
void TIM_SetOPM(TIM_TypeDef *TIMx)
{
	/* Check the parameters */
	assert_param(IS_TIM_LIST8_PERIPH(TIMx));

	while((TIMx == LPTIM) && (TIMx->CR & TIM_CR_BSY));	
	/* Set the OPM bit */		
	TIMx->CR |= TIM_CR_OPM;
	while((TIMx == LPTIM) && (TIMx->CR & TIM_CR_BSY));
}

/**
  * @brief  Set the sampling clock division for TIM peripheral.
  * @param  TIMx: where x can be 1, 8, or 21 to select the TIM peripheral.
  * @param  TIM_ClockDiv: sampling clock division.
  *   		This parameter can be one of the following values:
  *        	@arg TIM_CKD_DIV1: divide the TIM clock frequency into 1/1 as the sampling clock
  *        	@arg TIM_CKD_DIV2: divide the TIM clock frequency into 1/2 as the sampling clock
  *        	@arg TIM_CKD_DIV4: divide the TIM clock frequency into 1/4 as the sampling clock
  *        	@arg TIM_CKD_DIV8: divide the TIM clock frequency into 1/8 as the sampling clock
  *        	@arg TIM_CKD_DIV16: divide the TIM clock frequency into 1/16 as the sampling clock
  *        	@arg TIM_CKD_DIV32: divide the TIM clock frequency into 1/32 as the sampling clock
  * @retval None
  */
void TIM_SetClockDivision(TIM_TypeDef *TIMx, u32 TIM_ClockDiv)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_TIM_LIST5_PERIPH(TIMx));
	assert_param(IS_TIM_ClockDiv(TIM_ClockDiv));
	
	/* Get the TIMx CR value and Clear CKD bits */
	tmpreg = TIMx->CR & ~TIM_CR_CKD;
	/* Set CKD bit according to TIM_ClockDiv value */
	tmpreg |= TIM_ClockDiv;

#if defined (PT32G031x) || defined (PT32x060x) || defined (PT32x063x)|| defined (PT32x066x) || defined (PTM280x)
	while(TIMx == LPTIM && (TIMx->CR & TIM_CR_BSY));	
	/* Set the update bit */		
	TIMx->CR = tmpreg;
	while(TIMx == LPTIM && (TIMx->CR & TIM_CR_BSY));
#else
	/* Write to CR register*/
	TIMx->CR = tmpreg;
#endif
}

/**
  * @brief  Configure the count direction for the specified TIMx peripheral.
  * @param  TIMx: where x can be 1, 2, 3, 5, 6, 8, 20, 21, or 22 to select the 
  *    		TIM peripheral.
  * @param  Direction: This parameter can be up or down.
  *     	@arg TIM_Direction_Up: TIM Up Counting Mode.
  *     	@arg TIM_Direction_Down: TIM Down Counting Mode.
  * @retval None
  */
void TIM_DirectionConfig(TIM_TypeDef *TIMx, u32 Direction)
{
	u32 tmpreg;	
	/* Check the parameters */
	assert_param(IS_TIM_LIST3_PERIPH(TIMx));	
    assert_param(IS_TIM_Direction(Direction));
	/* Get the TIMx CR value and Clear DIR bits */
	tmpreg = TIMx->CR & ~TIM_CR_DIR;
	/* Set DIR bit according to Direction value */
	tmpreg |= Direction;
	/* Write to CR register*/
	TIMx->CR = tmpreg;
}

/**
  * @brief  Check whether the current counting direction is up or down.
  * @param  TIMx: where x can be 1, 2, 3, 5, 6, 8, 20, 21, or 22 to select the 
  * 		TIM peripheral
  * @return FlagStatus of TIM_CR_DIR (SET or RESET).
  */
FlagStatus TIM_GetDirection(TIM_TypeDef *TIMx)
{
	/* Check the parameters */
	assert_param(IS_TIM_LIST3_PERIPH(TIMx));	
	/* Get the direction flag */
	if((TIMx->CR & TIM_CR_DIR) != RESET)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/**
  * @brief  Initializes the TIMx peripheral slave mode function
  * @param  TIMx: where x can be 1, 5, 6,or 8 to select the TIM peripheral
  * @param  TIM_SlaveModeInit: pointer to a TIM_SlaveModeInitTypeDef
  *         structure that contains the configuration information for
  *         the specified TIM peripheral.
  * @retval None
  */
void TIM_SlaveModeInit(TIM_TypeDef *TIMx, TIM_SlaveModeInitTypeDef *TIM_SlaveModeInit)   
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_TIM_LIST1_PERIPH(TIMx));	
	assert_param(IS_TIM_TriggerFilter(TIM_SlaveModeInit->TIM_TriggerFilter));
	assert_param(IS_TIM_ExternalClock(TIM_SlaveModeInit->TIM_ExternalClock));
	assert_param(IS_TIM_TriggerPolarity(TIM_SlaveModeInit->TIM_TriggerPolarity));
	assert_param(IS_TIM_TriggerPrecaler(TIM_SlaveModeInit->TIM_TriggerPrecaler));		
	assert_param(IS_TIM_TriggerInput(TIM_SlaveModeInit->TIM_TriggerInput));
	assert_param(IS_TIM_SlaveMode(TIM_SlaveModeInit->TIM_SlaveMode));
	/* Can only write to TIS bit when SMS bit = 0 */
	if(TIMx->MSCR & TIM_MSCR_SMS)
	{
		TIMx->MSCR &= ~TIM_MSCR_SMS;
	}
	/* Configure TIMx: TriggerInput, SlaveMode, TriggerFilter, TriggerPrecaler, ExternalClock and TriggerPolarity */
	tmpreg = TIMx->MSCR & ~(TIM_MSCR_TIS | TIM_MSCR_SMS | TIM_MSCR_ECE | TIM_MSCR_ETP | TIM_MSCR_ETF | TIM_MSCR_ETPSC);	
	tmpreg |= (TIM_SlaveModeInit->TIM_TriggerInput | TIM_SlaveModeInit->TIM_SlaveMode | TIM_SlaveModeInit->TIM_TriggerFilter |
			   TIM_SlaveModeInit->TIM_TriggerPrecaler | TIM_SlaveModeInit->TIM_ExternalClock | TIM_SlaveModeInit->TIM_TriggerPolarity);
	/* Write to TIMx MSCR register */
	TIMx->MSCR = tmpreg;
}

/**
  * @brief  Fills each TIM_SlaveModeInit member with its default value.
  * @param  TIM_SlaveModeInit: pointer to a TIM_SlaveModeInitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void TIM_SlaveModeStructInit(TIM_SlaveModeInitTypeDef *TIM_SlaveModeInit)
{
	/* Set the default configuration */
	TIM_SlaveModeInit->TIM_TriggerInput = TIM_TriggerInput_ITR0;
	TIM_SlaveModeInit->TIM_SlaveMode = TIM_SlaveMode_Disable;
	TIM_SlaveModeInit->TIM_TriggerFilter = TIM_TriggerFilter_None;
	TIM_SlaveModeInit->TIM_TriggerPrecaler = TIM_TriggerPrecaler_None;
	TIM_SlaveModeInit->TIM_ExternalClock = TIM_ExternalClock_Disable;
	TIM_SlaveModeInit->TIM_TriggerPolarity = TIM_TriggerPolarity_Disable;
}

/**
  * @brief  Initializes the TIMx peripheral slave mode function
  * @param  TIMx: where x can be 1, 5, 6, or 8 to select the TIM peripheral
  * @param  TIM_MasterModeInit: pointer to a TIM_MasterModeInitTypeDef
  *         structure that contains the configuration information for
  *         the specified TIM peripheral.
  * @retval None
  */
void TIM_MasterModeInit(TIM_TypeDef *TIMx, TIM_MasterModeInitTypeDef *TIM_MasterModeInit)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_TIM_LIST1_PERIPH(TIMx));
	assert_param(IS_TIM_MasterMode(TIM_MasterModeInit->TIM_MasterMode));
	assert_param(IS_TIM_Synchronization(TIM_MasterModeInit->TIM_Synchronization));
	/* Get the TIMx MSCR value and Clear SYNC/TOS bits */
	tmpreg = TIMx->MSCR & ~(TIM_MSCR_SYNC | TIM_MSCR_TOS);
	/* Select trigger output and timer synchronization */
	tmpreg |= (TIM_MasterModeInit->TIM_MasterMode | TIM_MasterModeInit->TIM_Synchronization);
	TIMx->MSCR = tmpreg;
}

/**
  * @brief  Fills each TIM_MasterModeInit member with its default value.
  * @param  TIM_MasterModeInit: pointer to a TIM_MasterModeInitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void TIM_MasterModeStructInit(TIM_MasterModeInitTypeDef *TIM_MasterModeInit)
{
	/* Set the default configuration */
	TIM_MasterModeInit->TIM_MasterMode = TIM_MasterMode_Reset;
	TIM_MasterModeInit->TIM_Synchronization = TIM_Synchronization_Disable;
}

/**
  * @brief  Set Channel Polarity for TIM peripheral.
  * @param  TIMx: where x can be 1, 5, 6, 8, 20, 21, or 22 to select the TIM peripheral
  * @param  ChannelPolarity: all of IS_TIM_ChannelPolarity member.
  *			This parameter can be one of the following values:
  * 		@arg TIM_Channel1Polarity: Configure the TIMx channel 1 polarity.
  * 		@arg TIM_Channel2Polarity: Configure the TIMx channel 2 polarity.
  * 		@arg TIM_Channel3Polarity: Configure the TIMx channel 3 polarity.
  * 		@arg TIM_Channel4Polarity: Configure the TIMx channel 4 polarity.
  * 		@arg TIM_Channel1NPolarity: Configure the TIMx channel 1N polarity.
  * 		@arg TIM_Channel2NPolarity: Configure the TIMx channel 2N polarity. 
  * 		@arg TIM_Channel3NPolarity: Configure the TIMx channel 3N polarity.
  * 		@arg TIM_Channel4NPolarity: Configure the TIMx channel 4N polarity.
  * @retval None
  */
void TIM_ChannelPolarityEnable(TIM_TypeDef *TIMx,u32 ChannelPolarity)
{
	/* Check the parameters */
	assert_param(IS_TIM_LIST6_PERIPH(TIMx));
	assert_param(IS_TIM_ChannelPolarity(ChannelPolarity));	
	/* Set channel polarity enable*/
	TIMx->PCR |= ChannelPolarity;
}

/**
  * @brief  Reset Channel Polarity for TIM peripheral.
  * @param  TIMx: where x can be 1, 5, 6, 8, 20, 21, or 22 to select the TIM peripheral
  * @param  ChannelPolarity:all of IS_TIM_ChannelPolarity member.
  *			This parameter can be one of the following values:
  * 		@arg TIM_Channel1Polarity: Configure the TIMx channel 1 polarity.
  * 		@arg TIM_Channel2Polarity: Configure the TIMx channel 2 polarity.
  * 		@arg TIM_Channel3Polarity: Configure the TIMx channel 3 polarity.
  * 		@arg TIM_Channel4Polarity: Configure the TIMx channel 4 polarity.
  * 		@arg TIM_Channel1NPolarity: Configure the TIMx channel 1N polarity.
  * 		@arg TIM_Channel2NPolarity: Configure the TIMx channel 2N polarity. 
  * 		@arg TIM_Channel3NPolarity: Configure the TIMx channel 3N polarity.
  * 		@arg TIM_Channel4NPolarity: Configure the TIMx channel 4N polarity.
  * @retval None
  */
void TIM_ChannelPolarityDisable(TIM_TypeDef *TIMx,u32 ChannelPolarity)
{
	/* Check the parameters */
	assert_param(IS_TIM_LIST6_PERIPH(TIMx));
	assert_param(IS_TIM_ChannelPolarity(ChannelPolarity));	
	/* Set channel polarity disable*/
	TIMx->PCR &= ~ChannelPolarity;
}

/**
  * @brief  Initializes the TIMx peripheral input capture function according to
  *         the specified parameters in the CapInit.
  * @param  TIMx: where x can be 1, 8, or 21 to select the TIM peripheral
  * @param  CapInit: pointer to a TIM_ICInitTypeDef
  *         structure that contains the configuration information for
  *         the specified TIM peripheral.
  * @retval None
  */
void TIM_ICInit(TIM_TypeDef *TIMx, TIM_ICInitTypeDef *CapInit)
{
	u8 ch;
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_TIM_LIST4_PERIPH(TIMx));
	assert_param(IS_TIM_Channel(CapInit->TIM_Channel));
	assert_param(IS_TIM_ICRiseCapture(CapInit->TIM_ICRiseCapture));
	assert_param(IS_TIM_ICFallCapture(CapInit->TIM_ICFallCapture));
	assert_param(IS_TIM_ICFilter(CapInit->TIM_ICFilter));
	assert_param(IS_TIM_ICPrescaler(CapInit->TIM_ICPrescaler));
	assert_param(IS_TIM_ICResetCounter(CapInit->TIM_ICResetCounter));
	assert_param(IS_TIM_ICSource(CapInit->TIM_ICSource));
	
	ch = CapInit->TIM_Channel;
	
	/* Get the TIMx CAPR1 and CAPR2 register value */
	tmpreg = TIMx->CAPR[(ch & 0x2)>>1];
	tmpreg &= ~TIM_Init_CARPCH_MASK(ch & 0x1);
	tmpreg |= ((CapInit->TIM_ICRiseCapture << ((ch & 0x1) << 4)) | \
			   (CapInit->TIM_ICFallCapture << ((ch & 0x1) << 4)) | \
			   (CapInit->TIM_ICFilter << ((ch & 0x1) << 4)) | \
			   (CapInit->TIM_ICPrescaler << ((ch & 0x1) << 4)) | \
			   (CapInit->TIM_ICResetCounter << ((ch & 0x1) << 4)) | \
			   (CapInit->TIM_ICSource << ((ch & 0x1) << 4))); 	
	/* Write to TIMx CAPR1 and CAPR2 register */
	TIMx->CAPR[(ch & 0x2)>>1] = tmpreg;	
}

/**
  * @brief  Fills each CapInit member with its default value.
  * @param  CapInit: pointer to a TIM_ICInitTypeDef structure which will 
  *			be initialized.
  * @retval None
  */
void TIM_ICStructInit(TIM_ICInitTypeDef *CapInit)
{
	/* Set the default configuration */
	CapInit->TIM_Channel = TIM_Channel_1;
	CapInit->TIM_ICRiseCapture = TIM_ICRiseCapture_Enable;
	CapInit->TIM_ICFallCapture = TIM_ICFallCapture_Enable;
	CapInit->TIM_ICResetCounter = TIM_ICResetCounter_Enable;
	CapInit->TIM_ICSource = TIM_ICSource_ICS1;
	CapInit->TIM_ICFilter = TIM_ICFilter_64;
	CapInit->TIM_ICPrescaler = TIM_ICPrescaler_8;
}

/**
  * @brief  Initializes the TIMx peripheral output compare function according to
  *         the specified parameters in the OutInit.
  * @param  TIMx: where x can be 1, 5, 6, 8, 20, or 22 to select the TIM peripheral.
  * @param  OutInit: pointer to a TIM_OCInitTypeDef
  *         structure that contains the configuration information for
  *         the specified TIM peripheral.
  * @retval None
  */
void TIM_OCInit(TIM_TypeDef *TIMx, TIM_OCInitTypeDef *OutInit)
{
	u8 ch;
	u32 tmpreg1;
	/* Check the parameters */
	assert_param(IS_TIM_LIST1_PERIPH(TIMx));
	assert_param(IS_TIM_Channel(OutInit->TIM_Channel));
	assert_param(IS_TIM_OCMode(OutInit->TIM_OCMode));	
	assert_param(IS_TIM_OCOutput(OutInit->TIM_OCOutput));
	assert_param(IS_TIM_OCIdleState(OutInit->TIM_OCIdleState));	
	assert_param(IS_TIM_OCNOutput(OutInit->TIM_OCNOutput));	
	assert_param(IS_TIM_OCNIdleState(OutInit->TIM_OCNIdleState));	
	
	ch = OutInit->TIM_Channel;
	/* Get the TIMx OCMR register value */
	tmpreg1 = TIMx->OCMR & ~TIM_Init_OCMRCH_MASK(ch);
	tmpreg1 |=	((OutInit->TIM_OCMode << (4 * ch)) | \
				 (OutInit->TIM_OCIdleState << (1 * ch)) | \
				 (OutInit->TIM_OCOutput << (1 * ch))) |\
					(OutInit->TIM_OCNIdleState << (1 * ch)) | \
					(OutInit->TIM_OCNOutput << (1 * ch));
		
	/* Write to TIMx OCMR register */
	TIMx->OCMR = tmpreg1;
#if defined (PTM280x)
	/* Set the CHx Compare value */
	TIMx->OCR[ch] = OutInit->TIM_UpOCValue | (OutInit->TIM_DownOCValue<<16);
#else
	TIMx->OCR[ch] = OutInit->TIM_UpOCValue;
#endif
}

/**
  * @brief  Fills each OutInit member with its default value.
  * @param  OutInit: pointer to a TIM_OCInitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void TIM_OCStructInit(TIM_OCInitTypeDef *OutInit)
{
	/* Set the default configuration */
	OutInit->TIM_OCMode = TIM_OCMode_Timing;
	OutInit->TIM_OCIdleState = TIM_OCIdleState_High;
	OutInit->TIM_OCNIdleState = TIM_OCNIdleState_High;
	OutInit->TIM_OCOutput = TIM_OCOutput_Enable;
	OutInit->TIM_OCNOutput = TIM_OCNOutput_Enable;
}

/**
  * @brief  Initializes the TIMx peripheral brake and dead time function according to
  *         the specified parameters in the BKICRInit.
  * @param  TIMx: where x can be 1, 20, or 22 to select the TIM peripheral
  * @param  BKICRInit: pointer to a TIM_BKICRInitTypeDef
  *         structure that contains the configuration information for
  *         the specified TIM peripheral.
  * @retval None
  */
void TIM_BKICRInit(TIM_TypeDef *TIMx, TIM_BKICRInitTypeDef *BKICRInit)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_TIM_LIST2_PERIPH(TIMx));
	assert_param(IS_TIM_Break(BKICRInit->TIM_Break));
	assert_param(IS_TIM_BreakPolarity(BKICRInit->TIM_BreakPolarity));
	assert_param(IS_TIM_BreakSource(BKICRInit->TIM_BreakSource));
	assert_param(IS_TIM_BreakInputControl(BKICRInit->TIM_BreakInputControl));
	assert_param(IS_TIM_Breakfilter(BKICRInit->TIM_Breakfilter));	
	/* Get the TIMx BKICR register value */
	tmpreg = TIMx->BKICR & ~(TIM_BKICR_BKE | TIM_BKICR_BKP | TIM_BKICR_BKSC | TIM_BKICR_BKINFR | \
								TIM_BKICR_LOCKUPE | TIM_BKICR_PVDE | TIM_BKICR_SWE | TIM_BKICR_BKINE | \
								TIM_BKICR_CMP0E | TIM_BKICR_CMP1E | TIM_BKICR_CMP2E | TIM_BKICR_CMP3E);
	tmpreg |= (BKICRInit->TIM_Break | \
			   BKICRInit->TIM_BreakPolarity | \
			   BKICRInit->TIM_BreakInputControl  | \
			   BKICRInit->TIM_BreakSource  | \
			   BKICRInit->TIM_Breakfilter);
	/* Write to TIMx BKICR register */
	TIMx->BKICR = tmpreg;	
}

/**
  * @brief  Fills each BKICRInit member with its default value.
  * @param  BKICRInit: pointer to a TIM_BKICRInitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void TIM_BKICRStructInit(TIM_BKICRInitTypeDef *BKICRInit)
{
	/* Set the default configuration */
	BKICRInit->TIM_Break = TIM_Break_Enable;
	BKICRInit->TIM_BreakPolarity = TIM_BreakPolarity_Low;
	BKICRInit->TIM_BreakSource = TIM_BreakSource_TIMxBKIN;
	BKICRInit->TIM_BreakInputControl = TIM_BreakInput_OutputIdle;
	BKICRInit->TIM_Breakfilter = TIM_Breakfilter_4;
}

/**
  * @brief  Enable or Disable Software Break control for the specified TIM peripheral.
  * @param  TIMx: where x can be 1, 20, or 22 to select the TIM peripheral.
  * @param  NewState: new state for the TIMx peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TIM_SoftwareBreakCMD(TIM_TypeDef *TIMx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_TIM_LIST2_PERIPH(TIMx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the Software Break control */
		TIMx->BKICR |= TIM_BKICR_BKSC;
	}
	else
	{
		/* Disable the Software Break control */
		TIMx->BKICR &= ~TIM_BKICR_BKSC;	
	}
}

/**
  * @brief  Configure Break Sourse for the specified TIM peripheral.
  * @param  TIMx: where x can be 1, 20, 22 to select the TIM peripheral
  * @param  BreakSource: Specify the TIM break sources .
  *         This parameter can be any combination of the following values:
  *     	@arg TIM_BreakSource_LOCKUP: LockUp as brake input.
  *     	@arg TIM_BreakSource_PVD: PVD as brake input.
  *     	@arg TIM_BreakSource_Software: Software control as brake input.
  *     	@arg TIM_BreakSource_TIM1BKIN: TIM1_BKIN as brake input.
  *     	@arg TIM_BreakSource_CMP0: CMP0 as brake input.
  *     	@arg TIM_BreakSource_CMP1: CMP1 as brake input.
  *     	@arg TIM_BreakSource_CMP2: CMP2 as brake input.
  *     	@arg TIM_BreakSource_CMP3: CMP3 as brake input.
  * @param  NewState: new state of the Break Sourse.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TIM_BreakSourceConfig(TIM_TypeDef *TIMx,u32 BreakSource,FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_TIM_LIST2_PERIPH(TIMx));
	assert_param(IS_TIM_BreakSource(BreakSource));	
	
	if(NewState != DISABLE)
	{
		/* Enable the Break sources */
		TIMx->BKICR	|= BreakSource;
	}
	else
	{
		/* Disable the Break sources */
		TIMx->BKICR	&= ~BreakSource;	
	}
}

/**
  * @brief  Configure Dead Time value for the specified TIM peripheral.
  * @param  TIMx: where x can be 1, 20, or 22 to select the TIM peripheral
  * @param  value: TIM interrupt auto reload value.
  * @retval None
  */
void TIM_SetDeadTime(TIM_TypeDef *TIMx, u16 Value)
{
	/* Check the parameters */
	assert_param(IS_TIM_LIST2_PERIPH(TIMx));
	assert_param(IS_TIM_DeadTime(Value));
	/* Set the TIM DeadTimes */
	TIMx->DTCR = Value;
}

/**
  * @brief  Enable or disable the DMA counection function for the specified TIMx peripheral.
  * @param  TIMx: where x can be 1, 5, 6, or 8 to select the TIM peripheral
  * @param  DMA_Event: the source of DMA event.
  *         This parameter can be any combination of the following values:
  *         @arg TIM_DMARequest_ARDE
  *         @arg TIM_DMARequest_OC1DE
  *         @arg TIM_DMARequest_OC2DE
  *         @arg TIM_DMARequest_OC3DE
  *         @arg TIM_DMARequest_OC4DE
  *         @arg TIM_DMARequest_IC1RDE
  *         @arg TIM_DMARequest_IC1FDE
  *         @arg TIM_DMARequest_IC2RDE
  *         @arg TIM_DMARequest_IC2FDE
  *         @arg TIM_DMARequest_IC3RDE
  *         @arg TIM_DMARequest_IC3FDE
  *         @arg TIM_DMARequest_IC4RDE
  *         @arg TIM_DMARequest_IC4FDE
  *         @arg TIM_DMARequest_TDE
  *         @arg TIM_DMARequest_UDE
  * @param  NewState: This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void TIM_DMAConfig(TIM_TypeDef *TIMx, u64 DMA_Event, FunctionalState NewState)
{
	u64 tmp1, tmp2;
	/* Check the parameters */
	assert_param(IS_TIM_LIST1_PERIPH(TIMx));
	assert_param(IS_TIM_DMARequest(DMA_Event));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	tmp1 = DMA_Event & 0xFFFFFFFF;
	tmp2 = DMA_Event >> 32;
	if(NewState != DISABLE)
	{
		/* Enable DMA requests */
		TIMx->TDCR1 |= tmp1;
		TIMx->TDCR2 |= tmp2;
	}
	else
	{
		/* Disable DMA requests */
		TIMx->TDCR1 &= ~tmp1;
		TIMx->TDCR2 &= ~tmp2;
	}
}

/**
  * @brief  Enable or disable the ADC trigger function for the specified TIMx peripheral.
  * @param  TIMx: where x can be 1, 5, 6, 8, 20, or 22 to select the TIM peripheral.
  * @param  ADCTrigger: the mode of ADC trigger.
  *         This parameter can be one of the following values:
  *         @arg TIM_ADCTrigger_UOAE
  *         @arg TIM_ADCTrigger_DOAE
  *         @arg TIM_ADCTrigger_OC1UE
  *         @arg TIM_ADCTrigger_OC1DE
  *         @arg TIM_ADCTrigger_OC2UE
  *         @arg TIM_ADCTrigger_OC2DE
  *         @arg TIM_ADCTrigger_OC3UE
  *         @arg TIM_ADCTrigger_OC3DE
  *         @arg TIM_ADCTrigger_OC4UE
  *         @arg TIM_ADCTrigger_OC4DE
  *         @arg TIM_ADCTrigger_TAE
  *         @arg TIM_ADCTrigger_IC1RAE
  *         @arg TIM_ADCTrigger_IC1FAE
  *         @arg TIM_ADCTrigger_IC2RAE
  *         @arg TIM_ADCTrigger_IC2FAE
  *         @arg TIM_ADCTrigger_IC3RAE
  *         @arg TIM_ADCTrigger_IC3FAE
  *         @arg TIM_ADCTrigger_IC4RAE
  *         @arg TIM_ADCTrigger_IC4FAE
  * @param  ScanMode: specifies the scan mode selection.
  *			This parameter can be one of the following values: 
  *         @arg TIM_ScanMode_Regular: select regular trigger ADC.
  *         @arg TIM_ScanMode_Inject: select injected  trigger ADC.
  * @param  NewState: new state of the TIMx peripheral.
  *			This parameter can be: ENABLE or DISABLE.  
  * @retval None
  */
void TIM_ADCTrigger(TIM_TypeDef *TIMx, u32 ADCTrigger, TIM_ScanMode ScanMode, FunctionalState NewState)
{
	__IO u32 *trig;
	/* Check the parameters */
	assert_param(IS_TIM_LIST1_PERIPH(TIMx));
	assert_param(IS_TIM_ADCTrigger(ADCTrigger));
	assert_param(IS_TIM_ScanMode(ScanMode));

	if(ScanMode == TIM_ScanMode_Regular)
	{
		trig = &TIMx->TACR;
	}
	else
	{
		trig = &TIMx->JTACR;
	}
	if(NewState != DISABLE)
	{
		*trig |= ADCTrigger;
	}
	else
	{
		*trig &= ~ADCTrigger;
	}
}

/**
  * @brief  Check whether the specified TIM flag is set or not.
  * @param  TIMx: ALL Tim periph
  * @param  TIM_FLAG: Specify the flag to be checked.
  *         This parameter can be any conbination of the following values:
  *         @arg TIM_FLAG_ARF: auto reload flag. 
  *         @arg TIM_FLAG_OC1F: output compare 1 flag.
  *         @arg TIM_FLAG_OC2F: output compare 2 flag.
  *         @arg TIM_FLAG_OC3F: output compare 3 flag.
  *         @arg TIM_FLAG_OC4F: output compare 4 flag.
  *         @arg TIM_FLAG_IC1R: input capture 1 rising edge flag.
  *         @arg TIM_FLAG_IC1F: input capture 1 falling edge flag.
  *         @arg TIM_FLAG_IC2R: input capture 2 rising edge flag.
  *         @arg TIM_FLAG_IC2F: input capture 2 falling edge flag.
  *         @arg TIM_FLAG_IC3R: input capture 3 rising edge flag.
  *         @arg TIM_FLAG_IC3F: input capture 3 falling edge flag.
  *         @arg TIM_FLAG_IC4R: input capture 4 rising edge flag.
  *         @arg TIM_FLAG_IC4F: input capture 4 falling edge flag.
  *         @arg TIM_FLAG_BIF: Break input flag.
  *         @arg TIM_FLAG_TIF: Trigger input flag.
  *         @arg TIM_FLAG_UF: TIM update Flag.
  * @return FlagStatus: status of TIM_FLAG (SET or RESET).
  */
FlagStatus TIM_GetFlagStatus(TIM_TypeDef *TIMx, u64 TIM_FLAG)
{
	u64 tmp1, tmp2;
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	assert_param(IS_TIM_FLAG(TIM_FLAG));

	tmp1 = TIM_FLAG & 0xFFFFFFFF;
	tmp2 = TIM_FLAG >> 32;	
	/* Get the flags */
	if((TIMx->SR1 & tmp1) != RESET || (TIMx->SR2 & tmp2) != RESET)
	{
		return SET;
	}
	else
	{
		return RESET;
	}	
}

/**
  * @brief  Clear the specified TIMx's pending flags.
  * @param  TIMx: ALL Tim periph
  * @param  TIM_FLAG: Specify the flag to be checked.
  *         This parameter can be any conbination of the following values:
  *         @arg TIM_FLAG_ARF: auto reload flag. 
  *         @arg TIM_FLAG_OC1F: output compare 1 flag.
  *         @arg TIM_FLAG_OC2F: output compare 2 flag.
  *         @arg TIM_FLAG_OC3F: output compare 3 flag.
  *         @arg TIM_FLAG_OC4F: output compare 4 flag.
  *         @arg TIM_FLAG_IC1R: input capture 1 rising edge flag.
  *         @arg TIM_FLAG_IC1F: input capture 1 falling edge flag.
  *         @arg TIM_FLAG_IC2R: input capture 2 rising edge flag.
  *         @arg TIM_FLAG_IC2F: input capture 2 falling edge flag.
  *         @arg TIM_FLAG_IC3R: input capture 3 rising edge flag.
  *         @arg TIM_FLAG_IC3F: input capture 3 falling edge flag.
  *         @arg TIM_FLAG_IC4R: input capture 4 rising edge flag.
  *         @arg TIM_FLAG_IC4F: input capture 4 falling edge flag.
  *         @arg TIM_FLAG_BIF: Break input flag.
  *         @arg TIM_FLAG_TIF: Trigger input flag.
  *         @arg TIM_FLAG_UF: TIM update Flag.
  * @retval None
  */
void TIM_ClearFlag(TIM_TypeDef *TIMx, u64 TIM_FLAG)
{
	u64 tmp1, tmp2;
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	assert_param(IS_TIM_FLAG(TIM_FLAG));
	
	tmp1 = TIM_FLAG & 0xFFFFFFFF;
	tmp2 = TIM_FLAG >> 32;
	/* Clear the flags */
	TIMx->SR1 = tmp1;
	TIMx->SR2 = tmp2;
#if defined (PT32x066x) || defined (PT32x063x) || defined (PT32x060x)
	while(TIMx == LPTIM && (TIMx->SR1 & TIM_SR1_BSY));
	while(TIMx == LPTIM && (TIMx->SR2 & TIM_SR2_BSY));
#endif
}

/**
  * @brief  Enable or disable the specified TIM interrupts.
  * @param  TIMx: ALL Tim periph
  * @param  TIM_IT: Specify the TIM interrupts sources to be enabled or disabled.
  *         This parameter can be one of the following values:
*        	@arg TIM_IT_ARI: auto reload interrupt.
  *        	@arg TIM_IT_OC1I: output compare 1 interrupt.
  *        	@arg TIM_IT_OC2I: output compare 2 interrupt.
  *        	@arg TIM_IT_OC3I: output compare 3 interrupt.
  *        	@arg TIM_IT_OC4I: output compare 4 interrupt.
  *        	@arg TIM_IT_IC1I: input capture 1 interrupt.
  *        	@arg TIM_IT_IC2I: input capture 2 interrupt.
  *        	@arg TIM_IT_IC3I: input capture 3 interrupt.
  *        	@arg TIM_IT_IC4I: input capture 4 interrupt.
  *        	@arg TIM_IT_BKI: Break interrupt.
  *        	@arg TIM_IT_TIE: Trigger interrupt.
  *        	@arg TIM_IT_UIE: Update interrupt.
  * @param  NewState: This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void TIM_ITConfig(TIM_TypeDef *TIMx, u64 TIM_IT, FunctionalState NewState)
{
	u64 tmp1, tmp2;
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	assert_param(IS_TIM_IT(TIM_IT));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	tmp1 = TIM_IT & 0xFFFFFFFF;
	tmp2 = TIM_IT >> 32;
	if(NewState != DISABLE)
	{
		/* Enable the interrupt sources */
		if(tmp1 != 0)
		{
			TIMx->IER1 |= tmp1;
		}
		if(tmp2 != 0)
		{
			TIMx->IER2 |= tmp2;
		}
	}
	else
	{
		/* Disable the interrupt sources */
		if(tmp1 != 0)
		{
			TIMx->IER1 &= ~tmp1;
		}
		if(tmp2 != 0)
		{
			TIMx->IER2 &= ~tmp2;
		}
	}
}

/**
  * @brief  Set interrupt auto reload value for the specified TIM peripheral.
  * @param  TIMx: where x can be 1, 8, 20, or 22 to select the TIM peripheral
  * @param  value: TIM interrupt auto reload value.
  *     	This parameter must be a value between 0x0 and 0xF.
  * @retval None
  */
void TIM_SetInterruptAutoreload(TIM_TypeDef *TIMx, u8 Value)
{
	/* Check the parameters */
	assert_param(IS_TIM_LIST7_PERIPH(TIMx));
	assert_param(IS_TIM_RepeatTimes(TIMx, Value));
	/* Set the TIM INT_RepeatTimes */
	TIMx->ITARR = Value;
}

/**
  * @brief  Set value of Prescaler for the specified TIM peripheral.
  * @param  TIMx: ALL Tim periph
  * @param  value: Prescaler value.
  * @retval None
  */
void TIM_SetPrescaler(TIM_TypeDef *TIMx, u32 Value)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	/* Set the TIM Prescaler value */
	TIMx->PSCR = Value;
}

/**
  * @brief  Get prescaler value for the specified TIM peripheral.
  * @param  TIMx: ALL Tim periph
  * @retval prescaler value
  */
u32 TIM_GetPrescaler(TIM_TypeDef *TIMx)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	/* Get the TIM Prescaler value */
	return TIMx->PSCR;
}

/**
  * @brief  Set value of Cnt for the specified TIM peripheral.
  * @param  TIMx: ALL Tim periph
  * @param  value: Cnt value.
  * @retval None
  */
void TIM_SetCounter(TIM_TypeDef *TIMx, u32 Value)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	assert_param(IS_TIM_CounterValue(TIMx,Value));
	/* Set the TIM Counter value */
	TIMx->CNTR = Value;
}

/**
  * @brief  Get counter value for the specified TIM peripheral.
  * @param  TIMx: ALL Tim periph
  * @retval counter value
  */
u32 TIM_GetCounter(TIM_TypeDef *TIMx)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	/* Get the TIM Counter value */
	return TIMx->CNTR;
}

/**
  * @brief  Set auto reload value for the specified TIM peripheral.
  * @param  TIMx: ALL Tim periph.
  * @param  value: Auto reload value.
  * @retval None
  */
void TIM_SetAutoreload(TIM_TypeDef *TIMx, u32 Value)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	assert_param(IS_TIM_AutoReloadValue(TIMx,Value));
	/* Set the TIM auto reload value */
	TIMx->ARR = Value;
}

/**
  * @brief  Get auto reload value for the specified TIM peripheral.
  * @param  TIMx: ALL Tim periph.
  * @retval auto reload value.
  */
u32 TIM_GetAutoreload(TIM_TypeDef *TIMx)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	/* Get the TIM auto reload value */
	return TIMx->ARR;
}

/**
  * @brief  Set Output Compare Register for the specified TIM peripheral.
  * @param  TIMx: where x can be 1, 5, 6, 8, 20, or 22 to select the TIM peripheral.
  * @param  TIM_Channel: the output channel of TIM peripheral.
  *      	This parameter can be one of the following values:
  *        	@arg TIM_Channel_1: output channel 1 of TIM peripheral.
  *        	@arg TIM_Channel_2: output channel 2 of TIM peripheral.
  *        	@arg TIM_Channel_3: output channel 3 of TIM peripheral.
  *        	@arg TIM_Channel_4: output channel 4 of TIM peripheral.
  * @param  value: output compare value.
  * @retval None
  */
void TIM_SetOCxValue(TIM_TypeDef *TIMx, u8 TIM_Channel, u16 Value)
{	
	u8 ch;
	/* Check the parameters */
	assert_param(IS_TIM_LIST1_PERIPH(TIMx));
	assert_param(IS_TIM_Channel(TIM_Channel));
	/* Select the specified TIM channel */
	ch = TIM_Channel;
#if defined (PTM280x)
	/* Write to OCRx register*/
	TIMx->OCR[ch] = Value | (Value<<16);			
#else
	/* Write to OCRx register*/
	TIMx->OCR[ch] = Value;	
#endif
}


/**
  * @brief  Set output compare value when counting down for the specified TIM peripheral.
  * @param  TIMx: where x can be 1 or 8 to select the TIM peripheral
  * @param  TIM_Channel: the output channel of TIM peripheral.
  *      	This parameter can be one of the following values:
  *        	@arg TIM_Channel_1: output channel 1 of TIM peripheral
  *        	@arg TIM_Channel_2: output channel 2 of TIM peripheral
  *        	@arg TIM_Channel_3: output channel 3 of TIM peripheral
  *        	@arg TIM_Channel_4: output channel 4 of TIM peripheral
  * @param  value: output compare value.
  * @retval None
  */
void TIM_SetUpDownOCxValue(TIM_TypeDef *TIMx, u8 TIM_Channel, u16 UpValue, u16 DownValue)
{	
	u8 ch = 0;
	/* Check the parameters */
	assert_param(IS_TIM_LIST1_PERIPH(TIMx));
	assert_param(IS_TIM_Channel(TIM_Channel));
	/* Select the specified TIM channel */
	ch = TIM_Channel;
#if defined (PTM280x)
	/* Write to OCRx register*/
	TIMx->OCR[ch] = UpValue | (DownValue<<16);
#else
	/* Write to OCRx register*/
	TIMx->OCR[ch] = UpValue;		
#endif
}

/**
  * @brief  Get value of input capture channel for the specified TIM peripheral.
  * @param  TIMx: where x can be 1, 8, or 21 to select the TIM peripheral
  * @param  TIM_Channel: the input capture channel of TIM peripheral.
  *      	This parameter can be one of the following values:
  *        	@arg TIM_Channel_1: input capture channel 1 of TIM peripheral
  *        	@arg TIM_Channel_2: input capture channel 2 of TIM peripheral
  *        	@arg TIM_Channel_3: input capture channel 3 of TIM peripheral
  *        	@arg TIM_Channel_4: input capture channel 4 of TIM peripheral
  * @retval the value of input capture channel
  */
u16 TIM_GetICxValue(TIM_TypeDef *TIMx, u8 TIM_Channel)
{
	u16 value;
	u8 ch;
	/* Check the parameters */
	assert_param(IS_TIM_LIST4_PERIPH(TIMx));
	assert_param(IS_TIM_Channel(TIM_Channel));
	/* Select the specified TIM channel */
	ch = TIM_Channel;
	 /* Get the Capture Register value */
	value = TIMx->ICR[ch];
	return value;
}
