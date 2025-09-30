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



u16 capture_value = 0;
u8 flag = 0;

#if defined (PT32G031x) || defined (PTM280x)

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
* @brief 配置复用TIM输入捕获通道引脚
* @param 无
* @retval 无
*/
void CaptureTIM_GPIO_Config(void)
{
	/* 配置TIM管脚的复用功能 */
	GPIO_DigitalRemapConfig(CaptureTIM_CH_AFIO, CaptureTIM_CH_PIN, CaptureTIM_CH_AFx, ENABLE);
}

/**
* @brief 配置TIM工作方式
* @param 无
* @retval 无
*/
void CaptureTIM_Mode_Config(TIM_TypeDef *TIMx)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;//定义一个TIM_TimeBaseInitTypeDef类型的结构体
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	TIM_TimeBaseInitStruct.TIM_Prescaler = GetClockFreq(CLKSRC_HCLK)/1000-1;			//捕获时基为1ms
	TIM_TimeBaseInitStruct.TIM_AutoReloadValue = 0xFFFF;						   		//设置为最大65535ms
	TIM_TimeBaseInitStruct.TIM_Direction = TIM_Direction_Up;					 		//向上计数
	TIM_TimeBaseInitStruct.TIM_CenterAlignedMode = TIM_CenterAlignedMode_Disable; 		//不开启中心计数模式
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStruct);									//初始化CaptureTIM

	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;							//设置为通道1
	TIM_ICInitStruct.TIM_ICRiseCapture = TIM_ICRiseCapture_Disable;		 	//不使能输入上升沿捕获
	TIM_ICInitStruct.TIM_ICFallCapture = TIM_ICFallCapture_Enable;		 	//使能输入下降沿捕获
	TIM_ICInitStruct.TIM_ICResetCounter = TIM_ICResetCounter_Enable;   		//设置捕获后重置计数器
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPrescaler_None;					//设置为无分频
	TIM_ICInitStruct.TIM_ICFilter = TIM_ICFilter_4;							//设置滤波
	TIM_ICInitStruct.TIM_ICSource = TIM_ICSource_ICS1;						//通道1捕获源选择为ICS1
	TIM_ICInit(TIMx,&TIM_ICInitStruct);									//初始化捕获功能
	TIM_ITConfig(TIMx,TIM_IT_IC1I,ENABLE);                         		//使能通道1捕获中断
	
	NVIC_InitStruct.NVIC_IRQChannel = CaptureTIM_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIMx, ENABLE);													//开启CaptureTIM
}

/**
* @brief 配置TIM函数
* @param 无
* @retval 无
*/
void CaptureTIM_Driver()
{
	CaptureTIM_GPIO_Config();
	CaptureTIM_Mode_Config(CaptureTIM);
}

/**
  * @brief  配置各个外设时钟.
  * @param  无
  * @retval 无
  */
void RCC_Configuration(void)
{  
	/* CaptureTIM, UART, GPIO 时钟使能 */
	RCC_AHBPeriphClockCmd(CaptureTIM_CH_CLOCK|UART_Periph_GPIOx, ENABLE);
	RCC_APBPeriph1ClockCmd(CaptureTIM_Periph_CLOCK, ENABLE);
	RCC_APBPeriph2ClockCmd(APBPeriph2_UARTx, ENABLE);
#if defined	RETARGET_UART_CLKSRC
	RCC_Periph2ClockSourceConfig(RETARGET_UART_Periph,RETARGET_UART_CLKSRC);
#endif
}

int main (void)
{
	RCC_Configuration();
	UART_Driver(9600);
	CaptureTIM_Driver();
	printf("捕获通道输入周期脉冲\r\n");
	while(1)
	{
		if(flag == 1)
		{
			flag =0 ;
			printf("PWM周期脉宽：%dms\r\n",capture_value);
		}
	}
}

void CaptureTIM_Handler(void)
{
	if(TIM_GetFlagStatus(CaptureTIM,TIM_FLAG_IC1F)!=RESET)
	{
		capture_value = TIM_GetICxValue(CaptureTIM,TIM_Channel_1);
		TIM_ClearFlag(CaptureTIM,TIM_FLAG_IC1F);
		flag = 1;
	}
}

