/*
  *******************************************************************************
  * @file    PT32x0xx.h
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for interrupt number,MCU registers and bits
  ******************************************************************************
  * @attention
  *
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of the content or the  codes within the content.
  *
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  *
  *******************************************************************************
  */
#ifndef __PT32x0xx_H
#define __PT32x0xx_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined (__CC_ARM)
#pragma anon_unions
#endif

#define __CM0_REV				0 				/*!< Core Revision r0p0	*/
#define __MPU_PRESENT			0 				/*!< PT32x0xx do not provide MPU	*/
#define __NVIC_PRIO_BITS		2 				/*!< PT32x0xx uses 2 Bits for the Priority Levels  */
#define __Vendor_SysTickConfig	0 				/*!< Set to 1 if different SysTick Config is used  */

#if defined (PT32L060x8x6) || defined (PT32L060xBx6)
	#define PT32x060x
#elif defined (PT32L063x8x7) || defined (PT32L063xBx7)
	#define PT32x063x
#elif defined (PT32L066x8x6) || defined (PT32L066xBx6)
	#define PT32x066x
#elif defined (PT32x002x6x7)
	#define PT32x002x
#elif defined (PT32G031x4x7)
	#define PT32G031x
#elif defined (PT32x012x6x7)
	#define PT32x012x
#elif defined (PTM160x6x7)
	#define PTM160x
#elif defined (PTM280x6x7)
	#define PTM280x
#else
	#error "Please select first the target PT32x0xx device used in your application (in PT32x0xx.h file)"
#endif

#if defined (PT32L060x8x6) || defined (PT32L063x8x7) || defined (PT32L066x8x6)
	#define FLASH_SIZE_64K
	#define SRAM_SIZE_4K
#elif defined (PT32L060xBx6) ||	defined (PT32L063xBx7) || defined (PT32L066xBx6)
	#define FLASH_SIZE_120K
	#define SRAM_SIZE_8K
#elif defined (PT32G031x4x7)
	#define FLASH_SIZE_16K
	#define SRAM_SIZE_4K
#elif defined (PT32G031x6x7)
	#define FLASH_SIZE_32K
	#define SRAM_SIZE_4K
#elif defined (PT32x002x6x7) || defined (PT32x012x6x7) || defined (PTM160x6x7)
	#define FLASH_SIZE_32K
	#define SRAM_SIZE_3K
#elif defined (PTM280x6x7)
	#define FLASH_SIZE_32K
	#define SRAM_SIZE_8K
#else
  #error "Please select first the target PT32x0xx device used in your application (in PT32x0xx.h file)"
#endif

/** @addtogroup Configuration_section_for_CMSIS
  * @{
  */

/**
 * @brief PT32x0xx Interrupt Number Definition
 */
#if defined (PT32G031x) || defined (PTM280x)
typedef enum IRQn
{
	/******  Cortex-M0 Processor Exceptions Numbers ***************************************************/
	NMI_IRQn        = -14,			/*!< 2 Non Maskable Interrupt*/
	HardFault_IRQn	= -13,			/*!< 3 Cortex-M0 Hard Fault Interrupt*/
	SVCall_IRQn		= -5,			/*!< 11 Cortex-M0 SV Call Interrupt	*/
	PendSV_IRQn	    = -2,			/*!< 14 Cortex-M0 Pend SV Interrupt	*/
	SysTick_IRQn	= -1,			/*!< 15 Cortex-M0 System Tick Interrupt*/

	/****** MCU Specific Interrupt Numbers *******************************************************/
	PVD_IRQn 	  	=  0 ,  		/* PVD         Interrupt  */
	IFMC_IRQn     	=  3 ,  		/* IFMC        Interrupt  */
	DMA_IRQn      	=  4 ,  		/* DMA         Interrupt  */
	EXTIA_IRQn    	=  5 ,  		/* EXTIA       Interrupt  */
	EXTIB_IRQn    	=  6 , 			/* EXTIB       Interrupt  */
	CMP1_IRQn     	=  9 ,			/* CMP1        Interrupt  */
	ADC0_IRQn     	=  12,  		/* ADC         Interrupt  */
	TIM1_IRQn     	=  13,  		/* TIM1        Interrupt  */
	TIM4_IRQn     	=  15,  		/* TIM4        Interrupt  */
	TIM3_IRQn     	=  16,  		/* TIM3        Interrupt  */
	TIM2_IRQn     	=  17, 	  		/* TIM2        Interrupt  */
	ALU_IRQn      	=  18, 			/* ALU         Interrupt  */
	CMP0_IRQn     	=  21,  		/* CMP0        Interrupt  */
	TIM8_IRQn     	=  22,  		/* TIM8        Interrupt  */
	I2C0_IRQn     	=  23,  		/* I2C0        Interrupt  */
	SPI0_IRQn    	=  25,  		/* SPI0        Interrupt  */
	UART0_IRQn      =  27			/* UART0       Interrupt  */
} IRQn_Type;
#elif defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
typedef enum IRQn
{
	/******  Cortex-M0 Processor Exceptions Numbers ***************************************************/
	NMI_IRQn        = -14,			/*!< 2 Non Maskable Interrupt*/
	HardFault_IRQn	= -13,			/*!< 3 Cortex-M0 Hard Fault Interrupt*/
	SVCall_IRQn		= -5,			/*!< 11 Cortex-M0 SV Call Interrupt	*/
	PendSV_IRQn	    = -2,			/*!< 14 Cortex-M0 Pend SV Interrupt	*/
	SysTick_IRQn	= -1,			/*!< 15 Cortex-M0 System Tick Interrupt*/

	/****** MCU Specific Interrupt Numbers *******************************************************/
	PVD_IRQn 	  	=  0 ,  		/* PVD         Interrupt  */
	IFMC_IRQn     	=  2 ,  		/* IFMC        Interrupt  */
	EXTIA_IRQn    	=  3 ,  		/* EXTIA       Interrupt  */
	EXTIB_IRQn    	=  4 , 			/* EXTIB       Interrupt  */
	ADC0_IRQn    	=  5 , 			/* ADC         Interrupt  */
	CMP0_IRQn     	=  6 ,			/* CMP0        Interrupt  */
	I2C0_IRQn     	=  7,  			/* I2C0        Interrupt  */
	LPUART0_IRQn  	=  8, 		 	/* LPUART0     Interrupt  */
	USART0_IRQn  	=  9, 		 	/* USART0      Interrupt  */
	LEDPWM_IRQn		=  10, 		 	/* LEDPWM      Interrupt  */
	TIM4_IRQn     	=  11,  		/* TIM4	       Interrupt  */
	TIM20_IRQn     	=  12,  		/* TIM20       Interrupt  */
	TIM21_IRQn     	=  13,  		/* TIM21       Interrupt  */
	TIM22_IRQn     	=  14,  		/* TIM22       Interrupt  */
} IRQn_Type;
#elif defined (PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
typedef enum IRQn
{
	/******  Cortex-M0 Processor Exceptions Numbers ***************************************************/
	NMI_IRQn        = -14,			/*!< 2 Non Maskable Interrupt*/
	HardFault_IRQn	= -13,			/*!< 3 Cortex-M0 Hard Fault Interrupt*/
	SVCall_IRQn		= -5,			/*!< 11 Cortex-M0 SV Call Interrupt	*/
	PendSV_IRQn	    = -2,			/*!< 14 Cortex-M0 Pend SV Interrupt	*/
	SysTick_IRQn	= -1,			/*!< 15 Cortex-M0 System Tick Interrupt*/

	/****** MCU Specific Interrupt Numbers *******************************************************/
	PVD_IRQn 	  	=  0 ,  		/* PVD         Interrupt  */
	CLKFAIL_IRQn 	=  1 ,  		/* CLKFAIL     Interrupt  */
#if defined(PT32x063x)
	AES_IRQn		=  2 ,			/* AES         Interrupt  */
#endif
	IFMC_IRQn     	=  3 ,  		/* IFMC        Interrupt  */
	DMA_IRQn      	=  4 ,  		/* DMA         Interrupt  */
	EXTIA_IRQn    	=  5 ,  		/* EXTIA       Interrupt  */
	EXTIB_IRQn    	=  6 , 			/* EXTIB       Interrupt  */
	EXTIC_IRQn    	=  7 , 			/* EXTIC       Interrupt  */
	EXTID_IRQn    	=  8 , 			/* EXTID       Interrupt  */
	EXTIE_IRQn    	=  9 , 			/* EXTIE       Interrupt  */
	LPTIM_IRQn    	=  11, 			/* LPTIM       Interrupt  */
	SDADC0_IRQn     =  12,  		/* SDADC0      Interrupt  */
	TIM5_IRQn     	=  14,  		/* TIM5        Interrupt  */
	TIM4_IRQn     	=  15,  		/* TIM4        Interrupt  */
	TIM3_IRQn     	=  16,  		/* TIM3        Interrupt  */
	TIM2_IRQn     	=  17, 	  		/* TIM2        Interrupt  */
	LPUART1_IRQn    =  19, 			/* LPUART1     Interrupt  */
	LPUART0_IRQn    =  20, 			/* LPUART0     Interrupt  */
	I2C0_IRQn     	=  23,  		/* I2C0        Interrupt  */
	SPI0_IRQn    	=  25,  		/* SPI0        Interrupt  */
	UART0_IRQn      =  27,			/* UART0       Interrupt  */
	UART1_IRQn      =  28,			/* UART1       Interrupt  */
	RTC_IRQn        =  29,			/* RTC         Interrupt  */
	TIM6_IRQn     	=  30,  		/* TIM6        Interrupt  */
} IRQn_Type;
#endif
/**
  * @}
  */

/**
 * @brief Configuration of the Cortex-M0 Processor and Core Peripherals
 */
#include "core_cm0.h"
#include "PT32_Type.h"
#include "system_PT32x0xx.h"

/** @addtogroup Exported_types
  * @{
  */

/** @addtogroup Peripheral_registers_structures
  * @{
  */

/**
  * @brief CRC calculation unit
  */
typedef struct
{
	__IO u32 CR;       						/*!< Offset: 0x000  CRC Control Register     						(R/W) */
	__IO u32 SEEDR;     					/*!< Offset: 0x004  CRC Seed Register   		  					(R/W) */
	__IO u32 POLYR;     					/*!< Offset: 0x008  CRC polynomial Register  						(R/W) */
	__O  u32 DINR;      					/*!< Offset: 0x00C  CRC Data Input Register    						  (W) */
	__I  u32 DOUTR;     					/*!< Offset: 0x010  CRC Data Output Register  						(R)   */
} CRC_TypeDef;

/**
  * @brief Power Control
  */
typedef struct
{
	__IO u32 CR1;							/*!< Offset: 0x000  Control Register 1 								(R/W) */
	__IO u32 CR2;							/*!< Offset: 0x004  Control Register 2 								(R/W) */
	__IO u32 SR;							/*!< Offset: 0x008  Status Register 								(R/W) */
	__IO u32 VBUFCR;						/*!< Offset: 0x00C  VREFBUF Control Register 						(R/W) */
	__IO u32 SDADC0PCR;						/*!< Offset: 0x010  SDADC0 Power Control Register 					(R/W) */
} PWR_TypeDef;

/**
  * @brief Reset and Clock Control
  */
typedef struct
{
	__IO u32 APBRSTR1;  					/*!< Offset: 0x000 	 APB Peripheral Reset Register 1 				(R/W) */
	__IO u32 APBRSTR2;  					/*!< Offset: 0x004 	 APB Peripheral Reset Register 2 				(R/W) */
	__IO u32 APBRSTR3;						/*!< Offset: 0x008 	 APB Peripheral Reset Register 3  				(R/W) */
	__IO u32 APBRSTR4;						/*!< Offset: 0x00C 	 APB Peripheral Reset Register 4  				(R/W) */
	__IO u32 AHBRSTR;      					/*!< Offset: 0x010 	 AHB Peripheral Reset Register  				(R/W) */
	u32  RESERVED0[3];         				/*!< Reserved 													  		  */
	__IO u32 RSR;							/*!< Offset: 0x020 	 Reset Status Register             				(R/W) */
	__IO u32 RCR;							/*!< Offset: 0x024 	 Reset Control Register  	    				(R/W) */
	__O  u32 ASRCR;							/*!< Offset: 0x028 	 Advanced Software Reset Control Register      	  (W) */
	u32  RESERVED1;							/*!< Reserved 									   				 		  */
	__IO u32 APBENR1;						/*!< Offset: 0x030   APB Peripheral Clock Enable Register 1  		(R/W) */
	__IO u32 APBENR2;						/*!< Offset: 0x034   APB Peripheral Clock Enable Register 2  	    (R/W) */
	__IO u32 APBENR3;						/*!< Offset: 0x038   APB Peripheral Clock Enable Register 3         (R/W) */
	__IO u32 APBENR4;						/*!< Offset: 0x03C   APB Peripheral Clock Enable Register 4         (R/W) */
	__IO u32 AHBENR;						/*!< Offset: 0x040   AHB Peripheral Clock Enable Register           (R/W) */
	u32  RESERVED2[3];             			/*!< Reserved 													          */
	__IO u32 CFGR;							/*!< Offset: 0x050   Clock Configuration Register              		(R/W) */
	u32  RESERVED3[2];              		/*!< Reserved 													          */
	__IO u32 MCOR;							/*!< Offset: 0x05C   MCO Control Register              				(R/W) */
	__IO u32 PCSCR1;						/*!< Offset: 0x060   Peripheral Clock Source Configuration Register1(R/W) */
	__IO u32 PCSCR2;						/*!< Offset: 0x064   Peripheral Clock Source Configuration Register2(R/W) */
	u32  RESERVED4[2];             			/*!< Reserved 													          */
	__IO u32 HSICR;							/*!< Offset: 0x070   HSI Control Register              				(R/W) */
	__IO u32 LSICR;							/*!< Offset: 0x074   LSI Control Register              				(R/W) */
	__IO u32 HSECR;							/*!< Offset: 0x078   HSE Control Register              				(R/W) */
	__IO u32 LSECR;							/*!< Offset: 0x07C   LSE Control Register              				(R/W) */
	__IO u32 PLLCFGR1;						/*!< Offset: 0x080   PLL Configuration Register 1              		(R/W) */
	__IO u32 PLLCFGR2;						/*!< Offset: 0x084   PLL Configuration Register 2              		(R/W) */
} RCC_TypeDef;

/**
  * @brief System configuration controller
  */
typedef struct
{
	__IO u32 SR1; 							/*!< Offset: 0x000   Status Register 1 				   				(R/W) */
	u32  RESERVED0[4];             			/*!< Reserved 													          */
	__IO u32 IAPAR;  						/*!< Offset: 0x014   IAP Address Register  							(R/W) */
	__IO u32 SVTOR;    						/*!< Offset: 0x018   SRAM Interrupt Vector Table Address Register 	(R/W) */
	__IO u32 CR;    						/*!< Offset: 0x01C   Control Register 								  (W) */
} SYSCFG_TypeDef;

/**
  * @brief General Purpose I/O
  */
typedef struct
{
	__IO u32  DR;        					/*!< Offset: 0x000 Port Data Registe 						(R/W) */
	__O  u32  BSR;							/*!< Offset: 0x004 Port Bit Set Register  					  (W) */
	__O  u32  BRR; 							/*!< Offset: 0x008 Port Bit Reset Register  				  (W) */
	__IO u32  OESR;							/*!< Offset: 0x00C Output Enable Set Register  				(R/W) */
	__O  u32  OECR;							/*!< Offset: 0x010 Output Enable Clear Register  			  (W) */
	__IO u32  PUSR;							/*!< Offset: 0x014 Pull-Up Set Register						(R/W) */
	__O  u32  PUCR;							/*!< Offset: 0x018 Pull-Up Clear Register				  	  (W) */
	__IO u32  PDSR;							/*!< Offset: 0x01C Pull-Down Set Register  					(R/W) */
	__O  u32  PDCR;							/*!< Offset: 0x020 Pull-Down Clear Register  			  	  (W) */
    __IO u32  ODSR;							/*!< Offset: 0x024 Output Open-Drain Set Register  			(R/W) */
	__O  u32  ODCR;							/*!< Offset: 0x028 Output Open-Drain Clear Register  		  (W) */
	__IO u32  SCSR;							/*!< Offset: 0x02C Input Schmitt Set Register 				(R/W) */
	__O  u32  SCCR;							/*!< Offset: 0x030 Input Schmitt Clear Register				  (W) */
} GPIO_TypeDef;

/**
  * @brief Alternate Function  I/O
  */
typedef struct
{
	__IO u32  AFSR[2];	      				/*!< Offset: 0x000 Digtal Alternate Function Set Register1/2 		(R/W) */
	__O  u32  AFCR;    	    				/*!< Offset: 0x008 Digtal Alternate Function Clear Register  	      (W) */
	__IO u32  ANASR;						/*!< Offset: 0x00C Analog function Set Register  					(R/W) */
	__O  u32  ANACR;						/*!< Offset: 0x010 Analog function Clear Register  			          (W) */
} AFIO_TypeDef;

/**
  * @brief External Interrupt Controller
  */
typedef struct
{
	__IO u32  IER;     	  					/*!< Offset: 0x000 Interrupt Enable Register  	 					(R/W) */
	__O  u32  IDR;    						/*!< Offset: 0x004 Interrupt Disable Register    					  (W) */
	__IO u32  CFGR[2];          			/*!< Offset: 0x008 Interrupt Type Configuration Register 1 / 2 		(R/W) */
	__IO u32  SR;	    					/*!< Offset: 0x010 Interrupt Status Register  		 				(R/W) */
} EXTI_TypeDef;

/**
  * @brief DMA Controller
  */
typedef struct
{
	__IO u32 CSBAR;     					/*!< Offset: 0x000  DMA CHx Source Base Address Register              (R/W) */
	__IO u32 CDBAR;       					/*!< Offset: 0x004  DMA CHx Destination Dase Address Register         (R/W) */
	__IO u32 CNDTR;       					/*!< Offset: 0x008  DMA CHx Number of Data to Transfer Register       (R/W) */
	__IO u32 CCR;         					/*!< Offset: 0x00C  DMA CHx Control Register                          (R/W) */
	__I  u32 CCSAR;       					/*!< Offset: 0x010  DMA CHx Current Source Address Register      	  (R)   */
	__I  u32 CCDAR;       					/*!< Offset: 0x014  DMA CHx Current Destination Address Register      (R)   */
	__I  u32 CCNTR;       					/*!< Offset: 0x018  DMA CHx Counter Register                          (R)   */
} DMA_Channel_TypeDef;

typedef struct
{
	__IO u32 IER;         					/*!< Offset: 0x000  DMA Interrupt Enable Register         (R/W) */
	__IO u32 SR;          					/*!< Offset: 0x004  DMA Status Register                   (R) 	*/
	__IO u32 CHAPCR[2];   					/*!< Offset: 0x008  DMA Relevance Peripheral Register     (R/W) */
} DMA_TypeDef;

/**
  * @brief Direct Digital Synthesizer
  */
typedef struct
{
	__IO u32 CR1;       					/*!< Offset: 0x000  DDS Control Register 1 									(R/W) */
	__IO u32 PSCR;    						/*!< Offset: 0x004 	DDS Prescaler Register 									(R/W) */
	__IO u32 WGPSCR;    					/*!< Offset: 0x008 	DDS Wave Generator Prescaler Register 					(R/W) */
	__IO u32 WDSAR;    						/*!< Offset: 0x00C 	DDS Wave Data Start Address Register 					(R/W) */
	__IO u32 WDLENR;    					/*!< Offset: 0x010 	DDS Wave Data Length Register 							(R/W) */
	u32 RESERVED0[2];						/*!< Offset: 0x014 - 0x018  Reserved 						  					  */
	__IO u32 WOFR;    						/*!< Offset: 0x01C 	DDS Wave Offset Register 								(R/W) */
	__IO u32 SR;       						/*!< Offset: 0x020  DDS Status Register 									(R/W) */
} DDS_TypeDef;

/**
  * @brief Analog to Digital Converter
  */
typedef struct
{
	__IO u32 CR1;       					/*!< Offset: 0x000  ADC Control Register 1 									(R/W) */
	__IO u32 CR2;       					/*!< Offset: 0x004  ADC Control Register 2 									(R/W) */
	__IO u32 CR3;       					/*!< Offset: 0x008  ADC Control Register 3 									(R/W) */
	u32 RESERVED0;							/*!< Offset: 0x00C  Reserved 						  							  */
	__IO u32 CFGR1;       					/*!< Offset: 0x010  ADC Configuration Register 1 							(R/W) */
	__IO u32 CFGR2;       					/*!< Offset: 0x014  ADC Configuration Register 2 							(R/W) */
    __IO u32 CFGR3;       					/*!< Offset: 0x018  ADC Configuration Register 3 							(R/W) */
	__IO u32 IER;       					/*!< Offset: 0x01C  ADC Interrupt Enable Register 							(R/W) */
	__IO u32 SR;    						/*!< Offset: 0x020  ADC Status Register  								    (R/W) */
	__IO u32 AWDCR;       					/*!< Offset: 0x024  ADC Analog Watchdog Configuration Register 				(R/W) */
	__IO u32 AWDTR;       					/*!< Offset: 0x028  ADC Analog Watchdog Threshold Register					(R/W) */
    u32 RESERVED1;							/*!< Offset: 0x02C  Reserved 						  							  */
	__I  u32 DR;    						/*!< Offset: 0x030 	ADC data Register 										(R)   */
	__I u32 TSCALR1;       					/*!< Offset: 0x034  ADC Temperature Sensor Calibration Value Register 1 	(R)   */
	__I u32 TSCALR2;       					/*!< Offset: 0x038  ADC Temperature Sensor Calibration Value Register 2 	(R)   */
    u32 RESERVED2;							/*!< Reserved 									   				 			  	  */
	__IO u32 SCHR[5];						/*!< Offset: 0x040 - 0x050  ADC Scan Channel Configuration Register   		(R/W) */
	u32 RESERVED3[7];	    				/*!< Offset: 0x054 - 0x06C Reserved 						  				      */
	__IO u32 JSCHR[5];						/*!< Offset: 0x070 - 0x080  ADC Inject Scan Channel Configuration Register  (R/W) */
	 u32 RESERVED4[31];	    				/*!< Offset: 0x084 - 0x09C Reserved 						  				      */
	__I  u32 SCHDR[20];						/*!< Offset: 0x100 - 0x14C  ADC Scan channel y data register 				(R)   */
	 u32 RESERVED5[44];	    				/*!< Offset: 0x150 - 0x19C Reserved 						  				      */
	__I  u32 JSCHDR[20];					/*!< Offset: 0x200 - 0x24C  ADC Inject Scan Channel y Data Register 		(R)   */
} ADC_TypeDef;

/**
  * @brief Sigma Delta Analog to Digital Converter
  */
