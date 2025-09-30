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

#if defined (PT32x012x) || defined (PT32x002x)

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
* @brief 配置CMPn引脚的模拟功能和复用功能
* @param 无
* @retval 无
*/
void CMP_GPIO_Config(void)
{
	/* 配置CMP管脚的模拟引脚使能 */	 
	GPIO_AnalogRemapConfig(CMPn_Px_AFIO,CMPn_Px_PIN, ENABLE);   			//CMPn_Px
	GPIO_DigitalRemapConfig(CMPn_OUT_AFIO,CMPn_OUT_PIN,CMPn_OUT_AFx, ENABLE);	//CMPn_OUT复用使能
}

/**
* @brief 配置CMPn的工作模式
* @param 无
* @retval 无
*/
void CMP_Mode_Config(void)
{
	CMP_InitTypeDef CMP_InitStruct;	
	NVIC_InitTypeDef NVIC_InitStruct;
	/*CMPn初始化*/
	CMP_InitStruct.CMP_DigitalFilter = CMP_DigitalFilter_16;
	CMP_InitStruct.CMP_InitializationDelayTime = 20;
	CMP_InitStruct.CMP_NegativeInput = CMP_NegativeInput_LDAC;     //负端接LDAC
	CMP_InitStruct.CMP_OutputPolarity = CMP_OutputPolarity_Normal;
	CMP_InitStruct.CMP_PositiveInput = CMPn_PositiveInput;
	CMP_Init(CMPn,&CMP_InitStruct);	
	/*CMPn上下沿中断使能*/
	NVIC_InitStruct.NVIC_IRQChannel = CMPn_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
	CMP_ClearFlag(CMPn,CMP_CLEAR_FLAG_ALL);
	CMP_ITConfig(CMPn,CMP_IT_COF|CMP_IT_COR,ENABLE);
	CMP_Cmd(CMPn, ENABLE);	
}

/**
* @brief CMP驱动函数
* @param 无
* @retval 无
*/
void CMP_Driver(void)
{
	CMP_GPIO_Config();
	CMP_Mode_Config();
}

/**
* @brief LDAC驱动函数
* @param 无
* @retval 无
*/
void LDAC_Driver(void)
{
	LDAC_SetData(LDAC0,16);
	LDAC_Cmd(LDAC0,ENABLE);
}

/**
  * @brief  配置各个外设时钟.
  * @param  无
  * @retval 无
  */
void RCC_Configuration(void)
{  
	/* CMPn, UART, GPIO 时钟使能 */
	RCC_APBPeriph2ClockCmd(APBPeriph2_UARTx, ENABLE);
	RCC_APBPeriph3ClockCmd(CMPn_Periph_CLOCK, ENABLE);
	RCC_AHBPeriphClockCmd(CMPn_Px_CLOCK|CMPn_OUT_CLOCK|UART_Periph_GPIOx, ENABLE);
#if defined	RETARGET_UART_CLKSRC
	RCC_Periph2ClockSourceConfig(RETARGET_UART_Periph,RETARGET_UART_CLKSRC);
#endif
}

int main (void)
{
	RCC_Configuration();
	UART_Driver(9600);
	printf("CMP 比较器\r\n");
	LDAC_Driver();
	CMP_Driver();
	
    while(1)
	{
	}

}

/**
  * @brief 	CMPn中断服务函数
  * @param 	None
  * @retval None
*/
void CMPn_Handler(void)
{
	if(CMP_GetFlagStatus(CMPn,CMP_FLAG_COF))
	{
		printf("下降沿中断\r\n");
		CMP_ClearFlag(CMPn,CMP_FLAG_COF);
	}
	if(CMP_GetFlagStatus(CMPn,CMP_FLAG_COR))
	{
		printf("上升沿中断\r\n");
		CMP_ClearFlag(CMPn,CMP_FLAG_COR);
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


