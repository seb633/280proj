/*
  ******************************************************************************
  * @file    system_PT32x012x.c
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

/** @addtogroup PT32x012x_StdPeriph_Driver
  * @{
  */


/** @addtogroup CLK
  * @{
  */


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

/**
  * @}
  */

/**
* @brief ��ȡϵͳʱ��
* @param None
* @retval None
*/
u32 GetSystemClock(void)
{
	return HSI_VALUE;	
}

/** @defgroup CLK_Private_Functions
  * @{
  */

/**
  * @brief 	ʱ������
  * @param 	LSE_ENABLE	������ָ��,�ⲿ32Kʱ��ʹ�ܿ���
  * @retval None
*/
void CLOCK_Cmd(void)
{
#if (LSE_ENABLE ==1)
	#if (LSE_SRC ==0)
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
		GPIO_DigitalRemapConfig(AFIOB, GPIO_Pin_7, AFIO_AF_1, ENABLE);
		RCC_ClockSourceConfig(RCC_ClockSource_LSE, DISABLE);
		RCC_ClockSourceConfig(RCC_ClockSource_LSEExternal, ENABLE);    	//���32K
	#else
		RCC_ClockSourceConfig(RCC_ClockSource_LSEExternal, DISABLE);
		RCC_ClockSourceConfig(RCC_ClockSource_LSE, ENABLE);  //LSE����
	#endif
#else
    RCC_ClockSourceConfig(RCC_ClockSource_LSE, DISABLE);
#endif
}

/**
  * @brief 	ϵͳ��ʱ������
  * @param 	HCLK_DIV		������ָ��,HCLK�ķ�Ƶϵ��
  * @param 	PCLK_DIV		������ָ��,PCLK�ķ�Ƶϵ��
  * @retval None
  */
void CLOCK_SystemClockConfig(void)
{
	RCC_HCLKSetPrescaler(HCLK_DIV-1);	//HCLK��Ƶѡ��
	RCC_PCLKSetPrescaler(PCLK_DIV-1);	//PCLK��Ƶѡ��
}

/**
  * @brief  ϵͳ��ʱ���������
  * @param  MCO_ENABLE	������ָ��,ϵͳ��ʱ�����ʹ��
  * @param  MCO_SRC		ʱ�����Դѡ��
  *     	@arg0: ʱ�����ԴΪLSIʱ��
  *     	@arg1: ʱ�����ԴΪSYS_CLKʱ��
  *     	@arg2: ʱ�����ԴΪLSEʱ��
  * @param  MCO_PINSEL: ������ָ��,��������
  *     	@arg0: MCOʱ�������PB1
  * @param  MCO_DIV		MCOʱ������ķ�Ƶ����
  * @retval None
*/
void CLOCK_MCOConfig(void)
{
	#if (MCO_ENABLE == 1)
	
		#if (MCO_SRC == 0 )
			RCC_ClockSourceConfig(RCC_ClockSource_LSI, ENABLE);	//�ڲ���Ƶʱ��ʹ��
			RCC_SetMCOSource(RCC_ClockOutput_LSI);
		#endif
		
		#if (MCO_SRC == 1 )
			RCC_SetMCOSource(RCC_ClockOutput_SYSCLK);
		#endif
	
		#if (MCO_SRC == 2 )
			RCC_SetMCOSource(RCC_ClockOutput_LSE);
		#endif		
		
		RCC_MCOSetPrescaler(MCO_DIV<<4);		//MCO��Ƶ����
	
		#if (MCO_PINSEL == 0)
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
			GPIO_DigitalRemapConfig(AFIOB, GPIO_Pin_1, AFIO_AF_0, ENABLE);
		#endif
	#else

		RCC_SetMCOSource(RCC_ClockOutput_None);		//MCO�����
		
	#endif
}

/**
  * @brief 	Gets the value of clock frequence.
  * @param  clock: specify the clock selection.
  *        	@arg CLKSRC_SYSCLK: Get the system clock frequence
  *        	@arg CLKSRC_HCLK: Get the AHB clock frequence
  *        	@arg CLKSRC_PCLK: Get the APB clock frequence
  *        	@arg CLKSRC_HSI: Get the HSI clock frequence	
  *        	@arg CLKSRC_LSI: Get the LSI clock frequence
  *        	@arg CLKSRC_HSE: Get the HSE clock frequence	
  *        	@arg CLKSRC_LSE: Get the LSE clock frequence
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
	else if(clock == CLKSRC_LSE)
	{
		CLK_Freq = LSE_VALUE;	
	}	
	else 
	{
		SYSCLK = GetSystemClock();
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
* @brief ϵͳʱ�ӳ�ʼ��
* @param None
* @retval None
*/
void SystemInit(void)
{	
	/* ʱ��ʹ������ */
	CLOCK_Cmd();
	/* ϵͳ��ʱ��ѡ������ */
	CLOCK_SystemClockConfig();
	/* Flash �����ȴ����� */
	IFMC_SetLatency(WAIT_STATE);
	/* ϵͳ��ʱ��������� */
	CLOCK_MCOConfig();
}

//<<< end of configuration section >>>

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

