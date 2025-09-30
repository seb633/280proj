/**
  *******************************************************************************
  * @file    PT32x0xx_rcc.h
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the RCC firmware library.
  *
  *******************************************************************************
  * @attention
 *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of
  * the content or the codes within the content.
  *
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  *******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PT32x0xx_RCC_H
#define __PT32x0xx_RCC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "PT32x0xx.h"

/** @addtogroup RCC
  * @{
  */

/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/
/** @defgroup APB_peripheral1
  * @{
  */
#define RCC_APBPeriph1_TIM1                  		((u32)0x00000002)
#define RCC_APBPeriph1_TIM2                  		((u32)0x00000004)
#define RCC_APBPeriph1_TIM3                  		((u32)0x00000008)
#define RCC_APBPeriph1_TIM4                  		((u32)0x00000010)
#define RCC_APBPeriph1_TIM5                  		((u32)0x00000020)
#define RCC_APBPeriph1_TIM6                  		((u32)0x00000040)
#define RCC_APBPeriph1_TIM7                  		((u32)0x00000080)
#define RCC_APBPeriph1_TIM8                  		((u32)0x00000100)
#define RCC_APBPeriph1_LPTIM                  		((u32)0x00000400)
#define RCC_APBPeriph1_TIM20                  		((u32)0x00100000)
#define RCC_APBPeriph1_TIM21                 		((u32)0x00200000)
#define RCC_APBPeriph1_TIM22                		((u32)0x00400000)
#if defined (PT32G031x) || defined (PTM280x)
#define RCC_APBPeriph1_ALL                  		(RCC_APBPeriph1_TIM1 | RCC_APBPeriph1_TIM2 | RCC_APBPeriph1_TIM3 | \
														RCC_APBPeriph1_TIM4 | RCC_APBPeriph1_TIM8)
#elif defined (PT32x002x) || defined(PT32x012x) || defined (PTM160x)
#define RCC_APBPeriph1_ALL                  		(RCC_APBPeriph1_TIM2 | RCC_APBPeriph1_TIM4 | RCC_APBPeriph1_TIM20 | RCC_APBPeriph1_TIM21 | \
														RCC_APBPeriph1_TIM22)
#elif defined (PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define RCC_APBPeriph1_ALL 							(RCC_APBPeriph1_TIM2 | RCC_APBPeriph1_TIM3 | RCC_APBPeriph1_TIM4 | \
														RCC_APBPeriph1_TIM5 | RCC_APBPeriph1_TIM6 | RCC_APBPeriph1_LPTIM)
#else
#define RCC_APBPeriph1_ALL                  		(RCC_APBPeriph1_TIM1 | RCC_APBPeriph1_TIM2 | RCC_APBPeriph1_TIM3 | \
														RCC_APBPeriph1_TIM4 | RCC_APBPeriph1_TIM5 | RCC_APBPeriph1_TIM6 | \
														RCC_APBPeriph1_TIM7 | RCC_APBPeriph1_TIM8 | RCC_APBPeriph1_LPTIM | \
														RCC_APBPeriph1_TIM20 | RCC_APBPeriph1_TIM21 | RCC_APBPeriph1_TIM22)
#endif
#define IS_RCC_APB_PERIPH1(SEL)              		(((SEL) & ~RCC_APBPeriph1_ALL) == 0 && (SEL) != 0)

/** @defgroup APB_peripheral2
  * @{
  */
#define RCC_APBPeriph2_UART0                  		((u32)0x00000001)
#define RCC_APBPeriph2_UART1                  		((u32)0x00000002)
#define RCC_APBPeriph2_UART2                  		((u32)0x00000004)
#define RCC_APBPeriph2_UART3                  		((u32)0x00000008)
#define RCC_APBPeriph2_USART0                  		((u32)0x00000040)
#define RCC_APBPeriph2_LPUART0                  	((u32)0x00000100)
#define RCC_APBPeriph2_LPUART1                  	((u32)0x00000200)
#define RCC_APBPeriph2_IRTIM                  		((u32)0x00008000)
#define RCC_APBPeriph2_SPI0                  		((u32)0x00010000)
#define RCC_APBPeriph2_SPI1                  		((u32)0x00020000)
#define RCC_APBPeriph2_I2C0                  		((u32)0x01000000)
#define RCC_APBPeriph2_I2C1                  		((u32)0x02000000)
#define RCC_APBPeriph2_CAN0                  		((u32)0x10000000)
#if defined (PT32G031x) || defined (PTM280x)
#define RCC_APBPeriph2_ALL                  		(RCC_APBPeriph2_UART0 | RCC_APBPeriph2_SPI0 | RCC_APBPeriph2_I2C0)
#elif defined (PT32x002x) || defined(PT32x012x) || defined (PTM160x)
#define RCC_APBPeriph2_ALL                  		(RCC_APBPeriph2_USART0 | RCC_APBPeriph2_LPUART0 | RCC_APBPeriph2_I2C0)
#elif defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x)
#define RCC_APBPeriph2_ALL                  		(RCC_APBPeriph2_UART0 | RCC_APBPeriph2_UART1 | RCC_APBPeriph2_LPUART0 | \
														RCC_APBPeriph2_LPUART1 | RCC_APBPeriph2_IRTIM | RCC_APBPeriph2_SPI0 | \
														RCC_APBPeriph2_I2C0)
