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

#ifndef PT32x0xx_CONFIG_H
#define PT32x0xx_CONFIG_H

//<<< Use Configuration Wizard in Context Menu >>>
/*--------------------- Clock Configuration ----------------------------------
//
//    <h> 系统时钟配置(SYS_CLK)
//      <o0>    系统时钟源
//                <0=> HSI
//                <i> 默认系统时钟源 = HSI
//      <o1>  HCLK分频		
//                <i> 默认值为1(分频 = 1)
//                <i> 写入值范围：1-32(对应分频1-32)
//      <o2>  PCLK分频
//                <i> 默认值为1(分频 = 1)
//                <i> 写入值范围：1-32(对应分频1-32)
//    </h>
//
//    <h> IFMC 配置
//      <o3>   Flash 操作等待周期
//                <0=> 0 WS
//                <1=> 1 WS
//                <2=> 2 WS
//
//                <i> 0 WS:  1 KHz <= HCLK <= 24 MHz
//                <i> 1 WS: 24 MHz <  HCLK <= 48 MHz
//                <i> 2 WS: 48 MHz <  HCLK <= 64 MHz
//
//    </h>
//
//    <h>  MCO输出配置
//      <e4>    MCO输出使能
//                <i> 默认 = DISABLE
//        <o5>    MCO时钟源
//                <0=> HSI
//                <1=> LSI
//                <2=> SYS_CLK
//        <o6>    MCO输出引脚
//                <0=> PA5
//                <0=> PA14
//                <0=> PB10
//        <o7>   MCO输出分频
//                <0=> 1
//                <1=> 2
//                <2=> 4
//                <3=> 8
//                <4=> 16
//      </e>
//    </h>
*/

/** @defgroup CLK_Private_Defines
  * @{
  */
#define HCLK_SRC				(0)     /*!< 0: HSI, 								     								*/
#define HCLK_DIV				(1)     /*!< 1-32: DIV1-32       														*/
#define PCLK_DIV				(1)     /*!< 1-32: DIV1-32       														*/

#define WAIT_STATE				(2)   	/*!< 0: 0 WS,     1: 1 WS,    2: 2 WS,                                     		*/
#define MCO_ENABLE				(0)   	/*!< 0: DISABLE,  1: ENABLE                                               		*/
#define MCO_SRC					(2)     /*!< 0: HSI,      1:LSI,      2:SYS_CLK,  										*/
#define MCO_PINSEL				(0)   	/*!< 0: PA5,	  1:PA14,	  2:PB10      										*/
#define MCO_DIV					(2)     /*!< 0: DIV1,     1: DIV2,    2: DIV4,    3: DIV8,        4: DIV16				*/
/**
  * @}
  */
//<<< end of configuration section >>>

#define HSI_VALUE     			64000000UL
#define LSI_VALUE    			32768UL

/******************************RGB LED***********************************/
#define		LED_RED_GPIO_CLOCK			RCC_AHBPeriph_GPIOA
#define		LED_RED_PORT				GPIOA
#define		LED_RED_PIN		  			GPIO_Pin_2

#define		LED_GREEN_GPIO_CLOCK		RCC_AHBPeriph_GPIOA
#define		LED_GREEN_PORT				GPIOA
#define		LED_GREEN_PIN		  		GPIO_Pin_1

#define		LED_BLUE_GPIO_CLOCK			RCC_AHBPeriph_GPIOA
#define		LED_BLUE_PORT				GPIOA
#define		LED_BLUE_PIN		  		GPIO_Pin_0

/******************************USER_KEY定义***********************************/
#define		USER_KEY_GPIO_CLOCK			RCC_AHBPeriph_GPIOB
#define		USER_KEY_PORT				GPIOB
#define		USER_KEY_EXTI				EXTIB
#define		USER_KEY_PIN		  		GPIO_Pin_1
#define		USER_KEY_IRQn  				EXTIB_IRQn
#define		USER_KEY_Handler         	EXTIB_Handler

/******************************TimerLed***********************************/
#define 	TimerLed					TIM3
#define		TimerLed_Periph_CLOCK   	RCC_APBPeriph1_TIM3
#define 	TimerLed_IRQn				TIM3_IRQn
#define     TimerLed_Handler			TIM3_Handler

/******************************BreathLed***********************************/
#define		BreathLed_GPIO_CLOCK		RCC_AHBPeriph_GPIOA
#define		BreathLed_AFIO				AFIOA
#define		BreathLed_PIN		  		GPIO_Pin_0
#define		BreathLed_AFx				AFIO_AF_1

#define 	BreathTimer					TIM1
#define		BreathTimer_Periph_CLOCK   	RCC_APBPeriph1_TIM1
#define		BreathTimer_CHx    			TIM_Channel_1

/******************************CaptureTIM定义***********************************/
#define 	CaptureTIM					TIM8
#define		CaptureTIM_Periph_CLOCK   	RCC_APBPeriph1_TIM8
#define 	CaptureTIM_IRQn				TIM8_IRQn
#define     CaptureTIM_Handler			TIM8_Handler

#define 	CaptureTIM_CH_CLOCK			RCC_AHBPeriph_GPIOB
#define 	CaptureTIM_CH_AFIO			AFIOB
#define 	CaptureTIM_CH_PIN			GPIO_Pin_8
#define 	CaptureTIM_CH_AFx			AFIO_AF_3

