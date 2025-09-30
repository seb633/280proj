/******************************************************************************
  * @Example PT32x0xx_main
  * @author  应用开发团队
  * @version V1.0.0
  * @date    2023/11/07
  * @brief   Main program body
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

#if defined (PT32x012x) || defined (PT32x002x)

#define	UART_RECV_MAX_SIZE		256

typedef __PACKED_STRUCT UartRecvBuffType
{
	u16 write;
    u16 read;
	u8 buf[UART_RECV_MAX_SIZE];
}UartRecvBuffType;

u16 ReceiveData = 0;
u8 flag = 0;
UartRecvBuffType UartRecvBuff;

static void print_hex(u8 *pBuf, u16 size)
{
    u16 i;

	for(i = 0; i < size; i++)
    {
		USART_Async_SendData(USARTn,pBuf[i]);
    }
}

void UartBuffInit(void)
{
	UartRecvBuff.write = 0;
	UartRecvBuff.read = 0;
}

void UartRecvHandle(u8 ch)
{
	if(UartRecvBuff.read == (UartRecvBuff.write + 1)
		|| (UartRecvBuff.write == (UART_RECV_MAX_SIZE - 1) && UartRecvBuff.read == 0))
    {
        return ;
    }
	
	UartRecvBuff.buf[UartRecvBuff.write++] = ch;
	if(UartRecvBuff.write >= UART_RECV_MAX_SIZE)
	{
		UartRecvBuff.write = 0;
	}
}

u16 UartReadBytes(u8 *pBuf, u16 size)
{
	u16 readSize;
	
	if(size == 0)
	{
		return 0;
	}
	
	for(readSize = 0; readSize < size; readSize++)
	{
		if(UartRecvBuff.write != UartRecvBuff.read)
		{
			pBuf[readSize] = UartRecvBuff.buf[UartRecvBuff.read++];
			if(UartRecvBuff.read >= UART_RECV_MAX_SIZE)
			{
				UartRecvBuff.read = 0;
			}
		}
		else
		{
			break;
		}
	}
	
	return readSize;
}

/**
* @brief 配置USART的复用引脚
* @param 无
* @retval 无
*/
void USART_GPIO_Config(void)
{
	/* 配置UART管脚的复用功能 */
	GPIO_DigitalRemapConfig(USART_TX_AFIO, USART_TX_PIN, USART_TX_AFx, ENABLE);	
	GPIO_DigitalRemapConfig(USART_RX_AFIO, USART_RX_PIN, USART_RX_AFx, ENABLE);	
}

/**
* @brief 配置UART的工作模式
* @param 无
* @retval 无
*/
void USART_Mode_Config(u32 BaudRate)
{
	USART_Async_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	/*初始化USARTn*/
	USART_InitStruct.USART_BaudRate = BaudRate;
	USART_InitStruct.USART_DataLength = USART_DataLength_8;
	USART_InitStruct.USART_ParityMode = USART_Parity_None;
	USART_InitStruct.USART_Receiver = USART_Receiver_Enable;
	USART_InitStruct.USART_SampleRate = USART_SampleRate_4X;
	USART_InitStruct.USART_StopBitLength = USART_StopBitLength_1;
	USART_Async_Init(USARTn, &USART_InitStruct);
	/*设置中断类型与配置NVIC*/
	USART_ITConfig(USARTn,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USARTn,USART_IT_FE,ENABLE);
	NVIC_InitStruct.NVIC_IRQChannel = USARTn_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);	
	USART_Cmd(USARTn, ENABLE);
}

/**
* @brief USART驱动函数
* @param 无
* @retval 无
*/
void USART_Driver(u32 BaudRate)
{
	USART_GPIO_Config();
	USART_Mode_Config(BaudRate);
}

/**
  * @brief  配置各个外设时钟.
  * @param  无
  * @retval 无
  */
void RCC_Configuration(void)
{  
	/* USARTn, GPIO 时钟使能 */
	RCC_AHBPeriphClockCmd(USART_TX_CLOCK|USART_RX_CLOCK, ENABLE);
	RCC_APBPeriph2ClockCmd(USART_Periph_CLOCK,ENABLE); 
}


int main (void)
{
	u8 arr[] = {"hello world\r\n"};
	u8 buf[16];// = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	u16 size;
	
	RCC_Configuration();
	USART_Driver(9600);
	for(u8 i = 0;i<(sizeof(arr)/sizeof(u8));i++)
	{
		USART_Async_SendData(USARTn,arr[i]);
	}	
	UartBuffInit();
	while(1)
	{
		if((size = UartReadBytes(buf, 16)) > 0)
		{
			print_hex(buf, size);		
		}
	}
}

/**
* @brief USARTn中断服务函数
* @param None
* @retval None
*/
void USARTn_Handler(void)
{	
	if(USART_GetFlagStatus(USARTn,USART_FLAG_RXNE) != RESET)
	{
		ReceiveData = USART_Async_ReceiveData(USARTn);
		UartRecvHandle(ReceiveData);		
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
	printf("Wrong parameters value: file %s on line %ld\r\n", file, line);
	/* Infinite loop */
	while (1)
	{
	}
}
#endif


