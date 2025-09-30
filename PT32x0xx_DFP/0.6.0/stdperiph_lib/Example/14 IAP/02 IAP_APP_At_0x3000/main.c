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

u32 num,IAP_Status1,interupt_flag = 0;

#if defined (PT32G031x) || defined (PTM280x)

void SysTick_Init(void)
{
    /* SystemFrequency / 1000    1ms中断一次
    * SystemFrequency / 100000  10us中断一次
    * SystemFrequency / 1000000 1us中断一次
    */
    if (SysTick_Config(GetClockFreq(CLKSRC_HCLK) / 1000))
	{
        while (1);
    }
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
  * @brief  配置各个外设时钟.
  * @param  无
  * @retval 无
  */
void RCC_Configuration(void)
{  
	/* UART, GPIO 时钟使能 */
	RCC_AHBPeriphClockCmd(UART_Periph_GPIOx, ENABLE);
	RCC_APBPeriph2ClockCmd(APBPeriph2_UARTx,ENABLE); 
#if defined	RETARGET_UART_CLKSRC
	RCC_Periph2ClockSourceConfig(RETARGET_UART_Periph,RETARGET_UART_CLKSRC);
#endif
}

int main (void)
{
	RCC_Configuration();
	UART_Driver();
	
	printf("\r\n*****************APP 0x3000 START***************** ");
	SysTick_Init();
	while(interupt_flag == 0);
	printf("\r\n如果要跳转到0x0运行，请先下载工程01 IAP_APP_At_0x0");
	printf("\r\n是否跳转到APP 0x0     1----是   2----否");
	num=getchar();
	switch(num)
	{
		case 1:
			printf("\r\n已选择1--跳转到APP 0x0\r\n");
			SYSCFG->IAPAR = 0x0;   //IAP模式跳转地址寄存器
			RCC_AdvancedSoftwareReset(RCC_AdvancedSoftwareReset_CPU); //CPU Reset;
			break;
		case 2:
			printf("\r\n已选择2--不跳转");
			break;
			
		default:
			printf("\r\n错误字符");
			break;
	}
    while(1);
}

void SysTick_Handler(void)
{
	printf("\r\napp 0x3000 systick中断");
	SysTick->CTRL = 0;
	interupt_flag = 1;
}

#elif defined (PT32x002x) || defined(PT32x012x)

void SysTick_Init(void)
{
    /* SystemFrequency / 1000    1ms中断一次
    * SystemFrequency / 100000  10us中断一次
    * SystemFrequency / 1000000 1us中断一次
    */
    if (SysTick_Config(GetClockFreq(CLKSRC_HCLK) / 1000)) 
	{
        while (1);
    }
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
  * @brief  配置各个外设时钟.
  * @param  无
  * @retval 无
  */
void RCC_Configuration(void)
{  
	/* UART, GPIO 时钟使能 */
	RCC_AHBPeriphClockCmd(UART_Periph_GPIOx, ENABLE);
	RCC_APBPeriph2ClockCmd(APBPeriph2_UARTx,ENABLE); 
#if defined	RETARGET_UART_CLKSRC
	RCC_Periph2ClockSourceConfig(RETARGET_UART_Periph,RETARGET_UART_CLKSRC);
#endif
}

int main (void)
{
	RCC_Configuration();
	UART_Driver(9600);
	if((SYSCFG->SR1&SYSCFG_SR1_IAP) != SYSCFG_SR1_IAP)//判断是否使能IAP
	{
		SYSCFG_IAPConfig(ENABLE);
	}	
	printf("\r\n*****************APP 0x3000 START***************** ");
	SysTick_Init();
	while(interupt_flag == 0);
	printf("\r\n如果要跳转到0x0运行，请先下载工程01 IAP_APP_At_0x0");
	printf("\r\n是否跳转到APP 0x0     1----是   2----否");
	num=getchar();
	switch(num)
	{
		case 1:
			printf("\r\n已选择1--跳转到APP 0x0\r\n");
			SYSCFG->IAPAR = 0x0;   //IAP模式跳转地址寄存器
			RCC_AdvancedSoftwareReset(RCC_AdvancedSoftwareReset_CPU); //CPU Reset;
			break;
		case 2:
			printf("\r\n已选择2--不跳转");
			break;
			
		default:
			printf("\r\n错误字符");
			break;
	}
    while(1);
}

void SysTick_Handler(void)
{
	printf("\r\napp 0x3000 systick中断");
	SysTick->CTRL = 0;
	interupt_flag = 1;
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


