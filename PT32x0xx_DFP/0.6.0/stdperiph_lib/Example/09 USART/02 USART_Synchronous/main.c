/******************************************************************************
  * @Example PT32x0xx_main
  * @author  Ӧ�ÿ����Ŷ�
  * @version V1.0.0
  * @date    2023/11/07
  * @brief   Main program body
  *          
  ******************************************************************************
  * @attention	��������Ϊ��������9600��1λֹͣλ������żУ��
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

/* WIP(busy)��־��FLASH�ڲ�����д�� */
#define WIP_Flag                  		0x01
#define Dummy_Byte               		0xFF

/*�ȴ���ʱʱ��*/
#define USART_FLAG_TIMEOUT         		((u32)0x1000)
#define USART_LONG_TIMEOUT         		((u32)(10 * USART_FLAG_TIMEOUT))

/*��Ϣ���*/
#define FLASH_DEBUG_ON         			1

#define FLASH_INFO(fmt,arg...)          printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)         printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)         do{\
                                          if(FLASH_DEBUG_ON)\
                                          printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

/* CS����� */
#define CS_High							GPIO_SetBits(USART_CS_PORT, USART_CS_PIN);	
#define CS_Low							GPIO_ResetBits(USART_CS_PORT, USART_CS_PIN);
										  
/* USART read/write flash define*/
#define sFLASH_ID              		0xEF4017    //W25Q64
#define USART_FLASH_PageSize            256
#define USART_FLASH_PerWritePageSize    256

/*�����-��ͷ*******************************/
#define W25X_WriteEnable		      	0x06 
#define W25X_WriteDisable		      	0x04 
#define W25X_ReadStatusReg		    	0x05 
#define W25X_WriteStatusReg		    	0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      	0x0B 
#define W25X_FastReadDual		      	0x3B 
#define W25X_PageProgram		      	0x02 
#define W25X_BlockErase			      	0xD8 
#define W25X_SectorErase		      	0x20 
#define W25X_ChipErase			      	0xC7 
#define W25X_PowerDown			      	0xB9 
#define W25X_ReleasePowerDown	    	0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   		0x90 
#define W25X_JedecDeviceID		    	0x9F


static __IO u32 USARTTimeout = USART_LONG_TIMEOUT; 
static u16 USART_TIMEOUT_UserCallback(u8 errorCode);
u8 USART_FLASH_SendByte(USART_TypeDef* USARTx,u8 byte);

/**
  * @brief  ��ȡFLASH ID
  * @param 	��
  * @retval FLASH ID
  */
u32 USART_FLASH_ReadID(USART_TypeDef* USARTx)
{
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

	/* ��ʼͨѶ��CS�͵�ƽ */   
	CS_Low;

	/* ����JEDECָ���ȡID */
	USART_FLASH_SendByte(USARTx,W25X_JedecDeviceID);

	/* ��ȡһ���ֽ����� */
	Temp0 = USART_FLASH_SendByte(USARTx,Dummy_Byte);

	/* ��ȡһ���ֽ����� */
	Temp1 = USART_FLASH_SendByte(USARTx,Dummy_Byte);

	/* ��ȡһ���ֽ����� */
	Temp2 = USART_FLASH_SendByte(USARTx,Dummy_Byte);

	/* ֹͣͨѶ��CS�ߵ�ƽ */
	CS_High;

	/*�����������������Ϊ�����ķ���ֵ*/
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

	return Temp;
}


 /**
  * @brief  ��FLASH���� дʹ�� ����
  * @param  none
  * @retval none
  */
void USART_FLASH_WriteEnable(USART_TypeDef* USARTx)
{
	/* ͨѶ��ʼ��CS�� */
	CS_Low;

	/* ����дʹ������*/
	USART_FLASH_SendByte(USARTx ,W25X_WriteEnable);

	/*ͨѶ������CS�� */
	CS_High;
}

 /**
  * @brief  �ȴ�WIP(BUSY)��־����0�����ȴ���FLASH�ڲ�����д�����
  * @param  none
  * @retval none
  */
