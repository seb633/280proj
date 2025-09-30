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
* @brief IWDG驱动函数
* @param 无
* @retval 无
*/
void IWDG_Driver(u32 reloadvalue)
{	
	IWDG_LockCmd(IWDG,IWDG_LockKey_Unlock);//解锁IWDG寄存器
	IWDG_SetReload(IWDG, reloadvalue);//设置重装载寄存器值
	IWDG_ReloadCounter(IWDG);//喂狗(重载计数器)
	RCC_ResetConfig(RCC_ResetEnable_IWDG, ENABLE);//看门狗复位使能
	IWDG_Cmd(IWDG, ENABLE);//使能IWDG
	IWDG_LockCmd(IWDG, IWDG_LockKey_Lock);	//锁住IWDG寄存器
}

/**
* @brief IWDG喂狗函数
* @param 无
* @retval 无
*/
void IWDG_Feed(void)
{
	IWDG_LockCmd(IWDG, IWDG_LockKey_Unlock);//解锁IWDG寄存器
	IWDG_ReloadCounter(IWDG);//喂狗(重载计数器)
	IWDG_LockCmd(IWDG, IWDG_LockKey_Lock);//锁住IWDG寄存器	
}

/**
  * @brief  配置各个外设时钟.
  * @param  无
  * @retval 无
  */
void RCC_Configuration(void)
{  
	/* UART0, IWDG, GPIO 时钟使能 */
    RCC_AHBPeriphClockCmd(UART_Periph_GPIOx, ENABLE);
	RCC_APBPeriph2ClockCmd(APBPeriph2_UARTx, ENABLE);
	RCC_APBPeriph4ClockCmd(RCC_APBPeriph4_IWDG, ENABLE);	
#if defined	RETARGET_UART_CLKSRC
	RCC_Periph2ClockSourceConfig(RETARGET_UART_Periph,RETARGET_UART_CLKSRC);
#endif	
}

int main (void)
{
	RCC_Configuration();
	UART_Driver(9600);
	IWDG_Driver(32678);	//timeout=1s	
	if(RCC_GetResetFlagStatus(RCC_FLAG_IWDG)!=RESET)//判断是否发生独立看门狗复位
	{
		RCC_ClearResetFlag(RCC_FLAG_IWDG);//清除标志位
		printf("独立看门狗产生复位，此信息由程序复位而打印\r\n");
	}
	while(1)
	{
//		IWDG_Feed();//喂狗
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


