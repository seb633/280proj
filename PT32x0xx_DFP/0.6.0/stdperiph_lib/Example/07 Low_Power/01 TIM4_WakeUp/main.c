/******************************************************************************
  * @Example PT32x0xx_main
  * @author  Application development team
  * @version V1.0.0
  * @date    2023/11/07
  * @brief   Main program body
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
	u8 i, j,x;
	for(i=0; i<200; i++)
		for(j=0; j<200; j++)
			for(x=0; x<20; x++);
}

/**
* @brief 配置UART的复用引脚
* @param 无
* @retval 无
*/
void UART_GPIO_Config(void)
{ 
	GPIO_DigitalRemapConfig(UART_TX_PORT, UART_TX_PIN, UART_TX_AFIO,ENABLE);	//UART_TX数字功能复用
	GPIO_DigitalRemapConfig(UART_RX_PORT, UART_RX_PIN, UART_RX_AFIO,ENABLE);	//UART_RX数字功能复用
}

/**
* @brief 配置UART的工作模式
* @param 无
* @retval 无
*/
void UART_Mode_Config(u32 Baudrate)
{
	UART_InitTypeDef UART_InitStruct;

	/*初始化UART*/
	UART_InitStruct.UART_BaudRate = Baudrate;
	UART_InitStruct.UART_DataLength = UART_DataLength_8;
	UART_InitStruct.UART_StopBitLength = UART_StopBitLength_1;
	UART_InitStruct.UART_ParityMode = UART_Parity_None;
	UART_InitStruct.UART_Receiver = UART_Receiver_Enable;
	UART_InitStruct.UART_SampleRate = RETARGET_UART_SampleRate;
	UART_Init(RETARGET_UART_PORT, &UART_InitStruct);
	/*使能UART*/
	UART_Cmd(RETARGET_UART_PORT, ENABLE);
}

/**
* @brief UART驱动函数
* @param 无
* @retval 无
*/

void UART_Driver(u32 Baudrate)
{
	UART_Mode_Config(Baudrate);
	UART_GPIO_Config();
}

/**
* @brief TIM4工作方式配置
* @param 无
* @retval 无
*/
void TIM4_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	/*使能TIMER4的NVIC控制器*/
	NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x03;
	NVIC_Init(&NVIC_InitStruct);
	
	/* 驱动CNT计数器的时钟 = 32khz/(psc+1)*/
	TimeBaseInitStruct.TIM_Prescaler = 32-1;
	TimeBaseInitStruct.TIM_Direction = TIM_Direction_Down;
	TimeBaseInitStruct.TIM_AutoReloadValue = 1000-1;
	
	TIM_TimeBaseInit(TIM4, &TimeBaseInitStruct);
	TIM_ITConfig(TIM4, TIM_IT_ARI, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}

/**
* @brief 延时10ms
* @param None
* @retval None
*/
void Delay_10ms(void)
{
	u32 Timebase = 0;
	u32 temp;
	Timebase = GetClockFreq(CLKSRC_HCLK)/100;
	SysTick->LOAD = Timebase;
	SysTick->VAL = 0x00;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	do
	{
		temp = SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL = 0x00;
}

/**
  * @brief  配置各个外设时钟.
  * @param  无
  * @retval 无
  */
void RCC_Configuration(void)
{  
	/* TIM4, GPIO clocks enable */
	RCC_AHBPeriphClockCmd(UART_Periph_GPIOx, ENABLE);
	RCC_APBPeriph1ClockCmd(RCC_APBPeriph1_TIM4, ENABLE);
	RCC_APBPeriph2ClockCmd(APBPeriph2_UARTx,ENABLE);
#if defined	RETARGET_UART_CLKSRC
	RCC_Periph2ClockSourceConfig(RETARGET_UART_Periph,RETARGET_UART_CLKSRC);
#endif
}

int main (void)
{
	Delay_10ms();//等待芯片内部LDO稳定，不可删除
	RCC_Configuration();
	UART_Driver(9600);
	TIM4_Mode_Config();	
	while(1)
	{
		PWR_EnterDeepSleepMode(PWR_DeepSleepEntry_WFI);   //深度睡眠，中断唤醒
		printf("wake up\r\n");
		while(UART_GetFlagStatus(RETARGET_UART_PORT,UART_FLAG_TXC) == 0);
	}
}

/**
* @brief TIM4中断服务函数
* @param None
* @retval None
*/
void TIM4_Handler(void)
{
	TIM_ClearFlag(TIM4,TIM_FLAG_ARF);
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