typedef struct
{
	__IO u32 CR1;       					/*!< Offset: 0x000  SDADC Control Register 1 								(R/W) */
	__IO u32 CR2;       					/*!< Offset: 0x004  SDADC Control Register 2 								(R/W) */
	u32 RESERVED0;							/*!< Offset: 0x008  Reserved 						  							  */
	__IO u32 CR4;       					/*!< Offset: 0x00C  SDADC Control Register 4 								(R/W) */
	__IO u32 CFGR1;       					/*!< Offset: 0x010  SDADC Configuration Register 1 							(R/W) */
	__IO u32 CFGR2;       					/*!< Offset: 0x014  SDADC Configuration Register 2 							(R/W) */
    __IO u32 CFGR3;       					/*!< Offset: 0x018  SDADC Configuration Register 3 							(R/W) */
	__IO u32 IER;       					/*!< Offset: 0x01C  SDADC Interrupt Enable Register 						(R/W) */
	__IO u32 SR;    						/*!< Offset: 0x020  SDADC Status Register  								    (R/W) */
    u32 RESERVED1[2];						/*!< Offset: 0x024 - 0x028  Reserved 						  					  */
	__IO u32 PGACR;    						/*!< Offset: 0x02C  SDADC PGA Control Register  							(R/W) */
	__I  u32 DR;    						/*!< Offset: 0x030 	SDADC Data Register 									(R)   */
    u32 RESERVED2[179];						/*!< Offset: 0x034 - 0x2FC  Reserved 									   		  */
	__IO u32 DOFR;							/*!< Offset: 0x300  SDADC Data Offset Register   							(R/W) */
	__IO u32 CCR1;							/*!< Offset: 0x304  SDADC Common Control Register 1   						(R/W) */
} SDADC_TypeDef;

/**
  * @brief Digital to Analog Converter
  */
typedef struct
{
	__IO u32 CR1;       					/*!< Offset: 0x000  DAC Control Register 1 									(R/W) */
	__IO u32 DR;    						/*!< Offset: 0x004 	DAC data Register 										(R/W) */
	u32 RESERVED0[18];						/*!< Reserved																	  */
	__IO u32 CR2;       					/*!< Offset: 0x050  DAC Control Register 2 									(R/W) */
} DAC_TypeDef;

/**
  * @brief TIM
  */
typedef struct
{
	__IO u32 CR;        					/*!< Offset: 0x00  Control Register  					  	(R/W) */
	__IO u32 MSCR;      					/*!< Offset: 0x04  Master And Slave Mode Control Register 	(R/W) */
	u32 RESERVED0;							/*!< Reserved 									   				  */
	__IO u32 PCR;     						/*!< Offset: 0x0C  Polarity Control Register     			(R/W) */
	__IO u32 CAPR[2];     					/*!< Offset: 0x10  Capture Configuration Register   		(R/W) */
	u32 RESERVED1[2];						/*!< Reserved 									   				  */
	__IO u32 OCMR;   						/*!< Offset: 0x20  Output Compare Mode Register      		(R/W) */
	u32 RESERVED2;							/*!< Reserved 									   				  */
	__IO u32 BKICR;     					/*!< Offset: 0x28  Break Input Control Register      		(R/W) */
	__IO u32 DTCR;      					/*!< Offset: 0x2C  Dead Time Control Register       		(R/W) */
	__IO u32 TDCR1;    						/*!< Offset: 0x30  Trigger DMA Control Register 1       	(R/W) */
	__IO u32 TDCR2;     					/*!< Offset: 0x34  Trigger DMA Control Register 1    	  	(R/W) */
	__IO u32 TACR;     						/*!< Offset: 0x38  Regular Trigger ADC Control Register  	(R/W) */
	__IO u32 JTACR;     					/*!< Offset: 0x3C  Injected Trigger ADC Control Register  	(R/W) */
	__IO u32 SR1;    						/*!< Offset: 0x40  Status Register 1     					(R/W) */
	__IO u32 SR2;      						/*!< Offset: 0x44  Status Register 2    	  				(R/W) */
	__IO u32 IER1;      					/*!< Offset: 0x48  Interrupt Enable Register 1        		(R/W) */
	__IO u32 IER2;      					/*!< Offset: 0x4C  Interrupt Enable Register 2         		(R/W) */
	__IO u32 ITARR;     					/*!< Offset: 0x50  Interrupt Auto Reload Register        	(R/W) */
	__I  u32 ITCNTR;    					/*!< Offset: 0x54  Interrupt Counter Register        	  	(R/W) */
	__IO u32 PSCR;     						/*!< Offset: 0x58  Prescaler Register        	  			(R/W) */
	u32 RESERVED3;							/*!< Reserved 									   				  */
	__IO u32 CNTR;      					/*!< Offset: 0x60  Counter Register     	  				(R/W) */
	__IO u32 ARR;      						/*!< Offset: 0x64  Auto Reload Register         			(R/W) */
	__IO u32 OCR[4];   			 			/*!< Offset: 0x68-74  Output Compare Register           	(R/W) */
	__I  u32 ICR[4];    					/*!< Offset: 0x78-84  Input Capture Register            	(R)   */
}TIM_TypeDef;

/**
  * @brief Independent WATCHDOG
  */
typedef struct
{
	__IO u32 RLR;   						/*!< Offset: 0x000  IWDG Reload register	 (R/W) */
	__I  u32 CNTR; 							/*!< Offset: 0x004  IWDG COUNT register	 	 (R/W) */
	__IO u32 CR;    						/*!< Offset: 0x008  IWDG Control register    (R/W) */
	__O  u32 KR;  							/*!< Offset: 0x00C  IWDG Key register	 	   (W) */
	__I  u32 SR;   							/*!< Offset: 0x010  IWDG Status register	 (R)   */
	u32 RESERVED0[250];     				/*!< Reserved 									   */
	__IO u32 LR;    						/*!< Offset: 0x3FC  IWDG Lock register	 	 (R/W) */
} IWDG_TypeDef;

/**
  * @brief Serial Peripheral Interface
  */
typedef struct
{
	__IO u32 CR1;       					/*!< Offset: 0x000  SPI Control Register1 				   	(R/W) */
	__IO u32 CR2;       					/*!< Offset: 0x004  SPI Control Register2 				   	(R/W) */
	__IO u32 BR; 	   						/*!< Offset: 0x008  SPI Clock Prescaler Register 	   		(R/W) */
	u32 RESERVED0;      					/*!< Offset: 0x00C  Reserved 								  	  */
	__IO u32 IER;        					/*!< Offset: 0x010  SPI Interrupt Enable Register  	 		(R/W) */
	__IO u32 SR;       						/*!< Offset: 0x014  SPI Status Register1 				    (R)   */
	u32 RESERVED1[2];      					/*!< Offset: 0x018  Reserved 								  	  */
	__IO u32 DR;        					/*!< Offset: 0x020  SPI Data Register 				   	   	(R/W) */
	__O  u32 DRR;      						/*!< Offset: 0x024  SPI Buffer/FIFO Data Reset Register    	  (W) */
} SPI_TypeDef;

/**
  * @brief Inter-integrated Circuit Interface
  */
typedef struct
{
	__IO u32 CR;		    				/*!< Offset: 0x000 	I2C Control Register  	 			(R/W) */
	__I  u32 SR; 		    				/*!< Offset: 0x004 	I2C Status Register  		 		(R)   */
	__IO u32 DR;  	 	  					/*!< Offset: 0x008 	I2C Data Register  		 	 		(R/W) */
	__IO u32 OAR;	 						/*!< Offset: 0x00C 	I2C Own Address Register 			(R/W) */
	u32 RESERVED[2];      					/*!< Reserved 								  				  */
	__IO u32 CCR; 		 					/*!< Offset: 0x018 	I2C Clear Control Register 		    (R/W) */
} I2C_TypeDef;

/**
  * @brief Universal Asynchronous Receiver Transmitter
  */
typedef struct
{
	__IO u32 CR;		 					/*!< Offset: 0x000  UART Control Register				   				(R/W) */
	u32  RESERVED0;         				/*!< Reserved 									   				  		      */
	__IO u32 BRR;      						/*!< Offset: 0x008  UART Baud Rate Register			   					(R/W) */
	__IO u32 AR;      						/*!< Offset: 0x00C  UART Address Register			   					(R/W) */
	__IO u32 IER;    	 					/*!< Offset: 0x010  UART Interrupt Enable Register  					(R/W) */
	__IO u32 SR;     	 					/*!< Offset: 0x014  UART Status Register								(R/W) */
	__IO u32 DR;     	 					/*!< Offset: 0x018  UART Data Register					    			(R/W) */
	u32 RESERVED1;          				/*!< Reserved 									   				 			  */
	__IO u32 FITR;     						/*!< Offset: 0x020  UART Frame Interval Time Configuration Register	    (R/W) */
	__IO u32 TCR;     						/*!< Offset: 0x024  UART Timeout Control Register     		  	        (R/W) */
	u32 RESERVED2;          				/*!< Reserved 									   				 			  */
	__O  u32 FRR;  							/*!< Offset: 0x02C  UART FIFO Reset Register	 	   					  (W) */
	__IO u32 SCCR;     						/*!< Offset: 0x030  UART Smart Card Control Register     				(R/W) */
} UART_TypeDef;

/**
  * @brief Universal Synchronous/Asynchronous Receiver Transmitter
  */
typedef struct
{
	__IO u32 CR1;		 					/*!< Offset: 0x000  UART Control Register1				   				(R/W) */
	u32  RESERVED0;         				/*!< Reserved 									   				  		      */
	__IO u32 BRR;      						/*!< Offset: 0x008  UART Baud Rate Register			   					(R/W) */
	u32  RESERVED1;
	__IO u32 IER;    	 					/*!< Offset: 0x010  UART Interrupt Enable Register  					(R/W) */
	__IO u32 SR;     	 					/*!< Offset: 0x014  UART Status Register								(R/W) */
	__IO u32 DR;     	 					/*!< Offset: 0x018  UART Data Register					    			(R/W) */
	__IO u32 CR3;		 					/*!< Offset: 0x01C  UART Control Register 3				   				(R/W) */
	__IO u32 FITR;     						/*!< Offset: 0x020  UART Frame Interval Time Configuration Register	    (R/W) */
	__IO u32 TCR;     						/*!< Offset: 0x024  UART Timeout Control Register     		  	        (R/W) */
	u32 RESERVED2;          				/*!< Reserved 									   				 			  */
	__O  u32 FRR;  							/*!< Offset: 0x02C  UART FIFO Reset Register	 	   					  (W) */
} USART_TypeDef;

/**
  * @brief Infrared Interface
  */
typedef struct
{
	__IO u32 CR;       						/*!< Offset: 0x000  IRTIM Control Reister  				(R/W) */
	__IO u32 CFGR;       					/*!< Offset: 0x004  IRTIM Configuration Reister 		(R/W) */
} IRTIM_TypeDef;

/**
  * @brief Operational amplifiers
  */
typedef struct
{
	__IO u32 CR1;      						/*!< Offset: 0x000  OPA Control Register  					(R/W) */
	__IO u32 CR2;	    					/*!< Offset: 0x004  OPA Control Register  					(R/W) */
    __IO u32 OTR;	    					/*!< Offset: 0x008  OPA Offset Trimming Register  			(R/W) */
} OPA_TypeDef;

/**
  * @brief Comparator
  */
typedef struct
{
	__IO u32 CR1;       					/*!< Offset: 0x000  CMP Control Register1  				 	(R/W) */
	__IO u32 IER;        					/*!< Offset: 0x004  CMP Interrupt Enable Register  			(R/W) */
	__IO u32 SR;        					/*!< Offset: 0x008  CMP Status Register 					(R/W) */
	__IO u32 CR2;       					/*!< Offset: 0x00C  CMP Control Register2   			 	(R/W) */
	__IO u32 CR3;       					/*!< Offset: 0x010  CMP Control Register3   			 	(R/W) */
} CMP_TypeDef;

/**
  * @brief 5/7 bits Digital to Analog Converter
  */
typedef struct
{
	__IO u32 CR;       						/*!< Offset: 0x000  LDAC Control Register  					(R/W)*/
	__IO u32 DR;       						/*!< Offset: 0x004  LDAC Data Register  					(R/W) */
} LDAC_TypeDef;

/**
  * @brief Pulse-width modulation
  */
typedef struct
{
	__IO u32 CR1;       					/*!< Offset: 0x000  LEDPWM Control Reister1  					(R/W)*/
	__IO u32 CR2;       					/*!< Offset: 0x004  LEDPWM Control Reister2 					(R/W)*/
	__IO u32 DR;       						/*!< Offset: 0x008  LEDPWM data Reister 						(R/W)*/
	__IO u32 IER;       					/*!< Offset: 0x00C  LEDPWM Interrupt Enable Register 			(R/W)*/
	__IO u32 SR;       						/*!< Offset: 0x010  LEDPWM Status Register 					(R/W)*/
} LEDPWM_TypeDef;

/**
  * @brief Beep
  */
typedef struct
{
	__IO u32 CR;  							/*!< Offset: 0x000   BEEP Control Register				(R/W) */
} BEEP_TypeDef;

/**
  * @brief Real_Time Clock
  */
typedef struct
{
	__IO u32 CR;  							/*!< Offset: 0x000 RTC Control Register						(R/W) */
	__IO u32 CFGR;  						/*!< Offset: 0x004 RTC Configuration Register				(R/W) */
	__IO u32 WPR;  							/*!< Offset: 0x008 RTC Write Protection Register			(R/W) */
	__IO u32 TR;  							/*!< Offset: 0x00C RTC Time Register						(R/W) */
	__IO u32 DR;  							/*!< Offset: 0x010 RTC Data Register						(R/W) */
	__IO u32 IER;  							/*!< Offset: 0x014 RTC Interrupt Enable Register			(R/W) */
	__IO u32 SR;  							/*!< Offset: 0x018 RTC Status Register						(R/W) */
	__IO u32 ALARMR;  						/*!< Offset: 0x01C RTC Alarm Register						(R/W) */
	__IO u32 CALR;  						/*!< Offset: 0x020 RTC Calibration Register					(R/W) */
} RTC_TypeDef;

/** @defgroup LCD_TypeDef
* @{
*/
typedef struct
{
	__IO u32 CR1;            				/*!< Offset: 0x000  LCD Control Register 1   			 	(R/W) */
	__IO u32 CR2;            				/*!< Offset: 0x004  LCD Control Register 2   			 	(R/W) */
	__IO u32 GCR;   		    			/*!< Offset: 0x008  LCD Gray SET Register  				 	(R/W) */
	__IO u32 PSCR;  		      			/*!< Offset: 0x00C  LCD Prescaler Register  			 	(R/W) */
	__IO u32 BTCR;          				/*!< Offset: 0x010  LCD Blink Time Control Register   	 	(R/W) */
	__IO u32 BONCR;         				/*!< Offset: 0x014  LCD Blink On Control Register      	 	(R/W) */
	__IO u32 PCR1;          				/*!< Offset: 0x018  LCD Port Control Register 1      	 	(R/W) */
	__IO u32 PCR2;        					/*!< Offset: 0x01C  LCD Port Control Register 2      	 	(R/W) */
	__IO u32 PDCR1;       					/*!< Offset: 0x020  LCD Port Driver Control Register 1   	(R/W) */
	__IO u32 PDCR2;         				/*!< Offset: 0x024  LCD Port Driver Control Register 2   	(R/W) */
	__IO u32 PDTR;	        				/*!< Offset: 0x028  LCD Page Display Time Register       	(R/W) */
	__IO u32 SDPR;     	    				/*!< Offset: 0x02C  LCD Scroll Display Page Register     	(R/W) */
	__IO u32 PNR;        	  				/*!< Offset: 0x030  LCD Page Number Register             	(R/W) */
	u32  RESERVED0[3];
	__IO u32 VREFCR;        	  			/*!< Offset: 0x040  LCD Reference Voltage Control Register  (R/W) */
} LCD_TypeDef;

/**
  * @brief Advanced Encryption Standard
  */
typedef struct
{
	__IO u32 CR;  							/*!< Offset: 0x000 AES Control Register						(R/W) */
	__IO u32 IER;  							/*!< Offset: 0x004 AES Interrupt Register					(R/W) */
	__IO u32 SR;  							/*!< Offset: 0x008 AES Status Register						(R/W) */
	u32  RESERVED0;
	__IO u32 KR[8];  						/*!< Offset: 0x10-0x2C AES Key Register x					(R/W) */
	__IO u32 IVR[4];  						/*!< Offset: 0x30-0x3C AES Initialization Vector Register x	(R/W) */
	__IO u32 DINR[4];  						/*!< Offset: 0x40-0x4C AES Data Input Register x			(R/W) */
	u32  RESERVED1[4];
	__IO u32 DOUTR[4];  					/*!< Offset: 0x60-0x6C AES Data Output Register x			(R/W) */
} AES_TypeDef;

/**
  * @brief ALU
  */
typedef struct
{
	__IO u32 CR;  							/*!< Offset: 0x000  ALU Control Register            									(R/W) */
	__IO u32 IER;  							/*!< Offset: 0x004  ALU Interrupt Enable Register   									(R/W) */
	__IO u32 SR;    						/*!< Offset: 0x008  ALU Status Register             									(R/W) */
	__IO u32 OPDXR;    						/*!< Offset: 0x00C  ALU Operand X Register          									(R/W) */
	__IO u32 OPDYR;    						/*!< Offset: 0x010  ALU Operand Y Register          									(R/W) */
	__IO u32 OPDZR;    						/*!< Offset: 0x014  ALU Operand Z Register          									(R/W) */
	__IO u32 OPDWR;		      				/*!< Offset: 0x018  ALU Operand W Register 												(R/W) */
	__I  u32 DHR;    						/*!< Offset: 0x01C  ALU Data High 32-bit Register   									(R)   */
	__I  u32 DLR;    						/*!< Offset: 0x020  ALU Data Low 32-bit Register    									(R)   */
	__I  u32 DRR;    						/*!< Offset: 0x024  ALU Division Remainder Register 									(R)   */
    __IO u32 MAMAXHR;    					/*!< Offset: 0x028  ALU Multiply And Accumulate MAX Threshold High 32-bit Register   	(R/W) */
	__IO u32 MAMAXLR;    					/*!< Offset: 0x02C  ALU Multiply And Accumulate MAX Threshold Low 32-bit Register    	(R/W) */
	__IO u32 MAMINHR;    					/*!< Offset: 0x030  ALU Multiply And Accumulate MIN Threshold High 32-bit Register      (R/W) */
	__IO u32 MAMINLR;    					/*!< Offset: 0x034  ALU Multiply And Accumulate MIN Threshold Low 32-bit Register       (R/W) */
} ALU_TypeDef;

/**
  * @brief Flash controller
  */
typedef struct
{
	__IO u32 CR1; 							/*!< Offset: 0x000   IFMC Control Register 1 			(R/W) */
	__IO u32 CR2;   						/*!< Offset: 0x004   IFMC Control Register 2 		 	(R/W) */
	__IO u32 PSCR;   						/*!< Offset: 0x008   IFMC Prescaler Register 		 	(R/W) */
	__IO u32 KR1;    						/*!< Offset: 0x00C   IFMC Key Register 1 				(R/W) */
	__IO u32 KR2;  							/*!< Offset: 0x010   IFMC Key Register 2   	 			(R/W) */
	__IO u32 IER;  							/*!< Offset: 0x014   IFMC Interrupt Enable Register  	(R/W) */
	__IO u32 SR1;  							/*!< Offset: 0x018   IFMC Status Register 1  			(R/W) */
	u32  RESERVED0;             			/*!< Reserved 												  */
	__IO u32 AR;        					/*!< Offset: 0x020   IFMC Address Register   			(R/W) */
	__IO u32 DR1;  							/*!< Offset: 0x024   IFMC Data Register 1 				(R/W) */
	__IO u32 DR2;                       	/*!< Offset: 0x028   IFMC Data Register 2 				(R/W) */
	u32  RESERVED1;             			/*!< Reserved 												  */
	__IO u32 OPTCR;                     	/*!< Offset: 0x030   Option Control Register			(R/W) */
} IFMC_TypeDef;

/**
  * @brief Device Electronic Signature
  */
typedef struct
{
	__I  u32 CIR;      						/*!< Offset: 0x000 Chip Information Register           	(R)   */
	u32  RESERVED0[2];						/*!< Reserved 									   			  */
	__I  u32 CIDR[3];    					/*!< Offset: 0x00C~0x014 Chip ID Register x  			(R)   */
	__IO u32 UDIR[3];      					/*!< Offset: 0x018~0x020 User-Defined Information Register x (R/W) */
} ES_TypeDef;
/**
  * @}
  */

/** @addtogroup Peripheral_memory_map
  * @{
  */
#define SRAM_BASE             	(0x20000000UL)				/*!< Base address of system data RAM        */
#define FLASH_BASE            	(0x00000000UL)				/*!< Base address of embedded FLASH memory  */

/*!< Peripheral memory map */
#define APB_BASE              	(0x40000000UL)				/*!< Base address of : APB Peripherals      */
#define AHB_BASE              	(0x48000000UL)				/*!< Base address of : AHB Peripherals      */

/* AHB peripherals */
#define GPIOA_BASE        		(AHB_BASE + 0x00000000UL)
#define GPIOB_BASE        		(AHB_BASE + 0x00001000UL)
#define GPIOC_BASE        		(AHB_BASE + 0x00002000UL)
#define GPIOD_BASE        		(AHB_BASE + 0x00003000UL)
#define GPIOE_BASE        		(AHB_BASE + 0x00004000UL)
#define GPIOF_BASE        		(AHB_BASE + 0x00005000UL)

#define AFIOA_BASE				(AHB_BASE + 0x00000100UL)
#define AFIOB_BASE				(AHB_BASE + 0x00001100UL)
#define AFIOC_BASE				(AHB_BASE + 0x00002100UL)
#define AFIOD_BASE				(AHB_BASE + 0x00003100UL)
#define AFIOE_BASE				(AHB_BASE + 0x00004100UL)
#define AFIOF_BASE				(AHB_BASE + 0x00005100UL)

#define EXTIA_BASE				(AHB_BASE + 0x00000200UL)
#define EXTIB_BASE				(AHB_BASE + 0x00001200UL)
#define EXTIC_BASE				(AHB_BASE + 0x00002200UL)
#define EXTID_BASE				(AHB_BASE + 0x00003200UL)
#define EXTIE_BASE				(AHB_BASE + 0x00004200UL)
#define EXTIF_BASE				(AHB_BASE + 0x00005200UL)

#define LCDRAM_BASE				(AHB_BASE + 0x18000000UL)