/******************************BreakTIM***********************************/
#define 	BreakTIM					TIM1
#define		BreakTIM_Periph_CLOCK   	RCC_APBPeriph1_TIM1

#define 	BreakTIM_CH_CLOCK			RCC_AHBPeriph_GPIOB
#define 	BreakTIM_CH_AFIO			AFIOB
#define 	BreakTIM_CH_PIN				GPIO_Pin_11
#define 	BreakTIM_CH_AFx				AFIO_AF_3

#define 	BreakTIM_BKIN_CLOCK			RCC_AHBPeriph_GPIOA
#define 	BreakTIM_BKIN_AFIO			AFIOA
#define 	BreakTIM_BKIN_PIN			GPIO_Pin_11
#define 	BreakTIM_BKIN_AFx			AFIO_AF_3
#define 	BreakTIM_BKIN_GPIO			GPIOA

/******************************ADC定义***********************************/
#define     ADCn  						ADC0
#define		ADCn_Periph_CLOCK   		RCC_APBPeriph3_ADC0
#define		ADCn_PIN_CLOCK				RCC_AHBPeriph_GPIOA
#define    	ADCn_PIN_AFIO				AFIOA	
#define     ADCn_PIN         			GPIO_Pin_6
#define     ADCn_Channelx				ADC_Channel_6
#define		ADCn_IRQn  					ADC0_IRQn
#define		ADCn_Handler         		ADC0_Handler

/******************************CMP定义***********************************/
#define     CMPn  						CMP0
#define		CMPn_Periph_CLOCK 			RCC_APBPeriph3_CMP0
#define		CMPn_IRQn  					CMP0_IRQn
#define		CMPn_Handler         		CMP0_Handler
#define  	CMPn_PositiveInput			CMP_PositiveInput_CMPxP1

#define  	CMPn_Px_CLOCK				RCC_AHBPeriph_GPIOB
#define  	CMPn_Px_AFIO				AFIOB
#define  	CMPn_Px_PIN					GPIO_Pin_12

#define  	CMPn_OUT_CLOCK				RCC_AHBPeriph_GPIOA
#define  	CMPn_OUT_AFIO				AFIOA
#define  	CMPn_OUT_PIN				GPIO_Pin_9
#define  	CMPn_OUT_AFx				AFIO_AF_0

/******************************SPI定义***********************************/
#define     SPIn  						SPI0
#define		SPIn_Periph_CLOCK 			RCC_APBPeriph2_SPI0

#define		SPI_CS_CLOCK				RCC_AHBPeriph_GPIOB
#define		SPI_CS_AFIO					AFIOB
#define		SPI_CS_PIN		  			GPIO_Pin_9
#define  	SPI_CS_AFx					AFIO_AF_5

#define		SPI_SCK_CLOCK				RCC_AHBPeriph_GPIOB
#define		SPI_SCK_AFIO				AFIOB
#define		SPI_SCK_PIN		  			GPIO_Pin_8
#define  	SPI_SCK_AFx					AFIO_AF_5

#define		SPI_MOSI_CLOCK				RCC_AHBPeriph_GPIOB
#define		SPI_MOSI_AFIO				AFIOB
#define		SPI_MOSI_PIN		  		GPIO_Pin_5
#define  	SPI_MOSI_AFx				AFIO_AF_5

#define		SPI_MISO_CLOCK				RCC_AHBPeriph_GPIOB
#define		SPI_MISO_AFIO				AFIOB
#define		SPI_MISO_PIN		  		GPIO_Pin_6
#define  	SPI_MISO_AFx				AFIO_AF_5

/******************************I2C管脚定义***********************************/
#define     I2Cn  						I2C0
#define		I2Cn_Periph_CLOCK 			RCC_APBPeriph2_I2C0

#define 	I2C_SDA_CLOCK				RCC_AHBPeriph_GPIOA
#define 	I2C_SDA_AFIO				AFIOA
#define 	I2C_SDA_PIN					GPIO_Pin_10

#define 	I2C_SCL_CLOCK				RCC_AHBPeriph_GPIOA
#define 	I2C_SCL_AFIO				AFIOA
#define 	I2C_SCL_PIN					GPIO_Pin_11

#define 	I2C_SDA_AFx					AFIO_AF_6
#define 	I2C_SCL_AFx					AFIO_AF_6

/******************************IFMC定义***********************************/
#define    	IFMC_Address				0x00001800UL

/******************************DMA定义***********************************/
#define		DMAn						DMA0
#define		DMAn_Periph_CLOCK 			RCC_AHBPeriph_DMA0
#define		DMAn_CHNUM					DMA_CHNUM_0
#define 	DMAn_Channeln				DMA0_CH0
#define 	DMA_CH_SPIn_RX				DMA_CH_SPI0_RX

/******************************OPA定义***********************************/
#define   	OPAn						OPA0

#define   	OPAn_Px_CLOCK				RCC_AHBPeriph_GPIOA
#define   	OPAn_Px_AFIO				AFIOA
#define   	OPAn_Px_PIN					GPIO_Pin_3

#define   	OPAn_Nx_CLOCK				RCC_AHBPeriph_GPIOA
#define   	OPAn_Nx_AFIO				AFIOA
#define   	OPAn_Nx_PIN					GPIO_Pin_4

#define   	OPAn_OUT_CLOCK				RCC_AHBPeriph_GPIOA
#define   	OPAn_OUT_AFIO				AFIOA
#define   	OPAn_OUT_PIN				GPIO_Pin_5
















#endif 
