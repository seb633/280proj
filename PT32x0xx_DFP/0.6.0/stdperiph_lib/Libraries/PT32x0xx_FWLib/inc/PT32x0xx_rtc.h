/**
  ****************************************************************************
  * @file    PT32x0xx_rtc.h
  * @author  Application development team
  * @version V1.0.0
  * @date    2022/12/29
  * @brief    This file contains all the functions prototypes for the RTC firmware library
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PT32x0xx_RTC_H
#define PT32x0xx_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "PT32x0xx.h"



/** @addtogroup RTC
  * @{
  */ 
  
/* Exported types ------------------------------------------------------------*/

/** @defgroup RTC_ALL_PERIPH
* @{
*/
#if defined(PT32G031x) || defined(PT32x002x) || defined (PTM280x)
#define IS_RTC_ALL_PERIPH(SEL)      				(0)
#else
#define IS_RTC_ALL_PERIPH(SEL)      				((SEL) == RTC)
#endif

/** @defgroup RTC TimeFormat SET
* @{
*/
#define RTC_TF_12	                    			(RTC_CR_TF & 0x00000002)
#define RTC_TF_24	                    			(RTC_CR_TF & 0x00000000)
#define IS_RTC_TF(SEL)	            				((SEL) == RTC_TF_12  || \
								                       (SEL) == RTC_TF_24)
													   
/** @defgroup RTC BEACON
* @{
*/
#define RTC_BeaconOut_ENABLE	          			(RTC_CR_TBOE & 0x00000004)
#define RTC_BeaconOut_DISABLE	          			(RTC_CR_TBOE & 0x00000000)
#define IS_RTC_BEACON(SEL)	      					((SEL) == RTC_BeaconOut_ENABLE  || \
														(SEL) == RTC_BeaconOut_DISABLE)													   

/** @defgroup RTC Period
* @{
*/
#define IS_RTC_Period(SEL)                  		((SEL) <= 0xFFFF)

/** @defgroup RTC BCD Value
* @{
*/
#define IS_RTC_BCDValue(SEL)                		((SEL) <= 0xFFFF)

/** @defgroup RTC DEC Value
* @{
*/
#define IS_RTC_DECValue(SEL)                		((SEL) <= 0xFFFF)

typedef struct
{
    u32 Year;           		/*!< (DEC Format)Year value   */
    u32 Month;          		/*!< (DEC Format)Month value  */
    u32 Day;            		/*!< (DEC Format)Day value    */
    u32 Week;           		/*!<  Day of week value       */
    u32 Hour;           		/*!< (DEC Format)Hour value   */
    u32 Minute;         		/*!< (DEC Format)Minute value */
    u32 Second;         		/*!< (DEC Format)Second value */
	u32 PM;						/*!< 0-AM , 1-PM              */
    u32 TimeFormat;     		/*!< 12-Hour, 24-Hour         */
} RTC_Calendar;

/** @defgroup RTC DEC Time
* @{
*/
#define IS_DEC_Week(SEL)							((SEL) < 8 && (SEL) > 0)     
#define IS_DEC_Year(SEL)          					((SEL) <= 99)
#define IS_DEC_Month(SEL)							((SEL) < 13 && (SEL) > 0) 
#define IS_DEC_Day(SEL)								((SEL) < 32 && (SEL) > 0) 
#define IS_DEC_Hour(SEL)							((SEL) < 24) 
#define IS_DEC_Minute(SEL)     	    				((SEL) < 60) 
#define IS_DEC_Second(SEL)							((SEL) < 60) 

/** @defgroup RTC Time Types
* @{
*/
#define RTC_TIME_SEC		    	    			0  /*!< TIME Second  */
#define RTC_TIME_MIN		    	    			1  /*!< TIME Minutes  */
#define RTC_TIME_HOUR		    	    			2  /*!< TIME Hour  */
#define RTC_TIME_PM		   		   					3  /*!< TIME PM  */

#define RTC_TIME_DAY		    	    			4  /*!< TIME DAY  */
#define RTC_TIME_MON		    	    			5  /*!< TIME MON  */
#define RTC_TIME_WEEK		    	    			6  /*!< TIME WEEK  */
#define RTC_TIME_YEAR		   		   				7  /*!< TIME YEAR  */

#define RTC_ALM_SEC	         						8  /*!< TIME ALM_SEC  */
#define RTC_ALM_MIN	         						9  /*!< TIME ALM_MIN  */
#define RTC_ALM_HOUR	    		    			10 /*!< TIME ALM_HOUR  */
#define RTC_ALM_PM	            					11 /*!< TIME ALM_PM  */
#define IS_RTC_TIME(SEL)       	 					((SEL) <= RTC_ALM_PM) 

/** @defgroup RTC IT
* @{
*/
#define RTC_IT_HSEC									RTC_IER_HSEC
#define RTC_IT_SEC		     	 					RTC_IER_SEC 
#define RTC_IT_MIN	         	    				RTC_IER_MIN
#define RTC_IT_HOUR		     	    				RTC_IER_HOUR
#define RTC_IT_DAY		     	    				RTC_IER_DAY
#define RTC_IT_MON									RTC_IER_MON
#define RTC_IT_ALM									RTC_IER_ALM
#define RTC_IT_PED									RTC_IER_PED
#define RTC_IT_ALL                   				(RTC_IT_HSEC | RTC_IT_SEC | RTC_IT_MIN | RTC_IT_HOUR | RTC_IT_DAY | \
														RTC_IT_MON | RTC_IT_ALM | RTC_IT_PED)    
