/**
	****************************************************************************
  * @file    PT32x0xx_rtc.c
  * @author  Application development team
  * @version V1.0.0
  * @date    2022/12/29
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the RTC peripheral:
  *           + Initialization and Configuration
  *           + Interrupts and flags management
  *
  ******************************************************************************
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.  
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of
  * the content or the codes within the content.  
  * 
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved 
  ***************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "PT32x0xx_rtc.h"

/** @defgroup RTC
  * @brief RTC driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define RTC_UNLOCK_KEY            					((u32)0x1ACCE551)
#define RTC_CRUNLOCK_KEY          					((u32)0xAB56CD34)

#define RTC_DR_Year									(RTC_DR_YU | RTC_DR_YT)
#define RTC_DR_Month 								(RTC_DR_MU | RTC_DR_MT)
#define RTC_DR_Day    								(RTC_DR_DU | RTC_DR_DT)

#define RTC_TR_Hour 								(RTC_TR_HU | RTC_TR_HT)
#define RTC_TR_Minute								(RTC_TR_MU | RTC_TR_MT)
#define RTC_TR_Second								(RTC_TR_SU | RTC_TR_ST)

#define RTC_ALARM_Second							(RTC_ALARMR_SU | RTC_ALARMR_ST)
#define RTC_ALARM_Minute							(RTC_ALARMR_MU | RTC_ALARMR_MT)
#define RTC_ALARM_Hour								(RTC_ALARMR_HU | RTC_ALARMR_HT)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Enable or disable the specified RTC peripheral.
  * @param 	RTCx: can be RTC to select the RTC peripheral.  
  * @param  NewState: new state of the RTC peripheral.
  *        	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RTC_Cmd(RTC_TypeDef *RTCx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_RTC_ALL_PERIPH(RTCx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	/* unlock the CR register */	
	RTCx->CR = RTC_CRUNLOCK_KEY;		
	/* Waiting for CR register to be idle */	
	while(RTCx->CR & RTC_CR_BSY);
	
	if(NewState == ENABLE)
	{
		/* Enable the RTC */
		RTCx->CR |= RTC_CR_EN;
	}
	else
	{
		/* Disable the RTC */
		RTCx->CR &= (~RTC_CR_EN);
	}
	/* Waiting for CR register to be idle */
	while(RTCx->CR & RTC_CR_BSY);
}

/**
  * @brief 	Configure the RTC timeformat.
  * @param 	RTCx: can be RTC to select the RTC peripheral.  
  * @param  Format :Time format
  *   		This parameter can be one the following values:
  *        	@arg RTC_TF_12
  *        	@arg RTC_TF_24
  * @retval none
  */
void RTC_SetTimeFormat(RTC_TypeDef *RTCx, u32 Format)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_RTC_ALL_PERIPH(RTCx));
	assert_param(IS_RTC_TF(Format));

	tmpreg = RTCx->CR & ~RTC_CR_TF;
	tmpreg  |= Format;
	/* unlock the CR register */		
	RTCx->CR = RTC_CRUNLOCK_KEY;		
	/* Waiting for CR register to be idle */	
	while(RTCx->CR & RTC_CR_BSY);	
	RTCx->CR = tmpreg;	
	/* Waiting for CR register to be idle */	
	while(RTCx->CR & RTC_CR_BSY);
}

/**
  * @brief 	Enable or disable the beacon_output.
  * @param 	RTCx: can be RTC to select the RTC peripheral. 
  * @param  NewState: new state of the beacon output.
  *        	This parameter can be: ENABLE or DISABLE.
  * @retval none
  */
void RTC_BeaconOutConfig(RTC_TypeDef *RTCx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_RTC_ALL_PERIPH(RTCx));	
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	/* unlock the CR register */
	RTCx->CR = RTC_CRUNLOCK_KEY;
	
	while(RTCx->CR & RTC_CR_BSY);

	if(NewState != DISABLE)
	{
		/* Enable the beacon_output */
		RTCx->CR |= RTC_CR_TBOE;
	}
	else
	{
		/* Disable the beacon_output */
		RTCx->CR &= ~RTC_CR_TBOE;
	}
	
	while(RTCx->CR & RTC_CR_BSY);	
}