#else
#define RCC_APBPeriph2_ALL                  		(RCC_APBPeriph2_UART0 | RCC_APBPeriph2_UART1 | RCC_APBPeriph2_UART2 | \
														RCC_APBPeriph2_UART3 | RCC_APBPeriph2_USART0 | RCC_APBPeriph2_LPUART0 | \
														RCC_APBPeriph2_LPUART1 | RCC_APBPeriph2_IRTIM | RCC_APBPeriph2_SPI0 | \
														RCC_APBPeriph2_SPI1 | RCC_APBPeriph2_I2C0 | RCC_APBPeriph2_I2C1 | RCC_APBPeriph2_CAN0)
#endif
#define IS_RCC_APB_PERIPH2(SEL)              		(((SEL) & ~RCC_APBPeriph2_ALL) == 0 && (SEL) != 0)

/** @defgroup APB_peripheral3
  * @{
  */
#define RCC_APBPeriph3_ADC0                  		((u32)0x00000001)
#define RCC_APBPeriph3_SDADC0                  		((u32)0x00000002)
#define RCC_APBPeriph3_DAC0                  		((u32)0x00000010)
#define RCC_APBPeriph3_DAC1                  		((u32)0x00000020)
#define RCC_APBPeriph3_CMP0                  		((u32)0x00000100)
#define RCC_APBPeriph3_CMP1                  		((u32)0x00000200)
#define RCC_APBPeriph3_CMP2                  		((u32)0x00000400)
#define RCC_APBPeriph3_CMP3                     	((u32)0x00000800)
#define RCC_APBPeriph3_OPA                      	((u32)0x00010000)
#define RCC_APBPeriph3_LDAC                  		((u32)0x00020000)
#define RCC_APBPeriph3_DDS                  		((u32)0x00040000)
#define RCC_APBPeriph3_LCD                  		((u32)0x00100000)
#define RCC_APBPeriph3_LEDPWM                  		((u32)0x02000000)
#define RCC_APBPeriph3_BEEP                  		((u32)0x10000000)
#if defined (PT32G031x) || defined (PTM280x)
#define RCC_APBPeriph3_ALL                  		(RCC_APBPeriph3_ADC0 | RCC_APBPeriph3_CMP0 | RCC_APBPeriph3_CMP1 | \
														RCC_APBPeriph3_OPA | RCC_APBPeriph3_LDAC)
#elif defined (PT32x002x) || defined(PT32x012x)
#define RCC_APBPeriph3_ALL                  		(RCC_APBPeriph3_ADC0 | RCC_APBPeriph3_CMP0 | RCC_APBPeriph3_LDAC | RCC_APBPeriph3_LEDPWM)
#elif defined(PTM160x)
#define RCC_APBPeriph3_ALL							(RCC_APBPeriph3_ADC0 | RCC_APBPeriph3_CMP0 | RCC_APBPeriph3_OPA | RCC_APBPeriph3_LDAC | RCC_APBPeriph3_LEDPWM)
#elif defined(PT32x060x) || defined(PT32x063x)
#define RCC_APBPeriph3_ALL                  		(RCC_APBPeriph3_SDADC0 | RCC_APBPeriph3_DAC0 | RCC_APBPeriph3_OPA | \
														RCC_APBPeriph3_LCD | RCC_APBPeriph3_BEEP)
#elif defined(PT32x066x)
#define RCC_APBPeriph3_ALL                  		(RCC_APBPeriph3_SDADC0 | RCC_APBPeriph3_DAC0 | RCC_APBPeriph3_DAC1 | \
														RCC_APBPeriph3_OPA | RCC_APBPeriph3_DDS | RCC_APBPeriph3_LCD | RCC_APBPeriph3_BEEP)
#else
#define RCC_APBPeriph3_ALL                  		(RCC_APBPeriph3_ADC0 | RCC_APBPeriph3_SDADC0 | RCC_APBPeriph3_DAC0 | \
														RCC_APBPeriph3_DAC1 | RCC_APBPeriph3_CMP0 | RCC_APBPeriph3_CMP1 | \
														RCC_APBPeriph3_CMP2 | RCC_APBPeriph3_CMP3 | RCC_APBPeriph3_OPA | \
														RCC_APBPeriph3_LDAC | RCC_APBPeriph3_DDS | RCC_APBPeriph3_LCD | \
														RCC_APBPeriph3_PWM | RCC_APBPeriph3_BEEP)
#endif
#define IS_RCC_APB_PERIPH3(SEL)              		(((SEL) & ~RCC_APBPeriph3_ALL) == 0 && (SEL) != 0)

/** @defgroup APB_peripheral4
  * @{
  */
#define RCC_APBPeriph4_IWDG                  		((u32)0x00000001)
#define RCC_APBPeriph4_WWDG                  		((u32)0x00000002)
#define RCC_APBPeriph4_RTC                  		((u32)0x00000004)
#define RCC_APBPeriph4_AES                  		((u32)0x00000100)
#define RCC_APBPeriph4_CRC                     		((u32)0x00000200)
#define RCC_APBPeriph4_IFMC                     	((u32)0x00010000)
#if defined (PT32G031x) || defined (PTM280x)
#define RCC_APBPeriph4_ALL                			(RCC_APBPeriph4_IWDG | RCC_APBPeriph4_CRC)
#elif defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define RCC_APBPeriph4_ALL                			(RCC_APBPeriph4_IWDG | RCC_APBPeriph4_IFMC)
#elif defined(PT32x063x)
#define RCC_APBPeriph4_ALL                			(RCC_APBPeriph4_IWDG | RCC_APBPeriph4_RTC | RCC_APBPeriph4_AES | RCC_APBPeriph4_CRC)
#elif defined(PT32x060x) || defined(PT32x066x)
#define RCC_APBPeriph4_ALL                			(RCC_APBPeriph4_IWDG | RCC_APBPeriph4_RTC | RCC_APBPeriph4_CRC)
#else
#define RCC_APBPeriph4_ALL                			(RCC_APBPeriph4_IWDG | RCC_APBPeriph4_WWDG | RCC_APBPeriph4_RTC | \
														RCC_APBPeriph4_AES | RCC_APBPeriph4_CRC)
