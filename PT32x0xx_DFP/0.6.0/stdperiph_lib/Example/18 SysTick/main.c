/******************************************************************************
  * @Example PT32x0xx_main
  * @author  应用开发团队
  * @version V1.0.0
  * @date    2023/11/07
  * @brief   Systick定时1s闪烁一次绿灯
  *          
  ******************************************************************************
  * @attention	串口特性为：波特率9600，1位停止位，无奇偶校验
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers. 
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of the content or the  codes within the content.
  *
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  *
  *****************************************************************************/
	
#include "PT32x0xx.h"
#include "PT32x0xx_config.h"

u32 TimingDelay;


/**
* @brief 软件延时函数
* @param 无
* @retval 无
*/
void Software_Delay(void)
{
	u8 i, j,x;
	for(i=0; i<200; i++)
		for(j=0; j<200; j++)
			for(x=0; x<20; x++);
}

/**
* @brief LED驱动函数
* @param 无
* @retval 无
*/
void LED_Driver(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;				//定义一个GPIO_InitTypeDef类型的结构体
	GPIO_InitStructure.GPIO_Pin = LED_GREEN_PIN;		//选择要控制的GPIO引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OutPP;		//设置引脚模式为通用推挽输出
	GPIO_InitStructure.GPIO_Pull = GPIO_Pull_Up;		//上拉模式
	GPIO_Init(LED_GREEN_PORT, &GPIO_InitStructure);		//调用库函数，初始化GPIO
		
	GPIO_SetBits(LED_GREEN_PORT, LED_GREEN_PIN);		//设置GPIO状态
}

/**
* @brief SysTick驱动函数
* @param 无
* @retval 无
*/
void Systick_Driver(void)
{
	u32 HCLK = 0;
	HCLK = GetClockFreq(CLKSRC_HCLK);		//获得HCLK时钟
	if (SysTick_Config(HCLK / 100000))		//10us中断一次
	{ 
		while (1);
	}
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;//关闭SYSTIC计数器  
}

/**
* @brief 获取节拍程序，在SysTick中断函数 SysTick_Handler()调用
* @param 无
* @retval 无
*/
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}

/**
* @brief 设置毫秒级延时时间(10us为基底)
* @param 无
* @retval 无
*/
void Delay_ms(u32 nTime)
{ 
	TimingDelay = nTime*100;	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//使能SYSTIC计数器  
	while(TimingDelay != 0);
}

/**
  * @brief  配置各个外设时钟.
  * @param  无
  * @retval 无
  */
void RCC_Configuration(void)
{  
	/* GPIO时钟使能 */
	RCC_AHBPeriphClockCmd(UART_Periph_GPIOx, ENABLE);
}

int main (void)
{
	RCC_Configuration();
	LED_Driver();
	Systick_Driver();
	while(1)
	{
		GPIO_ReverseBits(LED_GREEN_PORT,LED_GREEN_PIN);
		Delay_ms(1000);
	}
}

/**
* @brief SysTick中断服务函数
* @param None
* @retval None
*/
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
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