/**
  * @brief 	Set the RTC Periodic Interrupt Interval.
  * @param 	RTCx: can be RTC to select the RTC peripheral. 
  * @param  Period: RTC Periodic Interrupt Interval.
  *        	This parameter can be a number between 0 and 0xFFFF.
  * @retval none
  */
void RTC_SetPeriodInterval(RTC_TypeDef *RTCx, u16 Period)
{
	/* Check the parameters */
	assert_param(IS_RTC_ALL_PERIPH(RTCx));	
	assert_param(IS_RTC_Period(Period));
	/* Write to the CFGR register */
	RTCx->CFGR = Period;
}

/**
  * @brief 	Lock or unlock the calendar register group.
  * @param 	RTCx: can be RTC to select the RTC peripheral. 
  * @param  NewState: new state of the UNLOCK.
  *        	This parameter can be: ENABLE or DISABLE.
  * @retval	none
  */
void RTC_UnlockCmd(RTC_TypeDef *RTCx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_RTC_ALL_PERIPH(RTCx));	
    assert_param(IS_FUNCTIONAL_STATE(NewState));
	
    /* Waiting for the WPR register to be idle */
    while(RTCx->WPR & RTC_WPR_BSY);

	if(NewState == ENABLE)
	{
		/* unlock the calendar register group */
		RTCx->WPR = RTC_UNLOCK_KEY;
       /* Waiting for unlocking status */
       while(RTCx->WPR & RTC_WPR_LOCK);
	}
	else
	{
		/* lock the calendar register group */
		RTCx->WPR = 0x0;
       /* Waiting for locking status */
       while((RTCx->WPR & RTC_WPR_LOCK) == 0);
	}
    /* Waiting for the WPR register to be idle */
    while(RTCx->WPR & RTC_WPR_BSY);
}

/**
  * @brief 	Convert BCD code to decimal.
  * @param 	RTCx:  can be RTC to select the RTC peripheral.
  * @param  Value :BCD code.
  * @retval decimal Value
  */
u16 RTC_BCDToDEC(u16 Value)
{
	/* Check the parameters */
	assert_param(IS_RTC_BCDValue(Value));
	
	return ((Value & 0xF000) >> 12) * 1000 + ((Value & 0xF00) >> 8) * 100 + ((Value & 0xF0) >> 4) * 10 + (Value & 0x0F);
}

/**
  * @brief 	Convert decimal to BCD code.
  * @param 	RTCx:  can be RTC to select the RTC peripheral.
  * @param  Value :decimal Value
  * @retval BCD code
  */
u16 RTC_DECToBCD(u16 Value)
{
	/* Check the parameters */
	assert_param(IS_RTC_DECValue(Value));
	
	return (Value / 1000 << 12) + (Value / 100 % 10 << 8) + ((Value / 10 % 10) << 4) + (Value % 10);
}

/**
  * @brief 	Set Current Date and Time.
  * @param 	RTCx: can be RTC to select the RTC peripheral.
  * @param  CalendarTime: struct of date time -- dec format.
  * @retval none
  */