/* APB peripherals */
#define IFMC_BASE    	    	(APB_BASE + 0x00000000UL)
#define CMP0_BASE	        	(APB_BASE + 0x00000800UL)
#define CMP1_BASE	        	(APB_BASE + 0x00000C00UL)
#define TIM2_BASE       		(APB_BASE + 0x00001000UL)
#define TIM3_BASE       		(APB_BASE + 0x00001400UL)
#define TIM4_BASE        		(APB_BASE + 0x00001C00UL)
#define BEEP_BASE        		(APB_BASE + 0x00002800UL)
#define CMP2_BASE	        	(APB_BASE + 0x00002C00UL)
#define IWDG_BASE       		(APB_BASE + 0x00003000UL)
#define SPI1_BASE        		(APB_BASE + 0x00003800UL)
#define CRC_BASE         		(APB_BASE + 0x00003C00UL)
#define LPUART0_BASE    	  	(APB_BASE + 0x00004000UL)
#define UART0_BASE    	  		(APB_BASE + 0x00004400UL)
#define LPUART1_BASE    	  	(APB_BASE + 0x00004C00UL)
#define I2C0_BASE         		(APB_BASE + 0x00005400UL)
#define I2C1_BASE         		(APB_BASE + 0x00005800UL)
#define UART2_BASE   	  		(APB_BASE + 0x00005C00UL)
#define UART3_BASE   	  		(APB_BASE + 0x00006000UL)
#define TIM8_BASE        		(APB_BASE + 0x00006400UL)
#define CMP3_BASE	        	(APB_BASE + 0x00006800UL)
#define LPTIM_BASE        		(APB_BASE + 0x00006C00UL)
#define TIM20_BASE       		(APB_BASE + 0x00007400UL)
#define TIM21_BASE       		(APB_BASE + 0x00007800UL)
#define TIM22_BASE       		(APB_BASE + 0x00007C00UL)
#define DDS_BASE       			(APB_BASE + 0x00008000UL)
#define IRTIM_BASE       		(APB_BASE + 0x00008400UL)
#define DMA0_CH0_BASE 			(APB_BASE + 0x0000F000UL)
#define DMA0_CH1_BASE 			(APB_BASE + 0x0000F020UL)
#define DMA0_CH2_BASE 			(APB_BASE + 0x0000F040UL)
#define DMA0_CH3_BASE 			(APB_BASE + 0x0000F060UL)
#define DMA0_CH4_BASE 			(APB_BASE + 0x0000F080UL)
#define DMA0_CH5_BASE 			(APB_BASE + 0x0000F0A0UL)
#define DMA0_BASE         		(APB_BASE + 0x0000F100UL)
#define LEDPWM_BASE				(APB_BASE + 0x0000F800UL)
#define USART0_BASE				(APB_BASE + 0x00010000UL)
#define DAC0_BASE          		(APB_BASE + 0x00011800UL)
#define DAC1_BASE          		(APB_BASE + 0x00011C00UL)
#define SDADC0_BASE          	(APB_BASE + 0x00012000UL)
#define ADC0_BASE          		(APB_BASE + 0x00012400UL)
#define TIM1_BASE        		(APB_BASE + 0x00012C00UL)
#define SPI0_BASE         		(APB_BASE + 0x00013000UL)
#define AES_BASE         		(APB_BASE + 0x00013400UL)
#define UART1_BASE   	  		(APB_BASE + 0x00013800UL)
#define RTC_BASE   	  			(APB_BASE + 0x00013C00UL)
#define LDAC0_BASE	      		(APB_BASE + 0x00014000UL)
#define TIM5_BASE	      		(APB_BASE + 0x00014400UL)
#define TIM6_BASE	      		(APB_BASE + 0x00014800UL)
#define TIM7_BASE	      		(APB_BASE + 0x00014C00UL)
#define LCD_BASE	      		(APB_BASE + 0x00015000UL)
#define OPA0_BASE	        	(APB_BASE + 0x00015C00UL)
#define OPA1_BASE	        	(APB_BASE + 0x00015D00UL)
#define OPA2_BASE	        	(APB_BASE + 0x00015E00UL)
#define OPA3_BASE	        	(APB_BASE + 0x00015F00UL)
#define RCC_BASE     	    	(APB_BASE + 0x00020000UL)
#define PWR_BASE        		(APB_BASE + 0x00020400UL)
#define ES_BASE		        	(APB_BASE + 0x00020800UL)
#define SYSCFG_BASE             (APB_BASE + 0x00020C00UL)
#define ALU_BASE    	  		(APB_BASE + 0x00030000UL)
/**
  * @}
  */

/** @addtogroup Peripheral_declaration
  * @{
  */
#define PWR					((PWR_TypeDef *) PWR_BASE)
#define RCC					((RCC_TypeDef *) RCC_BASE)
#define ES					((ES_TypeDef *) ES_BASE)
#define SYSCFG  			((SYSCFG_TypeDef *) SYSCFG_BASE)

#define GPIOA				((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB				((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC				((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD				((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE				((GPIO_TypeDef *) GPIOE_BASE)
#define GPIOF				((GPIO_TypeDef *) GPIOF_BASE)

#define AFIOA				((AFIO_TypeDef *) AFIOA_BASE)
#define AFIOB				((AFIO_TypeDef *) AFIOB_BASE)
#define AFIOC				((AFIO_TypeDef *) AFIOC_BASE)
#define AFIOD				((AFIO_TypeDef *) AFIOD_BASE)
#define AFIOE				((AFIO_TypeDef *) AFIOE_BASE)
#define AFIOF				((AFIO_TypeDef *) AFIOF_BASE)

#define EXTIA				((EXTI_TypeDef *) EXTIA_BASE)
#define EXTIB				((EXTI_TypeDef *) EXTIB_BASE)
#define EXTIC				((EXTI_TypeDef *) EXTIC_BASE)
#define EXTID				((EXTI_TypeDef *) EXTID_BASE)
#define EXTIE				((EXTI_TypeDef *) EXTIE_BASE)
#define EXTIF				((EXTI_TypeDef *) EXTIF_BASE)

#define LCDRAM				((u8 (*)[48]) LCDRAM_BASE)

#define UART0 				((UART_TypeDef *) UART0_BASE)
#define UART1 				((UART_TypeDef *) UART1_BASE)
#define UART2 				((UART_TypeDef *) UART2_BASE)
#define UART3 				((UART_TypeDef *) UART3_BASE)
#define LPUART0 			((UART_TypeDef *) LPUART0_BASE)
#define LPUART1 			((UART_TypeDef *) LPUART1_BASE)
#define USART0 				((USART_TypeDef*) USART0_BASE)

#define IFMC				((IFMC_TypeDef *) IFMC_BASE)
#define SDADC0   			((SDADC_TypeDef *) SDADC0_BASE)
#define ADC0   				((ADC_TypeDef *) ADC0_BASE)
#define I2C0				((I2C_TypeDef *) I2C0_BASE)
#define I2C1				((I2C_TypeDef *) I2C1_BASE)
#define SPI0  				((SPI_TypeDef *) SPI0_BASE)
#define SPI1				((SPI_TypeDef *) SPI1_BASE)
#define CMP0				((CMP_TypeDef *) CMP0_BASE)
#define CMP1				((CMP_TypeDef *) CMP1_BASE)
#define CMP2				((CMP_TypeDef *) CMP2_BASE)
#define CMP3				((CMP_TypeDef *) CMP3_BASE)
#define OPA0				((OPA_TypeDef *) OPA0_BASE)
#define OPA1				((OPA_TypeDef *) OPA1_BASE)
#define OPA2				((OPA_TypeDef *) OPA2_BASE)
#define OPA3				((OPA_TypeDef *) OPA3_BASE)
#define DAC0				((DAC_TypeDef *) DAC0_BASE)
#define DAC1				((DAC_TypeDef *) DAC1_BASE)
#define LDAC0				((LDAC_TypeDef *) LDAC0_BASE)
#define TIM1				((TIM_TypeDef *) TIM1_BASE)
#define TIM2				((TIM_TypeDef *) TIM2_BASE)
#define TIM3				((TIM_TypeDef *) TIM3_BASE)
#define TIM4				((TIM_TypeDef *) TIM4_BASE)
#define TIM5				((TIM_TypeDef *) TIM5_BASE)
#define TIM6				((TIM_TypeDef *) TIM6_BASE)
#define TIM7				((TIM_TypeDef *) TIM7_BASE)
#define TIM8				((TIM_TypeDef *) TIM8_BASE)
#define LPTIM				((TIM_TypeDef *) LPTIM_BASE)
#define TIM20				((TIM_TypeDef *) TIM20_BASE)
#define TIM21				((TIM_TypeDef *) TIM21_BASE)
#define TIM22				((TIM_TypeDef *) TIM22_BASE)
#define DDS					((DDS_TypeDef *) DDS_BASE)
#define IRTIM				((IRTIM_TypeDef *) IRTIM_BASE)
#define BEEP				((BEEP_TypeDef *) BEEP_BASE)
#define LEDPWM				((LEDPWM_TypeDef *) LEDPWM_BASE)
#define CRC					((CRC_TypeDef *) CRC_BASE)
#define IWDG				((IWDG_TypeDef *) IWDG_BASE)
#define DMA0_CH0  			((DMA_Channel_TypeDef *) DMA0_CH0_BASE)
#define DMA0_CH1  			((DMA_Channel_TypeDef *) DMA0_CH1_BASE)
#define DMA0_CH2  			((DMA_Channel_TypeDef *) DMA0_CH2_BASE)
#define DMA0_CH3  			((DMA_Channel_TypeDef *) DMA0_CH3_BASE)
#define DMA0_CH4  			((DMA_Channel_TypeDef *) DMA0_CH4_BASE)
#define DMA0_CH5  			((DMA_Channel_TypeDef *) DMA0_CH5_BASE)
#define DMA0   				((DMA_TypeDef *) DMA0_BASE)
#define ALU   				((ALU_TypeDef *) ALU_BASE)
#define AES   				((AES_TypeDef *) AES_BASE)
#define RTC   				((RTC_TypeDef *) RTC_BASE)
#define LCD					((LCD_TypeDef *) LCD_BASE)
/**
  * @}
  */

/** @addtogroup Peripheral_Registers_Bits_Definition
  * @{
  */
/******************************************************************************/
/*                                                                            */
/*                     Cyclic Redundancy Check (CRC)                          */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for CRC_CR register  *******************/
#define  CRC_CR_EN                         	((u32)0x00000001)        	/*!< CRC Enable */
#define  CRC_CR_RST                         ((u32)0x00000002)        	/*!< CRC reset control */
#define  CRC_CR_INS                         ((u32)0x00000004)        	/*!< CRC input select */
#define  CRC_CR_IBITR                       ((u32)0x00000008)        	/*!< CRC input bit sequence reversal */
#define  CRC_CR_IBYTER                      ((u32)0x00000010)        	/*!< CRC input byte sequence reversal */
#define  CRC_CR_OBITR                       ((u32)0x00000020)       	/*!< CRC output bit sequence reversal */

/*******************  Bit definition for CRC_SEEDR register  *******************/
#define  CRC_SEEDR_SEED                     ((u32)0x0000FFFF)        	/*!< CRC SEED */

/*******************  Bit definition for CRC_POLYR register  *******************/
#define  CRC_POLYR_POLY                     ((u32)0x0000FFFF)        	/*!< CRC POLY */

/*******************  Bit definition for CRC_DINR register  *******************/
#define  CRC_DINR_DIN                       ((u32)0x0000FFFF)       	/*!< CRC data input */

/*******************  Bit definition for CRC_DOUTR register  *******************/
#define  CRC_DOUTR_DOUT                     ((u32)0x0000FFFF)        	/*!< CRC data output */

/******************************************************************************/
/*                                                                            */
/*                            Power Control (PWR)                   	      */
/*                                                                            */
/******************************************************************************/
/********************  Bits definition for PWR_CR1 register  ******************/
#define  PWR_CR1_PVDE                 		((u32)0x00000001)    		/*!< Power voltage detection enable */
#define  PWR_CR1_PLS                  		((u32)0x00000070)    		/*!< Power level selection */
#define  PWR_CR1_PLF                  		((u32)0x00000100)    		/*!< PVD level status flag */
#define  PWR_CR1_KEY                  		((u32)0x0000FF00)    		/*!< Key Value */

/********************  Bits definition for PWR_CR2 register  ******************/
#define  PWR_CR2_FWUS                 		((u32)0x00000001)    		/*!< FLASH wakeup selection */

/********************  Bits definition for PWR_SR register   ******************/
#define  PWR_SR_LPRDY                 		((u32)0x00000001)    		/*!< LPR ready flag */

/********************  Bits definition for PWR_VBUFCR register  ***************/
#define  PWR_VBUFCR_MODE                 	((u32)0x00000003)    		/*!< Voltage reference buffer mode */
#define  PWR_VBUFCR_IRVS                 	((u32)0x00000010)    		/*!< Internal reference voltage selection */
#define  PWR_VBUFCR_KEY                 	((u32)0x0000FF00)    		/*!< Key value */
#define  PWR_VBUFCR_BGTF                 	((u32)0x00000100)    		/*!< BG2v0 offset trimming value source flag */
#define  PWR_VBUFCR_TRIM                 	((u32)0x003F0000)    		/*!< BG2v0 offset trimming value */

/********************  Bits definition for PWR_SDADC0PCR register  ************/
#define  PWR_SDADC0PCR_EN                 	((u32)0x00000001)    		/*!< SDADC0 power enable */
#define  PWR_SDADC0PCR_PCMS                 ((u32)0x00000004)    		/*!< SDADC0 power filter capacitor mode selection */
#define  PWR_SDADC0PCR_PVS                 	((u32)0x00000070)    		/*!< SDADC0 power voltage selection */
#define  PWR_SDADC0PCR_KEY                 	((u32)0x0000FF00)    		/*!< Key Value */
#define  PWR_SDADC0PCR_RDY                 	((u32)0x80000000)    		/*!< SDADC0 power ready flag */

/******************************************************************************/
/*                                                                            */
/*                       Reset And Clock Control��RCC��                   	  */
/*                                                                            */
/******************************************************************************/
/********************  Bits definition for RCC_APBRSTR1 register  ******************/
#define  RCC_APBRSTR1_TIM1            		((u32)0x00000002)        	/*!< TIM1 reset */
#define  RCC_APBRSTR1_TIM2              	((u32)0x00000004)        	/*!< TIM2 reset */
#define  RCC_APBRSTR1_TIM3             	 	((u32)0x00000008)        	/*!< TIM3 reset */
#define  RCC_APBRSTR1_TIM4            	 	((u32)0x00000010)        	/*!< TIM4 reset */
#define  RCC_APBRSTR1_TIM5              	((u32)0x00000020)        	/*!< TIM5 reset */
#define  RCC_APBRSTR1_TIM6             	 	((u32)0x00000040)        	/*!< TIM6 reset */
#define  RCC_APBRSTR1_TIM7              	((u32)0x00000080)        	/*!< TIM7 reset */
#define  RCC_APBRSTR1_TIM8             	 	((u32)0x00000100)        	/*!< TIM8 reset */
#define  RCC_APBRSTR1_LPTIM           		((u32)0x00000400)        	/*!< LPTIM reset */

/********************  Bits definition for RCC_APBRSTR2 register  ******************/
#define  RCC_APBRSTR2_UART0              	((u32)0x00000001)        	/*!< UART0 reset */
#define  RCC_APBRSTR2_UART1              	((u32)0x00000002)        	/*!< UART1 reset */
#define  RCC_APBRSTR2_UART2              	((u32)0x00000004)        	/*!< UART2 reset */
#define  RCC_APBRSTR2_UART3              	((u32)0x00000008)        	/*!< UART3 reset */
#define  RCC_APBRSTR2_USART0              	((u32)0x00000040)        	/*!< USART0 reset */
#define  RCC_APBRSTR2_LPUART0              	((u32)0x00000100)        	/*!< LPUART0 reset */
#define  RCC_APBRSTR2_LPUART1              	((u32)0x00000200)        	/*!< LPUART1 reset */
#define  RCC_APBRSTR2_IRTIM              	((u32)0x00008000)        	/*!< IRTIM reset */
#define  RCC_APBRSTR2_SPI0              	((u32)0x00010000)        	/*!< SPI0 reset) */
#define  RCC_APBRSTR2_SPI1              	((u32)0x00020000)        	/*!< SPI1 reset */
#define  RCC_APBRSTR2_I2C0              	((u32)0x01000000)        	/*!< I2C0 reset */
#define  RCC_APBRSTR2_I2C1              	((u32)0x02000000)        	/*!< I2C1 reset */
#define  RCC_APBRSTR2_CAN0              	((u32)0x10000000)        	/*!< CAN0 reset */

/********************  Bits definition for RCC_APBRSTR3 register  ******************/
#define  RCC_APBRSTR3_ADC0             	   	((u32)0x00000001)        	/*!< ADC0 reset */
#define  RCC_APBRSTR3_SDADC0             	((u32)0x00000002)        	/*!< SDADC0 reset */
#define  RCC_APBRSTR3_DAC0             	   	((u32)0x00000010)        	/*!< DAC0 reset */
#define  RCC_APBRSTR3_DAC1             	   	((u32)0x00000020)        	/*!< DAC1 reset */
#define  RCC_APBRSTR3_CMP0             	   	((u32)0x00000100)        	/*!< CMP0 reset */
#define  RCC_APBRSTR3_CMP1             	   	((u32)0x00000200)        	/*!< CMP1 reset */
#define  RCC_APBRSTR3_CMP2             	   	((u32)0x00000400)        	/*!< CMP2 reset */
#define  RCC_APBRSTR3_CMP3             	   	((u32)0x00000800)        	/*!< CMP3 reset */
#define  RCC_APBRSTR3_OPA             	   	((u32)0x00010000)        	/*!< OPA reset */
#define  RCC_APBRSTR3_LDAC             	   	((u32)0x00020000)        	/*!< LDAC reset */
#define  RCC_APBRSTR3_DDS             	   	((u32)0x00040000)        	/*!< DDS reset */
#define  RCC_APBRSTR3_LCD             	   	((u32)0x00100000)        	/*!< LCD reset */
#define  RCC_APBRSTR3_BEEP             	   	((u32)0x10000000)        	/*!< BEEP reset */

/********************  Bits definition for RCC_APBRSTR4 register  ******************/
#define  RCC_APBRSTR4_IWDG             	   	((u32)0x00000001)        	/*!< IWDG reset */
#define  RCC_APBRSTR4_WWDG             	   	((u32)0x00000002)        	/*!< WWDG reset */
#define  RCC_APBRSTR4_AES             	   	((u32)0x00000100)        	/*!< AES reset */
#define  RCC_APBRSTR4_CRC             	   	((u32)0x00000200)        	/*!< CRC reset */

/********************  Bits definition for RCC_AHBRSTR register  ******************/
#define  RCC_AHBRSTR_GPIOA              	((u32)0x00000001)        	/*!< GPIOA reset */
#define  RCC_AHBRSTR_GPIOB                	((u32)0x00000002)        	/*!< GPIOB reset */
#define  RCC_AHBRSTR_GPIOC              	((u32)0x00000004)        	/*!< GPIOC reset */
#define  RCC_AHBRSTR_GPIOD              	((u32)0x00000008)        	/*!< GPIOD reset */
#define  RCC_AHBRSTR_GPIOE              	((u32)0x00000010)        	/*!< GPIOE reset */
#define  RCC_AHBRSTR_GPIOF                	((u32)0x00000020)        	/*!< GPIOF reset */
#define  RCC_AHBRSTR_DMA0              	 	((u32)0x00000100)        	/*!< DMA0 reset */
#define  RCC_AHBRSTR_ALU                	((u32)0x00001000)        	/*!< ALU reset */

/********************  Bits definition for RCC_RSR register  ******************/
#define  RCC_RSR_POR                  		((u32)0x00000001)        	/*!< Power reset flag */
#define  RCC_RSR_PVD                		((u32)0x00000004)        	/*!< PVD reset flag */
#define  RCC_RSR_PIN                 		((u32)0x00000020)        	/*!< Pin reset flag */
#define  RCC_RSR_PLVD                 		((u32)0x00000040)        	/*!< Power low voltage detection reset flag */
#define  RCC_RSR_SFR                 		((u32)0x00001000)        	/*!< Software reset flag */
#define  RCC_RSR_LOCKUP                		((u32)0x00002000)        	/*!< LOCKUP reset flag */
#define  RCC_RSR_IWDG                 		((u32)0x00010000)        	/*!< IWDG reset flag */
#define  RCC_RSR_WWDG                		((u32)0x00020000)        	/*!< WWDG reset flag */
#define  RCC_RSR_HSE                 		((u32)0x00040000)        	/*!< HSE fail reset flag */
#define  RCC_RSR_PLL                 		((u32)0x00080000)        	/*!< PLL fail reset flag */
#define  RCC_RSR_RELOAD                 	((u32)0x02000000)        	/*!< Reload reset flag */
#define  RCC_RSR_CPU                 		((u32)0x04000000)        	/*!< CPU core reset flag */

/********************  Bits definition for RCC_RCR register  ******************/
#define  RCC_RCR_PVDE               		((u32)0x00000004)        	/*!< PVD reset enable */
#define  RCC_RCR_PLVDE               		((u32)0x00000040)        	/*!< Power low voltage detection reset enable */
#define  RCC_RCR_LOCKUPE             		((u32)0x00002000)        	/*!< LOCKUP reset enable */
#define  RCC_RCR_IWDGE              		((u32)0x00010000)        	/*!< IWDG reset enable */
#define  RCC_RCR_HSEE              			((u32)0x00040000)        	/*!< HSE fail reset enable */
#define  RCC_RCR_PLLE               		((u32)0x00080000)        	/*!< PLL fail reset enable */

/********************  Bits definition for RCC_ASRCR register  ******************/
#define  RCC_ASRCR_KEY              		((u32)0x0000FFFF)        	/*!< Key */

/********************  Bits definition for RCC_APBENR1 register  ******************/
#define  RCC_APBENR1_TIM1                  	((u32)0x00000002)        	/*!< TIM1 clock enable */
#define  RCC_APBENR1_TIM2                 	((u32)0x00000004)        	/*!< TIM2 clock enable */
#define  RCC_APBENR1_TIM3               	((u32)0x00000008)        	/*!< TIM3 clock enable */
#define  RCC_APBENR1_TIM4                 	((u32)0x00000010)        	/*!< TIM4 clock enable */
#define  RCC_APBENR1_TIM5                 	((u32)0x00000020)        	/*!< TIM5 clock enable */
#define  RCC_APBENR1_TIM6                 	((u32)0x00000040)        	/*!< TIM6 clock enable */
#define  RCC_APBENR1_TIM7                	((u32)0x00000080)        	/*!< TIM7 clock enable */
#define  RCC_APBENR1_TIM8                	((u32)0x00000100)        	/*!< TIM8 clock enable */
#define  RCC_APBENR1_LPTIM                	((u32)0x00000400)        	/*!< LPTIM clock enable */
#define  RCC_APBENR1_TIM20                	((u32)0x00100000)        	/*!< TIM20 clock enable */
#define  RCC_APBENR1_TIM21                	((u32)0x00200000)        	/*!< TIM21 clock enable */
#define  RCC_APBENR1_TIM22                	((u32)0x00400000)        	/*!< TIM22 clock enable */

/********************  Bits definition for RCC_APBENR2 register  ******************/
#define  RCC_APBENR2_UART0                  ((u32)0x00000001)        	/*!< UART0 clock enable */
#define  RCC_APBENR2_UART1                  ((u32)0x00000002)        	/*!< UART1 clock enable */
#define  RCC_APBENR2_UART2                  ((u32)0x00000004)        	/*!< UART2 clock enable */
#define  RCC_APBENR2_UART3                  ((u32)0x00000008)        	/*!< UART3 clock enable */
#define  RCC_APBENR2_USART0                	((u32)0x00000040)        	/*!< USART0 clock enable */
#define  RCC_APBENR2_LPUART0                ((u32)0x00000100)        	/*!< LPUART0 clock enable */
#define  RCC_APBENR2_LPUART1                ((u32)0x00000200)        	/*!< LPUART1 clock enable */
#define  RCC_APBENR2_IRTIM                	((u32)0x00008000)        	/*!< IRTIM clock enable */
#define  RCC_APBENR2_SPI0                   ((u32)0x00010000)        	/*!< SPI0 clock enable */
#define  RCC_APBENR2_SPI1                   ((u32)0x00020000)        	/*!< SPI1 clock enable */
#define  RCC_APBENR2_I2C0                   ((u32)0x01000000)        	/*!< I2C0 clock enable */
#define  RCC_APBENR2_I2C1                   ((u32)0x02000000)        	/*!< I2C1 clock enable */
#define  RCC_APBENR2_CAN0                   ((u32)0x10000000)        	/*!< CAN0 clock enable */

