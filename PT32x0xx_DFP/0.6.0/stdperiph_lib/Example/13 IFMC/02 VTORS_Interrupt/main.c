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


#if defined (PT32G031x) || defined (PTM280x)

#define		SYS_VECTOR_CNT  16

extern void Reset_Handler(void);
extern void NMI_Handler(void);
extern void HardFault_Handler(void);
u32 MapToSramADDR[192/4] = {0};
u32 interrupt = 0;

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
	UART_InitStruct.UART_SampleRate = UART_SampleRate_8X;
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
* @brief 配置TIM2工作方式
* @param 无
* @retval 无
*/
void TIM2_Int_Diver(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;						//定义一个NVIC_InitTypeDef类型的结构体
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;		//定义一个NVIC_InitTypeDef类型的结构体
	
	TIM_TimeBaseInitStruct.TIM_Prescaler = 6400-1;									//64M/6400=0.01MHZ
	TIM_TimeBaseInitStruct.TIM_AutoReloadValue = 10000;								//0.01MHZ*10000=1HZ
	TIM_TimeBaseInitStruct.TIM_Direction = TIM_Direction_Up;					 	//向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);								//初始化TIM2
	
	TIM_ITConfig(TIM2,TIM_IT_ARI,ENABLE);									//定时中断初始化
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;							//定时中断源设置
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x00;							//中断优先级设置
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;							//使能NVIC控制器
	NVIC_Init(&NVIC_InitStruct);											//初始化NVIC	
	
	TIM_Cmd(TIM2, ENABLE);													//开启TIM2
}

/**
* @brief 使能IVTS功能/将自定义中断向量表映射至SRAM
* @param 无
* @retval 无
*/
void IFMC_IVTS_Enable(void)
{
	__disable_irq();
	SYSCFG_VTORSEnable((u32)MapToSramADDR);
	__enable_irq();
}

/**
* @brief 自定义TIM2中断服务函数
* @param 无
* @retval 无
*/
void SRAM_TIM2_Handler(void)
{
	if(TIM_GetFlagStatus(TIM2,TIM_FLAG_ARF) != RESET)
	{
		TIM_ClearFlag(TIM2,TIM_FLAG_ARF);
		printf("开启IVTS功能,触发SRAM中的TIM2中断（此信息由中断服务函数打印）\r\n");
		interrupt++;
	}
}
/**
* @brief 自定义中断向量表
* @param 无
* @retval 无
*/

void IFMC_InterruptInSrramConfig(void)
{
	MapToSramADDR[1] = (u32)Reset_Handler;//Reset Handler
	MapToSramADDR[2] = (u32)NMI_Handler;//NMI Handler
	MapToSramADDR[3] = (u32)HardFault_Handler;//HardFault_Handler
	MapToSramADDR[SYS_VECTOR_CNT + TIM2_IRQn] = (u32)SRAM_TIM2_Handler;//将原先存放TIM2中断服务函数地址改存为自定义服务函数
}

/**
  * @brief  配置各个外设时钟.
  * @param  无
  * @retval 无
  */
void RCC_Configuration(void)
{  
	/* TIM2, UART0, GPIO 时钟使能 */
	RCC_AHBPeriphClockCmd(UART_Periph_GPIOx, ENABLE);
	RCC_APBPeriph1ClockCmd(RCC_APBPeriph1_TIM2,ENABLE); 
	RCC_APBPeriph2ClockCmd(APBPeriph2_UARTx, ENABLE); 
#if defined	RETARGET_UART_CLKSRC
	RCC_Periph2ClockSourceConfig(RETARGET_UART_Periph,RETARGET_UART_CLKSRC);
#endif
}

int main (void)
{
	RCC_Configuration();
	UART_Driver(9600);
	TIM2_Int_Diver();
	IFMC_InterruptInSrramConfig();
	while(1)
	{
		if(interrupt%2)
		{
			IFMC_IVTS_Enable();
		}
		else
		{
			SYSCFG_VTORSDisable();
		}
	}
}


/**
* @brief TIM2中断服务函数
* @param None
* @retval None
*/
void TIM2_Handler(void)
{
	extern u32 interrupt;
	if(TIM_GetFlagStatus(TIM2,TIM_FLAG_ARF) != RESET)
	{
		TIM_ClearFlag(TIM2,TIM_FLAG_ARF);
		printf("关闭IVTS功能,触发Flash中的TIM2中断（此信息由中断服务函数打印）\r\n");
		interrupt++;
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