void RTC_SetCalendarTime(RTC_TypeDef *RTCx, RTC_Calendar *CalendarTime)
{
	u16 temp1, temp4;
	u8 temp2, temp3, temp5, temp6, temp7;
	/* Check the parameters */
	assert_param(IS_RTC_ALL_PERIPH(RTCx));
	assert_param(IS_RTC_TF(CalendarTime->TimeFormat));   //0: 24-Hour,1: 12-Hour
	assert_param(IS_DEC_Week(CalendarTime->Week));
	assert_param(IS_DEC_Year(CalendarTime->Year));				
	assert_param(IS_DEC_Month(CalendarTime->Month));
	assert_param(IS_DEC_Day(CalendarTime->Day));
	assert_param(IS_DEC_Hour(CalendarTime->Hour));
	assert_param(IS_DEC_Minute(CalendarTime->Minute));
	assert_param(IS_DEC_Second(CalendarTime->Second));
	assert_param(IS_RTC_AM_PM(CalendarTime->PM));

	/*Waiting for calendar to be idle*/
	while(RTCx->TR & RTC_TR_BSY);
	while(RTCx->DR & RTC_DR_BSY);	

	/*Convert to BCD code*/
	temp1 = RTC_DECToBCD(CalendarTime->Year);
	temp2 = RTC_DECToBCD(CalendarTime->Month);
	temp3 = RTC_DECToBCD(CalendarTime->Day);
	temp4 = RTC_DECToBCD(CalendarTime->Hour) | (CalendarTime->PM << 8);
	temp5 = RTC_DECToBCD(CalendarTime->Minute);
	temp6 = RTC_DECToBCD(CalendarTime->Second);
	temp7 = CalendarTime->Week;

	RTC_SetTimeFormat(RTC, CalendarTime->TimeFormat);
	
	RTC_UnlockCmd(RTC, ENABLE);
	RTCx->TR = temp6 | (temp5 << 8) | (temp4 << 16);
	RTCx->DR = temp3 |	(temp2 << 8) | (temp7 << 13) | (temp1 << 16);

	/* Waiting for calendar to be idle */
	while(RTCx->TR & RTC_TR_BSY);
	while(RTCx->DR & RTC_DR_BSY);	
	/* Waiting for CR register to be idle */
	RTC_UnlockCmd(RTC, DISABLE);
}

/**
  * @brief 	Get Current Date and Time value.
  * @param 	RTCx: can be RTC to select the RTC peripheral.
  * @param  CalendarTime :struct of date time
  * @retval DEC code
  */
void RTC_GetCalendarTime(RTC_TypeDef *RTCx, RTC_Calendar *CalendarTime)
{
	u32 temp1, temp2;
	/* Check the parameters */
	assert_param(IS_RTC_ALL_PERIPH(RTCx));	
	
	/* Get calendar time */
	temp1 = RTCx->TR;

	/*Get calendar time */	
	temp2 = RTCx->DR;
	
	if(temp1 != RTCx->TR)
	{	
		temp1 = RTCx->TR;
		temp2 = RTCx->DR;		
	}
	
	CalendarTime->TimeFormat = (RTCx->CR & RTC_CR_TF) >> 1;		//0: 24-Hour,1: 12-Hour
	CalendarTime->Week = (temp2 & RTC_DR_WEEK) >> 13;			//(1~7) 1:monday,...,7 sunday
	CalendarTime->Year = RTC_BCDToDEC(temp2 & RTC_DR_Year) >> 16;
	CalendarTime->Month = RTC_BCDToDEC((temp2 & RTC_DR_Month) >> 8);
	CalendarTime->Day = RTC_BCDToDEC(temp2 & RTC_DR_Day);
	CalendarTime->Hour = RTC_BCDToDEC((temp1 & RTC_TR_Hour) >> 16);
	CalendarTime->Minute = RTC_BCDToDEC((temp1 & RTC_TR_Minute) >> 8);
	CalendarTime->Second = RTC_BCDToDEC(temp1 & RTC_TR_Second);
	CalendarTime->PM = (temp1 & RTC_TR_PM) >> 24;    
}

/**
  * @brief 	Get RTC current time.
  * @param 	RTCx: can be RTC to select the RTC peripheral.
  * @param	RTC_Time specifies the RTC Time value.
  *   		This parameter can be any combination of the following values:
  *     	@arg RTC_TIME_SEC :TIME Second
  *     	@arg  RTC_TIME_MIN :TIME Minutes  
  *     	@arg  RTC_TIME_HOUR :TIME Hour  
  *     	@arg  RTC_TIME_DAY :TIME DAY  
  *     	@arg  RTC_TIME_WEEK :TIME WEEK  
  *     	@arg  RTC_TIME_MON :TIME MON  
  *     	@arg  RTC_TIME_YEAR :TIME YEAR  
  *     	@arg  RTC_TIME_ASEC :TIME ALM_SEC  
  *     	@arg  RTC_TIME_AMIN :TIME ALM_MIN  
  *     	@arg  RTC_TIME_AHOUR :TIME ALM_HOUR  
  *     	@arg  RTC_TIME_PM :TIME ALM_HOUR  
  * @retval DEC code
  */
