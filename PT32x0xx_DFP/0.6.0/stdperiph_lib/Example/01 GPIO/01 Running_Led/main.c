/******************************************************************************
  * @Example PT32x0xx_main
  * @author  Application development team
  * @version V1.0.0
  * @date    2023/11/07
  * @brief   RGB灯轮流闪烁
  *          
  ******************************************************************************
  * @attention	
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers. 
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of the content or the  codes within the content.
  *
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  *
  *****************************************************************************/
	
#include "PT32x0xx.h"
#include "PT32x0xx_config.h"

/**
* @brief 软件延时函数
* @param 无
* @retval 无
*/
void Software_Delay(void)
{
	u8 i,j,x;
	for(i=0; i<200; i++)
		for(j=0; j<200; j++)
			for(x=0; x<100; x++);
}

/**
* @brief LED驱动函数
* @param 无
* @retval 无
*/
void LED_Driver(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义一个GPIO_InitTypeDef类型的结构体
	
	GPIO_InitStructure.GPIO_Pin = LED_RED_PIN;//选择要控制的GPIO引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OutPP;//设置引脚模式为通用推挽输出
	GPIO_InitStructure.GPIO_Pull = GPIO_Pull_Up;//上拉模式
	GPIO_Init(LED_RED_PORT, &GPIO_InitStructure);//调用库函数，初始化GPIO		
	GPIO_SetBits(LED_RED_PORT, LED_RED_PIN);//设置GPIO状态
	
	GPIO_InitStructure.GPIO_Pin = LED_GREEN_PIN;//选择要控制的GPIO引脚
	GPIO_Init(LED_GREEN_PORT, &GPIO_InitStructure);//调用库函数，初始化GPIO
	GPIO_SetBits(LED_GREEN_PORT, LED_GREEN_PIN);//设置GPIO状态	
	
	GPIO_InitStructure.GPIO_Pin = LED_BLUE_PIN;//选择要控制的GPIO引脚
	GPIO_Init(LED_BLUE_PORT, &GPIO_InitStructure);//调用库函数，初始化GPIO
	GPIO_SetBits(LED_BLUE_PORT, LED_BLUE_PIN);//设置GPIO状态
}

/**
  * @brief  配置各个外设时钟.
  * @param  无
  * @retval 无
  */
void RCC_Configuration(void)
{  
	/* GPIO 时钟使能 */
	RCC_AHBPeriphClockCmd(LED_RED_GPIO_CLOCK|LED_GREEN_GPIO_CLOCK|LED_BLUE_GPIO_CLOCK, ENABLE);
}

int main (void)
{
	RCC_Configuration();
	LED_Driver();
	while(1)
	{
		GPIO_ResetBits(LED_RED_PORT, LED_RED_PIN);
		Software_Delay();
		GPIO_SetBits(LED_RED_PORT, LED_RED_PIN);
		Software_Delay();
		
		GPIO_ResetBits(LED_GREEN_PORT, LED_GREEN_PIN);
		Software_Delay();
		GPIO_SetBits(LED_GREEN_PORT, LED_GREEN_PIN);
		Software_Delay();

		GPIO_ResetBits(LED_BLUE_PORT, LED_BLUE_PIN);
		Software_Delay();
		GPIO_SetBits(LED_BLUE_PORT, LED_BLUE_PIN);
		Software_Delay();		
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
	printf("Wrong parameters value: file %s on line %d\r\n", file, line);
	/* Infinite loop */
	while (1)
	{
	}
}
#endif