#endif
#define IS_RCC_APB_PERIPH4(SEL)              		(((SEL) & ~RCC_APBPeriph4_ALL) == 0 && (SEL) != 0)


/** @defgroup AHB_peripheral
  * @{
  */
#define RCC_AHBPeriph_GPIOA               			((u32)0x00000001)
#define RCC_AHBPeriph_GPIOB               			((u32)0x00000002)
#define RCC_AHBPeriph_GPIOC               			((u32)0x00000004)
#define RCC_AHBPeriph_GPIOD               			((u32)0x00000008)
#define RCC_AHBPeriph_GPIOE               			((u32)0x00000010)
#define RCC_AHBPeriph_GPIOF               			((u32)0x00000020)
#define RCC_AHBPeriph_DMA0                			((u32)0x00000100)
#define RCC_AHBPeriph_ALU                 			((u32)0x00001000)
#define RCC_AHBPeriph_AHB                 			((u32)0x80000000)
#if defined (PT32G031x) || defined (PTM280x)
#define RCC_AHBPeriph_ALL                 			(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_DMA0 | \
														RCC_AHBPeriph_ALU | RCC_AHBPeriph_AHB)
#elif defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define RCC_AHBPeriph_ALL                 			(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB)
#elif defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x)
#define RCC_AHBPeriph_ALL                 			(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | \
														RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_DMA0 | RCC_AHBPeriph_AHB)
#else
#define RCC_AHBPeriph_ALL                 			(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | \
														RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOF | \
														RCC_AHBPeriph_DMA0 | RCC_AHBPeriph_ALU | RCC_AHBPeriph_AHB)
#endif
#define IS_RCC_AHB_PERIPH(SEL)              		(((SEL) & ~RCC_AHBPeriph_ALL) == 0 && (SEL) != 0)

/** @defgroup AHB_RST_peripheral
  * @{
  */
#if defined (PT32G031x) || defined (PTM280x)
#define RCC_AHBPeriphRST_ALL						(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_DMA0 | RCC_AHBPeriph_ALU)
#elif defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define RCC_AHBPeriphRST_ALL  						(0)
#elif defined (PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define RCC_AHBPeriphRST_ALL						(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | \
														RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_DMA0)
#else
#define RCC_AHBPeriphRST_ALL						(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | \
														RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOF | \
														RCC_AHBPeriph_DMA0 | RCC_AHBPeriph_ALU)

#endif
#define IS_RCC_AHBRST_PERIPH(SEL)					(((SEL) & ~RCC_AHBPeriphRST_ALL) == 0 && (SEL) != 0)

/** @defgroup RCC_Reset_State_Flag
  * @{
  */
#define RCC_FLAG_POR       			 		 		RCC_RSR_POR
#define RCC_FLAG_PVD     			  		 		RCC_RSR_PVD
#define RCC_FLAG_PIN      		 			 		RCC_RSR_PIN
#define RCC_FLAG_PLVD      		 			 		RCC_RSR_PLVD
#define RCC_FLAG_SFR       		 			 		RCC_RSR_SFR
#define RCC_FLAG_LOCKUP       		 		 		RCC_RSR_LOCKUP
#define RCC_FLAG_IWDG       				 		RCC_RSR_IWDG
#define RCC_FLAG_WWDG     					 		RCC_RSR_WWDG
#define RCC_FLAG_HSE      					 		RCC_RSR_HSE
#define RCC_FLAG_PLL       		  		     		RCC_RSR_PLL
#define RCC_FLAG_RELOAD      			 	 		RCC_RSR_RELOAD
#define RCC_FLAG_CPU      		 			 		RCC_RSR_CPU

#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define RCC_FLAG_ALL     				     		(RCC_RSR_POR | RCC_RSR_PVD | RCC_RSR_PIN | RCC_RSR_PLVD | RCC_RSR_SFR | \
														RCC_RSR_LOCKUP | RCC_RSR_IWDG | RCC_RSR_RELOAD | RCC_RSR_CPU)
#elif defined (PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define RCC_FLAG_ALL     				     		(RCC_RSR_POR | RCC_RSR_PVD | RCC_RSR_PIN | RCC_RSR_PLVD | RCC_RSR_SFR | \
														RCC_RSR_LOCKUP | RCC_RSR_IWDG | RCC_RSR_HSE | RCC_RSR_RELOAD | RCC_RSR_CPU)
#else
#define RCC_FLAG_ALL     				     		(RCC_RSR_POR | RCC_RSR_PVD | RCC_RSR_PIN | RCC_RSR_PLVD | RCC_RSR_SFR | \
														RCC_RSR_LOCKUP | RCC_RSR_IWDG | RCC_RSR_WWDG | RCC_RSR_HSE | \
														RCC_RSR_PLL | RCC_RSR_RELOAD | RCC_RSR_CPU)