u16 RTC_GetTime(RTC_TypeDef *RTCx, u16 RTC_Time)
{
	u16 REG;
	/* Check the parameters */
	assert_param(IS_RTC_ALL_PERIPH(RTCx));	
	assert_param(IS_RTC_TIME(RTC_Time));

	switch(RTC_Time)
	{
		case RTC_TIME_SEC:
			REG = RTCx->TR & RTC_TR_Second;
			break;
		case RTC_TIME_MIN:
			REG = (RTCx->TR & RTC_TR_Minute) >> 8;
			break;
		case RTC_TIME_HOUR:
			REG = (RTCx->TR & RTC_TR_Hour) >> 16;
			break;		
		case RTC_TIME_PM:
			REG = (RTCx->TR & RTC_TR_PM) >> 24;
			break;		
		case RTC_TIME_DAY:
			REG = RTCx->DR & RTC_DR_Day;
			break;
		case RTC_TIME_WEEK:
			REG = (RTCx->DR & RTC_DR_WEEK) >> 13;
			break;
		case RTC_TIME_MON:
			REG = (RTCx->DR & RTC_DR_Month) >> 8;
			break;
		case RTC_TIME_YEAR:
			REG = (RTCx->DR & RTC_DR_Year) >> 16;
			break;
		case RTC_ALM_SEC:
			REG = RTCx->ALARMR & RTC_ALARM_Second;
			break;
		case RTC_ALM_MIN:
			REG = (RTCx->ALARMR & RTC_ALARM_Minute) >> 8;
			break;
		case RTC_ALM_HOUR:
			REG = (RTCx->ALARMR & RTC_ALARM_Hour) >> 16;
			break;
		case RTC_ALM_PM:
			REG = (RTCx->ALARMR & RTC_ALARMR_PM) >> 24;
			break;	
		default:
			REG = 0;
			break;
	}

	return RTC_BCDToDEC(REG);
}

/**
  * @brief 	Set RTC time.
  * @param 	RTCx: can be RTC to select the RTC peripheral.
  * @param	RTC_Time specifies the RTC Time value.
  *   		This parameter can be any combination of the following values:
  *     	@arg RTC_TIME_SEC : TIME Second
  *     	@arg  RTC_TIME_MIN : TIME Minutes
  *     	@arg  RTC_TIME_HOUR :TIME Hour
  *     	@arg  RTC_TIME_PM :TIME ALM_HOUR
  *     	@arg  RTC_TIME_DAY :TIME DAY
  *     	@arg  RTC_TIME_WEEK :TIME WEEK
  *     	@arg  RTC_TIME_MON :TIME MON
  *     	@arg  RTC_TIME_YEAR :TIME YEAR
  *     	@arg  RTC_TIME_ASEC :TIME ALM_SEC
  *     	@arg  RTC_TIME_AMIN :TIME ALM_MIN
  *     	@arg  RTC_TIME_AHOUR :TIME ALM_HOUR
  * @retval BCD code
  */