/********************  Bits definition for RCC_APBENR3 register  ******************/
#define  RCC_APBENR3_ADC0                 	((u32)0x00000001)        	/*!< ADC0 clock enable */
#define  RCC_APBENR3_SDADC0               	((u32)0x00000002)      		/*!< SDADC0 clock enable */
#define  RCC_APBENR3_DAC0                 	((u32)0x00000010)        	/*!< DAC0 clock enable */
#define  RCC_APBENR3_DAC1                 	((u32)0x00000020)        	/*!< DAC1 clock enable */
#define  RCC_APBENR3_CMP0                 	((u32)0x00000100)        	/*!< CMP0 clock enable */
#define  RCC_APBENR3_CMP1                 	((u32)0x00000200)        	/*!< CMP1 clock enable */
#define  RCC_APBENR3_CMP2                 	((u32)0x00000400)        	/*!< CMP2 clock enable */
#define  RCC_APBENR3_CMP3                 	((u32)0x00000800)        	/*!< CMP3 clock enable */
#define  RCC_APBENR3_OPA                 	((u32)0x00010000)        	/*!< OPA clock enable */
#define  RCC_APBENR3_LDAC                 	((u32)0x00020000)        	/*!< LDAC clock enable */
#define  RCC_APBENR3_DDS                 	((u32)0x00040000)        	/*!< DDS clock enable */
#define  RCC_APBENR3_LCD                 	((u32)0x00100000)        	/*!< LCD clock enable */
#define  RCC_APBENR3_LEDPWM                 ((u32)0x02000000)        	/*!< LEDPWM clock enable */
#define  RCC_APBENR3_BEEP                 	((u32)0x10000000)        	/*!< BEEP clock enable */

/********************  Bits definition for RCC_APBENR4 register  ******************/
#define  RCC_APBENR4_IWDG                  	((u32)0x00000001)        	/*!< IWDG clock enable */
#define  RCC_APBENR4_WWDG                  	((u32)0x00000002)        	/*!< WWDG clock enable */
#define  RCC_APBENR4_RTC                  	((u32)0x00000004)        	/*!< RTC clock enable */
#define  RCC_APBENR4_AES                  	((u32)0x00000100)        	/*!< AES clock enable */
#define  RCC_APBENR4_CRC                  	((u32)0x00000200)        	/*!< CRC clock enable */
#define  RCC_APBENR4_IFMC	                ((u32)0x00010000)        	/*!< IFMC clock enable */

/********************  Bits definition for RCC_AHBENR register  ******************/
#define  RCC_AHBENR_GPIOA                  	((u32)0x00000001)        	/*!< GPIOA clock enable */
#define  RCC_AHBENR_GPIOB                  	((u32)0x00000002)        	/*!< GPIOB clock enable */
#define  RCC_AHBENR_GPIOC                  	((u32)0x00000004)        	/*!< GPIOC clock enable */
#define  RCC_AHBENR_GPIOD                  	((u32)0x00000008)        	/*!< GPIOD clock enable */
#define  RCC_AHBENR_GPIOE                  	((u32)0x00000010)        	/*!< GPIOE clock enable */
#define  RCC_AHBENR_GPIOF                  	((u32)0x00000020)        	/*!< GPIOF clock enable */
#define  RCC_AHBENR_DMA0                  	((u32)0x00000100)        	/*!< DMA0 clock enable */
#define  RCC_AHBENR_ALU                  	((u32)0x00001000)        	/*!< ALU clock enable */
#define  RCC_AHBENR_AHB                  	((u32)0x80000000)        	/*!< AHB clock enable */

/********************  Bits definition for RCC_CFGR register  ******************/
#define  RCC_CFGR_SCW                  		((u32)0x0000000F)        	/*!< System clock switch */
#define  RCC_CFGR_SCWK                  	((u32)0x00000100)        	/*!< System clock after wakeup */
#define  RCC_CFGR_HPRE                  	((u32)0x001F0000)        	/*!< HCLK prescaler */
#define  RCC_CFGR_PPRE                  	((u32)0x1F000000)        	/*!< PCLK prescaler */

/********************  Bits definition for RCC_MCOR register  ******************/
#define  RCC_MCOR_COSRC                  	((u32)0x0000000F)        	/*!< Clock output source */
#define  RCC_MCOR_COPRE                  	((u32)0x000000F0)        	/*!< Clock output prescaler */

/********************  Bits definition for RCC_PCSCR1 register  ******************/
#define  RCC_PCSCR1_IWDG                  	((u32)0x00000003)        	/*!< IWDG clock source selection   	*/
#define  RCC_PCSCR1_TIM4                  	((u32)0x0000000C)        	/*!< TIM4 clock source selection 	*/
#define  RCC_PCSCR1_RTC                  	((u32)0x00000030)        	/*!< RTC clock source selection    	*/
#define  RCC_PCSCR1_BEEP                  	((u32)0x00000300)        	/*!< BEEP clock source selection 	*/
#define  RCC_PCSCR1_LCD                  	((u32)0x00000C00)        	/*!< LCD clock source selection    	*/
#define  RCC_PCSCR1_LPTIM                  	((u32)0x000F0000)        	/*!< LPTIM clock source selection 	*/

/********************  Bits definition for RCC_PCSCR2 register  ******************/
#define  RCC_PCSCR2_LPUART0              	((u32)0x0000000F)        	/*!< LPUART0 clock source selection */
#define  RCC_PCSCR2_LPUART1                 ((u32)0x000000F0)        	/*!< LPUART1 clock source selection */

/********************  Bits definition for RCC_HSICR register  ******************/
#define  RCC_HSICR_EN                  		((u32)0x00000001)        	/*!< HSI enable */

/********************  Bits definition for RCC_LSICR register  ******************/
#define  RCC_LSICR_EN                  		((u32)0x00000001)        	/*!< LSI enable */
#define  RCC_LSICR_FREQS                  	((u32)0x00000002)        	/*!< LSI frequency selection */
#define  RCC_LSICR_RDY                  	((u32)0x80000000)        	/*!< LSI ready flag */

/********************  Bits definition for RCC_HSECR register  ******************/
#define  RCC_HSECR_EN                  		((u32)0x00000001)        	/*!< HSE enable */
#define  RCC_HSECR_ECE                  	((u32)0x00000002)        	/*!< External clock enable */
#define  RCC_HSECR_AAC                  	((u32)0x00000004)        	/*!< Automatic amplitude control */
#define  RCC_HSECR_FREQ                  	((u32)0x00000008)        	/*!< HSE frequency range */
#define  RCC_HSECR_FDE                  	((u32)0x00000010)        	/*!< HSE fail detecting enable */
#define  RCC_HSECR_LPFB                  	((u32)0x00000100)        	/*!< Low pass filter bypass */
#define  RCC_HSECR_LPF                  	((u32)0x00003000)        	/*!< Low pass filter */
#define  RCC_HSECR_CAC                  	((u32)0x00070000)        	/*!< Current adjustment control */
#define  RCC_HSECR_FDR                  	((u32)0x00700000)        	/*!< Feedback resistance selection */
#define  RCC_HSECR_FAIL                  	((u32)0x40000000)        	/*!< HSE failure flag */
#define  RCC_HSECR_RDY                  	((u32)0x80000000)        	/*!< HSE ready flag */

/********************  Bits definition for RCC_LSECR register  ******************/
#define  RCC_LSECR_EN                  		((u32)0x00000001)        	/*!< LSE enable */
#define  RCC_LSECR_ECE                  	((u32)0x00000002)        	/*!< External clock enable */
#define  RCC_LSECR_LPFB                  	((u32)0x00000100)        	/*!< Low pass filter bypass */
#define  RCC_LSECR_LPF                  	((u32)0x00003000)        	/*!< Low pass filter */
#define  RCC_LSECR_CAC                  	((u32)0x000F0000)        	/*!< Current adjustment control */
#define  RCC_LSECR_FDR                  	((u32)0x00700000)        	/*!< Feedback resistance selection */
#define  RCC_LSECR_CGC                  	((u32)0x01000000)        	/*!< Current gear control */
#define  RCC_LSECR_RDY                  	((u32)0x80000000)        	/*!< LSE ready flag */

/********************  Bits definition for RCC_PLLCFGR1 register  ******************/
#define  RCC_PLLCFGR1_EN                  	((u32)0x00000001)        	/*!< PLL enable */
#define  RCC_PLLCFGR1_DELAY                 ((u32)0x00000002)        	/*!< Delay */
#define  RCC_PLLCFGR1_SRC                  	((u32)0x00000030)        	/*!< PLL entry clock source */
#define  RCC_PLLCFGR1_LPFB                 	((u32)0x00000100)        	/*!< Low pass filter bypass */
#define  RCC_PLLCFGR1_LPF                  	((u32)0x00003000)        	/*!< Low pass filter */
#define  RCC_PLLCFGR1_RDY                  	((u32)0x80000000)        	/*!< Ready flag */

/********************  Bits definition for RCC_PLLCFGR2 register  ******************/
#define  RCC_PLLCFGR2_MUL                  	((u32)0x000000FF)        	/*!< PLL multiplication factor */
#define  RCC_PLLCFGR2_DIV                  	((u32)0x0000FF00)        	/*!< PLL division factor */

/******************************************************************************/
/*                                                                            */
/*                   Sysytem Configuration Controller(SYSCFG)                 */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for SYSCFG_SR1 register  ******************/
#define  SYSCFG_SR1_IAP               		((u32)0x00000002)        	/*!< IAP enable flag */
#define  SYSCFG_SR1_VTOS               		((u32)0x00000004)        	/*!< Interrupt vector table location selection flag */

/*******************  Bit definition for SYSCFG_IAPAR register  ******************/
#define  SYSCFG_IAPAR_ADDR              	((u32)0xFFFFFFFF)        	/*!< IAP address */

/*******************  Bit definition for SYSCFG_SVTOR register  ******************/
#define  SYSCFG_SVTOR_ADDR              	((u32)0xFFFFFFFF)        	/*!< SRAM interrupt vector table address */

/*******************  Bit definition for SYSCFG_CR register  ******************/
#define  SYSCFG_CR_IAPE              		((u32)0x000000FF)        	/*!< IAP enable control */
#define  SYSCFG_CR_KEY              		((u32)0xFFFFFFFF)        	/*!< Key value */

/******************************************************************************/
/*                                                                            */
/*               Alternate Function Input/Output Ports (AFIO)                 */
/*                                                                            */
/******************************************************************************/
/****************** Bit definition for AFIO_AFSR1 register  ********************/
#define  AFIO_AFSR1_IO0            			((u32)0x0000000F)           /*!<Port x pin 0 digtal alternate function  */
#define  AFIO_AFSR1_IO1            			((u32)0x000000F0)           /*!<Port x pin 1 digtal alternate function  */
#define  AFIO_AFSR1_IO2            			((u32)0x00000F00)           /*!<Port x pin 2 digtal alternate function  */
#define  AFIO_AFSR1_IO3            			((u32)0x0000F000)           /*!<Port x pin 3 digtal alternate function  */
#define  AFIO_AFSR1_IO4            			((u32)0x000F0000)           /*!<Port x pin 4 digtal alternate function  */
#define  AFIO_AFSR1_IO5            			((u32)0x00F00000)           /*!<Port x pin 5 digtal alternate function  */
#define  AFIO_AFSR1_IO6            			((u32)0x0F000000)           /*!<Port x pin 6 digtal alternate function  */
#define  AFIO_AFSR1_IO7            			((u32)0xF0000000)         	/*!<Port x pin 7 digtal alternate function  */

/****************** Bit definition for AFIO_AFSR2 register  ********************/
#define  AFIO_AFSR2_IO8             		((u32)0x0000000F)        	/*!<Port x pin 8 digtal alternate function  */
#define  AFIO_AFSR2_IO9             		((u32)0x000000F0)           /*!<Port x pin 9 digtal alternate function  */
#define  AFIO_AFSR2_IO10            		((u32)0x00000F00)           /*!<Port x pin 10 digtal alternate function  */
#define  AFIO_AFSR2_IO11            		((u32)0x0000F000)           /*!<Port x pin 11 digtal alternate function  */
#define  AFIO_AFSR2_IO12            		((u32)0x000F0000)           /*!<Port x pin 12 digtal alternate function  */
#define  AFIO_AFSR2_IO13            		((u32)0x00F00000)           /*!<Port x pin 13 digtal alternate function  */
#define  AFIO_AFSR2_IO14            		((u32)0x0F000000)           /*!<Port x pin 14 digtal alternate function  */
#define  AFIO_AFSR2_IO15            		((u32)0xF0000000)           /*!<Port x pin 15 digtal alternate function  */

/******************************************************************************/
/*                                                                            */
/*                       Direct Memory Access (DMA)                           */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for DMA_CySBAR register  ********************/
#define  DMA_CSBAR_SBA                  	((u32)0xFFFFFFFF)           /*!< DMA source base address */

/*******************  Bit definition for DMA_CyDBAR register  ********************/
#define  DMA_CDBAR_DBA                      ((u32)0xFFFFFFFF)           /*!< DMA destination base address */

/*******************  Bit definition for DMA_CyNDTR register  ********************/
#define  DMA_CNDTR_NDT                      ((u32)0x0000FFFF)           /*!< Number of data to transfer */

/*******************  Bit definition for DMA_CyCR register  ********************/
#define  DMA_CCR_EN                         ((u32)0x00000001)           /*!< DMA channel enable */
#define  DMA_CCR_SINC                       ((u32)0x00000002)          	/*!< Source address increment control */
#define  DMA_CCR_DINC                       ((u32)0x00000004)           /*!< Destination address increment control */
#define  DMA_CCR_SSIZE                      ((u32)0x00000018)           /*!< Source data size  */
#define  DMA_CCR_BURST                      ((u32)0x000000E0)           /*!< Burst transfer configuration */
#define  DMA_CCR_DSIZE                      ((u32)0x00000300)           /*!< Destination data size */
#define  DMA_CCR_CPC                        ((u32)0x00000C00)           /*!< Channel priority config */
#define  DMA_CCR_DIR                        ((u32)0x00003000)           /*!< Direction */
#define  DMA_CCR_CIRC                       ((u32)0x00004000)           /*!< Circular mode */

/*******************  Bit definition for DMA_CyCSAR register  ********************/
#define  DMA_CCSAR_CSA                      ((u32)0xFFFFFFFF)           /*!< DMA current source address */

/*******************  Bit definition for DMA_CyCDAR register  ********************/
#define  DMA_CCDAR_CDA                      ((u32)0xFFFFFFFF)           /*!< DMA current destination address */

/*******************  Bit definition for DMA_CyCNTR register  ********************/
#define  DMA_CCNTR_CNT                      ((u32)0x0000FFFF)           /*!< DMA Counter value */

/*******************  Bit definition for DMA_IER register  ********************/
#define  DMA_IER_TC0E                       ((u32)0x00000001)           /*!< DMA channel 0 transfer complete interrupt enable */
#define  DMA_IER_TC1E                       ((u32)0x00000002)           /*!< DMA channel 1 transfer complete interrupt enable */
#define  DMA_IER_TC2E                       ((u32)0x00000004)           /*!< DMA channel 2 transfer complete interrupt enable */
#define  DMA_IER_TC3E                       ((u32)0x00000008)           /*!< DMA channel 3 transfer complete interrupt enable */
#define  DMA_IER_TC4E                       ((u32)0x00000010)           /*!< DMA channel 4 transfer complete interrupt enable */
#define  DMA_IER_TC5E                       ((u32)0x00000020)           /*!< DMA channel 5 transfer complete interrupt enable */
#define  DMA_IER_TE0E                       ((u32)0x00000100)           /*!< DMA channel 0 transfer error interrupt enable */
#define  DMA_IER_TE1E                       ((u32)0x00000200)           /*!< DMA channel 1 transfer error interrupt enable */
#define  DMA_IER_TE2E                       ((u32)0x00000400)           /*!< DMA channel 2 transfer error interrupt enable */
#define  DMA_IER_TE3E                       ((u32)0x00000800)           /*!< DMA channel 3 transfer error interrupt enable */
#define  DMA_IER_TE4E                       ((u32)0x00001000)           /*!< DMA channel 4 transfer error interrupt enable */
#define  DMA_IER_TE5E                       ((u32)0x00002000)           /*!< DMA channel 5 transfer error interrupt enable */
#define  DMA_IER_TH0E                       ((u32)0x00010000)           /*!< DMA channel 0 transfer over half interrupt enable */
#define  DMA_IER_TH1E                       ((u32)0x00020000)           /*!< DMA channel 1 transfer over half interrupt enable */
#define  DMA_IER_TH2E                       ((u32)0x00040000)           /*!< DMA channel 2 transfer over half interrupt enable */
#define  DMA_IER_TH3E                       ((u32)0x00080000)           /*!< DMA channel 3 transfer over half interrupt enable */
#define  DMA_IER_TH4E                       ((u32)0x00100000)           /*!< DMA channel 4 transfer over half interrupt enable */
#define  DMA_IER_TH5E                       ((u32)0x00200000)           /*!< DMA channel 5 transfer over half interrupt enable */

/*******************  Bit definition for DMA_SR register  ********************/
#define  DMA_SR_TC0F                        ((u32)0x00000001)           /*!< DMA channel 0 transfer complete interrupt flag */
#define  DMA_SR_TC1F                        ((u32)0x00000002)           /*!< DMA channel 1 transfer complete interrupt flag */
#define  DMA_SR_TC2F                        ((u32)0x00000004)           /*!< DMA channel 2 transfer complete interrupt flag */
#define  DMA_SR_TC3F                        ((u32)0x00000008)           /*!< DMA channel 3 transfer complete interrupt flag */
#define  DMA_SR_TC4F                        ((u32)0x00000010)           /*!< DMA channel 4 transfer complete interrupt flag */
#define  DMA_SR_TC5F                        ((u32)0x00000020)           /*!< DMA channel 5 transfer complete interrupt flag */
#define  DMA_SR_TE0F                        ((u32)0x00000100)           /*!< DMA channel 0 transfer error interrupt flag */
#define  DMA_SR_TE1F                        ((u32)0x00000200)           /*!< DMA channel 1 transfer error interrupt flag */
#define  DMA_SR_TE2F                        ((u32)0x00000400)           /*!< DMA channel 2 transfer error interrupt flag */
#define  DMA_SR_TE3F                        ((u32)0x00000800)           /*!< DMA channel 3 transfer error interrupt flag */
#define  DMA_SR_TE4F                        ((u32)0x00001000)           /*!< DMA channel 4 transfer error interrupt flag */
#define  DMA_SR_TE5F                        ((u32)0x00002000)           /*!< DMA channel 5 transfer error interrupt flag */
#define  DMA_SR_TH0F                        ((u32)0x00010000)           /*!< DMA channel 0 transfer over half flag */
#define  DMA_SR_TH1F                        ((u32)0x00020000)           /*!< DMA channel 1 transfer over half flag */
#define  DMA_SR_TH2F                        ((u32)0x00040000)           /*!< DMA channel 2 transfer over half flag */
#define  DMA_SR_TH3F                        ((u32)0x00080000)           /*!< DMA channel 3 transfer over half flag */
#define  DMA_SR_TH4F                        ((u32)0x00100000)           /*!< DMA channel 4 transfer over half flag */
#define  DMA_SR_TH5F                        ((u32)0x00200000)           /*!< DMA channel 5 transfer over half flag */
#define  DMA_SR_CEF                         ((u32)0x80000000)           /*!< DMA channel configuration error interrupt flag */

/*******************  Bit definition for DMA_CHAPCR1 register  ********************/
#define  DMA_CHAPCR1_CH0SEL                 ((u32)0x0000003F)           /*!< DMA channel 0 peripheral selection */
#define  DMA_CHAPCR1_CH1SEL                 ((u32)0x00003F00)           /*!< DMA channel 1 peripheral selection */
#define  DMA_CHAPCR1_CH2SEL                 ((u32)0x003F0000)           /*!< DMA channel 2 peripheral selection */
#define  DMA_CHAPCR1_CH3SEL                 ((u32)0x3F000000)           /*!< DMA channel 3 peripheral selection */

/*******************  Bit definition for DMA_CHAPCR2 register  ********************/
#define  DMA_CHAPCR2_CH4SEL                 ((u32)0x0000003F)          	/*!< DMA channel 4 peripheral selection */
#define  DMA_CHAPCR2_CH5SEL                 ((u32)0x00003F00)          	/*!< DMA channel 5 peripheral selection */

/******************************************************************************/
/*                                                                            */
/*                      Direct Digital Synthesizer (DDS)                      */
/*                                                                            */
/******************************************************************************/
/********************  Bits definition for DDS_CR1 register  ******************/
#define  DDS_CR1_EN                         ((u32)0x00000001)      		/*!< DDS enable */
#define  DDS_CR1_RST                        ((u32)0x00000002)      		/*!< DDS reset enable */
#define  DDS_CR1_WDMS                     	((u32)0x0000000C)      		/*!< Wave data mode selection */
#define  DDS_CR1_IQMS                    	((u32)0x00000030)      		/*!< I/Q signal mode selection */

/********************  Bits definition for DDS_PSCR register  *****************/
#define  DDS_PSCR_PSC                     	((u32)0x000001FF)      		/*!< DDS prescaler */

/********************  Bits definition for DDS_WGPSCR register  ***************/
#define  DDS_WGPSCR_PSC                 	((u32)0x000001FF)      		/*!< Wave generator prescaler */

/********************  Bits definition for DDS_WDSAR register  ****************/
#define  DDS_WDSAR_ADDR                 	((u32)0xFFFFFFFE)      		/*!< Wave data start address */

/********************  Bits definition for DDS_WDLENR register  ***************/
#define  DDS_WDLENR_LEN                 	((u32)0x000001FF)      		/*!< Wave data length */

/********************  Bits definition for DDS_WOFR register  *****************/
#define  DDS_WOFR_OFFSET                    ((u32)0x00000FFF)      		/*!< Wave offset */

/********************  Bits definition for DDS_SR register  *******************/
#define  DDS_SR_I                    		((u32)0x00000001)      		/*!< In-Phase status flag */
#define  DDS_SR_Q                    		((u32)0x00000002)      		/*!< Quadrature status flag */

/******************************************************************************/
/*                                                                            */
/*               Sigma Delta Analog to Digital Converter (SDADC)              */
/*                                                                            */
/******************************************************************************/
/********************  Bits definition for SDADC_CR1 register  ****************/
#define  SDADC_CR1_EN                      	((u32)0x00000001)      		/*!< SDADC enable */
#define  SDADC_CR1_MODE                    	((u32)0x00030000)      		/*!< SDADC working mode */
#define  SDADC_CR1_CCS                      ((u32)0x00100000)      		/*!< SDADC chopper clock source selection */