#endif
#define IS_RCC_ResetStatusFlag(SEL)       			(((SEL) & ~RCC_FLAG_ALL) == 0 && (SEL) != 0)

/**
  * @}
  */

/** @defgroup RCC_Reset_selection
  * @{
  */
#define RCC_ResetEnable_PVD                  		RCC_RCR_PVDE
#define RCC_ResetEnable_PLVD              			RCC_RCR_PLVDE
#define RCC_ResetEnable_LOCKUP       	      		RCC_RCR_LOCKUPE
#define RCC_ResetEnable_IWDG       	          		RCC_RCR_IWDGE
#define RCC_ResetEnable_HSE       	          		RCC_RCR_HSEE
#define RCC_ResetEnable_PLL       	          		RCC_RCR_PLLE

#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define RCC_ResetEnable_ALL     					(RCC_ResetEnable_PVD | RCC_ResetEnable_PLVD | RCC_ResetEnable_LOCKUP | RCC_ResetEnable_IWDG)
#elif defined (PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define RCC_ResetEnable_ALL     					(RCC_ResetEnable_PVD | RCC_ResetEnable_PLVD | RCC_ResetEnable_LOCKUP | \
														RCC_ResetEnable_IWDG | RCC_ResetEnable_HSE)
#else
#define RCC_ResetEnable_ALL     					(RCC_ResetEnable_PVD | RCC_ResetEnable_PLVD | RCC_ResetEnable_LOCKUP | \
														RCC_ResetEnable_IWDG | RCC_ResetEnable_HSE | RCC_ResetEnable_PLL)
#endif
#define IS_RCC_ResetEnable(SEL)   		        	(((SEL) & ~RCC_ResetEnable_ALL) == 0 && (SEL) != 0)

/**
  * @}
  */

/** @defgroup RCC_Key_selection
  * @{
  */
#define RCC_AdvancedSoftwareReset_RELOAD 	       	((u8)0x00)
#define RCC_AdvancedSoftwareReset_CPU	           	((u8)0x01)

#define RCC_KEY_RELOAD    							((u32)0x0000CD23)
#define RCC_KEY_CPU    							   	((u32)0x0000C57A)

#define IS_RCC_AdvancedSoftwareReset(SEL)   		((SEL) == RCC_AdvancedSoftwareReset_RELOAD || \
														(SEL) == RCC_AdvancedSoftwareReset_CPU)
/**
  * @}
  */

/** @defgroup RCC_system_clock_switch_source
  * @{
  */
#define RCC_SYSCLK_HSI								(RCC_CFGR_SCW & 0x00) 	//Internal Hight CLK Source
#define RCC_SYSCLK_HSE								(RCC_CFGR_SCW & 0x01) 	//Extern Hight CLK Source
#define RCC_SYSCLK_PLL								(RCC_CFGR_SCW & 0x02)	//Extern Low CLK Source
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PTM280x)
#define IS_RCC_SYSCLK(SEL) 							(0)
#elif defined (PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define IS_RCC_SYSCLK(SEL)							((SEL) == RCC_SYSCLK_HSI || (SEL) == RCC_SYSCLK_HSE)
#else
#define IS_RCC_SYSCLK(SEL) 							(((SEL) & ~RCC_CFGR_SCW) == 0 && (SEL) <= RCC_SYSCLK_PLL)
#endif
/**
  * @}
  */

/** @defgroup RCC_system_clock_after_wakeup_selection
  * @{
  */
#define RCC_SYSCLK_AfterWakeUp_HSI					(RCC_CFGR_SCWK & 0x000)
#define RCC_SYSCLK_AfterWakeUp_PrevClock			(RCC_CFGR_SCWK & 0x100)
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_RCC_SystemClockAfterWakeUp(SEL)   		(0)
#else
#define IS_RCC_SystemClockAfterWakeUp(SEL)   		((SEL) == RCC_SYSCLK_AfterWakeUp_HSI || (SEL) == RCC_SYSCLK_AfterWakeUp_PrevClock)
#endif
/**
  * @}
  */

/** @defgroup RCC_HCLK_prescaler_value
  * @{
  */
#define IS_RCC_HCLKPrescaler(SEL)   	 			((SEL) <= 31)
/**
  * @}
  */

/** @defgroup RCC_PCLK_prescaler_value
  * @{
  */
#define IS_RCC_PCLKPrescaler(PSC)   	 			((PSC) <= 31)
/**
  * @}
  */

/** @defgroup RCC_clock_output_source
  * @{
  */
#define RCC_ClockOutput_HSI			    			(RCC_MCOR_COSRC & 0x00)
#define RCC_ClockOutput_HSE			    			(RCC_MCOR_COSRC & 0x01)
#define RCC_ClockOutput_PLL			    			(RCC_MCOR_COSRC & 0x02)
#define RCC_ClockOutput_LSI			    			(RCC_MCOR_COSRC & 0x03)
#define RCC_ClockOutput_SYSCLK						(RCC_MCOR_COSRC & 0x04)
#define RCC_ClockOutput_LSE			    			(RCC_MCOR_COSRC & 0x05)
#define RCC_ClockOutput_None		    			(RCC_MCOR_COSRC & 0x06)
#if defined (PT32G031x) || defined (PTM280x)
#define IS_RCC_ClockOutputSource(SEL)   			((SEL) == RCC_ClockOutput_HSI || (SEL) == RCC_ClockOutput_LSI || \
											 			(SEL) == RCC_ClockOutput_SYSCLK || (SEL) == RCC_ClockOutput_None)
