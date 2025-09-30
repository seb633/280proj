/******************************************************************************
  * @file    PT32x0xx_lcd.h
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief    This file contains all the functions prototypes for the LCD firmware library
  *
  ******************************************************************************
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of
  * the content or the codes within the content.
  *
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  *******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PT32x0xx_LCD_H
#define PT32x0xx_LCD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "PT32x0xx.h"



/* Exported types ------------------------------------------------------------*/

/** @defgroup LCD_ALL_PERIPH
* @{
*/
#if defined(PT32G031x) || defined(PT32x012x) || defined (PTM160x) || defined(PTM280x)
#define IS_LCD_ALL_PERIPH(SEL)     				(0)
#else
#define IS_LCD_ALL_PERIPH(SEL)     				((SEL) == LCD)
#endif

/** @defgroup LCD_InitTypeDef
* @{
*/
typedef struct
{
	u32 LCD_Duty;									/*!< Specifies the duty.
																		This parameter can be value of @ref LCD_Duty */
	
	u32 LCD_Wave;									/*!< Specifies the wave control.
																		This parameter can be value of @ref LCD_WaveControl */
	
	u32 LCD_PageDisplayMode;						/*!< Specifies the page display mode.
																		This parameter can be value of @ref LCD_PageDisplayMode */
	
	u32 LCD_ScreenControlMode;						/*!< Specifies the screen control mode.
																		This parameter can be value of @ref LCD_ScreenControl */
	
	u32 LCD_DriveVoltageType;						/*!< Specifies the type of drive voltage.
																		This parameter can be value of @ref LCD_DriveVoltageType */
	
	u32 LCD_DriveMode;								/*!< Specifies the drive mode of C-type.
																		This parameter can be value of @ref LCD_DriveMode */
	
	u32 LCD_TotalResistance;						/*!< Specifies the total resistance.
																		This parameter can be value of @ref LCD_TotalResistance */

} LCD_InitTypeDef;

/* Exported constants --------------------------------------------------------*/

/** @defgroup LCD_Duty
* @{
*/
#define LCD_Duty_2	                      			(LCD_CR2_DUTY & 0x00000000)
#define LCD_Duty_4	                      			(LCD_CR2_DUTY & 0x00000001)
#define LCD_Duty_6	                      			(LCD_CR2_DUTY & 0x00000002)
#define LCD_Duty_8	                      			(LCD_CR2_DUTY & 0x00000003)
#define IS_LCD_DutyCycle(SEL)	         			(((SEL) & ~LCD_CR2_DUTY) == 0  &&  (SEL) <= LCD_Duty_8)

/** @defgroup LCD_Wave
* @{
*/
#define LCD_Wave_A		                  			(LCD_CR2_WAVE & 0x00000000)
#define LCD_Wave_B		                  			(LCD_CR2_WAVE & 0x00000010)
#define IS_LCD_WAVE(SEL)	          				((SEL) == LCD_Wave_A || (SEL) == LCD_Wave_B)

/** @defgroup LCD_PageDisplayMode
* @{
*/
#define LCD_PageDisplayMode_Fixed		   			(LCD_CR2_PDM & 0x00000000)
#define LCD_PageDisplayMode_Round		   	 		(LCD_CR2_PDM & 0x00000040)
#define LCD_PageDisplayMode_RoundOFF	   			(LCD_CR2_PDM & 0x00000080)
#define LCD_PageDisplayMode_Continue	   			(LCD_CR2_PDM & 0x000000C0)
#if defined(PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define IS_LCD_PageDisplay(SEL)	         			(((SEL) & ~LCD_CR2_PDM) == 0 && (SEL) <= LCD_PageDisplayMode_Fixed)
#else
#define IS_LCD_PageDisplay(SEL)	         			(((SEL) & ~LCD_CR2_PDM) == 0 && (SEL) <= LCD_PageDisplayMode_Continue)
#endif

/** @defgroup LCD_ScreenControl
* @{
*/
#define LCD_ScreenControlMode_None	        		(LCD_CR2_SCM & 0x00000000)
#define LCD_ScreenControlMode_Blink	        		(LCD_CR2_SCM & 0x00000100)
#define LCD_ScreenControlMode_ON	          		(LCD_CR2_SCM & 0x00000200)
#define LCD_ScreenControlMode_OFF	          		(LCD_CR2_SCM & 0x00000300)
#define IS_LCD_ScreenControl(SEL)           		(((SEL) & ~LCD_CR2_SCM) == 0  && (SEL) <= LCD_ScreenControlMode_OFF)

