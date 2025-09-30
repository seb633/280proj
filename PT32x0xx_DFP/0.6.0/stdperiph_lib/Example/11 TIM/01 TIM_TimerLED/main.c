/******************************************************************************
  * @Example PT32x0xx_main
  * @author  应用开发团队
  * @version V1.0.0
  * @date    2023/11/07
  * @brief   蓝灯每隔1秒闪烁一次
  *          
  ******************************************************************************
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers. 
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of the content or the  codes within the content.
  *
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  *
  *****************************************************************************/
	
#include "PT32x0xx.h"
#include "PT32x0xx_config.h"




u8 TIM_IT_FLAG = 0;

/**
  * @brief 	LED驱动函数
  * @param 	无
  * @retval 无
*/
void LED_Driver(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;				//定义一个GPIO_InitTypeDef类型的结构体
	GPIO_InitStructure.GPIO_Pin = LED_BLUE_PIN;			//选择要控制的GPIO引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OutPP;		//设置引脚模式为通用推挽输出
	GPIO_InitStructure.GPIO_Pull = GPIO_Pull_Up;		//上拉电阻
	GPIO_Init(LED_BLUE_PORT, &GPIO_InitStructure);		//调用库函数，初始化GPIO
		
	GPIO_SetBits(LED_BLUE_PORT, LED_BLUE_PIN);			//设置GPIO状态
}

/**
  * @brief 	配置TIMER_LED工作方式
  * @param 	无
  * @retval 无
*/
void TIMER_LED_Diver(TIM_TypeDef *TIMx)
{
	NVIC_InitTypeDef NVIC_InitStruct;							//定义一个NVIC_InitTypeDef类型的结构体
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;			//定义一个NVIC_InitTypeDef类型的结构体
	
	TIM_TimeBaseInitStruct.TIM_Prescaler = GetClockFreq(CLKSRC_PCLK)/10000-1;				
	TIM_TimeBaseInitStruct.TIM_AutoReloadValue = 10000-1;		//0.01MHZ/10000=1HZ
	TIM_TimeBaseInitStruct.TIM_Direction = TIM_Direction_Up;	//向上计数
	TIM_TimeBaseInitStruct.TIM_CenterAlignedMode = TIM_CenterAlignedMode_Disable;
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStruct);			//初始化TIM21
	
	TIM_ITConfig(TIMx,TIM_IT_ARI,ENABLE);						//定时中断初始化
	
	NVIC_InitStruct.NVIC_IRQChannel = TimerLed_IRQn;			//定时中断源设置
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x00;			//中断优先级设置
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			//使能NVIC控制器
	NVIC_Init(&NVIC_InitStruct);							//初始化NVIC	
	
	TIM_Cmd(TIMx, ENABLE);									//开启TimerLed
}

/**
  * @brief  配置各个外设时钟.
  * @param  无
  * @retval 无
  */
void RCC_Configuration(void)
{  
	/* TimerLed, GPIO clocks enable */
	RCC_AHBPeriphClockCmd(LED_BLUE_GPIO_CLOCK, ENABLE);
	RCC_APBPeriph1ClockCmd(TimerLed_Periph_CLOCK, ENABLE);
}


int main (void)
{
	RCC_Configuration();
	LED_Driver();
	TIMER_LED_Diver(TimerLed);
	while(1)
	{
		if(TIM_IT_FLAG)	//获取中断标志是否被设置
		{
			GPIO_ResetBits(LED_BLUE_PORT,LED_BLUE_PIN);
		}
		else
		{
			GPIO_SetBits(LED_BLUE_PORT,LED_BLUE_PIN);
		} 
	}
}

void TimerLed_Handler(void)
{
	if(TIM_GetFlagStatus(TimerLed,TIM_FLAG_ARF) != RESET)
	{
		TIM_ClearFlag(TimerLed,TIM_FLAG_ARF);
		TIM_IT_FLAG = ~TIM_IT_FLAG;
	}
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  */
void assert_failed(u8* file, u32 line)
{
	/* User can add his own implementation to report the file name and line number,
	   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	printf("Wrong parameters value: file %s on line %ld\r\n", file, line);
	/* Infinite loop */
	while (1)
	{
	}
}
#endif


