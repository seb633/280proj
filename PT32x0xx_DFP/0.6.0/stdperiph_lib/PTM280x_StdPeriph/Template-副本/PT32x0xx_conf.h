/******************************************************************************
  * @file   PT32x0xx_conf.h
  * @author  Application development team
  * @version V1.0.0
  * @date    2023/11/07
  * @brief    
  *          
  ******************************************************************************
  * @attention
  *
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers. 
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of the content or the  codes within the content.
  *
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  * 
  *****************************************************************************/

  

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PT32x0xx_CONF_H
#define PT32x0xx_CONF_H



#define RETARGET_UART0      			1

/* RETARGET definition to use UART function by C Library */

#define RETARGET_PORT        			RETARGET_UART0

#if (RETARGET_PORT == RETARGET_UART0)
	#define RETARGET_UART_PORT       	UART0
	#define APBPeriph2_UARTx       		RCC_APBPeriph2_UART0
	#define RETARGET_UART_IRQn   		UART0_IRQn
	#define RETARGET_UART_Handler		UART0_Handler
#else

#endif

/******************************UART0***********************************/
#define UART_Periph_GPIOx     			(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB)

#define UART_TX_PORT					AFIOA
#define UART_TX_PIN						GPIO_Pin_8

#define UART_RX_PORT					AFIOB
#define UART_RX_PIN						GPIO_Pin_2

#define UART_TX_AFIO					AFIO_AF_5
#define UART_RX_AFIO					AFIO_AF_5

#define RETARGET_UART_SampleRate		UART_SampleRate_8X

/* Includes ------------------------------------------------------------------*/
/* Comment the line below to disable peripheral header file inclusion */
#include "PT32x0xx_adc.h"
#include "PT32x0xx_cmp.h"
#include "PT32x0xx_crc.h"
#include "PT32x0xx_dma.h"
#include "PT32x0xx_es.h"
#include "PT32x0xx_exti.h"
#include "PT32x0xx_gpio.h"
#include "PT32x0xx_i2c.h"
#include "PT32x0xx_ifmc.h"
#include "PT32x0xx_iwdg.h"
#include "PT32x0xx_ldac.h"
#include "PT32x0xx_nvic.h"
#include "PT32x0xx_opa.h"
#include "PT32x0xx_pwr.h"
#include "PT32x0xx_rcc.h"
#include "PT32x0xx_spi.h"
#include "PT32x0xx_syscfg.h"
#include "PT32x0xx_tim.h"
#include "PT32x0xx_uart.h"









#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports 
  *         the name of the source file and the source line number of the call 
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, u32 line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif 


