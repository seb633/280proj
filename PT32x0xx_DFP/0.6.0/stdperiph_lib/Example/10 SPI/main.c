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


#if defined (PT32G031x)

/* SPI read/write flash define*/
#define FLASH_ID              			0XEF4017    //W25Q64
#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256

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

/*��Ϣ���*/
#define FLASH_DEBUG_ON         1

#define FLASH_INFO(fmt,arg...)           printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)          do{\
                                          if(FLASH_DEBUG_ON)\
                                          printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

/* WIP(busy)��־��FLASH�ڲ�����д�� */
#define WIP_Flag                  0x01
#define Dummy_Byte                0xFF
										  
/*�ȴ���ʱʱ��*/
#define SPIT_FLAG_TIMEOUT         ((u32)0x1000)
#define SPIT_LONG_TIMEOUT         ((u32)(10 * SPIT_FLAG_TIMEOUT))

static __IO u32  SPITimeout = SPIT_LONG_TIMEOUT; 
static u16 SPI_TIMEOUT_UserCallback(u8 errorCode);
u8 SPI_FLASH_SendByte(SPI_TypeDef* SPIx,u8 byte);
										  
/**
  * @brief  ��ȡFLASH ID
  * @param 	��
  * @retval FLASH ID
  */
u32 SPI_FLASH_ReadID(SPI_TypeDef* SPIx)
{
	u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

	/* ��ʼͨѶ��CS�͵�ƽ */   
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_Low);

	/* ����JEDECָ���ȡID */
	SPI_FLASH_SendByte(SPIx,W25X_JedecDeviceID);

	/* ��ȡһ���ֽ����� */
	Temp0 = SPI_FLASH_SendByte(SPIx,Dummy_Byte);

	/* ��ȡһ���ֽ����� */
	Temp1 = SPI_FLASH_SendByte(SPIx,Dummy_Byte);

	/* ��ȡһ���ֽ����� */
	Temp2 = SPI_FLASH_SendByte(SPIx,Dummy_Byte);

	/* ֹͣͨѶ��CS�ߵ�ƽ */
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_High);

	/*�����������������Ϊ�����ķ���ֵ*/
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

	return Temp;
}

 /**
  * @brief  ��FLASH���� дʹ�� ����
  * @param  none
  * @retval none
  */
void SPI_FLASH_WriteEnable(SPI_TypeDef* SPIx)
{
	/* ͨѶ��ʼ��CS�� */
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_Low);

	/* ����дʹ������*/
	SPI_FLASH_SendByte(SPIx ,W25X_WriteEnable);

	/*ͨѶ������CS�� */
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_High);
}

 /**
  * @brief  �ȴ�WIP(BUSY)��־����0�����ȴ���FLASH�ڲ�����д�����
  * @param  none
  * @retval none
  */
void SPI_FLASH_WaitForWriteEnd(SPI_TypeDef* SPIx)
{
	u8 FLASH_Status = 0;

	/* ѡ�� FLASH: CS �� */
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_Low);

	/* ���� ��״̬�Ĵ��� ���� */
	SPI_FLASH_SendByte(SPIx ,W25X_ReadStatusReg);

	/* ��FLASHæµ����ȴ� */
	do
	{
		/* ��ȡFLASHоƬ��״̬�Ĵ��� */
	FLASH_Status = SPI_FLASH_SendByte(SPIx ,Dummy_Byte);	 
	}
	while ((FLASH_Status & WIP_Flag) == SET);  /* ����д���־ */

	/* ֹͣ�ź�  FLASH: CS �� */
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_High);
}

 /**
  * @brief  ʹ��SPI����һ���ֽڵ�����
  * @param  byte��Ҫ���͵�����
  * @retval ���ؽ��յ�������
  */
u8 SPI_FLASH_SendByte(SPI_TypeDef* SPIx,u8 byte)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
	while (SPI_GetFlagStatus(SPIx, SPI_FLAG_TXE) == RESET)
	{
		if((SPITimeout--) == 0) 
			return SPI_TIMEOUT_UserCallback(0);
	}

	/* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
	SPI_SendData(SPIx, byte);

	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* �ȴ����ջ������ǿգ�RXNE�¼� */
	while (SPI_GetFlagStatus(SPIx, SPI_FLAG_BSY) == SET)
	{
		if((SPITimeout--) == 0) 
			return SPI_TIMEOUT_UserCallback(1);
	}

	/* ��ȡ���ݼĴ�������ȡ���ջ��������� */
	return SPI_ReceiveData(SPIx);
}