/********************  Bits definition for SDADC_CR2 register  ****************/
#define  SDADC_CR2_DMAE                    	((u32)0x00000004)         	/*!< SDADC DMA enable */

/********************  Bits definition for SDADC_CR4 register  ****************/
#define  SDADC_CR4_FOSR                    	((u32)0x0001FFFF)         	/*!< CIC filter oversampling ratio(decimation rate) */
#define  SDADC_CR4_FDRF                    	((u32)0x1F000000)         	/*!< CIC filter data maximum output resolution factor */

/********************  Bits definition for SDADC_CFGR1 register  **************/
#define  SDADC_CFGR1_RVSPS                 	((u32)0x00000003)         	/*!< Reference voltage source positive selection */
#define  SDADC_CFGR1_BGE                   	((u32)0x00000010)         	/*!< BGREF enable */

/********************  Bits definition for SDADC_CFGR2 register  **************/
#define  SDADC_CFGR2_TSE                   	((u32)0x00000100)        	/*!< Temperature sensor enable */
#define  SDADC_CFGR2_DISC                  	((u32)0x00007000)        	/*!< Number of conversions to be Discarded */
#define  SDADC_CFGR2_CHPS                  	((u32)0x003F0000)        	/*!< SDADC channel positive selection */
#define  SDADC_CFGR2_CHNS                  	((u32)0x3F000000)        	/*!< SDADC channel negative selection */

/********************  Bits definition for SDADC_CFGR3 register  **************/
#define  SDADC_CFGR3_PSC                    ((u32)0x0000007F)        	/*!< SDADC prescaler */
#define  SDADC_CFGR3_CDIV                  	((u32)0x00070000)        	/*!< Chopper clock divider */

/********************  Bits definition for SDADC_IER register  ****************/
#define  SDADC_IER_EOCIE                    ((u32)0x00000002)         	/*!< Regular channel end of conversion interrupt enable */

/********************  Bits definition for SDADC_SR register  *****************/
#define  SDADC_SR_EOC                      	((u32)0x00000002)         	/*!< Regular channel end of conversion flag */

/********************  Bits definition for SDADC_PGACR register  **************/
#define  SDADC_PGACR_PGA0E                  ((u32)0x00000001)        	/*!< PGA0 enable */
#define  SDADC_PGACR_GAIN0                 	((u32)0x0000000E)        	/*!< PGA0 gain value selection */
#define  SDADC_PGACR_PCM0                  	((u32)0x00000030)        	/*!< PGA0 chopper working mode selection */
#define  SDADC_PGACR_PGA1E                 	((u32)0x00000100)        	/*!< PGA1 enable */
#define  SDADC_PGACR_GAIN1                 	((u32)0x00000E00)        	/*!< PGA1 gain value selection */

/********************  Bits definition for SDADC_DR register  *****************/
#define  SDADC_DR_DATA                     	((u32)0x00FFFFFF)         	/*!< SDADC data */

/********************  Bits definition for SDADC_DOFR register  ***************/
#define  SDADC_DOFR_OFFSET                 	((u32)0x00FFFFFF)        	/*!< SDADC data offset value */
#define  SDADC_DOFR_KEY                   	((u32)0xFF000000)        	/*!< Key value */

/********************  Bits definition for SDADC_CCR1 register  ***************/
#define  SDADC_CCR1_CHOPE                  	((u32)0x00000003)        	/*!< SDADC chopper enable */

/******************************************************************************/
/*                                                                            */
/*                      Analog to Digital Converter (ADC)                     */
/*                                                                            */
/******************************************************************************/
/********************  Bits definition for ADC_CR1 register  ******************/
#define  ADC_CR1_EN                      	((u32)0x00000001)      		/*!< ADC enable */
#define  ADC_CR1_SOC                        ((u32)0x00000010)         	/*!< ADC start of regular conversion  */
#define  ADC_CR1_JSOC                       ((u32)0x00000100)         	/*!< ADC start of injected conversion */

/********************  Bits definition for ADC_CR2 register  ******************/
#define  ADC_CR2_MODE                       ((u32)0x00000001)         	/*!< ADC mode */
#define  ADC_CR2_ALIGN                      ((u32)0x00000002)         	/*!< ADC ALIGN */
#define  ADC_CR2_DMAE                       ((u32)0x00000004)         	/*!< ADC DMA enable */
#define  ADC_CR2_CONV                       ((u32)0x00000300)         	/*!< ADC conversion control */
#define  ADC_CR2_SCCNT                      ((u32)0x00007000)         	/*!< Single conversion count control */
#define  ADC_CR2_TRIGS                      ((u32)0x000F0000)         	/*!< ADC trigger source selection */
#define  ADC_CR2_TIMS                       ((u32)0x01F00000)         	/*!< ADC timer trigger source selection */
#define  ADC_CR2_DFC                        ((u32)0x70000000)         	/*!< Digital filter configuration */

/********************  Bits definition for ADC_CR3 register  ******************/
#define  ADC_CR3_SCANE                    	((u32)0x00000001)         	/*!< ADC regular scan enable */
#define  ADC_CR3_JSCANE                     ((u32)0x00000002)         	/*!< ADC injected scan enable */
#define  ADC_CR3_JTRIGS                     ((u32)0x000000F0)         	/*!< ADC injected trigger source selection */
#define  ADC_CR3_JTIMS                      ((u32)0x00001F00)         	/*!< ADC injected timer trigger source selection */
#define  ADC_CR3_SCNT                       ((u32)0x001F0000)         	/*!< ADC regular scan channel number configuration */
#define  ADC_CR3_JSCNT                      ((u32)0x1F000000)         	/*!< ADC injected scan channel number configuration */

/********************  Bits definition for ADC_CFGR1 register  ******************/
#define  ADC_CFGR1_RVSPS                    ((u32)0x00000003)         	/*!< Reference voltage source positive selection */
#define  ADC_CFGR1_BGE                      ((u32)0x00000010)         	/*!< BGREF enable */
#define  ADC_CFGR1_BGS                      ((u32)0x00000020)         	/*!< BGREF source selection */
#define  ADC_CFGR1_BGOE                     ((u32)0x00000040)         	/*!< BGREF output */
#define  ADC_CFGR1_KEY                      ((u32)0x0000FF00)         	/*!< Key value */
#define  ADC_CFGR1_BGTF                     ((u32)0x00000100)         	/*!< BGREF offset trimming value source flag */
#define  ADC_CFGR1_TRIM2                    ((u32)0x003F0000)         	/*!< BG2v0 offset trimming value */
#define  ADC_CFGR1_TRIM4                    ((u32)0x3F000000)         	/*!< BG4v0 offset trimming value */

/********************  Bits definition for ADC_CFGR2 register  ******************/
#define  ADC_CFGR2_INNS                     ((u32)0x00000001)        	/*!< ADC Input negative selection */
#define  ADC_CFGR2_TSE                      ((u32)0x00000100)        	/*!< Temperature sensor enable */
#define  ADC_CFGR2_CHS                      ((u32)0x003F0000)        	/*!< ADC channel selection */

/********************  Bits definition for ADC_CFGR3 register  ******************/
#define  ADC_CFGR3_PSC                      ((u32)0x000000FF)         	/*!< ADC prescaler */
#define  ADC_CFGR3_RDTC                     ((u32)0x0000FF00)        	/*!< ADC ready time control */
#define  ADC_CFGR3_SETUP                    ((u32)0x00FF0000)        	/*!< ADC channel setup time */
#define  ADC_CFGR3_SMP                      ((u32)0xFF000000)         	/*!< ADC sample time */

/********************  Bits definition for ADC_IER register  ******************/
#define  ADC_IER_EOCIE                      ((u32)0x00000002)         	/*!< ADC regular channel end of conversion interrupt enable */
#define  ADC_IER_JEOCIE                     ((u32)0x00000004)         	/*!< ADC injected channel end of conversion interrupt enable */
#define  ADC_IER_EOSIE                      ((u32)0x00000010)         	/*!< ADC regular channel end of scan interrupt enable */
#define  ADC_IER_JEOSIE                     ((u32)0x00000020)         	/*!< ADC injected channel end of scan interrupt enable*/
#define  ADC_IER_AWDIE                      ((u32)0x00000100)         	/*!< ADC Analog watchdog interrupt enable */

/********************  Bits definition for ADC_SR register  ******************/
#define  ADC_SR_RDY                         ((u32)0x00000001)         	/*!< ADC ready flag */
#define  ADC_SR_EOC                         ((u32)0x00000002)         	/*!< ADC regular channel end of conversion flag */
#define  ADC_SR_JEOC                        ((u32)0x00000004)         	/*!< ADC injected channel end of conversion flag */
#define  ADC_SR_EOS                         ((u32)0x00000010)         	/*!< ADC regular channel end of scan flag */
#define  ADC_SR_JEOS                        ((u32)0x00000020)         	/*!< ADC injected channel end of scan flag */
#define  ADC_SR_AWD                         ((u32)0x00000100)         	/*!< Analog watchdog flag */
#define  ADC_SR_AWDCH                       ((u32)0x003F0000)         	/*!< Analog watchdog channel selection */

/********************  Bits definition for ADC_AWDCR register  ******************/
#define  ADC_AWDCR_AWDE                     ((u32)0x00000001)         	/*!< Analog watchdog enable */
#define  ADC_AWDCR_CHC                      ((u32)0x00000002)         	/*!< Analog watchdog channel control */
#define  ADC_AWDCR_CHS                      ((u32)0x003F0000)         	/*!< Analog watchdog channel selection */

/********************  Bits definition for ADC_AWDTR register  ******************/
#define  ADC_AWDTR_LT                       ((u32)0x0000FFFF)         	/*!< Analog watchdog lower threshold */
#define  ADC_AWDTR_HT                       ((u32)0xFFFF0000)         	/*!< Analog watchdog higher threshold */

/********************  Bits definition for ADC_DR register  ******************/
#define  ADC_DR_DATA                        ((u32)0x0000FFFF)         	/*!< ADC data */

/********************  Bits definition for ADC_TSCALR1 register  ******************/
#define  ADC_TSCALR1_TSCAL1                 ((u32)0xFFFFFFFF)         	/*!< Temperature sensor calibration value 1 */

/********************  Bits definition for ADC_TSCALR2 register  ******************/
#define  ADC_TSCALR2_TSCAL2                 ((u32)0xFFFFFFFF)         	/*!< Temperature sensor calibration value 2 */

/********************  Bits definition for ADC_SCHR1 register  ******************/
#define  ADC_SCHR1_CH0                      ((u32)0x0000003F)         	/*!< ADC scan channel 0 selection */
#define  ADC_SCHR1_CH1                      ((u32)0x00003F00)         	/*!< ADC scan channel 1 selection */
#define  ADC_SCHR1_CH2                      ((u32)0x003F0000)         	/*!< ADC scan channel 2 selection */
#define  ADC_SCHR1_CH3                      ((u32)0x3F000000)         	/*!< ADC scan channel 3 selection */

/********************  Bits definition for ADC_SCHR2 register  ******************/
#define  ADC_SCHR2_CH4                    	((u32)0x0000003F)         	/*!< ADC scan channel 4 selection */
#define  ADC_SCHR2_CH5                      ((u32)0x00003F00)         	/*!< ADC scan channel 5 selection */
#define  ADC_SCHR2_CH6                      ((u32)0x003F0000)         	/*!< ADC scan channel 6 selection */
#define  ADC_SCHR2_CH7                      ((u32)0x3F000000)         	/*!< ADC scan channel 7 selection */

/********************  Bits definition for ADC_SCHR3 register  ******************/
#define  ADC_SCHR3_CH8                      ((u32)0x0000003F)         	/*!< ADC scan channel 8 selection */
#define  ADC_SCHR3_CH9                      ((u32)0x00003F00)         	/*!< ADC scan channel 9 selection */
#define  ADC_SCHR3_CH10                     ((u32)0x003F0000)         	/*!< ADC scan channel 10 selection */
#define  ADC_SCHR3_CH11                     ((u32)0x3F000000)         	/*!< ADC scan channel 11 selection */

/********************  Bits definition for ADC_SCHR4 register  ******************/
#define  ADC_SCHR4_CH12                     ((u32)0x0000003F)         	/*!< ADC scan channel 12 selection */
#define  ADC_SCHR4_CH13                     ((u32)0x00003F00)         	/*!< ADC scan channel 13 selection */
#define  ADC_SCHR4_CH14                     ((u32)0x003F0000)         	/*!< ADC scan channel 14 selection */
#define  ADC_SCHR4_CH15                     ((u32)0x3F000000)         	/*!< ADC scan channel 15 selection */

/********************  Bits definition for ADC_SCHR5 register  ******************/
#define  ADC_SCHR5_CH16                     ((u32)0x0000003F)         	/*!< ADC scan channel 16 selection */
#define  ADC_SCHR5_CH17                     ((u32)0x00003F00)         	/*!< ADC scan channel 17 selection */
#define  ADC_SCHR5_CH18                     ((u32)0x003F0000)         	/*!< ADC scan channel 18 selection */
#define  ADC_SCHR5_CH19                     ((u32)0x3F000000)         	/*!< ADC scan channel 19 selection */

/********************  Bits definition for ADC_JSCHR1 register  ******************/
#define  ADC_JSCHR1_CH0                    	((u32)0x0000003F)         	/*!< ADC inject scan channel 0 selection */
#define  ADC_JSCHR1_CH1                    	((u32)0x00003F00)         	/*!< ADC inject scan channel 1 selection */
#define  ADC_JSCHR1_CH2                    	((u32)0x003F0000)         	/*!< ADC inject scan channel 2 selection */
#define  ADC_JSCHR1_CH3                    	((u32)0x3F000000)         	/*!< ADC inject scan channel 3 selection */

/********************  Bits definition for ADC_JSCHR2 register  ******************/
#define  ADC_JSCHR2_CH4                    	((u32)0x0000003F)         	/*!< ADC inject scan channel 4 selection */
#define  ADC_JSCHR2_CH5                    	((u32)0x00003F00)         	/*!< ADC inject scan channel 5 selection */
#define  ADC_JSCHR2_CH6                     ((u32)0x003F0000)         	/*!< ADC inject scan channel 6 selection */
#define  ADC_JSCHR2_CH7                     ((u32)0x3F000000)         	/*!< ADC inject scan channel 7 selection */

/********************  Bits definition for ADC_JSCHR3 register  ******************/
#define  ADC_JSCHR3_CH8                     ((u32)0x0000003F)         	/*!< ADC inject scan channel 8 selection */
#define  ADC_JSCHR3_CH9                     ((u32)0x00003F00)         	/*!< ADC inject scan channel 9 selection */
#define  ADC_JSCHR3_CH10                    ((u32)0x003F0000)         	/*!< ADC inject scan channel 10 selection */
#define  ADC_JSCHR3_CH11                    ((u32)0x3F000000)         	/*!< ADC inject scan channel 11 selection */

/********************  Bits definition for ADC_JSCHR4 register  ******************/
#define  ADC_JSCHR4_CH12                    ((u32)0x0000003F)         	/*!< ADC inject scan channel 12 selection */
#define  ADC_JSCHR4_CH13                    ((u32)0x00003F00)         	/*!< ADC inject scan channel 13 selection */
#define  ADC_JSCHR4_CH14                    ((u32)0x003F0000)         	/*!< ADC inject scan channel 14 selection */
#define  ADC_JSCHR4_CH15                    ((u32)0x3F000000)         	/*!< ADC inject scan channel 15 selection */

/********************  Bits definition for ADC_JSCHR5 register  ******************/
#define  ADC_JSCHR5_CH16                    ((u32)0x0000003F)         	/*!< ADC inject scan channel 16 selection */
#define  ADC_JSCHR5_CH17                    ((u32)0x00003F00)         	/*!< ADC inject scan channel 17 selection */
#define  ADC_JSCHR5_CH18                    ((u32)0x003F0000)         	/*!< ADC inject scan channel 18 selection */
#define  ADC_JSCHR5_CH19                   	((u32)0x3F000000)         	/*!< ADC inject scan channel 19 selection */

/********************  Bits definition for ADC_SCHDRy register  ******************/
#define  ADC_SCHDR_DATA                    	((u32)0x0000FFFF)         	/*!< ADC scan channel data */

/********************  Bits definition for ADC_JSCHDRy register  *****************/
#define  ADC_JSCHDR_DATA                   	((u32)0x0000FFFF)         	/*!< ADC inject scan channel data */

/******************************************************************************/
/*                                                                            */
/*                      Digital to Analog Converter (DAC)                     */
/*                                                                            */
/******************************************************************************/
/********************  Bits definition for DAC_CR1 register  ******************/
#define  DAC_CR1_EN                         ((u32)0x00000001)      		/*!< DAC enable */
#define  DAC_CR1_ALIGN                      ((u32)0x00000010)         	/*!< DAC data align */
#define  DAC_CR1_RVSPS                      ((u32)0x00000300)         	/*!< DAC Reference voltage source positive selection */
#define  DAC_CR1_PM                      	((u32)0x00010000)         	/*!< DAC power mode */
#define  DAC_CR1_OE                      	((u32)0x00100000)         	/*!< DAC output enable */

/********************  Bits definition for DAC_DR register  ******************/
#define  DAC_DR_DATA                        ((u32)0x0000FFFF)         	/*!< DAC data */

/********************  Bits definition for DAC_CR2 register  ******************/
#define  DAC_CR2_OFE                        ((u32)0x00000001)      		/*!< DAC output MFB filter enable */
#define  DAC_CR2_FCFS                      	((u32)0x00000F00)         	/*!< MFB filter cutoff frequency selection */

/******************************************************************************/
/*                                                                            */
/*                           Advanced Timer (TIM)                             */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for TIM_CR1 register   *******************/
#define  TIM_CR_EN                      	((u32)0x00000001)       	/*!< Counter enable */
#define  TIM_CR_UG                      	((u32)0x00000002)       	/*!< Update generation */
#define  TIM_CR_OPM                    		((u32)0x00000004)       	/*!< One-pulse mode */
#define  TIM_CR_CPC                  		((u32)0x00000010)       	/*!< Compare preloaded control */
#define  TIM_CR_CKD                    		((u32)0x00000700)       	/*!< Sampling clock division */
#define  TIM_CR_BSY                    		((u32)0x00008000)       	/*!< TIM busy */
#define  TIM_CR_DIR                     	((u32)0x00010000)       	/*!< Direction */
#define  TIM_CR_CMS                     	((u32)0x00060000)       	/*!< Center-aligned mode selection */
#define  TIM_CR_DBGE                     	((u32)0x80000000)       	/*!< TIM debug */

/*******************  Bit definition for TIM_MSCR register  *******************/
#define  TIM_MSCR_TIS                    	((u32)0x0000000F)           /*!< Trigger input selection */
#define  TIM_MSCR_SMS                    	((u32)0x00000070)           /*!< Slave mode selection */
#define  TIM_MSCR_SYNC                    	((u32)0x00000100)           /*!< TIM synchronization enable */
#define  TIM_MSCR_TOS                    	((u32)0x0000F000)           /*!< Trigger output selection */
#define  TIM_MSCR_ECE                  		((u32)0x00010000)           /*!< External clock enable */
#define  TIM_MSCR_ETP                    	((u32)0x00020000)           /*!< External trigger polarity */
#define  TIM_MSCR_ETF                    	((u32)0x07000000)           /*!< External trigger filter */
#define  TIM_MSCR_ETPSC                   	((u32)0x70000000)           /*!< External trigger prescaler */

/*******************  Bit definition for TIM_PCR register  *******************/
#define  TIM_PCR_CC1P                     	((u32)0x00000001)           /*!< Capture/Compare 1 polarity */
#define  TIM_PCR_C1NP                     	((u32)0x00000002)           /*!< Compare 1 complementary output polarity */
#define  TIM_PCR_CC2P                     	((u32)0x00000010)           /*!< Capture/Compare 2 polarity */
#define  TIM_PCR_C2NP                     	((u32)0x00000020)           /*!< Compare 2 complementary output polarity */
#define  TIM_PCR_CC3P                     	((u32)0x00000100)           /*!< Capture/Compare 3 polarity */
#define  TIM_PCR_C3NP                     	((u32)0x00000200)           /*!< Compare 3 complementary output polarity */
#define  TIM_PCR_CC4P                     	((u32)0x00001000)           /*!< Capture/Compare 4 polarity */
#define  TIM_PCR_C4NP                     	((u32)0x00002000)           /*!< Compare 4 complementary output polarity */

/*******************  Bit definition for TIM_CAPR1 register  *******************/
#define  TIM_CAPR1_IC1R                  	((u32)0x00000001)       	/*!< input capture 1 rising capture enable */
#define  TIM_CAPR1_IC1F                  	((u32)0x00000002)       	/*!< input capture 1 falling capture enable */
#define  TIM_CAPR1_IC1RC                 	((u32)0x00000004)       	/*!< input capture 1 capture reset counter enable */
#define  TIM_CAPR1_IC1PSC                  	((u32)0x00000030)       	/*!< Input capture 1 prescaler */
#define  TIM_CAPR1_ICS1S                  	((u32)0x000000C0)       	/*!< ICS1 selection */
#define  TIM_CAPR1_IC1S                  	((u32)0x00000700)       	/*!< input capture 1 source selection */
#define  TIM_CAPR1_IC1FR                 	((u32)0x00007000)       	/*!< Input capture 1 filter */
#define  TIM_CAPR1_IC2R                  	((u32)0x00010000)       	/*!< input capture 2 rising capture enable */
#define  TIM_CAPR1_IC2F                  	((u32)0x00020000)       	/*!< input capture 2 falling capture enable */
#define  TIM_CAPR1_IC2RC                  	((u32)0x00040000)       	/*!< input capture 2 capture reset counter enable */
#define  TIM_CAPR1_IC2PSC                 	((u32)0x00300000)       	/*!< Input capture 2 prescaler */
#define  TIM_CAPR1_IC2S                  	((u32)0x07000000)       	/*!< input capture 2 source selection */
#define  TIM_CAPR1_IC2FR                  	((u32)0x70000000)       	/*!< Input capture 2 filter */

/*******************  Bit definition for TIM_CAPR2 register  *******************/
#define  TIM_CAPR2_IC3R                  	((u32)0x00000001)       	/*!< input capture 3 rising capture enable */
#define  TIM_CAPR2_IC3F                  	((u32)0x00000002)       	/*!< input capture 3 falling capture enable */
#define  TIM_CAPR2_IC3RC                 	((u32)0x00000004)       	/*!< input capture 3 capture reset counter enable */
#define  TIM_CAPR2_IC3PSC                  	((u32)0x00000030)       	/*!< Input capture 3 prescaler */
#define  TIM_CAPR2_IC3S                  	((u32)0x00000700)       	/*!< input capture 3 source selection */
#define  TIM_CAPR2_IC3FR                 	((u32)0x00007000)      		/*!< Input capture 3 filter */
#define  TIM_CAPR2_IC4R                  	((u32)0x00010000)      		/*!< input capture 4 rising capture enable */
#define  TIM_CAPR2_IC4F                  	((u32)0x00020000)      		/*!< input capture 4 falling capture enable */
#define  TIM_CAPR2_IC4RC                  	((u32)0x00040000)      		/*!< input capture 4 capture reset counter enable */
#define  TIM_CAPR2_IC4PSC                 	((u32)0x00300000)      		/*!< Input capture 4 prescaler */
#define  TIM_CAPR2_IC4S                  	((u32)0x07000000)      		/*!< input capture 4 source selection */
#define  TIM_CAPR2_IC4FR                  	((u32)0x70000000)      		/*!< Input capture 4 filter */