#elif defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_RCC_ClockOutputSource(SEL)   			((SEL) == RCC_ClockOutput_HSI || (SEL) == RCC_ClockOutput_LSI || \
											 			(SEL) == RCC_ClockOutput_SYSCLK || (SEL) == RCC_ClockOutput_LSE || (SEL) == RCC_ClockOutput_None)
#elif defined (PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define IS_RCC_ClockOutputSource(SEL)				(((SEL) & ~RCC_MCOR_COSRC) == 0 && (SEL) != RCC_ClockOutput_PLL && (SEL) <= RCC_ClockOutput_None)
#else
#define IS_RCC_ClockOutputSource(SEL)				(((SEL) & ~RCC_MCOR_COSRC) == 0 && (SEL) <= RCC_ClockOutput_None)
#endif
/**
  * @}
  */

/** @defgroup RCC_clock_output_Prescale_value
  * @{
  */
#define RCC_ClockOutputPrescale_None      	  		(RCC_MCOR_COPRE & 0x00)
#define RCC_ClockOutputPrescale_2      		  		(RCC_MCOR_COPRE & 0x10)
#define RCC_ClockOutputPrescale_4      		  		(RCC_MCOR_COPRE & 0x20)
#define RCC_ClockOutputPrescale_8      		  		(RCC_MCOR_COPRE & 0x30)
#define RCC_ClockOutputPrescale_16      	  		(RCC_MCOR_COPRE & 0x40)
#define IS_RCC_ClockOutputPrescale(SEL)   			(((SEL) & ~RCC_MCOR_COPRE) == 0 && (SEL) <= RCC_ClockOutputPrescale_16)

/**
  * @}
  */

/** @defgroup RCC_periph1_clock_source
  * @{
  */
#define RCC_Periph1_IWDG							(0x00)
#define RCC_Periph1_TIM4							(0x01)
#define RCC_Periph1_RTC								(0x02)
#define RCC_Periph1_LCD								(0x03)
#define RCC_Periph1_BEEP							(0x04)
#define RCC_Periph1_LPTIM							(0x05)
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_RCC_Periph1(SEL) 						((SEL) <= RCC_Periph1_TIM4)
#elif  defined (PT32G031x) || defined (PTM280x)
#define IS_RCC_Periph1(SEL) 						(0)
#else
#define IS_RCC_Periph1(SEL) 						((SEL) <= RCC_Periph1_LPTIM)
#endif

#define RCC_IWDG_CLKSRC_LSI              			(RCC_PCSCR1_IWDG & 0x0)
#define RCC_IWDG_CLKSRC_LSE              			(RCC_PCSCR1_IWDG & 0x1)
#if defined (PT32G031x) || defined (PTM280x)
#define IS_RCC_IWDG_CLKSRC(SEL)              		(0)
#else
#define IS_RCC_IWDG_CLKSRC(SEL)              		((SEL) == RCC_IWDG_CLKSRC_LSI || (SEL) == RCC_IWDG_CLKSRC_LSE)
#endif

#define RCC_TIM4_CLKSRC_LSI              			(RCC_PCSCR1_TIM4 & 0x0)
#define RCC_TIM4_CLKSRC_LSE              			(RCC_PCSCR1_TIM4 & 0x4)
#if defined (PT32G031x) || defined (PTM280x)
#define IS_RCC_TIM4_CLKSRC(SEL)              		(0)
#else
#define IS_RCC_TIM4_CLKSRC(SEL)              		((SEL) == RCC_TIM4_CLKSRC_LSI || (SEL) == RCC_TIM4_CLKSRC_LSE)
#endif

#define RCC_RTC_CLKSRC_LSI              			(RCC_PCSCR1_RTC & 0x00)
#define RCC_RTC_CLKSRC_LSE              			(RCC_PCSCR1_RTC & 0x10)
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_RCC_RTC_CLKSRC(SEL)              		(0)
#else
#define IS_RCC_RTC_CLKSRC(SEL)              		((SEL) == RCC_RTC_CLKSRC_LSI || (SEL) == RCC_RTC_CLKSRC_LSE)
#endif

#define RCC_BEEP_CLKSRC_LSI              			(RCC_PCSCR1_BEEP & 0x000)
#define RCC_BEEP_CLKSRC_LSE              			(RCC_PCSCR1_BEEP & 0x100)
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_RCC_BEEP_CLKSRC(SEL)              		(0)
#else
#define IS_RCC_BEEP_CLKSRC(SEL)              		((SEL) == RCC_BEEP_CLKSRC_LSI || (SEL) == RCC_BEEP_CLKSRC_LSE)
#endif

#define RCC_LCD_CLKSRC_LSI              			(RCC_PCSCR1_LCD & 0x000)
#define RCC_LCD_CLKSRC_LSE              			(RCC_PCSCR1_LCD & 0x400)
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_RCC_LCD_CLKSRC(SEL)              		(0)
#else
#define IS_RCC_LCD_CLKSRC(SEL)              		((SEL) == RCC_LCD_CLKSRC_LSI || (SEL) == RCC_LCD_CLKSRC_LSE)
#endif