void RTC_SetTime(RTC_TypeDef *RTCx, u16 RTC_Time, u16 Time)
{
	u32 tmpreg;
	u16 REG;
	/* Check the parameters */
	assert_param(IS_RTC_TIME(RTC_Time));

	REG = RTC_DECToBCD(Time);
	RTC_UnlockCmd(RTC, ENABLE);

	if(RTC_Time <= RTC_TIME_PM)
	{
		while(RTCx->TR & RTC_TR_BSY);
		tmpreg = RTCx->TR;
	}
	else if(RTC_Time <= RTC_TIME_YEAR)
	{
		while(RTCx->DR & RTC_DR_BSY);
		tmpreg = RTCx->DR;
	}
	else
	{
		tmpreg = RTCx->ALARMR;
	}
	
	switch(RTC_Time)
	{
		case RTC_TIME_SEC:
			tmpreg &= ~RTC_TR_Second;
			RTCx->TR = tmpreg | REG;
			break;
		case RTC_TIME_MIN:
			tmpreg &= ~RTC_TR_Minute;
			RTCx->TR |= tmpreg | (REG << 8);
			break;
		case RTC_TIME_HOUR:
			tmpreg &= ~RTC_TR_Hour;
			RTCx->TR |= tmpreg | (REG << 16);
			break;		
		case RTC_TIME_PM:
			tmpreg &= ~RTC_TR_PM;
			RTCx->TR |= tmpreg | (REG << 24);
			break;		
		case RTC_TIME_DAY:
			tmpreg &= ~RTC_DR_Day;
			RTCx->DR |= tmpreg | REG;
			break;
		case RTC_TIME_MON:
			tmpreg &= ~RTC_DR_Month;
			RTCx->DR |= tmpreg | (REG << 8);
			break;		
		case RTC_TIME_WEEK:
			tmpreg &= ~RTC_DR_WEEK;
			RTCx->DR |= tmpreg | (REG << 13);
			break;
		case RTC_TIME_YEAR:
			tmpreg &= ~RTC_DR_Year;
			RTCx->DR |= tmpreg | (REG << 16);
			break;
		case RTC_ALM_SEC:
			tmpreg &= ~RTC_ALARM_Second;
			RTCx->ALARMR |= tmpreg | REG;
			break;
		case RTC_ALM_MIN:
			tmpreg &= ~RTC_ALARM_Minute;
			RTCx->ALARMR |= tmpreg | (REG << 8);
			break;
		case RTC_ALM_HOUR:
			tmpreg &= ~RTC_ALARM_Hour;
			RTCx->ALARMR |= tmpreg | (REG << 16);
			break;
		case RTC_ALM_PM:
			tmpreg &= ~RTC_ALARMR_PM;
			RTCx->ALARMR |= tmpreg | (REG << 24);
			break;		
		default:
			break;
	}
}

/**
  * @brief  Enable or disable the specified RTC interrupts.
  * @param 	RTCx: can be RTC to select the RTC peripheral.  
  * @param  RTC_IT: specifies the RTC interrupts sources to be enabled or disabled.
  *   		This parameter can be any combination of the following values:
  *     	@arg RTC_IT_HSEC : Half_seconds interrupt
  *     	@arg RTC_IT_SEC: Seconds interrupt
  *     	@arg RTC_IT_MIN: Minutes interrupt
  *     	@arg RTC_IT_HOUR: Hours interrupt
  *     	@arg RTC_IT_DAY: Days interrupt
  *     	@arg RTC_IT_MON: Months interrupt
  *     	@arg RTC_IT_ALM: Alarm interrupt
  *     	@arg RTC_IT_PED: Periodic interrupt
  * 		@param  NewState: new state of the specified RTC interrupts.
  *   		This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RTC_ITConfig(RTC_TypeDef *RTCx,u16 RTC_IT, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_RTC_ALL_PERIPH(RTCx));	
	assert_param(IS_RTC_IT(RTC_IT));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		RTCx->IER |= RTC_IT;
	}
	else
	{
		RTCx->IER &= ~RTC_IT;
	}
}

/**
  * @brief  Clear the RTC's pending flags.
  * @param 	RTCx: can be RTC to select the RTC peripheral.  
  * @param  RTC_FLAG: specifies the flag to clear.
  *   		This parameter can be any combination of the following values:
  *     	@arg RTC_FLAG_HSEC
  *     	@arg RTC_FLAG_SEC
  *     	@arg RTC_FLAG_MIN
  *     	@arg RTC_FLAG_HOUR
  *     	@arg RTC_FLAG_DAY
  *     	@arg RTC_FLAG_MON
  *     	@arg RTC_FLAG_ALM
  *     	@arg RTC_FLAG_PED
  * @retval None
  */
void RTC_ClearFlag(RTC_TypeDef *RTCx, u8 RTC_FLAG)
{
	/* Check the parameters */
	assert_param(IS_RTC_FLAG(RTC_FLAG));

	/* Clear the corresponding RTC flag */
	RTCx->SR = RTC_FLAG;
}

/**
  * @brief  Check whether the specified RTC flag is set or not.
  * @param 	RTCx:  can be RTC to select the RTC peripheral.  
  * @param  RTC_FLAG: specifies the flag to check.
  *   		This parameter can be one the following values:
  *     	@arg RTC_FLAG_HSEC
  *     	@arg RTC_FLAG_SEC
  *     	@arg RTC_FLAG_MIN
  *     	@arg RTC_FLAG_HOUR
  *     	@arg RTC_FLAG_DAY
  *     	@arg RTC_FLAG_MON
  *     	@arg RTC_FLAG_ALM
  *     	@arg RTC_FLAG_PED
  * @retval The new state of RTC_FLAG (SET or RESET).
  */