/*******************  Bit definition for TIM_OCMR register  *******************/
#define  TIM_OCMR_OC1M                   	((u32)0x00000007)       	/*!< output compare 1 mode */
#define  TIM_OCMR_OC1CE                  	((u32)0x00000008)      		/*!< output compare 1 clear enable */
#define  TIM_OCMR_OC2M                   	((u32)0x00000070)      		/*!< output compare 2 mode */
#define  TIM_OCMR_OC2CE                  	((u32)0x00000080)      		/*!< output compare 2 clear enable */
#define  TIM_OCMR_OC3M                   	((u32)0x00000700)      		/*!< output compare 3 mode */
#define  TIM_OCMR_OC3CE                  	((u32)0x00000800)      		/*!< output compare 3 clear enable */
#define  TIM_OCMR_OC4M                   	((u32)0x00007000)      		/*!< output compare 4 mode */
#define  TIM_OCMR_OC4CE                  	((u32)0x00008000)      		/*!< output compare 4 clear enable */
#define  TIM_OCMR_OIS1              		((u32)0x00010000)       	/*!< Output Idle state 1 */
#define  TIM_OCMR_OIS2                 	 	((u32)0x00020000)      		/*!< Output Idle state 2 */
#define  TIM_OCMR_OIS3                 	 	((u32)0x00040000)      		/*!< Output Idle state 3 */
#define  TIM_OCMR_OIS4                   	((u32)0x00080000)      		/*!< Output Idle state 4 */
#define  TIM_OCMR_OIS1N           			((u32)0x00100000)      		/*!< Output Idle state 1 complementary */
#define  TIM_OCMR_OIS2N             		((u32)0x00200000)      		/*!< Output Idle state 2 complementary */
#define  TIM_OCMR_OIS3N             		((u32)0x00400000)     		/*!< Output Idle state 3 complementary */
#define  TIM_OCMR_OIS4N             		((u32)0x00800000)       	/*!< Output Idle state 4 complementary */
#define  TIM_OCMR_OC1NE             		((u32)0x01000000)       	/*!< Compare output 1 complementary output enable */
#define  TIM_OCMR_OC2NE              		((u32)0x02000000)      		/*!< Compare output 2 complementary output enable */
#define  TIM_OCMR_OC3NE                  	((u32)0x04000000)      		/*!< Compare output 3 complementary output enable */
#define  TIM_OCMR_OC4NE              		((u32)0x08000000)      		/*!< Compare output 4 complementary output enable */
#define  TIM_OCMR_OC1E             		   	((u32)0x10000000)      		/*!< Compare output 1 output enable */
#define  TIM_OCMR_OC2E              		((u32)0x20000000)      		/*!< Compare output 2 output enable */
#define  TIM_OCMR_OC3E               		((u32)0x40000000)      		/*!< Compare output 3 output enable */
#define  TIM_OCMR_OC4E              		((u32)0x80000000)      		/*!< Compare output 4 output enable */

/*******************  Bit definition for TIM_BKICR register  *******************/
#define  TIM_BKICR_BKE                 		((u32)0x00000001)       	/*!< Break enable */
#define  TIM_BKICR_BKP                  	((u32)0x00000002)       	/*!< Break polarity */
#define  TIM_BKICR_BKIC                   	((u32)0x00000004)      		/*!< break input control */
#define  TIM_BKICR_BKSC                 	((u32)0x00000010)      		/*!< Break software control enable */
#define  TIM_BKICR_BKINFR					((u32)0x00000700)      		/*!< BKIN Input filter */
#define  TIM_BKICR_LOCKUPE                 	((u32)0x00010000)      		/*!< LOCKUP break input enable */
#define  TIM_BKICR_PVDE                 	((u32)0x00020000)      		/*!< PVD break input enable */
#define  TIM_BKICR_SWE                 		((u32)0x00100000)      		/*!< SW break input enable */
#define  TIM_BKICR_BKINE                 	((u32)0x00200000)      		/*!< BKIN Break input enable */
#define  TIM_BKICR_CMP0E                  	((u32)0x01000000)      		/*!< CMP0 Break input enable */
#define  TIM_BKICR_CMP1E                   	((u32)0x02000000)      		/*!< CMP1 Break input enable */
#define  TIM_BKICR_CMP2E                 	((u32)0x04000000)      		/*!< CMP2 Break input enable */
#define  TIM_BKICR_CMP3E                 	((u32)0x08000000)      		/*!< CMP3 Break input enable */

/*******************  Bit definition for TIM_DTCR register  *******************/
#define  TIM_DTCR_DT                     	((u32)0x00000FFF)       	/*!< Dead-time value */

/*******************  Bit definition for TIM_TDCR1 register  *******************/
#define  TIM_TDCR1_ARDE                   	((u32)0x00000001)       	/*!< auto reload DMA request enable */
#define  TIM_TDCR1_OC1DE                  	((u32)0x00000002)       	/*!< output compare 1 DMA request enable */
#define  TIM_TDCR1_OC2DE                  	((u32)0x00000004)      		/*!< output compare 2 DMA request enable */
#define  TIM_TDCR1_OC3DE                  	((u32)0x00000008)      		/*!< output compare 3 DMA request enable */
#define  TIM_TDCR1_OC4DE                  	((u32)0x00000010)    	 	/*!< output compare 4 DMA request enable */
#define  TIM_TDCR1_IC1RDE                 	((u32)0x00010000)      		/*!< input capture 1 rising edge DMA request enable */
#define  TIM_TDCR1_IC1FDE                 	((u32)0x00020000)    	 	/*!< input capture 1 falling edge DMA request enable */
#define  TIM_TDCR1_IC2RDE                 	((u32)0x00040000)    	 	/*!< input capture 2 rising edge DMA request enable */
#define  TIM_TDCR1_IC2FDE                 	((u32)0x00080000)     		/*!< input capture 2 falling edge DMA request enable */
#define  TIM_TDCR1_IC3RDE                 	((u32)0x00100000)      		/*!< input capture 3 rising edge DMA request enable */
#define  TIM_TDCR1_IC3FDE                 	((u32)0x00200000)    	 	/*!< input capture 3 falling edge DMA request enable */
#define  TIM_TDCR1_IC4RDE                 	((u32)0x00400000)   	 	/*!< input capture 4 rising edge DMA request enable */
#define  TIM_TDCR1_IC4FDE                 	((u32)0x00800000)     		/*!< input capture 4 falling edge DMA request enable */

/*******************  Bit definition for TIM_TDCR2 register  *******************/
#define  TIM_TDCR2_TDE                   	((u32)0x00000002)       	/*!< trigger DMA request enable */
#define  TIM_TDCR2_UDE                  	((u32)0x00000004)       	/*!< update DMA request enable */

/*******************  Bit definition for TIM_TACR register  *******************/
#define  TIM_TACR_UOAE                     	((u32)0x00000001)     	 	/*!< UpOVF trigger ADC enable */
#define  TIM_TACR_DOAE                    	((u32)0x00000002)     	 	/*!< DownOVF trigger ADC enable */
#define  TIM_TACR_OC1UE                    	((u32)0x00000004)       	/*!< OC1U trigger ADC enable */
#define  TIM_TACR_OC1DE                    	((u32)0x00000008)     	 	/*!< OC1D trigger ADC enable */
#define  TIM_TACR_OC2UE                    	((u32)0x00000010)     	 	/*!< OC2U trigger ADC enable */
#define  TIM_TACR_OC2DE                     ((u32)0x00000020)     	 	/*!< OC2D trigger ADC enable */
#define  TIM_TACR_OC3UE                     ((u32)0x00000040)       	/*!< OC3U trigger ADC enable */
#define  TIM_TACR_OC3DE                   	((u32)0x00000080)     	 	/*!< OC3D trigger ADC enable */
#define  TIM_TACR_OC4UE                     ((u32)0x00000100)     	 	/*!< OC4U trigger ADC enable */
#define  TIM_TACR_OC4DE                    	((u32)0x00000200)     	 	/*!< OC4D trigger ADC enable */
#define  TIM_TACR_TAE                    	((u32)0x00000400)     	 	/*!< TEV trigger ADC enable */
#define  TIM_TACR_IC1RAE                    ((u32)0x00010000)      	/*!< IC1R trigger ADC enable */
#define  TIM_TACR_IC1FAE                    ((u32)0x00020000)       	/*!< IC1F trigger ADC enable */
#define  TIM_TACR_IC2RAE                    ((u32)0x00040000)       	/*!< IC2R trigger ADC enable */
#define  TIM_TACR_IC2FAE                    ((u32)0x00080000)       	/*!< IC2F trigger ADC enable */
#define  TIM_TACR_IC3RAE                    ((u32)0x00100000)       	/*!< IC3R trigger ADC enable */
#define  TIM_TACR_IC3FAE                    ((u32)0x00200000)       	/*!< IC3F trigger ADC enable */
#define  TIM_TACR_IC4RAE                    ((u32)0x00400000)       	/*!< IC4R trigger ADC enable */
#define  TIM_TACR_IC4FAE                    ((u32)0x00800000)       	/*!< IC4F trigger ADC enable */

/*******************  Bit definition for TIM_JTACR register  *******************/
#define  TIM_JTACR_UOAE                     ((u32)0x00000001)     	 	/*!< UpOVF trigger ADC enable */
#define  TIM_JTACR_DOAE                    	((u32)0x00000002)     	 	/*!< DownOVF trigger ADC enable */
#define  TIM_JTACR_OC1UE                    ((u32)0x00000004)       	/*!< OC1U trigger ADC enable */
#define  TIM_JTACR_OC1DE                    ((u32)0x00000008)     	 	/*!< OC1D trigger ADC enable */
#define  TIM_JTACR_OC2UE                    ((u32)0x00000010)     	 	/*!< OC2U trigger ADC enable */
#define  TIM_JTACR_OC2DE                    ((u32)0x00000020)     	 	/*!< OC2D trigger ADC enable */
#define  TIM_JTACR_OC3UE                    ((u32)0x00000040)       	/*!< OC3U trigger ADC enable */
#define  TIM_JTACR_OC3DE                   	((u32)0x00000080)     	 	/*!< OC3D trigger ADC enable */
#define  TIM_JTACR_OC4UE                    ((u32)0x00000100)     	 	/*!< OC4U trigger ADC enable */
#define  TIM_JTACR_OC4DE                    ((u32)0x00000200)     	 	/*!< OC4D trigger ADC enable */
#define  TIM_JTACR_TAE                    	((u32)0x00000400)     	 	/*!< TEV trigger ADC enable */
#define  TIM_JTACR_IC1RAE                   ((u32)0x00010000)      	/*!< IC1R trigger ADC enable */
#define  TIM_JTACR_IC1FAE                   ((u32)0x00020000)       	/*!< IC1F trigger ADC enable */
#define  TIM_JTACR_IC2RAE                   ((u32)0x00040000)       	/*!< IC2R trigger ADC enable */
#define  TIM_JTACR_IC2FAE                   ((u32)0x00080000)       	/*!< IC2F trigger ADC enable */
#define  TIM_JTACR_IC3RAE                   ((u32)0x00100000)       	/*!< IC3R trigger ADC enable */
#define  TIM_JTACR_IC3FAE                   ((u32)0x00200000)       	/*!< IC3F trigger ADC enable */
#define  TIM_JTACR_IC4RAE                   ((u32)0x00400000)       	/*!< IC4R trigger ADC enable */
#define  TIM_JTACR_IC4FAE                   ((u32)0x00800000)       	/*!< IC4F trigger ADC enable */

/*******************  Bit definition for TIM_SR1 register  *******************/
#define  TIM_SR1_ARF                      	((u32)0x00000001)       	/*!< auto reload flag */
#define  TIM_SR1_OC1F                     	((u32)0x00000002)      		/*!< output compare 1 flag */
#define  TIM_SR1_OC2F                     	((u32)0x00000004)      		/*!< output compare 2 flag */
#define  TIM_SR1_OC3F                     	((u32)0x00000008)      		/*!< output compare 3 flag */
#define  TIM_SR1_OC4F                     	((u32)0x00000010)      		/*!< output compare 4 flag */
#define  TIM_SR1_PHASE                     	((u32)0x00008000)      		/*!< Phase shift function flag */
#define  TIM_SR1_IC1R                     	((u32)0x00010000)       	/*!< input capture 1 rising edge flag */
#define  TIM_SR1_IC1F                     	((u32)0x00020000)      		/*!< input capture 1 falling edge flag */
#define  TIM_SR1_IC2R                     	((u32)0x00040000)       	/*!< input capture 2 rising edge flag */
#define  TIM_SR1_IC2F                     	((u32)0x00080000)       	/*!< input capture 2 falling edge flag */
#define  TIM_SR1_IC3R                     	((u32)0x00100000)      		/*!< input capture 3 rising edge flag */
#define  TIM_SR1_IC3F                     	((u32)0x00200000)       	/*!< input capture 3 falling edge flag */
#define  TIM_SR1_IC4R                     	((u32)0x00400000)       	/*!< input capture 4 rising edge flag */
#define  TIM_SR1_IC4F                     	((u32)0x00800000)       	/*!< input capture 4 falling edge flag */
#define  TIM_SR1_BSY                     	((u32)0x80000000)      		/*!< Register busy */

/*******************  Bit definition for TIM_SR2 register  *******************/
#define  TIM_SR2_BIF                      	((u32)0x00000001)       	/*!< Break input flag */
#define  TIM_SR2_TIF                    	((u32)0x00000002)      		/*!< Trigger input flag */
#define  TIM_SR2_UF                     	((u32)0x00000004)      		/*!< update flag */
#define  TIM_SR2_ARRUF                     	((u32)0x00010000)      		/*!< ARR update completed flag */
#define  TIM_SR2_OCR1UF                   	((u32)0x00020000)      		/*!< OCR1 update completed flag */
#define  TIM_SR2_DOWN                     	((u32)0x01000000)      		/*!< Counter direction change up to down */
#define  TIM_SR2_UP                     	((u32)0x02000000)      		/*!< Counter direction change down to up */
#define  TIM_SR2_BSY                     	((u32)0x80000000)      		/*!< Register busy */

/*******************  Bit definition for TIM_IER1 register  *******************/
#define  TIM_IER1_ARIE                      ((u32)0x00000001)       	/*!< Auto reload interrupt enable */
#define  TIM_IER1_OC1IE                     ((u32)0x00000002)      		/*!< output compare 1 interrupt enable */
#define  TIM_IER1_OC2IE                     ((u32)0x00000004)      		/*!< output compare 2 interrupt enable */
#define  TIM_IER1_OC3IE                     ((u32)0x00000008)      		/*!< output compare 3 interrupt enable */
#define  TIM_IER1_OC4IE                     ((u32)0x00000010)      		/*!< output compare 4 interrupt enable */
#define  TIM_IER1_IC1IE                     ((u32)0x00000100)      		/*!< input capture 1 interrupt enable */
#define  TIM_IER1_IC2IE                     ((u32)0x00000200)       	/*!< input capture 2 interrupt enable */
#define  TIM_IER1_IC3IE                     ((u32)0x00000400)      		/*!< input capture 3 interrupt enable */
#define  TIM_IER1_IC4IE                     ((u32)0x00000800)       	/*!< input capture 4 interrupt enable */

/*******************  Bit definition for TIM_IER2 register  *******************/
#define  TIM_IER2_BKIE                      ((u32)0x00000001)       	/*!< Break interrupt enable */
#define  TIM_IER2_TIE                     	((u32)0x00000002)      		/*!< Trigger interrupt enable */
#define  TIM_IER2_UIE                     	((u32)0x00000004)      		/*!< Update interrupt enable */
#define  TIM_IER2_ARRUIE                   	((u32)0x00010000)      		/*!< ARR update completed interrupt enable */
#define  TIM_IER2_OCR1UIE                  	((u32)0x00020000)      		/*!< OCR1 update completed interrupt enable */
#define  TIM_IER2_DOWNIE                  	((u32)0x01000000)      		/*!< Counter direction change to down interrupt enable */
#define  TIM_IER2_UPIE                     	((u32)0x02000000)      		/*!< Counter direction change to up interrupt enable */

/*******************  Bit definition for TIM_ITARR register  *******************/
#define  TIM_ITARR_ITARR                 	((u32)0x0000000F)          	/*!< TIM interrupt auto reload */

/*******************  Bit definition for TIM_ITCNTR register  *******************/
#define  TIM_ITCNTR_ITCNT                 	((u32)0x0000000F)           /*!< TIM interrupt counter value */

/*******************  Bit definition for TIM_PSC register   *******************/
#define  TIM_PSCR_PSC                     	((u32)0x0000FFFF)           /*!< Prescaler value */
#define  TIM3_PSCR_PSC                     	((u32)0xFFFFFFFF)           /*!< TIM3 Prescaler value */

/*******************  Bit definition for TIM_CNTR register  *******************/
#define  TIM_CNTR_CNT                     	((u32)0x0000FFFF)           /*!< Counter value */
#define  TIM3_CNTR_CNT                     	((u32)0xFFFFFFFF)           /*!< TIM3 Counter value */

/*******************  Bit definition for TIM_ARR register  *******************/
#define  TIM_ARR_ARR                     	((u32)0x0000FFFF)         	/*!< Auto reload value */
#define  TIM3_ARR_ARR                     	((u32)0xFFFFFFFF)         	/*!< TIM3 Auto reload value */

/*******************  Bit definition for TIM_OCR1 register  *******************/
#define  TIM_OCR1_OCR                    	((u32)0x0000FFFF)           /*!< output compare 1 value */
#define  TIM_OCR1_OCRD                    	((u32)0xFFFF0000)           /*!< output compare value when counting down */

/*******************  Bit definition for TIM_OCR2 register  *******************/
#define  TIM_OCR2_OCR                    	((u32)0x0000FFFF)           /*!< output compare 2 value */
#define  TIM_OCR2_OCRD                    	((u32)0xFFFF0000)           /*!< output compare value when counting down */

/*******************  Bit definition for TIM_OCR3 register  *******************/
#define  TIM_OCR3_OCR                    	((u32)0x0000FFFF)           /*!< output compare 3 value */
#define  TIM_OCR3_OCRD                    	((u32)0xFFFF0000)           /*!< output compare value when counting down */

/*******************  Bit definition for TIM_OCR4 register  *******************/
#define  TIM_OCR4_OCR                    	((u32)0x0000FFFF)           /*!< output compare 4 value */
#define  TIM_OCR4_OCRD                    	((u32)0xFFFF0000)           /*!< output compare value when counting down */

/*******************  Bit definition for TIM_ICR1 register  *******************/
#define  TIM_ICR1_ICR                    	((u32)0x0000FFFF)       	/*!< input capture 1 value */
#define  TIM_ICR1_STS                    	((u32)0x00010000)       	/*!< input capture status */

/*******************  Bit definition for TIM_ICR2 register  *******************/
#define  TIM_ICR2_ICR                    	((u32)0x0000FFFF)       	/*!< input capture 2 value */
#define  TIM_ICR2_STS                    	((u32)0x00010000)       	/*!< input capture status */

/*******************  Bit definition for TIM_ICR3 register  *******************/
#define  TIM_ICR3_ICR                    	((u32)0x0000FFFF)       	/*!< input capture 3 value */
#define  TIM_ICR3_STS                    	((u32)0x00010000)       	/*!< input capture status */

/*******************  Bit definition for TIM_ICR4 register  *******************/
#define  TIM_ICR4_ICR                    	((u32)0x0000FFFF)       	/*!< input capture 4 value */
#define  TIM_ICR4_STS                    	((u32)0x00010000)       	/*!< input capture status */

/******************************************************************************/
/*                                                                            */
/*                        Independent WatchDog (IWDG)                         */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for WDG_RLR register  *******************/
#define  IWDG_RLR_RL                  		((u32)0xFFFFFFFF)       	/*!< Reload value */

/*******************  Bit definition for WDG_Count register  *******************/
#define  IWDG_CNTR_CNT                		((u32)0xFFFFFFFF)        	/*!< Counter value */

/*******************  Bit definition for WDG_CR register  *******************/
#define  IWDG_CR_EN                         ((u32)0x00000001)        	/*!< Enable */
#define  IWDG_CR_DBGE                       ((u32)0x80000000)       	/*!< IWDG debug Control */

/*******************  Bit definition for WDG_KR register  *******************/
#define  IWDG_KR_KEY                        ((u32)0xFFFFFFFF)       	/*!< key value */

/*******************  Bit definition for WDG_SR register  *******************/
#define  IWDG_SR_HDF                        ((u32)0x00000001)       	/*!< hungry dog flag */

/*******************  Bit definition for WDG_LR register  ********************/
#define  IWDG_LR_KEY                       	((u32)0xFFFFFFFF)        	/*!< lock key */
#define  IWDG_LR_LOCK                      	((u32)0x00000001)        	/*!< lock status */

/******************************************************************************/
/*                                                                            */
/*                     Serial Peripheral Interface (SPI)                      */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for SPI_CR1 register  ********************/
#define  SPI_CR1_EN				            ((u32)0x00000001)        	/* SPI Enable*/
#define  SPI_CR1_CSS				        ((u32)0x00000002)        	/* CSS control selection */
#define  SPI_CR1_DFF                        ((u32)0x000000F0)        	/* Data frame format */
#define  SPI_CR1_CPOL		                ((u32)0x00000100)        	/* Clock Polarity */
#define  SPI_CR1_CPHA		                ((u32)0x00000200)        	/* Clock Phase */
#define  SPI_CR1_MSM			            ((u32)0x00001000)        	/* Master-slave mode */
#define  SPI_CR1_SOD			            ((u32)0x00002000)        	/* SPI slave output disable */
#define  SPI_CR1_FF			                ((u32)0x00004000)        	/* Frame format */
#define  SPI_CR1_RXDMAE			            ((u32)0x00010000)        	/* Rx DMA request enable */
#define  SPI_CR1_TXDMAE			            ((u32)0x00020000)        	/* Tx DMA request enable */
#define  SPI_CR1_DMATC			            ((u32)0x00100000)        	/* DMA trigger control */
#define  SPI_CR1_LBM			            ((u32)0x80000000)        	/* Loopback mode */

/*******************  Bit definition for SPI_CR2 register  ********************/
#define  SPI_CR2_SWCS				    	((u32)0x00000001)         	/* Software control CSS */

/********************  Bit definition for SPI_BR register  ********************/
#define  SPI_BR_BR                          ((u32)0x000000FF)         	/* Prescaler factor */
#define  SPI_BR_PSC                         ((u32)0x00FF0000)         	/* Prescaler */