#define RCC_LPTIM_CLKSRC_PCLK              			(RCC_PCSCR1_LPTIM & 0x00000)
#define RCC_LPTIM_CLKSRC_HSI              			(RCC_PCSCR1_LPTIM & 0x10000)
#define RCC_LPTIM_CLKSRC_SYSCLK             		(RCC_PCSCR1_LPTIM & 0x20000)
#define RCC_LPTIM_CLKSRC_LSI              			(RCC_PCSCR1_LPTIM & 0x30000)
#define RCC_LPTIM_CLKSRC_LSE              			(RCC_PCSCR1_LPTIM & 0x40000)
#define RCC_LPTIM_CLKSRC_LPTIM_IC1              	(RCC_PCSCR1_LPTIM & 0x50000)
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_RCC_LPTIM_CLKSRC(SEL)   		        	(0)
#elif defined (PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define IS_RCC_LPTIM_CLKSRC(SEL)   		        	(((SEL) & ~RCC_PCSCR1_LPTIM) == 0 && (SEL) >= RCC_LPTIM_CLKSRC_SYSCLK && \
														 (SEL) <= RCC_LPTIM_CLKSRC_LPTIM_IC1)
#else
#define IS_RCC_LPTIM_CLKSRC(SEL)   		        	(((SEL) & ~RCC_PCSCR1_LPTIM) == 0 && (SEL) <= RCC_LPTIM_CLKSRC_LPTIM_IC1)
#endif
/**
  * @}
  */

/** @defgroup RCC_periph2_clock_source
  * @{
  */
#define RCC_Periph2_LPUART0							(0x00)
#define RCC_Periph2_LPUART1							(0x01)
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_RCC_Periph2(SEL) 						((SEL) == RCC_Periph2_LPUART0)
#elif defined (PT32G031x) || defined (PTM280x)
#define IS_RCC_Periph2(SEL) 						(0)
#else
#define IS_RCC_Periph2(SEL) 						((SEL) <= RCC_Periph2_LPUART1)
#endif

#define RCC_LPUART0_CLKSRC_PCLK              		(RCC_PCSCR2_LPUART0 & 0x0)
#define RCC_LPUART0_CLKSRC_HSI              		(RCC_PCSCR2_LPUART0 & 0x1)
#define RCC_LPUART0_CLKSRC_LSI              		(RCC_PCSCR2_LPUART0 & 0x2)
#define RCC_LPUART0_CLKSRC_LSE              		(RCC_PCSCR2_LPUART0 & 0x3)
#define RCC_LPUART0_CLKSRC_HCLK             		(RCC_PCSCR2_LPUART0 & 0x4)
#if defined (PT32x002x) || defined (PT32x012x) || defined (PT32x060x) || defined (PT32x063x) || defined (PT32x066x) || defined (PTM160x)
#define IS_RCC_LPUART0_CLKSRC(SEL)   		        ((SEL) <= RCC_LPUART0_CLKSRC_LSE)
#elif defined (PT32G031x) || defined (PTM280x)
#define IS_RCC_LPUART0_CLKSRC(SEL)   		        (0)
#else
#define IS_RCC_LPUART0_CLKSRC(SEL)   		        ((SEL) <= RCC_LPUART0_CLKSRC_HCLK)
#endif

#define RCC_LPUART1_CLKSRC_PCLK              		(RCC_PCSCR2_LPUART1 & 0x00)
#define RCC_LPUART1_CLKSRC_HSI              		(RCC_PCSCR2_LPUART1 & 0x10)
#define RCC_LPUART1_CLKSRC_LSI              		(RCC_PCSCR2_LPUART1 & 0x20)
#define RCC_LPUART1_CLKSRC_LSE              		(RCC_PCSCR2_LPUART1 & 0x30)
#define RCC_LPUART1_CLKSRC_HCLK             		(RCC_PCSCR2_LPUART1 & 0x40)
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_RCC_LPUART1_CLKSRC(SEL)   		        (0)
#elif defined (PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define IS_RCC_LPUART1_CLKSRC(SEL)   		        (((SEL) & ~RCC_PCSCR2_LPUART1) == 0 && (SEL) <= RCC_LPUART1_CLKSRC_LSE)
#else
#define IS_RCC_LPUART1_CLKSRC(SEL)   		        (((SEL) & ~RCC_PCSCR2_LPUART1) == 0 && (SEL) <= RCC_LPUART1_CLKSRC_HCLK)
#endif
/**
  * @}
  */

/** @defgroup RCC_clock_source
  * @{
  */
#define RCC_ClockSource_HSI							(0x00) 	//Internal Hight CLK Source
#define RCC_ClockSource_LSI							(0x01) 	//Internal Low CLK Source
#define RCC_ClockSource_HSE 						(0x02) 	//Crystal Hight CLK  Source
#define RCC_ClockSource_HSEExternal					(0x03) 	//Extern Hight CLK Source
#define RCC_ClockSource_PLL							(0x04)	//PLL CLK Source
#define RCC_ClockSource_LSE							(0x05) 	//Crystal Low CLK Source
#define RCC_ClockSource_LSEExternal					(0x06) 	//Extern Low CLK Source

#if defined (PT32G031x) || defined (PTM280x)
#define IS_RCC_ClockSource(SEL) 					((SEL) == RCC_ClockSource_LSI)
#elif defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_RCC_ClockSource(SEL) 					((SEL) == RCC_ClockSource_LSI || (SEL) == RCC_ClockSource_LSE || (SEL) == RCC_ClockSource_LSEExternal)
#elif defined (PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define IS_RCC_ClockSource(SEL) 					((SEL) != RCC_ClockSource_PLL && (SEL) <= RCC_ClockSource_LSEExternal)
#else
#define IS_RCC_ClockSource(SEL) 					((SEL) <= RCC_ClockSource_LSEExternal)
#endif
/**
  * @}
  */