/**
  * @brief  �ȴ���ʱ�ص�����
  * @param  None.
  * @retval None.
  */
static  u16 SPI_TIMEOUT_UserCallback(u8 errorCode)
{
	/* �ȴ���ʱ��Ĵ���,���������Ϣ */
	FLASH_ERROR("SPI �ȴ���ʱ!errorCode = %d",errorCode);
	return 0;
}								  
										  
/**
* @brief �����ʱ����
* @param ��
* @retval ��
*/
void Software_Delay(void)
{
	u8 i, j,x;
	for(i=0; i<200; i++)
		for(j=0; j<200; j++)
			for(x=0; x<20; x++);
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
* @brief ����SPI�ĸ�������
* @param ��
* @retval ��
*/
void SPI_GPIO_Config(void)
{
	GPIO_DigitalRemapConfig(SPI_CS_AFIO, SPI_CS_PIN, SPI_CS_AFx, ENABLE);		//SPI_CS���ֹ��ܸ���
	GPIO_DigitalRemapConfig(SPI_SCK_AFIO, SPI_SCK_PIN, SPI_SCK_AFx, ENABLE);	//SPI_SCK���ֹ��ܸ���
	GPIO_DigitalRemapConfig(SPI_MOSI_AFIO, SPI_MOSI_PIN, SPI_MOSI_AFx, ENABLE);	//SPI_MOSI���ֹ��ܸ���
	GPIO_DigitalRemapConfig(SPI_MISO_AFIO, SPI_MISO_PIN, SPI_MISO_AFx, ENABLE);	//SPI_MISO���ֹ��ܸ���   
}

/**
* @brief ����SPI�Ĺ���ģʽ
* @param ��
* @retval ��
*/
void SPI_MasterMode_Config(SPI_TypeDef* SPIx)
{
	SPI_InitTypeDef SPI_InitStructure;//����һ��SPI_InitTypeDef���͵Ľṹ��
	SPI_StructInit(&SPI_InitStructure);//ʹ��Ĭ��ֵ��ʼ��SPI_InitStructure

	SPI_InitStructure.SPI_MasterSlaveMode = SPI_MasterSlaveMode_Master;//����Ϊ����ģʽ
	SPI_InitStructure.SPI_ClockPolarity = SPI_ClockPolarity_High;//ʱ�Ӽ��ԣ�����״̬ʱ��SCK���ָߵ�ƽ
	SPI_InitStructure.SPI_ClockPhase = SPI_ClockPhase_2Edge;//ʱ����λ�����ݲ����ӵڶ���ʱ�ӱ��ؿ�ʼ
	SPI_InitStructure.SPI_CSS = SPI_CSS_SoftwareControl;//CS������ƣ�Ĭ�ϳ�ʼֵΪ�ߵ�ƽ
	SPI_InitStructure.SPI_Prescaler = 100;//SPIʱ�ӷ�Ƶϵ��
	SPI_InitStructure.SPI_BaudRate = 64; //SPI������Ԥ��Ƶ f(sck)=f(pclk)/(SPI_Prescaler*(1+SPI_Prescaler))
	SPI_Init(SPIx, &SPI_InitStructure);//��ʼ��SPI
	SPI_Cmd(SPIx, ENABLE);//ʹ��SPI
}

/**
* @brief SPI��������
* @param ��
* @retval ��
*/
void SPI_Driver(SPI_TypeDef* SPIx)
{
	SPI_GPIO_Config();
	SPI_MasterMode_Config(SPIx);
}

/**
* @brief Nor Flash��ȡ����ID����
* @param ��
* @retval ��
*/
u32 FLASH_ReadID(SPI_TypeDef* SPIx)
{
	u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
	/* ��ʼͨѶ��CS�͵�ƽ */   
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_Low);
	/* ����JEDECָ���ȡID */
	SPI_FLASH_SendByte(SPIx,W25X_JedecDeviceID);
	/* ��ȡһ���ֽ����� */
	Temp0 = SPI_FLASH_SendByte(SPIx,0x00);
	/* ��ȡһ���ֽ����� */
	Temp1 = SPI_FLASH_SendByte(SPIx,0x00);
	/* ��ȡһ���ֽ����� */
	Temp2 = SPI_FLASH_SendByte(SPIx,0x00);
	/* ֹͣͨѶ��CS�ߵ�ƽ */
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_High);

	/*�����������������Ϊ�����ķ���ֵ*/
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
	return Temp;
}

