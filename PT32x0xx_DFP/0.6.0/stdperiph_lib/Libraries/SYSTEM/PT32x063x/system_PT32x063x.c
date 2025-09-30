/*
  ******************************************************************************
  * @file    system_PT32x063x.c
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

/** @addtogroup PT32x063x_StdPeriph_Driver
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
	if(RCC_GetSystemClockSelection() == 0)
	{
		return HSI_VALUE;	
	}
	else 
	{
		return MCU_HSE_FREQUENCE;
	}	
}

/** @defgroup CLK_Private_Functions
  * @{
  */

/**
* @brief ʱ��Դ����
* @param HSI_ENABLE	������ָ��,Ƭ��RC48Mʱ��ʹ�ܿ���
* @retval None
*/
void CLOCK_Cmd(void)
{
	#if (HSE_ENABLE == 1)
		#if (HSE_SRC==0)
			GPIO_DigitalRemapConfig(AFIOB, GPIO_Pin_8, AFIO_AF_0, ENABLE);
			GPIO_DigitalRemapConfig(AFIOB, GPIO_Pin_9, AFIO_AF_0, ENABLE);
			RCC_ClockSourceConfig(RCC_ClockSource_HSE, ENABLE);
		#else
			RCC_ClockSourceConfig(RCC_ClockSource_HSEExternal, ENABLE);
		#endif
	#else
		RCC_ClockSourceConfig(RCC_ClockSource_HSE, DISABLE);
	#endif

	#if (HSI_ENABLE == 1)
		RCC_ClockSourceConfig(RCC_ClockSource_HSI, ENABLE);	//ʹ���ڲ�RC
	#else
	//	RCC_ClockSourceConfig(RCC_ClockSource_HSI, DISABLE);	//�ر��ڲ�RC
	#endif

	#if (LSE_ENABLE ==1)
		#if (LSE_SRC ==0)
		  RCC_ClockSourceConfig(RCC_ClockSource_LSEExternal, ENABLE);
		#else
			GPIO_DigitalRemapConfig(AFIOB, GPIO_Pin_10, AFIO_AF_0, ENABLE);
			GPIO_DigitalRemapConfig(AFIOB, GPIO_Pin_11, AFIO_AF_0, ENABLE);
			RCC_ClockSourceConfig(RCC_ClockSource_LSE, ENABLE);
		#endif
	#else
		RCC_ClockSourceConfig(RCC_ClockSource_LSE, DISABLE);
	#endif
}

/**
* @brief ϵͳ��ʱ������
* @param HCLK_SRC		������ָ��,��������
*     	@arg 0: ��ʱ��Դѡ���ڲ�RC48M
* @param HCLK_DIV		������ָ��,HCLK�ķ�Ƶϵ��
* @param PCLK_DIV		������ָ��,PCLK�ķ�Ƶϵ��
* @retval None
*/
void CLOCK_SystemClockConfig(void)
{
	#if (HCLK_SRC == 0 )
		#if (HSI_ENABLE == 0)
			#error "ϵͳʱ��Դ����HSI ��δʹ��"
		#else

		#endif
	#endif	
	#if (HCLK_SRC == 1 )
		#if (HSE_ENABLE == 0)
			#error "ϵͳʱ��Դ����HSE ��δʹ��"
		#else
			Wait_ClockReady();
			RCC_SetSystemClock(RCC_SystemClock_HSE);
			//ʱ�Ӱ�ȫ���ƴ���
			if (HSI_ENABLE == 0)	RCC_ClockSourceConfig(RCC_ClockSource_HSI, DISABLE);	//�ر��ڲ�RC 32M
		#endif
	#endif
	RCC_HCLKSetPrescaler(HCLK_DIV-1);	//HCLK��Ƶѡ��
	RCC_PCLKSetPrescaler(PCLK_DIV-1);	//PCLK��Ƶѡ��
	
	if((RCC_GetSystemClockSelection()) != HCLK_SRC)	//ϵͳʱ��ѡ�����
	{
		while(1);
	}
	//ϵͳʱ����˯�߻��Ѻ��Զ��л���˯��֮ǰ��ʱ��
	RCC_SetSystemClockAfterWakeUp(RCC_SYSCLK_AfterWakeUp_PrevClock);
}

/**
* @brief ϵͳ��ʱ���������
* @param MCO_ENABLE	������ָ��,ϵͳ��ʱ�����ʹ��
* @param MCO_PINSEL		������ָ��,��������
*     	@arg 0: ϵͳ��ʱ�������PC15
* @param MCO_DIV		ϵͳ��ʱ������ķ�Ƶ����
* @retval None
*/
void CLOCK_MCOConfig(void)
{
	#if (MCO_ENABLE == 1)
		#if (MCO_SRC == 0 )
			#if (HSI_ENABLE == 0)
				#error "ϵͳʱ��Դ����HSI ��δʹ��"
			#else
				RCC_SetMCOSource(RCC_ClockOutput_HSI);
			#endif
		#endif
	
		#if (MCO_SRC == 1 )
			#if (HSE_ENABLE == 0)
				#error "ϵͳʱ��Դ����HSE ��δʹ��"
			#else
				RCC_SetMCOSource(RCC_ClockOutput_HSE);
			#endif
		#endif
	
		#if (MCO_SRC == 2 )
			RCC_ClockSourceConfig(RCC_ClockSource_LSI, ENABLE);	//�ڲ���Ƶʱ��ʹ��
			RCC_SetMCOSource(RCC_ClockOutput_LSI);
		#endif
		
		#if (MCO_SRC == 3 )
			RCC_SetMCOSource(RCC_ClockOutput_SYSCLK);
		#endif
		#if (MCO_SRC == 4 )		
			RCC_SetMCOSource(RCC_ClockOutput_LSE);
		#endif		
	
		RCC_MCOSetPrescaler(MCO_DIV<<4);
	
		#if (MCO_PINSEL == 0)
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
			GPIO_DigitalRemapConfig(AFIOC, GPIO_Pin_15, AFIO_AF_2, ENABLE);
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
	else if(clock == CLKSRC_HSE)
	{
		CLK_Freq = MCU_HSE_FREQUENCE;
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

