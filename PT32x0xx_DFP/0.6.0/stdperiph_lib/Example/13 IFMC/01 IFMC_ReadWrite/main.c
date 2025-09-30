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

#if defined(PT32G031x) || defined (PTM280x)

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
	/* UART, IFMC 时钟使能 */
	RCC_AHBPeriphClockCmd(UART_Periph_GPIOx, ENABLE);
	RCC_APBPeriph2ClockCmd(APBPeriph2_UARTx, ENABLE);
#if defined	RETARGET_UART_CLKSRC
	RCC_Periph2ClockSourceConfig(RETARGET_UART_Periph,RETARGET_UART_CLKSRC);
#endif
}

int main (void)
{
	RCC_Configuration();
	UART_Driver(9600);
	IFMC_ErasePage(IFMC_Address);
	IFMC_ProgramWord(IFMC_Address,123);
	IFMC_ProgramWord(IFMC_Address+0x4,456);
	IFMC_ProgramWord(IFMC_Address+0x8,0xCC);
	IFMC_ProgramWord(IFMC_Address+0xC,0xDD);
	printf("主程序区0x%lx: %ld\r\n",IFMC_Address,IFMC_ReadWord(IFMC_Address));
	printf("主程序区0x%lx: %ld\r\n",IFMC_Address,IFMC_ReadWord(IFMC_Address+4));
	printf("主程序区0x%lx: %X\r\n",IFMC_Address,IFMC_ReadByte(IFMC_Address+8));
	printf("主程序区0x%lx: %X\r\n",IFMC_Address,IFMC_ReadByte(IFMC_Address+0xC));
	while(1);
}

#elif defined (PT32x012x) || defined(PT32x002x)

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
	UART_InitStruct.UART_SampleRate = UART_SampleRate_4X;
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
	/* UART, IFMC 时钟使能 */
	RCC_AHBPeriphClockCmd(UART_Periph_GPIOx, ENABLE);
	RCC_APBPeriph4ClockCmd(RCC_APBPeriph4_IFMC,ENABLE);
	RCC_APBPeriph2ClockCmd(APBPeriph2_UARTx, ENABLE);
#if defined	RETARGET_UART_CLKSRC
	RCC_Periph2ClockSourceConfig(RETARGET_UART_Periph,RETARGET_UART_CLKSRC);
#endif
}

int main (void)
{
	RCC_Configuration();
	UART_Driver(9600);
	IFMC_ErasePage(IFMC_Address);
	IFMC_ProgramWord(IFMC_Address,123);
	IFMC_ProgramWord(IFMC_Address+0x4,456);
	IFMC_ProgramWord(IFMC_Address+0x8,0xCC);
	IFMC_ProgramWord(IFMC_Address+0xC,0xDD);
	printf("主程序区0x%lx: %ld\r\n",IFMC_Address,IFMC_ReadWord(IFMC_Address));
	printf("主程序区0x%lx: %ld\r\n",IFMC_Address,IFMC_ReadWord(IFMC_Address+4));
	printf("主程序区0x%lx: %X\r\n",IFMC_Address,IFMC_ReadByte(IFMC_Address+8));
	printf("主程序区0x%lx: %X\r\n",IFMC_Address,IFMC_ReadByte(IFMC_Address+0xC));
	while(1);
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