/**
* @brief Nor Flash��ȡ���ֽں���
* @param ��
* @retval ��
*/
void Flash_ReadByte(SPI_TypeDef* SPIx,u32 addr)
{
	u8 data[10]={0},i;
	/* ��ʼͨѶ��CS�͵�ƽ */   
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_Low);
	/* ����ָ���ȡ���� */
	SPI_FLASH_SendByte(SPIx,W25X_ReadData);
	SPI_FLASH_SendByte(SPIx,(addr&0xFF0000)>>16);
	SPI_FLASH_SendByte(SPIx,(addr&0xFF00)>>8);
	SPI_FLASH_SendByte(SPIx,addr&0xFF);
	for(i=0;i<10;i++)
	{
		data[i]=SPI_FLASH_SendByte(SPIx,Dummy_Byte);
	}
	printf("address: 0x%lx\r\n",addr);
	for(i=0;i<10;i++)
	{
		printf("read data%d:%x\r\n",i,data[i]);
		while(!UART_GetFlagStatus(UART0,UART_FLAG_TXE));
	}
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_High);
}

/**
* @brief Nor Flash���������
* @param ��
* @retval ��
*/
void Flash_SectorErase(SPI_TypeDef* SPIx,u32 addr)
{
	//ִ��дʹ��
	SPI_FLASH_WriteEnable(SPIx);
	//������������
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_Low);
	SPI_FLASH_SendByte(SPIx,W25X_SectorErase);
	SPI_FLASH_SendByte(SPIx,(addr&0xFF0000)>>16);
	SPI_FLASH_SendByte(SPIx,(addr&0xFF00)>>8);
	SPI_FLASH_SendByte(SPIx,addr&0xFF);
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_High);
	SPI_FLASH_WaitForWriteEnd(SPIx);
}

/**
* @brief Nor Flash���ֽ�д�뺯��
* @param ��
* @retval ��
*/
void Flash_WriteByte(SPI_TypeDef* SPIx,u32 addr)
{
	u8 i,data[10]={0};
	//ִ��дʹ��
	SPI_FLASH_WriteEnable(SPIx);
	//��������д��
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_Low);
	SPI_FLASH_SendByte(SPIx,W25X_PageProgram);
	SPI_FLASH_SendByte(SPIx,(addr&0xFF0000)>>16);
	SPI_FLASH_SendByte(SPIx,(addr&0xFF00)>>8);
	SPI_FLASH_SendByte(SPIx,addr&0xFF);
	for(i = 0; i<10; i++)
	{
		data[i]=i;
		SPI_FLASH_SendByte(SPIx,data[i]);	
	}
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_High);
	SPI_FLASH_WaitForWriteEnd(SPIx);
}

/**
* @brief Nor Flashдʹ�ܺ���
* @param ��
* @retval ��
*/
void FLASH_ReadWrite(SPI_TypeDef* SPIx,u32 addr)
{
	Flash_SectorErase(SPIx,addr);
	Flash_ReadByte(SPIx,addr);
	Flash_WriteByte(SPIx,addr);
	Flash_ReadByte(SPIx,addr);
}

/**
  * @brief  ���ø�������ʱ��.
  * @param  ��
  * @retval ��
  */
void RCC_Configuration(void)
{  
	/* UART0, GPIO ʱ��ʹ�� */
	RCC_AHBPeriphClockCmd(UART_Periph_GPIOx|SPI_CS_CLOCK|SPI_SCK_CLOCK|SPI_MOSI_CLOCK|SPI_MISO_CLOCK, ENABLE);
	RCC_APBPeriph2ClockCmd(APBPeriph2_UARTx | SPIn_Periph_CLOCK, ENABLE); 
#if defined	RETARGET_UART_CLKSRC
	RCC_Periph2ClockSourceConfig(RETARGET_UART_Periph,RETARGET_UART_CLKSRC);
#endif
}


int main (void)
{
	u32 DeviceID;
	
	RCC_Configuration();
	UART_Driver(9600);
	SPI_Driver(SPIn);
	printf("���ڶ�ȡFlash��Ϣ\r\n");
	DeviceID = FLASH_ReadID(SPIn);
	printf("Device ID:%lx\r\n",DeviceID);
	Software_Delay();
	FLASH_ReadWrite(SPIn,0x000);
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
	printf("Wrong parameters value: file %s on line %d\r\n", file, line);
	/* Infinite loop */
	while (1)
	{
	}
}
#endif