#define IS_RTC_IT(SEL)               				(((SEL) & ~RTC_IT_ALL) == 0 && (SEL) != 0)

/** @defgroup RTC FLAG
* @{ 
*/
#define RTC_FLAG_HSEC               				RTC_SR_HSEC
#define RTC_FLAG_SEC               	 				RTC_SR_SEC
#define RTC_FLAG_MIN                				RTC_SR_MIN
#define RTC_FLAG_HOUR               				RTC_SR_HOUR
#define RTC_FLAG_DAY                				RTC_SR_DAY
#define RTC_FLAG_MON                				RTC_SR_MON
#define RTC_FLAG_ALM                				RTC_SR_ALM
#define RTC_FLAG_PED                				RTC_SR_PED
#define RTC_FLAG_ALL                				(RTC_FLAG_HSEC | RTC_FLAG_SEC | RTC_FLAG_MIN | RTC_FLAG_HOUR | RTC_FLAG_DAY | \
														RTC_FLAG_MON | RTC_FLAG_ALM | RTC_FLAG_PED)     
#define IS_RTC_FLAG(SEL)             				(((SEL) & ~RTC_FLAG_ALL) == 0 && (SEL) != 0)

/** @defgroup RTC Alarm
* @{ 
*/
typedef struct
{
    u32 Hour;           		/*!< (DEC Format)Hour value   */
    u32 Minute;         		/*!< (DEC Format)Minute value */
    u32 Second;         		/*!< (DEC Format)Second value */
	u32 Week; 					/*!< Day of week value       */
    u32 PM;						/*!< 0-AM , 1-PM              */
    u32 TimeFormat;     		/*!< 0: 24-Hour,1: 12-Hour       */
} RTC_Alarm;


/** @defgroup IS_RTC_AM_PM **/
#define RTC_HOUR_AM                   				0
#define RTC_HOUR_PM                   				1
#define IS_RTC_AM_PM(SEL)             				((SEL) == RTC_HOUR_AM || (SEL) == RTC_HOUR_PM)

/** @defgroup IS_RTC_AWEEK **/
#define RTC_AWEEK_Monday             				(RTC_ALARMR_WEEK & 0x02000000)
#define RTC_AWEEK_Tuesday            				(RTC_ALARMR_WEEK & 0x04000000)
#define RTC_AWEEK_Wednesday          				(RTC_ALARMR_WEEK & 0x08000000)
#define RTC_AWEEK_Thursday           				(RTC_ALARMR_WEEK & 0x10000000)
#define RTC_AWEEK_Friday             				(RTC_ALARMR_WEEK & 0x20000000)
#define RTC_AWEEK_Saturday           				(RTC_ALARMR_WEEK & 0x40000000)
#define RTC_AWEEK_Sunday             				(RTC_ALARMR_WEEK & 0x80000000)
#define RTC_AWEEK_ALL                				(RTC_AWEEK_Monday | RTC_AWEEK_Tuesday | RTC_AWEEK_Wednesday | RTC_AWEEK_Thursday | \
														RTC_AWEEK_Friday | RTC_AWEEK_Saturday | RTC_AWEEK_Sunday)
#define IS_RTC_AWEEK(SEL)            				(((SEL) & ~RTC_AWEEK_ALL) == 0 && (SEL) != 0)

/** @defgroup IS_RTC_TrimValue **/
#define IS_RTC_TrimValue(SEL)               		((SEL) <= 0x1FF)     

/** @defgroup IS_RTC_TME **/
#define RTC_TrimMode_ENABLE	          				(RTC_CALR_CALE & 0x00000001)
#define RTC_TrimMode_DISABLE	       				(RTC_CALR_CALE & 0x00000000)
#define IS_RTC_TME(SEL)	        					((SEL) == RTC_TrimMode_ENABLE || (SEL) == RTC_TrimMode_DISABLE)


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/
void RTC_Cmd(RTC_TypeDef *RTCx, FunctionalState NewState);
void RTC_SetTimeFormat(RTC_TypeDef *RTCx, u32 Format);
void RTC_BeaconOutConfig(RTC_TypeDef *RTCx, FunctionalState NewState);
void RTC_SetPeriodInterval(RTC_TypeDef *RTCx, u16 Period);
void RTC_UnlockCmd(RTC_TypeDef *RTCx, FunctionalState NewState);
void RTC_SetCalendarTime(RTC_TypeDef *RTCx, RTC_Calendar *CalendarTime);
void RTC_GetCalendarTime(RTC_TypeDef *RTCx, RTC_Calendar *CalendarTime);
u16 RTC_GetTime(RTC_TypeDef *RTCx, u16 RTC_Time);
void RTC_SetTime(RTC_TypeDef *RTCx,u16 RTC_Time, u16 Time);
void RTC_ITConfig(RTC_TypeDef *RTCx,u16 RTC_IT, FunctionalState NewState);
void RTC_ClearFlag(RTC_TypeDef *RTCx, u8 RTC_FLAG);
FlagStatus RTC_GetFlagStatus(RTC_TypeDef *RTCx, u32 RTC_FLAG);
void RTC_GetAlarmTime(RTC_TypeDef *RTCx, RTC_Alarm *AlarmTime);
void RTC_SetAlarmTime(RTC_TypeDef *RTCx, RTC_Alarm *AlarmTime);
void RTC_SetTrim(RTC_TypeDef *RTCx, u16 Value);
void RTC_TrimModeConfig(RTC_TypeDef *RTCx, FunctionalState NewState);
/**
  * @}
  */


#ifdef __cplusplus
}
#endif

#endif 