void USART_FLASH_WaitForWriteEnd(USART_TypeDef* USARTx)
{
	u8 FLASH_Status = 0;

	/* ѡ�� FLASH: CS �� */
	CS_Low;

	/* ���� ��״̬�Ĵ��� ���� */
	USART_FLASH_SendByte(USARTx ,W25X_ReadStatusReg);

	/* ��FLASHæµ����ȴ� */
	do
	{
		/* ��ȡFLASHоƬ��״̬�Ĵ��� */
		FLASH_Status = USART_FLASH_SendByte(USARTx ,Dummy_Byte);	 
	}
	while ((FLASH_Status & WIP_Flag) == SET);  /* ����д���־ */

	/* ֹͣ�ź�  FLASH: CS �� */
	CS_High;
}

 /**
  * @brief  ʹ��USART����һ���ֽڵ�����
  * @param  byte��Ҫ���͵�����
  * @retval ���ؽ��յ�������
  */
u8 USART_FLASH_SendByte(USART_TypeDef* USARTx,u8 byte)
{
	 USARTTimeout = USART_FLAG_TIMEOUT;
	/* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
	{
		if((USARTTimeout--) == 0) 
			return USART_TIMEOUT_UserCallback(0);
	}

	/* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
	USART_Sync_SendData(USARTx, byte);

	USARTTimeout = USART_FLAG_TIMEOUT;
	/* �ȴ����ջ������ǿգ�RXNE�¼� */
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXC) == RESET)
	{
		if((USARTTimeout--) == 0) 
			return USART_TIMEOUT_UserCallback(1);
	}

	/* ��ȡ���ݼĴ�������ȡ���ջ��������� */
	return USART_Sync_ReceiveData(USARTx);
}

/**
  * @brief  �ȴ���ʱ�ص�����
  * @param  None.
  * @retval None.
  */
static u16 USART_TIMEOUT_UserCallback(u8 errorCode)
{
	/* �ȴ���ʱ��Ĵ���,���������Ϣ */
	FLASH_ERROR("USART �ȴ���ʱ!errorCode = %d",errorCode);
	return 0;
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
	NVIC_InitTypeDef NVIC_InitStruct;

	/*��ʼ��UART*/
	UART_InitStruct.UART_BaudRate = Baudrate;
	UART_InitStruct.UART_DataLength = UART_DataLength_8;
	UART_InitStruct.UART_StopBitLength = UART_StopBitLength_1;
	UART_InitStruct.UART_ParityMode = UART_Parity_None;
	UART_InitStruct.UART_Receiver = UART_Receiver_Enable;
	UART_InitStruct.UART_SampleRate = RETARGET_UART_SampleRate;
	UART_Init(RETARGET_UART_PORT, &UART_InitStruct);
	/*�����ж�����������NVIC*/
	UART_ITConfig(RETARGET_UART_PORT,UART_IT_RXNE,ENABLE);
	NVIC_InitStruct.NVIC_IRQChannel = RETARGET_UART_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
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
* @brief ����SPI�ĸ�������
* @param ��
* @retval ��
*/
void USART_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_DigitalRemapConfig(USART_MOSI_AFIO, USART_MOSI_PIN, USART_TX_AFx,ENABLE);	//MOSI���ֹ��ܸ���
	GPIO_DigitalRemapConfig(USART_MISO_AFIO, USART_MISO_PIN, USART_RX_AFx,ENABLE);	//MISO���ֹ��ܸ���
	GPIO_DigitalRemapConfig(USART_SCK_AFIO, USART_SCK_PIN, USART_SCK_AFx,ENABLE);	//SCK���ֹ��ܸ���
	//CS�������
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OutPP;
	GPIO_InitStruct.GPIO_Pin = USART_CS_PIN;
	GPIO_InitStruct.GPIO_Pull = GPIO_Pull_NoPull;
	GPIO_Init(USART_CS_PORT, &GPIO_InitStruct);
	CS_High;
}

/**
* @brief ����SPI�Ĺ���ģʽ
* @param ��
* @retval ��
*/
void USART_MasterMode_Config(USART_TypeDef* USARTx)
{
	USART_Sync_InitTypeDef USART_InitStruct;
	
	USART_InitStruct.USART_BaudRate = 100;
	USART_InitStruct.USART_ClockPhase = USART_ClockPhase_1Edge;
	USART_InitStruct.USART_ClockPolarity = USART_ClockPolarity_Low;
	USART_InitStruct.USART_FrameFormat = USART_FrameFormat_MSB;
	USART_InitStruct.USART_MasterSlaveMode = USART_MasterSlaveMode_Master;
	USART_InitStruct.USART_Receiver = USART_Receiver_Enable;
	USART_Sync_Init(USARTx, &USART_InitStruct);
	USART_Cmd(USARTx, ENABLE);//ʹ��USARTx
}