#elif defined (PT32x002x) || defined (PT32x012x) 

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
* @brief 配置复用TIM输入捕获通道引脚
* @param 无
* @retval 无
*/
void CaptureTIM_GPIO_Config(void)
{
	/* 配置TIM管脚的复用功能 */
	GPIO_DigitalRemapConfig(CaptureTIM_CH_AFIO, CaptureTIM_CH_PIN, CaptureTIM_CH_AFx, ENABLE);
}

/**
* @brief 配置TIM工作方式
* @param 无
* @retval 无
*/
void CaptureTIM_Mode_Config(TIM_TypeDef *TIMx)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;//定义一个TIM_TimeBaseInitTypeDef类型的结构体
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	TIM_TimeBaseInitStruct.TIM_Prescaler = GetClockFreq(CLKSRC_HCLK)/1000-1;			//捕获时基为1ms
	TIM_TimeBaseInitStruct.TIM_AutoReloadValue = 0xFFFF;						   		//设置为最大65535ms
	TIM_TimeBaseInitStruct.TIM_Direction = TIM_Direction_Up;					 		//向上计数
	TIM_TimeBaseInitStruct.TIM_CenterAlignedMode = TIM_CenterAlignedMode_Disable; 		//不开启中心计数模式
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStruct);									//初始化CaptureTIM

	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;							//设置为通道1
	TIM_ICInitStruct.TIM_ICRiseCapture = TIM_ICRiseCapture_Disable;		 	//不使能输入上升沿捕获
	TIM_ICInitStruct.TIM_ICFallCapture = TIM_ICFallCapture_Enable;		 	//使能输入下降沿捕获
	TIM_ICInitStruct.TIM_ICResetCounter = TIM_ICResetCounter_Enable;   		//设置捕获后重置计数器
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPrescaler_None;					//设置为无分频
	TIM_ICInitStruct.TIM_ICFilter = TIM_ICFilter_8;							//设置滤波
	TIM_ICInitStruct.TIM_ICSource = TIM_ICSource_ICS1;						//通道1捕获源选择为ICS1
	TIM_ICInit(TIMx,&TIM_ICInitStruct);									//初始化捕获功能
	TIM_ITConfig(TIMx,TIM_IT_IC1I,ENABLE);                         		//使能通道1捕获中断
	
	NVIC_InitStruct.NVIC_IRQChannel = CaptureTIM_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIMx, ENABLE);													//开启CaptureTIM
}

/**
* @brief 配置TIM函数
* @param 无
* @retval 无
*/
void CaptureTIM_Driver()
{
	CaptureTIM_GPIO_Config();
	CaptureTIM_Mode_Config(CaptureTIM);
}

/**
  * @brief  配置各个外设时钟.
  * @param  无
  * @retval 无
  */
void RCC_Configuration(void)
{  
	/* CaptureTIM, UART, GPIO 时钟使能 */
	RCC_AHBPeriphClockCmd(CaptureTIM_CH_CLOCK|UART_Periph_GPIOx, ENABLE);
	RCC_APBPeriph1ClockCmd(CaptureTIM_Periph_CLOCK, ENABLE);
	RCC_APBPeriph2ClockCmd(APBPeriph2_UARTx, ENABLE);
#if defined	RETARGET_UART_CLKSRC
	RCC_Periph2ClockSourceConfig(RETARGET_UART_Periph,RETARGET_UART_CLKSRC);
#endif
}

int main (void)
{
	RCC_Configuration();
	UART_Driver(9600);
	CaptureTIM_Driver();
	printf("捕获通道输入周期脉冲\r\n");
	while(1)
	{
		if(flag == 1)
		{
			flag =0 ;
			printf("PWM周期脉宽：%dms\r\n",capture_value);
		}
	}
}

void CaptureTIM_Handler(void)
{
	if(TIM_GetFlagStatus(CaptureTIM,TIM_FLAG_IC1F)!=RESET)
	{
		capture_value = TIM_GetICxValue(CaptureTIM,TIM_Channel_1);
		flag = 1;
		TIM_UpdateEvent(CaptureTIM);
		TIM_ClearFlag(CaptureTIM,TIM_FLAG_IC1F);
	}
}

#endif


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


