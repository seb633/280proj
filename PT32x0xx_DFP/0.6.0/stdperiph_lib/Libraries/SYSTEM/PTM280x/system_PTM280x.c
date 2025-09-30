/*
  ******************************************************************************
  * @file    system_PTM280x.c
  * @author  Application development team
  * @version V1.0.0
  * @date    2023/11/07
  * @brief
  ******************************************************************************
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers. 
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of the content or the  codes within the content.
  *
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "PT32x0xx.h"
#include "PT32x0xx_config.h"




/** @defgroup CLK_Private_Defines
  * @{
  */

/**
  * @}
  */
void Wait_ClockReady(void)
{
	u8 i, j;
	for(i = 0; i < 100; i++)
		for(j = 0; j < 48; j++);
}

/** @defgroup CLK_Private_Functions
  * @{
  */

/**
* @brief 时钟源配置
* @param HSI_ENABLE	配置向导指定,片内RC48M时钟使能控制
* @retval None
*/
void CLOCK_Cmd(void)
{
}

/**
* @brief 系统主时钟设置
* @param HCLK_SRC		配置向导指定,参数如下
*     	@arg 0: 主时钟源选择内部RC48M
* @param HCLK_DIV		配置向导指定,HCLK的分频系数
* @param PCLK_DIV		配置向导指定,PCLK的分频系数
* @retval None
*/
void CLOCK_SystemClockConfig(void)
{
	RCC_HCLKSetPrescaler(HCLK_DIV-1);	//HCLK分频选择
	RCC_PCLKSetPrescaler(PCLK_DIV-1);	//PCLK分频选择
}

/**
* @brief 系统主时钟输出配置
* @param MCO_ENABLE	配置向导指定,系统主时钟输出使能
* @param MCO_PINSEL		配置向导指定,参数如下
*     	@arg 0: 系统主时钟输出到PA5/PA14/PB10
* @param MCO_DIV		系统主时钟输出的分频控制
* @retval None
*/
void CLOCK_MCOConfig(void)
{
	#if (MCO_ENABLE == 1)
		#if (MCO_SRC == 0 )
				RCC_SetMCOSource(RCC_ClockOutput_HSI);
		#endif
	
		#if (MCO_SRC == 1 )
			RCC_ClockSourceConfig(RCC_ClockSource_LSI, ENABLE);	//内部低频时钟使能
			RCC_SetMCOSource(RCC_ClockOutput_LSI);
		#endif
		
		#if (MCO_SRC == 2 )
			RCC_SetMCOSource(RCC_ClockOutput_SYSCLK);
		#endif	
	
		RCC_MCOSetPrescaler(MCO_DIV<<4);
	
		#if (MCO_PINSEL == 0)
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
			GPIO_DigitalRemapConfig(AFIOA, GPIO_Pin_5, AFIO_AF_6, ENABLE);
		#endif
		
		#if (MCO_PINSEL == 1)
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
			GPIO_DigitalRemapConfig(AFIOA, GPIO_Pin_14, AFIO_AF_6, ENABLE);
		#endif
		
		#if (MCO_PINSEL == 2)
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
			GPIO_DigitalRemapConfig(AFIOB, GPIO_Pin_10, AFIO_AF_6, ENABLE);
		#endif
	#else
		RCC_SetMCOSource(RCC_ClockOutput_None);
	#endif
}

/**
  * @brief  Gets the value of clock frequence.
  * @param  clock: specify the clock selection.
	*        @arg CLKSRC_SYSCLK: Get the system clock frequence
	*        @arg CLKSRC_HCLK: Get the AHB clock frequence
	*        @arg CLKSRC_PCLK: Get the APB clock frequence
	*        @arg CLKSRC_HSI: Get the HSI clock frequence	
	*        @arg CLKSRC_LSI: Get the LSI clock frequence
	*        @arg CLKSRC_HSE: Get the HSE clock frequence	
	*        @arg CLKSRC_LSE: Get the LSE clock frequence
  * @retval Clock frequence value
  */
u32 GetClockFreq(u8 clock)
{
	u32 CLK_Freq;
	u32 SYSCLK;
	u32 HCLK_Freq;
	u32 hclkDiv;
	u32 pclkDiv;
	
	/* Check the parameters */
	assert_param(IS_ClockSource(clock));
	
	if(clock == CLKSRC_HSI)
	{
	  CLK_Freq = HSI_VALUE;
	}
	else if(clock == CLKSRC_LSI)
	{
		CLK_Freq = LSI_VALUE;	
	}
	else 
	{
		SYSCLK = HSI_VALUE;
		if(clock == CLKSRC_SYSCLK)
		{
			CLK_Freq = SYSCLK;
		}
		else
		{
			hclkDiv = (RCC->CFGR & RCC_CFGR_HPRE) >> 16 ;
			HCLK_Freq = SYSCLK / (hclkDiv + 1);
			if(clock == CLKSRC_HCLK)
			{
				CLK_Freq = HCLK_Freq;
			}
			else
			{
				pclkDiv = (RCC->CFGR & RCC_CFGR_PPRE) >> 24;
				CLK_Freq = HCLK_Freq / (pclkDiv + 1);
			}
		}
	}
	return CLK_Freq;
}

/**
* @brief 系统时钟初始化
* @param None
* @retval None
*/
void SystemInit(void)
{	
	/* 时钟使能配置 */
	CLOCK_Cmd();
	/* 系统主时钟选择配置 */
	CLOCK_SystemClockConfig();
	/* Flash 操作等待周期 */
	IFMC_SetLatency(WAIT_STATE);
	/* 系统主时钟输出配置 */
	CLOCK_MCOConfig();
}
/**
  * @}
  */


/**
  * @}
  */


/**
  * @}
  */


/******************* Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved *****END OF FILE****/