/**
* @brief USARTxͬ��ģʽ��������
* @param ��
* @retval ��
*/
void USART_Driver(USART_TypeDef* USARTx)
{
	USART_GPIO_Config();
	USART_MasterMode_Config(USARTx);
}

/**
* @brief Nor Flash��ȡ���ֽں���
* @param ��
* @retval ��
*/
void Flash_ReadByte(USART_TypeDef* USARTx,u32 addr)
{
	u8 data[10]={0},i;
	/* ��ʼͨѶ��CS�͵�ƽ */   
	CS_Low;
	/* ����ָ���ȡ���� */
	USART_FLASH_SendByte(USARTx,W25X_ReadData);
	USART_FLASH_SendByte(USARTx,(addr&0xFF0000)>>16);
	USART_FLASH_SendByte(USARTx,(addr&0xFF00)>>8);
	USART_FLASH_SendByte(USARTx,addr&0xFF);
	for(i=0;i<10;i++)
	{
		data[i]=USART_FLASH_SendByte(USARTx,Dummy_Byte);
	}
	printf("address: 0x%lX\r\n",addr);
	for(i=0;i<10;i++)
	{
		printf("read data%d:%X\r\n",i,data[i]);
		while(!UART_GetFlagStatus(LPUART0,UART_FLAG_TXE));
	}
	CS_High;
}

/**
* @brief Nor Flash���������
* @param ��
* @retval ��
*/
void Flash_SectorErase(USART_TypeDef* USARTx,u32 addr)
{
	//ִ��дʹ��
	USART_FLASH_WriteEnable(USARTx);
	//������������
	CS_Low;
	USART_FLASH_SendByte(USARTx,W25X_SectorErase);
	USART_FLASH_SendByte(USARTx,(addr&0xFF0000)>>16);
	USART_FLASH_SendByte(USARTx,(addr&0xFF00)>>8);
	USART_FLASH_SendByte(USARTx,addr&0xFF);
	CS_High;
	USART_FLASH_WaitForWriteEnd(USARTx);
}

/**
* @brief Nor Flash���ֽ�д�뺯��
* @param ��
* @retval ��
*/
void Flash_WriteByte(USART_TypeDef* USARTx,u32 addr)
{
	u8 i,data[10]={0};
	//ִ��дʹ��
	USART_FLASH_WriteEnable(USARTx);
	//��������д��
	CS_Low;
	USART_FLASH_SendByte(USARTx,W25X_PageProgram);
	USART_FLASH_SendByte(USARTx,(addr&0xFF0000)>>16);
	USART_FLASH_SendByte(USARTx,(addr&0xFF00)>>8);
	USART_FLASH_SendByte(USARTx,addr&0xFF);
	for(i=0;i<10;i++)
	{
		data[i]=i;
		USART_FLASH_SendByte(USARTx,data[i]);	
	}
	CS_High;
	USART_FLASH_WaitForWriteEnd(USARTx);
}

/**
* @brief Nor Flashдʹ�ܺ���
* @param ��
* @retval ��
*/
void FLASH_ReadWrite(USART_TypeDef* USARTx,u32 addr)
{
	Flash_SectorErase(USARTx,addr);
	Flash_ReadByte(USARTx,addr);
	Flash_WriteByte(USARTx,addr);
	Flash_ReadByte(USARTx,addr);
}


/**
  * @brief  ���ø�������ʱ��.
  * @param  ��
  * @retval ��
  */
void RCC_Configuration(void)
{  
	/* USART, RETARGET_UART, GPIO ʱ��ʹ�� */
	RCC_AHBPeriphClockCmd(UART_Periph_GPIOx|USART_CS_CLOCK|USART_MOSI_CLOCK|USART_MISO_CLOCK|USART_SCK_CLOCK, ENABLE);
	RCC_APBPeriph2ClockCmd(USART_Periph_CLOCK | APBPeriph2_UARTx,ENABLE); 
#if defined	RETARGET_UART_CLKSRC
	RCC_Periph2ClockSourceConfig(RETARGET_UART_Periph,RETARGET_UART_CLKSRC);
#endif
}

int main (void)
{
	u32 DeviceID;
	RCC_Configuration();

	UART_Driver(9600);
	USART_Driver(USART0);
	printf("���ڶ�ȡFlash��Ϣ\r\n");
	DeviceID = USART_FLASH_ReadID(USART0);
	printf("Flash Device ID:%lx\r\n",DeviceID);
	FLASH_ReadWrite(USART0,0x000);
	while(1)
	{
	
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


