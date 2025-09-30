/**
********************************************************************************
* @file    PT32x0xx_gpio.h
* @author  Application development team
* @version V1.0.0
* @date    2025/2/13
* @brief   This file contains all the functions prototypes for the GPIO firmware
*          library
*
********************************************************************************
* @attention
*
* The firmware provided is for guidance only. The codes are offered for the convenience of the customers.
* Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of
* the content or the codes within the content.
*
* Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
*********************************************************************************
**/

#ifndef PT32x0xx_GPIO_H
#define PT32x0xx_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PT32x0xx.h"

/** @defgroup GPIO_ALL_PERIPH
* @{
*/
#if defined (PT32x002x) || defined(PT32G031x) || defined(PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_GPIO_ALL_PERIPH(SEL)			((SEL) == GPIOA || (SEL) == GPIOB)
#elif defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x)
#define IS_GPIO_ALL_PERIPH(SEL)			((SEL) == GPIOA || (SEL) == GPIOB || (SEL) == GPIOC || (SEL) == GPIOD || (SEL) == GPIOE)
#else
#define IS_GPIO_ALL_PERIPH(SEL)			((SEL) == GPIOA || (SEL) == GPIOB || (SEL) == GPIOC || (SEL) == GPIOD || (SEL) == GPIOE || (SEL) == GPIOF)
#endif

/** @defgroup AFIO_ALL_PERIPH
* @{
*/
#if defined (PT32x002x) || defined(PT32G031x) || defined(PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_AFIO_ALL_PERIPH(SEL)			((SEL) == AFIOA || (SEL) == AFIOB)
#elif defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x)
#define IS_AFIO_ALL_PERIPH(SEL)			((SEL) == AFIOA || (SEL) == AFIOB || (SEL) == AFIOC || (SEL) == AFIOD || (SEL) == AFIOE)
#else
#define IS_AFIO_ALL_PERIPH(SEL)			((SEL) == AFIOA || (SEL) == AFIOB || (SEL) == AFIOC || (SEL) == AFIOD || (SEL) == AFIOE || (SEL) == AFIOF)
#endif

/** @defgroup GPIO_Mode
* @{
*/
typedef enum
{
	GPIO_Mode_In   = 0x00,
	GPIO_Mode_OutPP,
	GPIO_Mode_OutOD,
} GPIO_ModeInitTypeDef;
#define IS_GPIO_Mode(SEL) 				((SEL) == GPIO_Mode_In || (SEL) == GPIO_Mode_OutPP || (SEL) == GPIO_Mode_OutOD)

/** @defgroup GPIO_Pull
* @{
*/
typedef enum
{
	GPIO_Pull_NoPull = 0x00,
	GPIO_Pull_Up     = 0x01,
	GPIO_Pull_Down   = 0x02,
	GPIO_Pull_UpDown = 0x03,
} GPIO_PullInitTypeDef;
#if defined(PT32G031x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM280x)
#define IS_GPIO_Pull(SEL) 				((SEL) == GPIO_Pull_NoPull || (SEL) == GPIO_Pull_Up || (SEL) == GPIO_Pull_Down)
#else
#define IS_GPIO_Pull(SEL) 				((SEL) == GPIO_Pull_NoPull || (SEL) == GPIO_Pull_Up || (SEL) == GPIO_Pull_Down || GPIO_Pull_UpDown)
#endif

/** @defgroup GPIO_InitTypeDef
* @{
*/
typedef struct
{
	u32 GPIO_Pin;              			//!< Specifies the GPIO pins to be configured.This parameter can be any value of @ref GPIO_Pin
	GPIO_ModeInitTypeDef GPIO_Mode;     //!<Specifies the operating mode for the selected pins.This parameter can be a value of @ref GPIO_Mode
	GPIO_PullInitTypeDef GPIO_Pull;     //!< Specifies the operating Pull-up/Pull down for the selected pins.This parameter can be a value of @ref GPIO_Pull
} GPIO_InitTypeDef;