/** @defgroup LCD_DriveVoltageType
* @{
*/
#define LCD_DriveVoltageType_C	          			(LCD_CR2_TYPE & 0x00000000)
#define LCD_DriveVoltageType_R	          			(LCD_CR2_TYPE & 0x00000400)
#define LCD_DriveVoltageType_REXT	      			(LCD_CR2_TYPE & 0x00000800)
#if defined(PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define IS_LCD_DriveVoltage(SEL)	         		((SEL) ==  LCD_DriveVoltageType_C || (SEL) ==  LCD_DriveVoltageType_R)
#else
#define IS_LCD_DriveVoltage(SEL)	         		((SEL) ==  LCD_DriveVoltageType_C || (SEL) ==  LCD_DriveVoltageType_R || \
														(SEL) ==  LCD_DriveVoltageType_REXT)
#endif

/** @defgroup LCD_DriveMode
* @{
*/
#define LCD_DriveMode_1								(LCD_CR2_CSEL & 0x00000000)
#define LCD_DriveMode_3								(LCD_CR2_CSEL & 0x00001000)
#define LCD_DriveMode_1p5							(LCD_CR2_CSEL & 0x00002000)
#if defined(PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define IS_LCD_DriveMode(SEL)						((SEL) == LCD_DriveMode_1 || (SEL) == LCD_DriveMode_3)
#else
#define IS_LCD_DriveMode(SEL)						((SEL) == LCD_DriveMode_1 || (SEL) == LCD_DriveMode_3 || (SEL) == LCD_DriveMode_1p5)
#endif

/** @defgroup LCD_TotalResistance
* @{
*/
#define LCD_TotalResistance_45k	        			(LCD_CR2_RSEL & 0x00000000)
#define LCD_TotalResistance_360k	      			(LCD_CR2_RSEL & 0x00004000)
#define IS_LCD_TotalResistance(SEL)	            	((SEL) == LCD_TotalResistance_360k || (SEL) == LCD_TotalResistance_45k)

/** @defgroup LCD_GDR
* @{
*/
#define LCD_Grayscale100	        				(LCD_GCR_GDR & 0x0E)
#define LCD_Grayscale92		        				(LCD_GCR_GDR & 0x0C)
#define LCD_Grayscale85		         				(LCD_GCR_GDR & 0x0A)
#define LCD_Grayscale80		         				(LCD_GCR_GDR & 0x08)
#define LCD_Grayscale75		         				(LCD_GCR_GDR & 0x06)
#define LCD_Grayscale70		         				(LCD_GCR_GDR & 0x04)
#define LCD_Grayscale66	         	 				(LCD_GCR_GDR & 0x02)
#define LCD_Grayscale63	         	 				(LCD_GCR_GDR & 0x00)
#define IS_LCD_Grayscale(SEL)	        			(((SEL) & ~LCD_GCR_GDR)==0 && (SEL) <= LCD_Grayscale100)

/** @defgroup LCD_Prescaler
* @{
*/
#define IS_LCD_Prescaler(SEL)	             		((SEL) < 32)

/** @defgroup LCD_FrameRate
* @{
*/
#define IS_LCD_FrameRate(SEL)	             		(((SEL) <= 120) &&((SEL) >= 25))

/** @defgroup LCD_BlinkTime
* @{
*/
#define IS_LCD_BlinkTime(SEL)	         			((SEL) < 256)

/** @defgroup LCD_LightTime
* @{
*/
#define IS_LCD_LightTime(SEL)	             		((SEL) < 256)

/** @defgroup LCD_DisplayTime
* @{
*/
#if defined(PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define IS_LCD_DisplayTime(SEL)	           			(0)
#else
#define IS_LCD_DisplayTime(SEL)	           			((SEL) < 256)
#endif

/** @defgroup LCD_ScrollDisplayPage
* @{
*/
#if defined(PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define IS_LCD_ScrollDisplayPage(SEL)	       		(0)
#else
#define IS_LCD_ScrollDisplayPage(SEL)	       		((SEL) < 16)
#endif

/** @defgroup LCD_PageNumber
* @{
*/
#if defined(PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define IS_LCD_PageNumber(SEL)	             		(0)
#else
#define IS_LCD_PageNumber(SEL)	             		((SEL) < 16)
#endif

/** @defgroup LCD_DRIVER
* @{
*/
typedef enum
{
	LCD_DRIVER_SEG = 0,
	LCD_DRIVER_COM
} LCD_DRIVER;

#define IS_LCD_DRIVER(SEL)	         				((SEL) == LCD_DRIVER_SEG || (SEL) == LCD_DRIVER_COM)

/** @defgroup LCD_PAGEs/REGs
* @{
*/
#if defined(PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define LCD_MAX_PAGES                      			1
#define LCD_PAGE_REGS                      			48
#else
#define LCD_MAX_PAGES                      			16
#define LCD_PAGE_REGS                      			48
#endif

