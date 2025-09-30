/******************************************************************************
  * @Example PT32x0xx_main
  * @author  Application development team
  * @version V1.0.0
  * @date    2023/11/07
  * @brief   按下USER_KEY,改变蓝灯的亮灭
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



u8 KET_IntFlag = 0;

/**
* @brief 配置LED的GPIO引脚
* @param 无
* @retval 无
*/
void LED_Driver(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;				//定义一个GPIO_InitTypeDef类型的结构体
	GPIO_InitStructure.GPIO_Pin = LED_BLUE_PIN;			//选择要控制的GPIO引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OutPP;		//设置引脚模式为通用推挽输出
	GPIO_InitStructure.GPIO_Pull = GPIO_Pull_Down;		//设置下拉电阻
	GPIO_Init(LED_BLUE_PORT, &GPIO_InitStructure);		//调用库函数，初始化GPIO
	
	GPIO_SetBits(LED_BLUE_PORT, LED_BLUE_PIN);			//设置GPIO状态
}

/**
* @brief 配置按键中断
* @param 无
* @retval 无
*/
void KEY_Int_Driver(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;					//定义一个NVIC_InitTypeDef类型的结构体
	GPIO_InitTypeDef GPIO_InitStructure;				//定义一个GPIO_InitTypeDef类型的结构体
	
	/*初始化GPIO*/
	GPIO_InitStructure.GPIO_Pin = USER_KEY_PIN;			//选择要控制的GPIO引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_In;		//设置引脚模式为数字输入
	GPIO_InitStructure.GPIO_Pull = GPIO_Pull_Up;		//设置上拉电阻
	GPIO_Init(GPIOB, &GPIO_InitStructure);				//调用库函数，初始化GPIO
	
	/*端口中断触发类型选择上升沿和下降沿*/
	EXTI_TriggerTypeConfig(USER_KEY_EXTI,USER_KEY_PIN, EXTI_Trigger_RisingFalling);
	EXTI_ITConfig(USER_KEY_EXTI,USER_KEY_PIN,ENABLE);

	/*设置NVIC控制器*/
	NVIC_InitStruct.NVIC_IRQChannel = USER_KEY_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x00;
	NVIC_Init(&NVIC_InitStruct);
}

/**
  * @brief  配置各个外设时钟.
  * @param  无
  * @retval 无
  */
void RCC_Configuration(void)
{  
	/* GPIO 时钟使能 */
	RCC_AHBPeriphClockCmd(LED_BLUE_GPIO_CLOCK|USER_KEY_GPIO_CLOCK, ENABLE);
}

int main (void)
{
	RCC_Configuration();
	LED_Driver();
	KEY_Int_Driver();
	while(1)
	{
		if(KET_IntFlag)
		{
			GPIO_ReverseBits(LED_BLUE_PORT, LED_BLUE_PIN);
			KET_IntFlag = 0;
		}
	}
}

/**
* @brief KEY中断服务函数
* @param None
* @retval None
*/
void USER_KEY_Handler(void)
{
	if(EXTI_GetITStatus(USER_KEY_EXTI,USER_KEY_PIN)!=RESET)
	{
		EXTI_ClearFlag(USER_KEY_EXTI,USER_KEY_PIN);	//清除中断标志位;
		KET_IntFlag = 1;
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