/** @defgroup GPIO_Pin
* @{
*/
#define GPIO_Pin_0                 		((u16)0x0001)  //!< Pin 0 selected
#define GPIO_Pin_1                 		((u16)0x0002)  //!< Pin 1 selected
#define GPIO_Pin_2                 		((u16)0x0004)  //!< Pin 2 selected
#define GPIO_Pin_3                 		((u16)0x0008)  //!< Pin 3 selected
#define GPIO_Pin_4                 		((u16)0x0010)  //!< Pin 4 selected
#define GPIO_Pin_5                 		((u16)0x0020)  //!< Pin 5 selected
#define GPIO_Pin_6                 		((u16)0x0040)  //!< Pin 6 selected
#define GPIO_Pin_7                 		((u16)0x0080)  //!< Pin 7 selected
#define GPIO_Pin_8                 		((u16)0x0100)  //!< Pin 8 selected
#define GPIO_Pin_9                 		((u16)0x0200)  //!< Pin 9 selected
#define GPIO_Pin_10                		((u16)0x0400)  //!< Pin 10 selected
#define GPIO_Pin_11                		((u16)0x0800)  //!< Pin 11 selected
#define GPIO_Pin_12                		((u16)0x1000)  //!< Pin 12 selected
#define GPIO_Pin_13                		((u16)0x2000)  //!< Pin 13 selected
#define GPIO_Pin_14                		((u16)0x4000)  //!< Pin 14 selected
#define GPIO_Pin_15                		((u16)0x8000)  //!< Pin 15 selected
#define GPIO_Pin_All               		((u16)0xFFFF)  //!< All pins selected
#define IS_GPIO_Pin(SEL)        		(((SEL) & ~GPIO_Pin_All) == 0 && (SEL) != 0)

/** @defgroup AFIO_selection
* @{
*/
#define AFIO_AF_None           			((u8)0x00)	//!<  AF None
#define AFIO_AF_0            			((u8)0x01)	//!<  AF 0 selection
#define AFIO_AF_1            			((u8)0x02)	//!<  AF 1 selection
#define AFIO_AF_2            			((u8)0x03)	//!<  AF 2 selection
#define AFIO_AF_3            			((u8)0x04)	//!<  AF 3 selection
#define AFIO_AF_4            			((u8)0x05)	//!<  AF 4 selection
#define AFIO_AF_5            			((u8)0x06)	//!<  AF 5 selection
#define AFIO_AF_6            			((u8)0x07)	//!<  AF 6 selection
#define AFIO_AF_7            			((u8)0x08)	//!<  AF 7 selection
#define AFIO_AF_8            			((u8)0x09)	//!<  AF 8 selection
#define AFIO_AF_9            			((u8)0x0A)	//!<  AF 9 selection
#define AFIO_AF_10            			((u8)0x0B)	//!<  AF 10 selection
#define AFIO_AF_11            			((u8)0x0C)	//!<  AF 11 selection
#define AFIO_AF_12            			((u8)0x0D)	//!<  AF 12 selection
#define AFIO_AF_13            			((u8)0x0E)	//!<  AF 13 selection
#define AFIO_AF_14            			((u8)0x0F)	//!<  AF 14 selection

#define IS_AFIO_AF(SEL)					((SEL) <= AFIO_AF_14)
								 								 											 																 
/*! function declaration and definition*/
void GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_InitStruct);
void GPIO_StructInit(GPIO_InitTypeDef *GPIO_InitStruct);
u16 GPIO_ReadData(GPIO_TypeDef *GPIOx);
u8 GPIO_ReadDataBit(GPIO_TypeDef *CMSDK_Px, u32 GPIO_Pin);
void GPIO_ReverseBits(GPIO_TypeDef *GPIOx, u32 GPIO_Pin);
void GPIO_SetBits(GPIO_TypeDef *GPIOx, u32 GPIO_Pin);
void GPIO_ResetBits(GPIO_TypeDef *GPIOx, u32 GPIO_Pin);
void GPIO_PullUpConfig(GPIO_TypeDef *GPIOx, u32 GPIO_Pin, FunctionalState NewState);
void GPIO_PullDownConfig(GPIO_TypeDef *GPIOx, u32 GPIO_Pin, FunctionalState NewState);
void GPIO_SchmittConfig(GPIO_TypeDef *GPIOx, u32 GPIO_Pin, FunctionalState NewState);
void GPIO_DigitalRemapConfig(AFIO_TypeDef *AFIOx, u16 GPIO_Pin, u8 AFIO_AF, FunctionalState NewState);
void GPIO_AnalogRemapConfig(AFIO_TypeDef *AFIOx, u32 GPIO_Pin, FunctionalState NewState);

#ifdef __cplusplus
}
#endif

#endif /*__PT32x0xx_GPIO_H */