/** @defgroup LCD_Page_SEL
* @{
*/
#define LCD_Page_0									0
#define LCD_Page_1									1
#define LCD_Page_2									2
#define LCD_Page_3									3
#define LCD_Page_4									4
#define LCD_Page_5									5
#define LCD_Page_6									6
#define LCD_Page_7									7
#define LCD_Page_8									8
#define LCD_Page_9									9
#define LCD_Page_10									10
#define LCD_Page_11									11
#define LCD_Page_12									12
#define LCD_Page_13									13
#define LCD_Page_14									14
#define LCD_Page_15									15
#if defined(PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define IS_LCD_Page(SEL)	       					((SEL) == LCD_Page_0)
#else
#define IS_LCD_Page(SEL)	       					((SEL) <= LCD_Page_15)
#endif

/** @defgroup LCD_PinSource_SEL
* @{
*/
#define LCD_PinSource0								0
#define LCD_PinSource1								1
#define LCD_PinSource2								2
#define LCD_PinSource3								3
#define LCD_PinSource4								4
#define LCD_PinSource5								5
#define LCD_PinSource6								6
#define LCD_PinSource7								7
#define LCD_PinSource8								8
#define LCD_PinSource9								9
#define LCD_PinSource10								10
#define LCD_PinSource11								11
#define LCD_PinSource12								12
#define LCD_PinSource13								13
#define LCD_PinSource14								14
#define LCD_PinSource15								15
#define LCD_PinSource16								16
#define LCD_PinSource17								17
#define LCD_PinSource18								18
#define LCD_PinSource19								19
#define LCD_PinSource20								20
#define LCD_PinSource21								21
#define LCD_PinSource22								22
#define LCD_PinSource23								23
#define LCD_PinSource24								24
#define LCD_PinSource25								25
#define LCD_PinSource26								26
#define LCD_PinSource27								27
#define LCD_PinSource28								28
#define LCD_PinSource29								29
#define LCD_PinSource30								30
#define LCD_PinSource31								31
#define LCD_PinSource32								32
#define LCD_PinSource33								33
#define LCD_PinSource34								34
#define LCD_PinSource35								35
#define LCD_PinSource36								36
#define LCD_PinSource37								37
#define LCD_PinSource38								38
#define LCD_PinSource39								39
#define LCD_PinSource40								40
#define LCD_PinSource41								41
#define LCD_PinSource42								42
#define LCD_PinSource43								43
#define LCD_PinSource44								44
#define LCD_PinSource45								45
#define LCD_PinSource46								46
#define LCD_PinSource47								47
#define LCD_PinSource48								48
#define IS_LCD_PinSource(SEL)	             		((SEL) <= LCD_PinSource48)

/** @defgroup LCD_PIN_SEL
* @{
*/
#define LCD_PIN_0	              					((u64)0x000000000001)
#define LCD_PIN_1	              					((u64)0x000000000002)
#define LCD_PIN_2	              					((u64)0x000000000004)
#define LCD_PIN_3	              					((u64)0x000000000008)
#define LCD_PIN_4	             					((u64)0x000000000010)
#define LCD_PIN_5	              					((u64)0x000000000020)
#define LCD_PIN_6	              					((u64)0x000000000040)
#define LCD_PIN_7	              					((u64)0x000000000080)
#define LCD_PIN_8	              					((u64)0x000000000100)
#define LCD_PIN_9	              					((u64)0x000000000200)
#define LCD_PIN_10	              					((u64)0x000000000400)
#define LCD_PIN_11	              					((u64)0x000000000800)
#define LCD_PIN_12	              					((u64)0x000000001000)
#define LCD_PIN_13	              					((u64)0x000000002000)
#define LCD_PIN_14	              					((u64)0x000000004000)
#define LCD_PIN_15	              					((u64)0x000000008000)
#define LCD_PIN_16	              					((u64)0x000000010000)
#define LCD_PIN_17	              					((u64)0x000000020000)
#define LCD_PIN_18	              					((u64)0x000000040000)
#define LCD_PIN_19	              					((u64)0x000000080000)
#define LCD_PIN_20	              					((u64)0x000000100000)
#define LCD_PIN_21	              					((u64)0x000000200000)
#define LCD_PIN_22	              					((u64)0x000000400000)
#define LCD_PIN_23	              					((u64)0x000000800000)
#define LCD_PIN_24	              					((u64)0x000001000000)
#define LCD_PIN_25	              					((u64)0x000002000000)
#define LCD_PIN_26	              					((u64)0x000004000000)
#define LCD_PIN_27	              					((u64)0x000008000000)
#define LCD_PIN_28	              					((u64)0x000010000000)
#define LCD_PIN_29	              					((u64)0x000020000000)
#define LCD_PIN_30	              					((u64)0x000040000000)
#define LCD_PIN_31	              					((u64)0x000080000000)
#define LCD_PIN_32	              					((u64)0x000100000000)
#define LCD_PIN_33	              					((u64)0x000200000000)
#define LCD_PIN_34	              					((u64)0x000400000000)
#define LCD_PIN_35	              					((u64)0x000800000000)
#define LCD_PIN_36	              					((u64)0x001000000000)
#define LCD_PIN_37	              					((u64)0x002000000000)
#define LCD_PIN_38	              					((u64)0x004000000000)
#define LCD_PIN_39	              					((u64)0x008000000000)
#define LCD_PIN_40	              					((u64)0x010000000000)
#define LCD_PIN_41	              					((u64)0x020000000000)
#define LCD_PIN_42	              					((u64)0x040000000000)
#define LCD_PIN_43	              					((u64)0x080000000000)
#define LCD_PIN_44	              					((u64)0x100000000000)
#define LCD_PIN_45	              					((u64)0x200000000000)
#define LCD_PIN_46	              					((u64)0x400000000000)
#define LCD_PIN_47	              					((u64)0x800000000000)
#define LCD_PIN_MASK	            				((u64)0xFFFFFFFFFFFF)
#define IS_LCD_PIN(SEL)	             				(((SEL) & LCD_PIN_MASK) != 0)