FlagStatus RTC_GetFlagStatus(RTC_TypeDef *RTCx, u32 RTC_FLAG)
{
	FlagStatus bitstatus = RESET;
	/* Check the parameters */
	assert_param(IS_RTC_FLAG(RTC_FLAG));

	if((RTCx->SR & RTC_FLAG) != RESET)
	{
		bitstatus = SET;
	}
	else
	{
		bitstatus = RESET;
	}

	return bitstatus;
}

/**
  * @brief 	Get Current Alarm Time value.
  * @param 	RTCx:  can be RTC to select the RTC peripheral. 
  * @param  AlarmTime: struct of date time.
  * @retval none
  */
void RTC_GetAlarmTime(RTC_TypeDef *RTCx, RTC_Alarm *AlarmTime)
{
	AlarmTime->TimeFormat = (RTCx->CR & RTC_CR_TF) >> 1;
	AlarmTime->PM = (RTCx->ALARMR & RTC_ALARMR_PM) >> 24;
	AlarmTime->Hour = RTC_BCDToDEC((RTCx->ALARMR & (RTC_ALARMR_HU | RTC_ALARMR_HT)) >> 16);
	AlarmTime->Minute = RTC_BCDToDEC((RTCx->ALARMR & (RTC_ALARMR_MU | RTC_ALARMR_MT)) >> 8);
	AlarmTime->Second = RTC_BCDToDEC(RTCx->ALARMR & (RTC_ALARMR_SU | RTC_ALARMR_ST));
	AlarmTime->Week = RTCx->ALARMR & RTC_ALARMR_WEEK;
}

/**
  * @brief 	Set Alarm Date and Time.
  * @param 	RTCx: can be RTC to select the RTC peripheral. 
  * @param  AlarmTime :struct of date time -- dec format.
  * @retval none
  */
void RTC_SetAlarmTime(RTC_TypeDef *RTCx, RTC_Alarm *AlarmTime)
{
	u32 temp1,temp2;
	u8 temp3;
	/* Check the parameters */
	assert_param(IS_DEC_Hour(AlarmTime->Hour));
	assert_param(IS_DEC_Minute(AlarmTime->Minute));
	assert_param(IS_DEC_Second(AlarmTime->Second));
	assert_param(IS_RTC_AWEEK(AlarmTime->Week));
	assert_param(IS_RTC_AM_PM(AlarmTime->PM));

	temp1 = (RTC_DECToBCD(AlarmTime->Hour) << 16) | (AlarmTime->PM << 24);
	temp2 = RTC_DECToBCD(AlarmTime->Minute) << 8;
	temp3 = RTC_DECToBCD(AlarmTime->Second);	

	RTCx->ALARMR = temp3 | temp2 | temp1 | AlarmTime->Week;
}

/**
  * @brief 	Set RTC trim value.
  * @param 	RTCx: can be RTC to select the RTC peripheral. 
  * @param  Value: RTC trim Value.
  * @retval none
  */
void RTC_SetTrim(RTC_TypeDef *RTCx, u16 Value)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_RTC_TrimValue(Value));
	
	tmpreg = RTCx->CALR & ~RTC_CALR_TRIM;
	tmpreg |= (Value << 16);
	RTC_UnlockCmd(RTC, ENABLE);
	RTCx->CALR = tmpreg;
	RTC_UnlockCmd(RTC, DISABLE);
}


/**
  * @brief 	Enable or disable the trim mode.
  * @param 	RTCx: can be RTC to select the RTC peripheral. 
  * @param  NewState: new state of the trim mode.
  *        	This parameter can be: ENABLE or DISABLE.
  * @retval none
  */
void RTC_TrimModeConfig(RTC_TypeDef *RTCx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	RTC_UnlockCmd(RTC, ENABLE);
	if(NewState != DISABLE)
	{
		RTCx->CALR |= RTC_CALR_CALE;
	}
	else
	{
		RTCx->CALR &= ~RTC_CALR_CALE;
	}
	RTC_UnlockCmd(RTC, DISABLE);
}
/**
  * @}
  */

