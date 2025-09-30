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

u16 DMA_TX_BUF[10] = {1,2,3,4,5,6,7,8,9,10};
u16 DMA_RX_BUF[10] = {0};

#if defined (PT32G031x) || defined (PTM280x)

/**
* @brief ����DMA����ģʽ
* @param ��
* @retval ��
*/
void DMA_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStruct;

	DMA_StructInit(&DMA_InitStruct);

	DMA_InitStruct.DMA_SourceBaseAddress = (u32)DMA_TX_BUF;//Դ��ַ
	DMA_InitStruct.DMA_DestinationBaseAddress = (u32)DMA_RX_BUF;//Ŀ�ĵ�ַ
	DMA_InitStruct.DMA_NumberOfData = 10;//Ҫ�������ݵ�����
	DMA_InitStruct.DMA_SourceDataSize = DMA_SourceDataSize_Half;//����Դ��DMA�������Ĵ������ݿ��
	DMA_InitStruct.DMA_DestinationDataSize = DMA_DestinationDataSize_Half;//DMA������������Ŀ�ĵصĴ������ݿ��
	DMA_InitStruct.DMA_SourceAddressIncrement = DMA_SourceAddressIncrement_Enable;//DMAԴ��ַ����
	DMA_InitStruct.DMA_DestinationAddressIncrement = DMA_DestinationAddressIncrement_Enable;//DMAĿ�ĵ�ַ����
	DMA_InitStruct.DMA_Direction = DMA_Direction_MemoryToMemory;//���䷽��
	DMA_InitStruct.DMA_CircularMode = DMA_CircularMode_Disable;//������ѭ��ģʽ

	DMA_Init(DMAn_Channeln,&DMA_InitStruct);//��ʼ��DMA

	DMA_Cmd(DMAn_Channeln,ENABLE);//ʹ��
}

/**
* @brief ����UART�ĸ�������
* @param ��
* @retval ��
*/
void UART_GPIO_Config(void)
{ 
	GPIO_DigitalRemapConfig(UART_TX_PORT, UART_TX_PIN, UART_TX_AFIO,ENABLE);	//UART_TX���ֹ��ܸ���
	GPIO_DigitalRemapConfig(UART_RX_PORT, UART_RX_PIN, UART_RX_AFIO,ENABLE);	//UART_RX���ֹ��ܸ���
}

/**
* @brief ����UART�Ĺ���ģʽ
* @param ��
* @retval ��
*/
void UART_Mode_Config(u32 Baudrate)
{
	UART_InitTypeDef UART_InitStruct;

	/*��ʼ��UART*/
	UART_InitStruct.UART_BaudRate = Baudrate;
	UART_InitStruct.UART_DataLength = UART_DataLength_8;
	UART_InitStruct.UART_StopBitLength = UART_StopBitLength_1;
	UART_InitStruct.UART_ParityMode = UART_Parity_None;
	UART_InitStruct.UART_Receiver = UART_Receiver_Enable;
	UART_InitStruct.UART_SampleRate = RETARGET_UART_SampleRate;
	UART_Init(RETARGET_UART_PORT, &UART_InitStruct);
	/*ʹ��UART*/
	UART_Cmd(RETARGET_UART_PORT, ENABLE);
}

/**
* @brief UART��������
* @param ��
* @retval ��
*/
void UART_Driver(u32 Baudrate)
{
	UART_Mode_Config(Baudrate);
	UART_GPIO_Config();
}

/**
  * @brief  ���ø�������ʱ��.
  * @param  ��
  * @retval ��
  */
void RCC_Configuration(void)
{  
	/* UART, GPIO, DMA ʱ��ʹ�� */
	RCC_AHBPeriphClockCmd(DMAn_Periph_CLOCK|UART_Periph_GPIOx, ENABLE);
	RCC_APBPeriph2ClockCmd(APBPeriph2_UARTx,ENABLE); 
#if defined	RETARGET_UART_CLKSRC
	RCC_Periph2ClockSourceConfig(RETARGET_UART_Periph,RETARGET_UART_CLKSRC);
#endif
}

int main (void)
{
	u8 i = 0;
	u8 flag = 0;
	
	RCC_Configuration();
	/*UART����*/
	UART_Driver(9600);
	/*DMA����*/
	DMA_Mode_Config();
	while(1)
	{
		if((DMA_GetNumberOfData(DMAn_Channeln) == 0)&&(flag == 0))
		{
			for(i=0;i<10;i++)
			{
				printf("DMA_RX_BUF[%d] = %d\r\n",i,DMA_RX_BUF[i]);
			}
			DMA_Cmd(DMAn_Channeln,DISABLE);
			flag = 1;
		}
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