/********************  Bit definition for SPI_IER register  ********************/
#define  SPI_IER_RXNE                       ((u32)0x00000001)         	/* Receive/Receive-FIFO not empty interrupt enable */
#define  SPI_IER_RXHF                       ((u32)0x00000002)         	/* Receive FIFO half full interrupt enable */
#define  SPI_IER_OVR                        ((u32)0x00000008)         	/* Receive/Receive-FIFO overflow interrupt enable */
#define  SPI_IER_RXTO                       ((u32)0x00000010)         	/* Receive/Receive-FIFO-clear timeout interrupt enable */
#define  SPI_IER_TXE                       	((u32)0x00000100)         	/* Transmit/Transmit-FIFO empty interrupt enable */
#define  SPI_IER_TXHE                       ((u32)0x00000200)         	/* Transmit FIFO half empty interrupt enable */

/********************  Bit definition for SPI_SR register  ********************/
#define  SPI_SR_RXNE                       	((u32)0x00000001)         	/* Receive/Receive-FIFO not empty flag */
#define  SPI_SR_RXHF                        ((u32)0x00000002)         	/* Receive FIFO half full flag */
#define  SPI_SR_RXF                        	((u32)0x00000004)         	/* Receive FIFO full flag */
#define  SPI_SR_OVR                       	((u32)0x00000008)         	/* Receive/Receive-FIFO overflow flag */
#define  SPI_SR_RXTO                       	((u32)0x00000010)         	/* Receive/Receive-FIFO-clear timeout flag */
#define  SPI_SR_TXE                       	((u32)0x00000100)         	/* Transmit/Transmit-FIFO empty flag */
#define  SPI_SR_TXHE                       	((u32)0x00000200)         	/* Transmit FIFO half empty flag */
#define  SPI_SR_TNF                       	((u32)0x00000400)         	/* Transmit FIFO is not full flag */
#define  SPI_SR_BSY                       	((u32)0x00008000)         	/* busy flag */
#define  SPI_SR_RXFS                       	((u32)0x000F0000)         	/* Receiver FIFO Status */
#define  SPI_SR_TXFS                       	((u32)0x0F000000)         	/* Transmit FIFO Status */

/********************  Bit definition for SPI_DR register  ********************/
#define  SPI_DR_DR                        	((u32)0x0000FFFF)             	/* Data Register */

/********************  Bit definition for SPI_DRR register  ********************/
#define  SPI_DRR_KEY						((u32)0x0000FFFF)         	/* Buffer/FIFO data reset key  */

/******************************************************************************/
/*                                                                            */
/*                   Inter-integrated Circuit Interface (I2C)                 */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for I2C_CR register  *******************/
#define  I2C_CR_EN                      	((u32)0x00000001)        	/*!< I2C enable */
#define  I2C_CR_ACK                         ((u32)0x00000004)        	/*!< Acknowledge enable */
#define  I2C_CR_SI                          ((u32)0x00000008)        	/*!< I2C status flag */
#define  I2C_CR_STOP                        ((u32)0x00000010)        	/*!< Stop generation */
#define  I2C_CR_START                       ((u32)0x00000020)        	/*!< Start generation */
#define  I2C_CR_HCT                         ((u32)0x0000FF00)        	/*!< High level detection coefficient of time */
#define  I2C_CR_PSC                         ((u32)0x03FF0000)        	/*!< Prescaler */

/*******************  Bit definition for I2C_SR register  *******************/
#define  I2C_SR_SR                        	((u32)0x0000001F)      	 	/*!< I2C bus status */

/*******************  Bit definition for I2C_DR register  *******************/
#define  I2C_DR_DR                          ((u32)0x000000FF)        	/*!< I2C data */

/*******************  Bit definition for I2C_OAR register  *******************/
#define  I2C_OAR_BC							((u32)0x00000001)        	/*!< Broadcast response control */
#define  I2C_OAR_ADDR                       ((u32)0x000000FE)        	/*!< I2C addr */

/*******************  Bit definition for I2C_CCR register  *******************/
#define  I2C_CCR_EN                         ((u32)0x00000001)        	/*!< I2C disable */
#define  I2C_CCR_ACK                        ((u32)0x00000004)        	/*!< Clear Acknowledge control) */
#define  I2C_CCR_SI                         ((u32)0x00000008)        	/*!< Clear I2C interrupt flag */
#define  I2C_CCR_STOP                       ((u32)0x00000010)        	/*!< Clear stop generation */
#define  I2C_CCR_START                      ((u32)0x00000020)        	/*!< Clear start generation */
#define  I2C_CCR_HCT                        ((u32)0x0000FF00)        	/*!< Clear high level detection time */
#define  I2C_CCR_PSC                        ((u32)0x03FF0000)        	/*!< Clear prescaler */

/******************************************************************************/
/*                                                                            */
/*           Universal Asynchronous Receiver Transmitter (UART)      		  */
/*                                                                            */
/******************************************************************************/
/******************  Bit definition for UART_CR register  *******************/
#define  UART_CR_EN                       	((u32)0x00000001)        	/*!< UART enable */
#define  UART_CR_RE                       	((u32)0x00000002)        	/*!< Receiver enable */
#define  UART_CR_RXDMAE                     ((u32)0x00000004)        	/*!< DMA receive mode enable */
#define  UART_CR_TXDMAE                     ((u32)0x00000008)        	/*!< DMA transmit mode enable */
#define  UART_CR_WP                       	((u32)0x00000070)        	/*!< Word length and parity config */
#define  UART_CR_STOP                     	((u32)0x00000300)        	/*!< Stop bit length */
#define  UART_CR_DMATC                     	((u32)0x00003000)        	/*!< DMA trigger control */
#define  UART_CR_BSY                     	((u32)0x00008000)        	/*!< Register busy */
#define  UART_CR_RXP                      	((u32)0x00010000)        	/*!< Receiver polarity control */
#define  UART_CR_TXP                      	((u32)0x00020000)       	/*!< Transmitter polarity control */
#define  UART_CR_SLME                     	((u32)0x00100000)        	/*!< Single-line mode enable */
#define  UART_CR_SLDS                     	((u32)0x00200000)        	/*!< Single-line direction setting */
#define  UART_CR_MMWU                     	((u32)0x01000000)        	/*!< Mute mode wakeup method */
#define  UART_CR_MME                     	((u32)0x02000000)        	/*!< Mute mode enable */
#define  UART_CR_LPWU                     	((u32)0x04000000)        	/*!< Low power mode wakeup method */
#define  UART_CR_LBM                     	((u32)0x80000000)        	/*!< Loop back mode */

/******************  Bit definition for UART_BRR register  *******************/
#define  UART_BRR_BR                      	((u32)0x0000FFFF)        	/*!< Baud rate control  */
#define  UART_BRR_SR                      	((u32)0x00070000)        	/*!< Sample rate */

/******************  Bit definition for UART_AR register  *******************/
#define  UART_AR_AR                      	((u32)0x000000FF)        	/*!< Address  */

/******************  Bit definition for UART_IER register  *******************/
#define  UART_IER_RXNE                    	((u32)0x00000001)        	/*!< Receive/Receive-FIFO not empty interrupt enable */
#define  UART_IER_RXHF                    	((u32)0x00000002)        	/*!< Receive-FIFO half full interrupt enable */
#define  UART_IER_RXF                    	((u32)0x00000004)        	/*!< Receive-FIFO full interrupt enable */
#define  UART_IER_RXTO                    	((u32)0x00000008)        	/*!< Receive/Receive-FIFO-clear timeout interrupt enable */
#define  UART_IER_IDLE                   	((u32)0x00000010)        	/*!< Idle frame detected interrupt enable */
#define  UART_IER_PE                      	((u32)0x00000020)        	/*!< parity error interrupt enable */
#define  UART_IER_FE                      	((u32)0x00000040)        	/*!< frame error interrupt enable */
#define  UART_IER_OVR                     	((u32)0x00000080)        	/*!< Receive/Receive-FIFO overflow interrupt enable */
#define  UART_IER_TXE                     	((u32)0x00000100)        	/*!< Transmit/Transmit-FIFO empty interrupt enable */
#define  UART_IER_TXHE                     	((u32)0x00000200)        	/*!< Transmit FIFO half empty interrupt enable */
#define  UART_IER_TXF                     	((u32)0x00000400)        	/*!< Transmit FIFO full interrupt enable */
#define  UART_IER_TXC                     	((u32)0x00000800)        	/*!< Transmit complete interrupt enable */
#define  UART_IER_LPWU                     	((u32)0x00002000)        	/*!< Low power wakeup interrupt enable */
#define  UART_IER_NACK                      ((u32)0x00010000)        	/*!< Receive NACK interrupt enable */

/******************  Bit definition for UART_SR register  *******************/
#define  UART_SR_RXNE                     	((u32)0x00000001)        	/*!< Receive/Receive-FIFO not empty flag */
#define  UART_SR_RXHF                    	((u32)0x00000002)        	/*!< Receive FIFO half full flag */
#define  UART_SR_RXF                    	((u32)0x00000004)        	/*!< Receive FIFO full flag */
#define  UART_SR_RXTO                     	((u32)0x00000008)        	/*!< Receive/Receive-FIFO-clear timeout flag */
#define  UART_SR_IDLE                     	((u32)0x00000010)        	/*!< Idle frame detected flag */
#define  UART_SR_PE                       	((u32)0x00000020)        	/*!< Parity error flag */
#define  UART_SR_FE                       	((u32)0x00000040)        	/*!< frame error flag */
#define  UART_SR_OVR                      	((u32)0x00000080)        	/*!< Receive/Receive-FIFO overflow flag */
#define  UART_SR_TXE                     	((u32)0x00000100)        	/*!< Transmit/Transmit-FIFO empty flag */
#define  UART_SR_TXHE                     	((u32)0x00000200)        	/*!< Transmit FIFO half empty flag */
#define  UART_SR_TXF                     	((u32)0x00000400)        	/*!< Transmit FIFO full flag */
#define  UART_SR_TXC                      	((u32)0x00000800)        	/*!< Transmit complete flag */
#define  UART_SR_MUTE                      	((u32)0x00001000)        	/*!< Mute mode flag */
#define  UART_SR_LPWU                      	((u32)0x00002000)        	/*!< Low power wakeup flag */
#define  UART_SR_NACK                      	((u32)0x00010000)        	/*!< Receive NACK flag */

/******************  Bit definition for UART_DR register  *******************/
#define  UART_DR_DR                       	((u32)0x000001FF)        	/*!< UART data */

/******************  Bit definition for UART_FITR register  *******************/
#define  UART_FITR_FIT                    	((u32)0x000000FF)        	/*!< Frame interval time*/

/******************  Bit definition for UART_TCR register  *******************/
#define  UART_TCR_TO                      	((u32)0x000000FF)        	/*!< timeout */

/******************  Bit definition for UART_FRR register  *******************/
#define  UART_FRR_KEY                      	((u32)0x000000FF)        	/*!< FIFO reset key */

/******************  Bit definition for UART_SCCR register *******************/
#define  UART_SCCR_SCEN                  	((u32)0x00000001)        	/*!< Smart card mode enable */
#define  UART_SCCR_DIR                 		((u32)0x00000002)       	/*!< Data transfer direction control */
#define  UART_SCCR_PSC                 		((u32)0x00000070)        	/*!< Prescaler value */

/******************************************************************************/
/*                                                                            */
/*      Universal Synchronous/Asynchronous Receiver Transmitter (USART)       */
/*                                                                            */
/******************************************************************************/
/******************  Bit definition for USART_CR1 register  *******************/
#define	 USART_CR1_EN						((u32)0x00000001)			/*!< USART enable*/
#define	 USART_CR1_RE						((u32)0x00000002)			/*!< Receiver enable*/
#define  USART_CR1_RXDMAE					((u32)0x00000004)			/*!< Rx DMA request enable*/
#define  USART_CR1_TXDMAE					((u32)0x00000008)			/*!< Tx DMA request enable*/
#define  USART_CR1_WP						((u32)0x00000070)			/*!< Word length and parity config*/
#define  USART_CR1_CPOL						((u32)0x00000010)			/*!< Clock polarity*/
#define  USART_CR1_CPHA						((u32)0x00000020)			/*!< Clock phase*/
#define  USART_CR1_MODE						((u32)0x00000080)			/*!< Mode selection*/
#define  USART_CR1_STOP						((u32)0x00000300)			/*!< Stop bit length*/
#define  USART_CR1_DMATC					((u32)0x00003000)			/*!< DMA trigger control*/
#define  USART_CR1_RXP						((u32)0x00010000)			/*!< Receiver polarity control*/
#define  USART_CR1_TXP						((u32)0x00020000)			/*!< Transmitter polarity control*/
#define  USART_CR1_SLME						((u32)0x00100000)			/*!< Single-line mode enable*/
#define  USART_CR1_SLDS						((u32)0x00200000)			/*!< Single-line direction setting*/
#define  USART_CR1_MSM						((u32)0x01000000)			/*!< Master-slave mode*/
#define  USART_CR1_FF						((u32)0x02000000)			/*!< Frame format*/
#define  USART_CR1_LBM						((u32)0x80000000)			/*!< loopback mode*/

/******************  Bit definition for USART_BRR register  *******************/
#define  USART_BRR_BR                      	((u32)0x0000FFFF)        	/*!< Baud rate control  */
#define  USART_BRR_SR                      	((u32)0x00070000)        	/*!< Sample rate */

/******************  Bit definition for USART_IER register  *******************/
#define  USART_IER_RXNE                    	((u32)0x00000001)        	/*!< Receive/Receive-FIFO not empty interrupt enable */
#define  USART_IER_RXHF                    	((u32)0x00000002)        	/*!< Receive-FIFO half full interrupt enable */
#define  USART_IER_RXF                    	((u32)0x00000004)        	/*!< Receive-FIFO full interrupt enable */
#define  USART_IER_RXTO                    	((u32)0x00000008)        	/*!< Receive/Receive-FIFO-clear timeout interrupt enable */
#define  USART_IER_IDLE                   	((u32)0x00000010)        	/*!< Idle frame detected interrupt enable */
#define  USART_IER_PE                      	((u32)0x00000020)        	/*!< parity error interrupt enable */
#define  USART_IER_FE                      	((u32)0x00000040)        	/*!< frame error interrupt enable */
#define  USART_IER_OVR                     	((u32)0x00000080)        	/*!< Receive/Receive-FIFO overflow interrupt enable */
#define  USART_IER_TXE                     	((u32)0x00000100)        	/*!< Transmit/Transmit-FIFO empty interrupt enable */
#define  USART_IER_TXHE                     ((u32)0x00000200)        	/*!< Transmit FIFO half empty interrupt enable */
#define  USART_IER_TXF                     	((u32)0x00000400)        	/*!< Transmit FIFO full interrupt enable */
#define  USART_IER_TXC                     	((u32)0x00000800)        	/*!< Transmit complete interrupt enable */
#define  USART_IER_SCKTC                    ((u32)0x00001000)        	/*!< SCK transmit complete interrupt enable */

/******************  Bit definition for USART_SR register  *******************/
#define  USART_SR_RXNE                     	((u32)0x00000001)        	/*!< Receive/Receive-FIFO not empty flag */
#define  USART_SR_RXHF                    	((u32)0x00000002)        	/*!< Receive FIFO half full flag */
#define  USART_SR_RXF                    	((u32)0x00000004)        	/*!< Receive FIFO full flag */
#define  USART_SR_RXTO                     	((u32)0x00000008)        	/*!< Receive/Receive-FIFO-clear timeout flag */
#define  USART_SR_IDLE                     	((u32)0x00000010)        	/*!< Idle frame detected flag */
#define  USART_SR_PE                       	((u32)0x00000020)        	/*!< Parity error flag */
#define  USART_SR_FE                       	((u32)0x00000040)        	/*!< frame error flag */
#define  USART_SR_OVR                      	((u32)0x00000080)        	/*!< Receive/Receive-FIFO overflow flag */
#define  USART_SR_TXE                     	((u32)0x00000100)        	/*!< Transmit/Transmit-FIFO empty flag */
#define  USART_SR_TXHE                     	((u32)0x00000200)        	/*!< Transmit FIFO half empty flag */
#define  USART_SR_TXF                     	((u32)0x00000400)        	/*!< Transmit FIFO full flag */
#define  USART_SR_TXC                      	((u32)0x00000800)        	/*!< Transmit complete flag */
#define  USART_SR_SCKTC                    	((u32)0x00001000)        	/*!< SCK transmit complete flag */

/******************  Bit definition for USART_DR register  *******************/
#define  USART_DR_DR                       	((u32)0x000001FF)        	/*!< UART data */

/******************  Bit definition for USART_CR3 register  *******************/
#define  USART_CR3_CNT                      ((u32)0x00FFFFFF)        	/*!< Number of SCK clocks */

/******************  Bit definition for USART_FITR register  *******************/
#define  USART_FITR_FIT                    	((u32)0x000000FF)        	/*!< Frame interval time*/

/******************  Bit definition for USART_TCR register  *******************/
#define  USART_TCR_TO                      	((u32)0x000000FF)        	/*!< timeout */

/******************  Bit definition for USART_FRR register  *******************/
#define  USART_FRR_KEY                      ((u32)0x000000FF)        	/*!< FIFO reset key */

/******************************************************************************/
/*                                                                            */
/*                      	Infrared Interface (IRTIM)                	      */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for IRTIM_CR register  *******************/
#define  IRTIM_CR_EN						((u32)0x00000001)       	/*!< IRTIM enable */
#define  IRTIM_CR_OP						((u32)0x00000002)       	/*!< IRTIM output polarity */

/*******************  Bit definition for IRTIM_CFGR register  *****************/
#define  IRTIM_CFGR_DSS						((u32)0x0000001F)       	/*!< Data source selection */
#define  IRTIM_CFGR_CSS						((u32)0x00001F00)       	/*!< Carrier source selection */
#define  IRTIM_CFGR_DSP						((u32)0x00010000)       	/*!< Data source polarity */

/******************************************************************************/
/*                                                                            */
/*                      Operational Amplifiers (OPA)                	      */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for OPA_CR1 register  *******************/
#define  OPA_CR1_EN							((u32)0x00000001)       	/*!< OPA enable */
#define  OPA_CR1_OUTE						((u32)0x00000002)       	/*!< OPA output enable */
#define  OPA_CR1_VCMS						((u32)0x0000000C)       	/*!< PGA VCM selection */
#define  OPA_CR1_PM							((u32)0x00000030)       	/*!< OPA power mode */
#define  OPA_CR1_PGA						((u32)0x00000300)       	/*!< PGA mode */
#define  OPA_CR1_GAIN						((u32)0x0000F000)       	/*!< PGA programmable amplifier gain value */
#define  OPA_CR1_INP0E						((u32)0x00010000)       	/*!< OPA positive intput 0 enable */
#define  OPA_CR1_INP1E						((u32)0x00020000)       	/*!< OPA positive intput 1 enable */
#define  OPA_CR1_INP2E						((u32)0x00040000)       	/*!< OPA positive intput 2 enable */
#define  OPA_CR1_INN0E						((u32)0x00100000)       	/*!< OPA negative intput 0 enable */
#define  OPA_CR1_INN1E						((u32)0x00200000)       	/*!< OPA negative intput 1 enable */
#define  OPA_CR1_INN2E						((u32)0x00400000)       	/*!< OPA negative intput 2 enable */
#define  OPA_CR1_INN3E						((u32)0x00800000)       	/*!< OPA negative intput 3 enable */
#define  OPA_CR1_OUT0E						((u32)0x01000000)       	/*!< OPA output 0 enable */
#define  OPA_CR1_OUT1E						((u32)0x02000000)       	/*!< OPA output 1 enable */
#define  OPA_CR1_OUT2E						((u32)0x04000000)       	/*!< OPA output 2 enable */
#define  OPA_CR1_OUT3E						((u32)0x08000000)       	/*!< OPA output 3 enable */
#define  OPA_CR1_VGE						((u32)0x10000000)       	/*!< OPA virtual ground enable */

/*******************  Bit definition for OPA_CR2 register  *******************/
#define  OPA_CR2_CALE						((u32)0x00000003)       	/*!< Calibration mode enable */
#define  OPA_CR2_ACMS						((u32)0x00000010)       	/*!< Automatic calibration mode selection */

/*******************  Bit definition for OPA_OTR register  *******************/
#define  OPA_OTR_TRIM						((u32)0x000000FF)       	/*!< Offset trimming value */
#define  OPA_OTR_CAL						((u32)0x00000100)       	/*!< OPA calibration output */
#define  OPA_OTR_KEY						((u32)0x00FF0000)       	/*!< Key value */
#define  OPA_OTR_OTF						((u32)0x00010000)       	/*!< Offset trimming value source flag */

/******************************************************************************/
/*                                                                            */
/*                            Comparators (CMP)                				  */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for CMP_CR1 register  ********************/
#define  CMP_CR1_EN                   		((u32)0x00000001)         	/*!< Comparator enable */
#define  CMP_CR1_CPMS                 		((u32)0x00000002)        	/*!< Comparator power mode selection */
#define  CMP_CR1_OPC                  		((u32)0x00000004)        	/*!< Comparator output polarity control */
#define  CMP_CR1_HYSE                  		((u32)0x00000008)        	/*!< Comparator hysteresis enable */
#define  CMP_CR1_CNS                  		((u32)0x00000070) 			/*!< Comparator negative intput selection */
#define  CMP_CR1_DFC                  		((u32)0x00000700)         	/*!< Digital filter configuration */
#define  CMP_CR1_HVS                  		((u32)0x00007000)         	/*!< Comparator hysteresis voltage selection */
#define  CMP_CR1_CPS                  		((u32)0x00070000)        	/*!< Comparator positive input selection */
#define  CMP_CR1_SYNC                 		((u32)0x00100000)			/*!< Comparator output synchronization control */
#define  CMP_CR1_PPC                  		((u32)0x00200000)			/*!< Comparator output PWM polarity control */
#define  CMP_CR1_PWMS                 		((u32)0x1F000000)			/*!< Comparator output PWM selection */

/********************  Bit definition for CMP_IE register  ********************/
#define  CMP_IER_COF                   		((u32)0x00000001)         	/*!< Comparator output falling edge interrupt enable */
#define  CMP_IER_COR                   		((u32)0x00000002)         	/*!< Comparator output rising edge interrupt enable */
#define  CMP_IER_COFW                  		((u32)0x00000004)         	/*!< Comparator output falling edge wake up interrupt enable */
#define  CMP_IER_CORW                  		((u32)0x00000008)         	/*!< Comparator output rising edge wake up interrupt enable */

/********************  Bit definition for CMP_SR register  ********************/
#define  CMP_SR_COF                   		((u32)0x00000001)         	/*!< Comparator output falling edge flag */
#define  CMP_SR_COR                   		((u32)0x00000002)         	/*!< Comparator output rising edge flag */
#define  CMP_SR_COFW                  		((u32)0x00000004)         	/*!< Comparator output falling edge wake up flag */
#define  CMP_SR_CORW                  		((u32)0x00000008)         	/*!< Comparator output rising edge wake up flag */
#define  CMP_SR_CRS                   		((u32)0x00010000)         	/*!< Comparator result */
#define  CMP_SR_CPS                   		((u32)0x07000000)         	/*!< Current comparator positive intput */