/** @defgroup RCC_LSI_Frequency
  * @{
  */
#define RCC_LSIFreq_38400Hz							(RCC_LSICR_FREQS & 0x0)
#define RCC_LSIFreq_32768Hz							(RCC_LSICR_FREQS & 0x2)
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_RCC_LSIFreq(SEL) 						(0)
#else
#define IS_RCC_LSIFreq(SEL) 						((SEL) == RCC_LSIFreq_38400Hz || (SEL) == RCC_LSIFreq_32768Hz)
#endif
/**
  * @}
  */
  
/** @defgroup RCC_HSE_frequency_range
  * @{
  */
#define RCC_HSEFreqRange_4_10MHz					(RCC_HSECR_FREQ & 0x0)
#define RCC_HSEFreqRange_10_25MHz					(RCC_HSECR_FREQ & 0x8)
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_RCC_HSEFreqRange(SEL) 					(0)
#else
#define IS_RCC_HSEFreqRange(SEL) 					((SEL) == RCC_HSEFreqRange_4_10MHz || (SEL) == RCC_HSEFreqRange_10_25MHz)
#endif
/**
  * @}
  */

/** @defgroup RCC_ClockFilterSource
  * @{
  */
#define RCC_CLKF_LPFB                  				((u32)0x00000100)
 
#define RCC_CLKF_HSE								(0x00)
#define RCC_CLKF_LSE								(0x01)
#define RCC_CLKF_PLL								(0x02)
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_RCC_ClockFilterSource(SEL) 				((SEL) == RCC_CLKF_LSE)
#elif defined (PT32G031x) || defined (PTM280x)
#define IS_RCC_ClockFilterSource(SEL)				(0)
#elif defined (PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define IS_RCC_ClockFilterSource(SEL) 				((SEL) <= RCC_CLKF_LSE)
#else
#define IS_RCC_ClockFilterSource(SEL) 				((SEL) <= RCC_CLKF_PLL)
#endif
/**
  * @}
  */

/** @defgroup RCC_HSE_low_pass_filter
  * @{
  */
#define RCC_HSE_Filter_4ns							(RCC_HSECR_LPF & 0x0000)
#define RCC_HSE_Filter_6ns							(RCC_HSECR_LPF & 0x1000)
#define RCC_HSE_Filter_8ns							(RCC_HSECR_LPF & 0x2000)
#define RCC_HSE_Filter_10ns							(RCC_HSECR_LPF & 0x3000)
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_RCC_HSEFilterCfg(SEL)   		        	(0)
#else
#define IS_RCC_HSEFilterCfg(SEL)   		        	(((SEL) & ~RCC_HSECR_LPF) == 0 && (SEL) <= RCC_HSE_Filter_10ns)
#endif

/** @defgroup RCC_LSE_low_pass_filter
  * @{
  */
#define RCC_LSE_Filter_9ns							(RCC_LSECR_LPF & 0x0000)
#define RCC_LSE_Filter_11ns							(RCC_LSECR_LPF & 0x1000)
#define RCC_LSE_Filter_15ns							(RCC_LSECR_LPF & 0x2000)
#define RCC_LSE_Filter_20ns							(RCC_LSECR_LPF & 0x3000)
#if defined (PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
#define IS_RCC_LSEFilterCfg(SEL)   		        	((SEL) == RCC_LSE_Filter_15ns || (SEL) == RCC_LSE_Filter_20ns)
#elif defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_RCC_LSEFilterCfg(SEL)   		        	(0)
#else
#define IS_RCC_LSEFilterCfg(SEL)   		        	(((SEL) & ~RCC_LSECR_LPF) == 0 && (SEL) <= RCC_LSE_Filter_20ns)
#endif

/** @defgroup RCC_PLL_low_pass_filter
  * @{
  */
#define RCC_PLL_Filter_2ns							(RCC_PLLCFGR1_LPF & 0x0000)
#define RCC_PLL_Filter_4ns							(RCC_PLLCFGR1_LPF & 0x1000)
#define RCC_PLL_Filter_6ns							(RCC_PLLCFGR1_LPF & 0x2000)
#define RCC_PLL_Filter_8ns							(RCC_PLLCFGR1_LPF & 0x3000)
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PT32x060x) || defined (PT32x063x) || defined (PT32x066x) || defined (PTM160x) || defined (PTM280x)
#define IS_RCC_PLLFilterCfg(SEL)   		        	(0)
#else
#define IS_RCC_PLLFilterCfg(SEL)   		        	(((SEL) & ~RCC_PLLCFGR1_LPF) == 0 && (SEL) <= RCC_PLL_Filter_8ns)
#endif

/**
  * @}
  */

/** @defgroup RCC_HSE_feedback_resistance
  * @{
  */
#define RCC_HSE_FDR_1100k							(RCC_HSECR_FDR & 0x000000)
#define RCC_HSE_FDR_800k							(RCC_HSECR_FDR & 0x100000)
#define RCC_HSE_FDR_500k							(RCC_HSECR_FDR & 0x200000)
#define RCC_HSE_FDR_200k							(RCC_HSECR_FDR & 0x300000)
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_RCC_HSE_FDR(SEL)   		        		(0)
#else
#define IS_RCC_HSE_FDR(SEL)   		        		(((SEL) & ~RCC_HSECR_FDR) == 0 && (SEL) <= RCC_HSE_FDR_200k)
#endif
/**
  * @}
  */

