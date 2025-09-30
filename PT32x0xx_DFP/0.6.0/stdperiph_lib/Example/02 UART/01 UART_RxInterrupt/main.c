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

#if defined (PT32G031x) || defined(PTM280x)

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
        printf("%02x ", pBuf[i]);
    }
    printf("\n");
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
	NVIC_InitTypeDef NVIC_InitStruct;

	/*初始化UART*/
	UART_InitStruct.UART_BaudRate = Baudrate;
	UART_InitStruct.UART_DataLength = UART_DataLength_8;
	UART_InitStruct.UART_StopBitLength = UART_StopBitLength_1;
	UART_InitStruct.UART_ParityMode = UART_Parity_None;
	UART_InitStruct.UART_Receiver = UART_Receiver_Enable;
	UART_InitStruct.UART_SampleRate = RETARGET_UART_SampleRate;
	UART_Init(RETARGET_UART_PORT, &UART_InitStruct);
	/*设置中断类型与配置NVIC*/
	UART_ITConfig(RETARGET_UART_PORT,UART_IT_RXNE,ENABLE);
	NVIC_InitStruct.NVIC_IRQChannel = RETARGET_UART_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
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
  * @brief  配置各个外设时钟
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
	u8 buf[16];// = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	u16 size;
	
	RCC_Configuration();
	UART_Driver(9600);
	printf("澎湃微欢迎使用！\r\n");
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
* @brief UART中断服务函数
* @param None
* @retval None
*/
void RETARGET_UART_Handler(void)
{
	if(UART_GetFlagStatus(RETARGET_UART_PORT,UART_FLAG_RXNE) != RESET)
	{
		ReceiveData = UART_ReceiveData(RETARGET_UART_PORT);
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
	printf("Wrong parameters value: file %s on line %d\r\n", file, line);
	/* Infinite loop */
	while (1)
	{
	}
}
#endif