/** @defgroup LCD_COM_SEL
* @{
*/
#define LCD_COM_0									0x01
#define LCD_COM_1									0x02
#define LCD_COM_2									0x04
#define LCD_COM_3									0x08
#define LCD_COM_4									0x10
#define LCD_COM_5									0x20
#define LCD_COM_6									0x40
#define LCD_COM_7									0x80
#define IS_LCD_COM(SEL)	           					((SEL) <= LCD_COM_7)

/** @defgroup LCD_Key
* @{
*/
#define LCD_VrefKey_User 							((u32)0x2c00)
#define LCD_VrefKey_Factory 						((u32)0x6300)

/** @defgroup LCD_VrefUserValue
* @{
*/
#define IS_LCD_VrefUserValue(SEL)	             	((SEL) < 16)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/

void LCD_Init(LCD_TypeDef *LCDx, LCD_InitTypeDef *LCD_InitStruct);
void LCD_StructInit(LCD_InitTypeDef *LCD_InitStruct);
void LCD_Cmd(LCD_TypeDef *LCDx, FunctionalState NewState);
void LCD_UpdataCmd(LCD_TypeDef *LCDx, FunctionalState NewState);
FlagStatus LCD_GetBusyStatus(LCD_TypeDef *LCDx);
void LCD_PageDisplayModeConfig(LCD_TypeDef *LCDx, u32 DispalyMode);
void LCD_ScreenControlModeConfig(LCD_TypeDef *LCDx, u32 Mode);
void LCD_GrayscaleCmd(LCD_TypeDef *LCDx, FunctionalState NewState);
void LCD_GrayscaleConfig(LCD_TypeDef *LCDx, u16 Grayscale);
void LCD_SetPrescaler(LCD_TypeDef *LCDx, u32 Prescaler);
void LCD_SetFrameRate(LCD_TypeDef *LCDx, u32 Duty, u32 FrameRate);
void LCD_SetBlinkTime(LCD_TypeDef *LCDx, u32 Time);
void LCD_SetLightingTime(LCD_TypeDef *LCDx, u32 Time);
void LCD_PinConfig(LCD_TypeDef *LCDx, u64 LCD_Pin, FunctionalState NewState);
void LCD_PinDriverConfig(LCD_TypeDef *LCDx, u64 LCD_Pin, u8 Driver);
void LCD_SetPageDisplayTime(LCD_TypeDef *LCDx, u16 DisplayTime);
void LCD_SetScrollDisplayPage(LCD_TypeDef *LCDx, u8 MaxPage);
void LCD_PageNumberConfig(LCD_TypeDef *LCDx, u8 PageNumber);
void LCD_VrefUserModeConfig(LCD_TypeDef *LCDx, u8 Value);
void LCD_VrefFactoryModeConfig(LCD_TypeDef *LCDx);
void LCD_ClearPage(LCD_TypeDef *LCDx, u32 PageNum);
void LCD_ClearAllPages(LCD_TypeDef *LCDx);
void LCD_SetDisplayCOM(LCD_TypeDef *LCDx, u32 LCD_PinSource, u32 COM, FunctionalState NewState);
#ifdef __cplusplus
}
#endif

#endif /*__PT32x0xx_LCD_H */