/** @defgroup RCC_LSE_feedback_resistance
  * @{
  */
#define RCC_LSE_FDR_8M								(RCC_LSECR_FDR & 0x000000)
#define RCC_LSE_FDR_6M								(RCC_LSECR_FDR & 0x100000)
#define RCC_LSE_FDR_5M								(RCC_LSECR_FDR & 0x200000)
#define RCC_LSE_FDR_3M								(RCC_LSECR_FDR & 0x300000)
#if defined (PT32G031x) || defined (PTM280x)
#define IS_RCC_LSE_FDR(SEL)   		        		(0)
#else
#define IS_RCC_LSE_FDR(SEL)   		        		(((SEL) & ~RCC_LSECR_FDR) == 0 && (SEL) <= RCC_LSE_FDR_3M)
#endif
/**
  * @}
  */

/** @defgroup RCC_PLL_clock_source
  * @{
  */
#define RCC_PLLClockSource_HSI						(RCC_PLLCFGR1_SRC & 0x00)
#define RCC_PLLClockSource_HSE						(RCC_PLLCFGR1_SRC & 0x10)
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x)|| defined (PT32x060x) || defined (PT32x063x) || defined (PT32x066x) || defined (PTM160x) || defined (PTM280x)
#define IS_RCC_PLLClockSource(SEL)					(0)
#else
#define IS_RCC_PLLClockSource(SEL)   				((SEL) == RCC_PLLClockSource_HSI || (SEL) == RCC_PLLClockSource_HSE)
#endif
/**
  * @}
  */

/** @defgroup RCC_PLLSRC_Delay
  * @{
  */
#define RCC_PLLSRC_DELAY_256						(RCC_PLLCFGR1_DELAY & 0x00)
#define RCC_PLLSRC_DELAY_256						(RCC_PLLCFGR1_DELAY & 0x00)
#define RCC_PLLSRC_DELAY_512						(RCC_PLLCFGR1_DELAY & 0x02)
#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x)|| defined (PT32x060x) || defined (PT32x063x) || defined (PT32x066x) || defined (PTM160x) || defined (PTM280x)
#define IS_RCC_PLLSRC_DELAY(SEL)   					(0)
#else
#define IS_RCC_PLLSRC_DELAY(SEL)   					((SEL) == RCC_PLLSRC_DELAY_256 || (SEL) == RCC_PLLSRC_DELAY_512)
#endif


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/
void RCC_APBPeriph1ResetCmd(u32 RCC_APBPeriph, FunctionalState NewState);
void RCC_APBPeriph2ResetCmd(u32 RCC_APBPeriph, FunctionalState NewState);
void RCC_APBPeriph3ResetCmd(u32 RCC_APBPeriph, FunctionalState NewState);
void RCC_APBPeriph4ResetCmd(u32 RCC_APBPeriph, FunctionalState NewState);
void RCC_AHBPeriphResetCmd(u32 RCC_AHBPeriph, FunctionalState NewState);
FlagStatus RCC_GetResetFlagStatus(u32 RCC_FLAG);
void RCC_ClearResetFlag(u32 RCC_FLAG);
void RCC_ResetConfig(u32 ResetSource, FunctionalState NewState);
void RCC_AdvancedSoftwareReset(u8 RCC_AdvancedSoftwareResetSource);
void RCC_APBPeriph1ClockCmd(u32 RCC_APBPeriph, FunctionalState NewState);
void RCC_APBPeriph2ClockCmd(u32 RCC_APBPeriph, FunctionalState NewState);
void RCC_APBPeriph3ClockCmd(u32 RCC_APBPeriph, FunctionalState NewState);
void RCC_APBPeriph4ClockCmd(u32 RCC_APBPeriph, FunctionalState NewState);
void RCC_AHBPeriphClockCmd(u32 RCC_AHBPeriph, FunctionalState NewState);
void RCC_SetSystemClock(u32 SystemClock);
u8 RCC_GetSystemClockSelection(void);
void RCC_SetSystemClockAfterWakeUp(u32 Clock);
void RCC_HCLKSetPrescaler(u32 Prescaler);
void RCC_PCLKSetPrescaler(u32 Prescaler);
void RCC_SetMCOSource(u32 Source);
void RCC_MCOSetPrescaler(u32 PrescaleSelcection);
void RCC_Periph1ClockSourceConfig(u32 Periph, u32 Source);
void RCC_Periph2ClockSourceConfig(u32 Periph, u32 Source);
void RCC_ClockSourceConfig(u32 ClockSource, FunctionalState NewState);
void RCC_LSIFreqConfig(u32 freq);
void RCC_HSEAmplitudeConfig(FunctionalState NewState);
void RCC_HSEFreqRangeConfig(u32 Range);
void RCC_HSEFailDetectConfig(FunctionalState NewState);
void RCC_ClockFilterBypassConfig(u32 ClockSource, FunctionalState NewState);
void RCC_HSEFilterConfig(u32 FilterCfg);
void RCC_LSEFilterConfig(u32 FilterCfg);
void RCC_PLLFilterConfig(u32 FilterCfg);
void RCC_HSEFeedbackResistanceConfig(u32 FDR);
void RCC_LSEFeedbackResistanceConfig(u32 FDR);
void RCC_PLLClockSourceConfig(u32 PLLClockSource);
void RCC_PLLClockSourceDelayConfig(u32 Delay);
/**
  * @}
  */
#ifdef __cplusplus
}
#endif
#endif /*__PT32x0xx_RCC_H */