/********************  Bit definition for CMP_CR2 register  ********************/
#define  CMP_CR2_DELAY	              		((u32)0x000007FF)         	/*!< Comparator Initialization delay time */
#define  CMP_CR2_DW	              			((u32)0x00030000)         	/*!< Port deglitch width */
#define  CMP_CR2_CKD	              		((u32)0xFF000000)         	/*!< Sampling clock division */

/********************  Bit definition for CMP_CR3 register  ********************/
#define  CMP_CR3_P0E                  		((u32)0x00000001)         	/*!< positive intput 0 scan enable */
#define  CMP_CR3_P1E                  		((u32)0x00000002)         	/*!< positive intput 1 scan enable */
#define  CMP_CR3_P2E                  		((u32)0x00000004)         	/*!< positive intput 2 scan enable */
#define  CMP_CR3_P3E                  		((u32)0x00000008)         	/*!< positive intput 3 scan enable */
#define  CMP_CR3_P4E                  		((u32)0x00000010)         	/*!< positive intput 4 scan enable */
#define  CMP_CR3_P5E                  		((u32)0x00000020)         	/*!< positive intput 5 scan enable */
#define  CMP_CR3_P6E                  		((u32)0x00000040)         	/*!< positive intput 6 scan enable */
#define  CMP_CR3_SCAN                  		((u32)0x0000FF00)         	/*!< Comparator positive intput Scan Time */

/******************************************************************************/
/*                                                                            */
/*                5/7 bits Digital to Analog Converters (LDAC)                */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for LDAC_CR register  *******************/
#define  LDAC_CR_EN							((u32)0x00000001)			/*!< LDAC enable */
#define  LDAC_CR_OE							((u32)0x00000002)			/*!< LDAC output enable */
#define  LDAC_CR_RVSPS						((u32)0x00000010)			/*!< Reference voltage source positive selection */
#define  LDAC_CR_PM							((u32)0x00010000)			/*!< Power mode */

/*******************  Bit definition for LDAC_DR register  *******************/
#define  LDAC_DR_DATA						((u32)0x0000001F)			/*!< LDAC data */

/******************************************************************************/
/*                                                                            */
/*           		Pulse Width Modulation (LEDPWM)             			  */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for LEDPWM_CR1 register  *******************/
#define  LEDPWM_CR1_EN						((u32)0x00000001)			/*!< LEDPWM enable */
#define  LEDPWM_CR1_CC						((u32)0x00000300)			/*!< Check control */
#define  LEDPWM_CR1_OP						((u32)0x00010000)			/*!< Output polarity */
#define  LEDPWM_CR1_DCC						((u32)0x00100000)			/*!< Data code cycle control */

/*******************  Bit definition for LEDPWM_CR2 register  *******************/
#define  LEDPWM_CR2_DUTY0					((u32)0x000000FF)			/*!< Code 0 positive duty */
#define  LEDPWM_CR2_DUTY1					((u32)0x0000FF00)			/*!< Code 1 positive duty */
#define  LEDPWM_CR2_CYCLE0					((u32)0x00FF0000)			/*!< Code 0 cycle */
#define  LEDPWM_CR2_CYCLE1					((u32)0xFF000000)			/*!< Code 1 cycle */

/*******************  Bit definition for LEDPWM_DR register  *******************/
#define  LEDPWM_DR_DATA						((u32)0x00FFFFFF)			/*!< Data code */
#define  LEDPWM_DR_LEN						((u32)0x07000000)			/*!< Data code length*/
#define  LEDPWM_DR_CE						((u32)0x80000000)			/*!< Check enable */

/*******************  Bit definition for LEDPWM_IER register  *******************/
#define  LEDPWM_IER_TXE						((u32)0x00000001)			/*!< Data code Transmit empty interrupt enable */
#define  LEDPWM_IER_TXC						((u32)0x00000002)			/*!< Data code Transmit completed interrupt enable */

/*******************  Bit definition for LEDPWM_SR register  *******************/
#define  LEDPWM_SR_TXE						((u32)0x00000001)			/*!< Data code Transmit empty flag */
#define  LEDPWM_SR_TXC						((u32)0x00000002)			/*!< Data code Transmit completed flag */

/******************************************************************************/
/*                                                                            */
/*                                   BEEP                                 	  */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for BEEP_CR register  ********************/
#define  BEEP_CR_EN                       	((u32)0x00000001)       	/*!< BEEP enable */
#define  BEEP_CR_DIV                      	((u32)0x0000FF00)       	/*!< BEEP Clock divider */
#define  BEEP_CR_HSCE                     	((u32)0x00010000)       	/*!< BEEP high sink current enable */

/******************************************************************************/
/*                                                                            */
/*                   		Real Time Clock (RTC)          		              */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for RTC_CR register **********************/
#define  RTC_CR_EN	                  		((u32)0x00000001)        	/*!< RTC enable */
#define  RTC_CR_TF	                  		((u32)0x00000002)        	/*!< Time format */
#define  RTC_CR_TBOE	                  	((u32)0x00000004)        	/*!< Time beacon output enable */
#define  RTC_CR_BSY		                  	((u32)0x00008000)        	/*!< Register busy */

/*******************  Bit definition for RTC_CFGR register **********************/
#define  RTC_CFGR_PINT	                  	((u32)0x0000FFFF)        	/*!< Periodic Interrupt Interval */

/*******************  Bit definition for RTC_WPR register **********************/
#define  RTC_WPR_LOCK	                  	((u32)0x00000001)        	/*!< Lock status */
#define  RTC_WPR_BSY	                  	((u32)0x00000002)        	/*!< Register busy */

/*******************  Bit definition for RTC_TR register **********************/
#define  RTC_TR_SU		                  	((u32)0x0000000F)        	/*!< Second units in BCD format */
#define  RTC_TR_ST		                  	((u32)0x00000070)        	/*!< Second tens in BCD format */
#define  RTC_TR_MU		                  	((u32)0x00000F00)        	/*!< Minute units in BCD format */
#define  RTC_TR_MT		                  	((u32)0x00007000)        	/*!< Minute tens in BCD format */
#define  RTC_TR_HU		                  	((u32)0x000F0000)        	/*!< Hour units in BCD format */
#define  RTC_TR_HT		                  	((u32)0x00300000)        	/*!< Hour tens in BCD format */
#define  RTC_TR_PM		                  	((u32)0x01000000)        	/*!< AM/PM notation*/
#define  RTC_TR_BSY		                  	((u32)0x80000000)        	/*!< Register busy*/

/*******************  Bit definition for RTC_DR register **********************/
#define  RTC_DR_DU		                  	((u32)0x0000000F)        	/*!< Day units in BCD format */
#define  RTC_DR_DT		                  	((u32)0x00000030)        	/*!< Day tens in BCD format */
#define  RTC_DR_MU		                  	((u32)0x00000F00)        	/*!< Month units in BCD format */
#define  RTC_DR_MT		                  	((u32)0x00001000)        	/*!< Month tens in BCD format */
#define  RTC_DR_WEEK		               	((u32)0x0000E000)        	/*!< Week day */
#define  RTC_DR_YU		                  	((u32)0x000F0000)        	/*!< Year units in BCD format */
#define  RTC_DR_YT		                  	((u32)0x00F00000)        	/*!< Year tens in BCD format */
#define  RTC_DR_BSY		                  	((u32)0x80000000)        	/*!< Register busy*/

/*******************  Bit definition for RTC_IER register **********************/
#define  RTC_IER_HSEC		                ((u32)0x00000001)        	/*!< Half-second update interrupt enable */
#define  RTC_IER_SEC		                ((u32)0x00000002)        	/*!< Second update interrupt enable */
#define  RTC_IER_MIN		                ((u32)0x00000004)        	/*!< Minute update interrupt enable */
#define  RTC_IER_HOUR		                ((u32)0x00000008)        	/*!< Hour update interrupt enable */
#define  RTC_IER_DAY		                ((u32)0x00000010)        	/*!< Day update interrupt enable */
#define  RTC_IER_MON		                ((u32)0x00000020)        	/*!< Month update interrupt enable */
#define  RTC_IER_ALM		                ((u32)0x00000040)        	/*!< Alarm interrupt enable */
#define  RTC_IER_PED		                ((u32)0x00000080)        	/*!< Periodic interrupt enable */

/*******************  Bit definition for RTC_SR register **********************/
#define  RTC_SR_HSEC		                ((u32)0x00000001)        	/*!< Half-second update flag */
#define  RTC_SR_SEC		             	  	((u32)0x00000002)        	/*!< Second update flag */
#define  RTC_SR_MIN		           		    ((u32)0x00000004)        	/*!< Minute update flag */
#define  RTC_SR_HOUR		                ((u32)0x00000008)        	/*!< Hour update flag */
#define  RTC_SR_DAY		                	((u32)0x00000010)        	/*!< Day update flag */
#define  RTC_SR_MON		              	 	((u32)0x00000020)        	/*!< Month update flag */
#define  RTC_SR_ALM		              	 	((u32)0x00000040)        	/*!< Alarm flag */
#define  RTC_SR_PED		              		((u32)0x00000080)        	/*!< Periodic flag */
#define  RTC_SR_TFC		              		((u32)0x00010000)        	/*!< Time format changed Flag */
#define  RTC_SR_BSY		              		((u32)0x80000000)        	/*!< RTC busy */

/*******************  Bit definition for RTC_ALARMR register **********************/
#define  RTC_ALARMR_SU		                ((u32)0x0000000F)        	/*!< Second units in BCD format */
#define  RTC_ALARMR_ST		                ((u32)0x00000070)        	/*!< Second tens in BCD format */
#define  RTC_ALARMR_MU		                ((u32)0x00000F00)        	/*!< Minute units in BCD format */
#define  RTC_ALARMR_MT		                ((u32)0x00007000)        	/*!< Minute tens in BCD format */
#define  RTC_ALARMR_HU		                ((u32)0x000F0000)        	/*!< Hour units in BCD format */
#define  RTC_ALARMR_HT		                ((u32)0x00300000)        	/*!< Hour tens in BCD format */
#define  RTC_ALARMR_PM		                ((u32)0x01000000)        	/*!< AM/PM notation*/
#define  RTC_ALARMR_WEEK		            ((u32)0xFE000000)        	/*!< Week*/

/*******************  Bit definition for RTC_CALR register **********************/
#define  RTC_CALR_CALE		                ((u32)0x00000001)        	/*!< Calibration mode enable */
#define  RTC_CALR_TRIM		                ((u32)0x01FF0000)        	/*!< Trim Value */

/******************************************************************************/
/*                                                                            */
/*                       Liquid Crystal Display (LCD)                         */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for LCD_CR1 register  ********************/
#define  LCD_CR1_EN							((u32)0x00000001)         	/*!< LCD enable */
#define  LCD_CR1_DU  						((u32)0x00000002)			/*!< Display data update */
#define  LCD_CR1_BSY   						((u32)0x80000000)			/*!< Register busy */
/*******************  Bit definition for LCD_CR2 register  ********************/
#define  LCD_CR2_DUTY  						((u32)0x00000003)			/*!< LCD duty cycle */
#define  LCD_CR2_WAVE  						((u32)0x00000010)			/*!< waveform control */
#define  LCD_CR2_PDM   						((u32)0x000000C0)			/*!< Page display mode */
#define  LCD_CR2_SCM	  					((u32)0x00000300)			/*!< Screen Control mode */
#define  LCD_CR2_TYPE  						((u32)0x00000C00)			/*!< LCD drive voltage type */
#define  LCD_CR2_CSEL  						((u32)0x00003000)			/*!< C-drive mode selection */
#define  LCD_CR2_RSEL  						((u32)0x00004000)			/*!< R-drive total resistance selection */

/*******************  Bit definition for LCD_GCR register  *******************/
#define  LCD_GCR_EN	  						((u32)0x00000001)			/*!< Grayscale Control Enable */
#define  LCD_GCR_GDR  						((u32)0x0000000E)			/*!< Grayscale adjustment control */

/*******************  Bit definition for LCD_PSC register  *******************/
#define  LCD_PSCR_PSC   					((u32)0x0000001F)			/*!< Prescaler */

/*******************  Bit definition for LCD_BTCR register  ******************/
#define  LCD_BTCR_BT  						((u32)0x000000FF)			/*!< Blink Time */

/*******************  Bit definition for LCD_BON register  *******************/
#define  LCD_BONCR_ON   					((u32)0x000000FF)			/*!< Blinking Lighting Time */

/*******************  Bit definition for LCD_PCR1 register  *******************/
#define  LCD_PCR1_PE   						((u32)0xFFFFFFFF)			/*!< LCDx pin enable */

/*******************  Bit definition for LCD_PCR2 register  *******************/
#define  LCD_PCR2_PE   						((u32)0x0000FFFF)			/*!< LCDx pin enable */

/*******************  Bit definition for LCD_PDCR1 register  *******************/
#define  LCD_PDCR1_PDC 						((u32)0xFFFFFFFF)			/*!< LCDx pin driver control */

/*******************  Bit definition for LCD_PDCR2 register  *******************/
#define  LCD_PDCR2_PDC 						((u32)0x0000FFFF)			/*!< LCDx pin driver control */

/*******************  Bit definition for LCD_PDTR register  *******************/
#define  LCD_PDTR_PDT 						((u32)0x000000FF)			/*!< Page display time */

/*******************  Bit definition for LCD_SDPR register  *******************/
#define  LCD_SDPR_SDP 						((u32)0x0000000F)			/*!< Scroll display max page */

/*******************  Bit definition for LCD_PNR register  *******************/
#define  LCD_PNR_PN	  						((u32)0x0000000F)			/*!< Page Number */

/*******************  Bit definition for LCD_VREFCR register  *******************/
#define  LCD_VREFCR_RS	  					((u32)0x0000000F)			/*!< Reference voltage selection */
#define  LCD_VREFCR_KEY	  					((u32)0x0000FF00)			/*!< Key value */
#define  LCD_VREFCR_RSF	  					((u32)0x00000100)			/*!< LCD VREF status flag */

/******************************************************************************/
/*                                                                            */
/*                   Advanced Encryption Standard (AES)                       */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for AES_CR register **********************/
#define  AES_CR_START	                  	((u32)0x00000001)        	/*!< Start computation */
#define  AES_CR_MODE	                  	((u32)0x00000030)        	/*!< AES operating mode */
#define  AES_CR_CMS	               		   	((u32)0x00000700)        	/*!< Chaining mode selection */
#define  AES_CR_KSIZE	               		((u32)0x00003000)        	/*!< Key size selection */

/*******************  Bit definition for AES_IER register **********************/
#define  AES_IER_CC		                  	((u32)0x00000001)        	/*!< Computation completed interrupt enable */

/*******************  Bit definition for AES_SR register **********************/
#define  AES_SR_CC		                  	((u32)0x00000001)        	/*!< Computation completed flag */

/*******************  Bit definition for AES_KR register **********************/
#define  AES_KR_KEY		                  	((u32)0xFFFFFFFF)        	/*!< key */

/*******************  Bit definition for AES_IVR register **********************/
#define  AES_IVR_IV		                  	((u32)0xFFFFFFFF)        	/*!< Initialization vector input */

/*******************  Bit definition for AES_DINR register **********************/
#define  AES_DINR_DIN		                ((u32)0xFFFFFFFF)        	/*!< Data input */

/*******************  Bit definition for AES_DOUTR register **********************/
#define  AES_DOUTR_DOUT		                ((u32)0xFFFFFFFF)        	/*!< Data output */

/******************************************************************************/
/*                                                                            */
/*                       Arithmetic Logic Unit (ALU)                          */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for ALU_CR register  ********************/
#define  ALU_CR_MODE                        ((u32)0x00000700)           /*!< Operation mode selection */
#define  ALU_CR_QFMT                        ((u32)0x001F0000)          	/*!< Q format data type */
#define  ALU_CR_SIGN                        ((u32)0x01000000)           /*!< Operand sign bit enable */
#define  ALU_CR_MACL                        ((u32)0x02000000)           /*!< Multiply and accumulate operation limit enable */

/*******************  Bit definition for ALU_IE register  ********************/
#define  ALU_IER_ERRIE                      ((u32)0x00000002)           /*!< Operation error interrupt enable */
#define  ALU_IER_OVFIE                      ((u32)0x00000004)           /*!< Operation overflow interrupt enable */

/*******************  Bit definition for ALU_SR register  ********************/
#define  ALU_SR_ERR                         ((u32)0x00000002)          	/*!< Operation error flag */
#define  ALU_SR_OVF                         ((u32)0x00000004)          	/*!< Operation overflow flag */
#define  ALU_SR_BSY                         ((u32)0x00000008)          	/*!< Operation busy flag */

/*******************  Bit definition for ALU_OPDXR register  ********************/
#define  ALU_OPDXR_OPDX                     ((u32)0xFFFFFFFF)           /*!< Operand X */

/*******************  Bit definition for ALU_OPDYR register  ********************/
#define  ALU_OPDYR_OPDY                     ((u32)0xFFFFFFFF)           /*!< Operand Y */

/*******************  Bit definition for ALU_OPDZR register  ********************/
#define  ALU_OPDZR_OPDZ                     ((u32)0xFFFFFFFF)           /*!< Operand Z */

/*******************  Bit definition for ALU_OPDWR register  ********************/
#define  ALU_OPDWR_OPDW                     ((u32)0xFFFFFFFF)           /*!< Operand W */

/*******************  Bit definition for ALU_DHR register  ********************/
#define  ALU_DHR_DATAH                      ((u32)0xFFFFFFFF)           /*!< Data high 32-bit */

/*******************  Bit definition for ALU_DLR register  ********************/
#define  ALU_DLR_DATAL                      ((u32)0xFFFFFFFF)           /*!< Data low 32-bit */

/*******************  Bit definition for ALU_DRR register  ********************/
#define  ALU_DRR_ROD                        ((u32)0xFFFFFFFF)           /*!< Remainder of division */

/*******************  Bit definition for ALU_MAMAXHR register  ********************/
#define  ALU_MAMAXHR_MAXH                   ((u32)0xFFFFFFFF)           /*!< MAC maximum threshold high 32-bit */

/*******************  Bit definition for ALU_MAMAXLR register  ********************/
#define  ALU_MAMAXLR_MAXL                   ((u32)0xFFFFFFFF)           /*!< MAC maximum threshold low 32-bit */

/*******************  Bit definition for ALU_MAMINHR register  ********************/
#define  ALU_MAMINHR_MINH                  	((u32)0xFFFFFFFF)           /*!< MAC minimum threshold high 32-bit */

/*******************  Bit definition for ALU_MAMINLR register  ********************/
#define  ALU_MAMINLR_MINL                   ((u32)0xFFFFFFFF)           /*!< MAC minimum threshold low 32-bit */

/******************************************************************************/
/*                                                                            */
/*                   On-chip Flash Memory Controller (IFMC)                   */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for IFMC_CR1 register  ******************/
#define  IFMC_CR1_WAIT               		((u32)0x00000007)        	/*!< Instruction fetch wait cycle */
#define  IFMC_CR1_AINC                  	((u32)0x00000010)        	/*!< Address auto increment control */

/*******************  Bit definition for IFMC_CR2 register  ******************/
#define  IFMC_CR2_PG                    	((u32)0x00000001)        	/*!< Programming */
#define  IFMC_CR2_PER                   	((u32)0x00000002)        	/*!< Page erase */
#define  IFMC_CR2_MER                  		((u32)0x00000004)        	/*!< Mass erase */

/*******************  Bit definition for IFMC_PSCR register  ******************/
#define  IFMC_PSCR_PSC                   	((u32)0x000000FF)         	/*!< Prescaler value */
#define  IFMC_PSCR_PSCF                   	((u32)0x00010000)         	/*!< Prescaler status flag */
#define  IFMC_PSCR_KEY                   	((u32)0x00FF0000)        	/*!< Prescaler Key */

/*******************  Bit definition for IFMC_KR1 register  ******************/
#define  IFMC_KR1_KEY               		((u32)0x0000FFFF)         	/*!< IFMC key */
#define  IFMC_KR1_AREA               		((u32)0x00000007)         	/*!< IFMC AREA */
#define  IFMC_KR1_UKEY                   	((u32)0xFFFF0000)         	/*!< Register unlock key */
#define  IFMC_KR1_LOCK                   	((u32)0x00010000)         	/*!< Register lock status */

/*******************  Bit definition for IFMC_KR2 register  ******************/
#define  IFMC_KR2_KEY               		((u32)0x0000FFFF)         	/*!< Security key */
#define  IFMC_KR2_LOCK1                  	((u32)0x00000001)         	/*!< CR2 lock status */
#define  IFMC_KR2_UKEY                   	((u32)0xFFFF0000)         	/*!< Register unlock key */
#define  IFMC_KR2_LOCK2                   	((u32)0x00010000)         	/*!< Register lock status */

/*******************  Bit definition for IFMC_IER register  ******************/
#define  IFMC_IER_WCIE                  	((u32)0x00000001)        	/*!< Write operation complete interrupt enable */
#define  IFMC_IER_PECIE                 	((u32)0x00000002)        	/*!< Page erase operation complete interrupt enable */
#define  IFMC_IER_MECIE                 	((u32)0x00000004)        	/*!< Mass erase operation complete interrupt enable */
#define  IFMC_IER_CERIE                 	((u32)0x00000010)        	/*!< Operation command error interrupt enable */
#define  IFMC_IER_AERIE                 	((u32)0x00000020)        	/*!< Address error interrupt enable */
#define  IFMC_IER_KERIE                 	((u32)0x00000040)        	/*!< Key error interrupt enable */

/*******************  Bit definition for IFMC_SR1 register  *******************/
#define  IFMC_SR1_WC                    	((u32)0x00000001)        	/*!< Write operation complete */
#define  IFMC_SR1_PEC                   	((u32)0x00000002)        	/*!< Page erase operation complete */
#define  IFMC_SR1_MEC                   	((u32)0x00000004)        	/*!< Mass erase operation complete */
#define  IFMC_SR1_CERR                  	((u32)0x00000010)        	/*!< Operation command error */
#define  IFMC_SR1_AERR                  	((u32)0x00000020)        	/*!< Address error */
#define  IFMC_SR1_KERR               		((u32)0x00000040)        	/*!< Key error */

/*******************  Bit definition for IFMC_AR register  *******************/
#define  IFMC_AR_AR                     	((u32)0xFFFFFFFF)        	/*!< IFMC words addr */

/*******************  Bit definition for IFMC_DR1  register  *******************/
#define  IFMC_DR1_DR                   		((u32)0xFFFFFFFF)        	/*!< IFMC data 1 */

/*******************  Bit definition for IFMC_DR2  register  *******************/
#define  IFMC_DR2_DR                   		((u32)0xFFFFFFFF)        	/*!< IFMC data 2 */

/*******************  Bit definition for IFMC_OPTCR register *******************/
#define  IFMC_OPTCR_RDP                   	((u32)0x000000FF)        	/*!< Read protect */
#define  IFMC_OPTCR_KEY                   	((u32)0xFFFFFFFF)        	/*!< Key value */
/**
  * @}
  */

#ifdef USE_STDPERIPH_DRIVER
#include "PT32x0xx_conf.h"
#endif
#endif
#ifdef __cplusplus
}
#endif
