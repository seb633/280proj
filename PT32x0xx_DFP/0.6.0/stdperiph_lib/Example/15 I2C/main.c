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



u8 arry_write[10] = {0,1,2,3,4,5,6,7,8,9};
u8 arry_read[10] = {0};

/**
* @brief 软件延时函数
* @param 无
* @retval 无
*/
void Software_Delay(void)
{
	u8 i, j,x;
	for(i=0; i<200; i++)
		for(j=0; j<200; j++)
			for(x=0; x<20; x++);
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
* @brief 配置I2C的复用引脚
* @param 无
* @retval 无
*/
void I2C_GPIO_Config(void)
{
	/* 配置I2C管脚的复用功能 */
	GPIO_DigitalRemapConfig(I2C_SDA_AFIO, I2C_SDA_PIN, I2C_SDA_AFx, ENABLE);	//I2C0 SDA
	GPIO_DigitalRemapConfig(I2C_SCL_AFIO, I2C_SCL_PIN, I2C_SCL_AFx, ENABLE);	//I2C0 SCL
}

/**
* @brief 配置I2C的工作模式
* @param 无
* @retval 无
*/
void I2C_Mode_Config(I2C_TypeDef *I2Cx)
{  
	I2C_InitTypeDef I2C_InitStruct;
	
	I2C_InitStruct.I2C_Acknowledge = I2C_Acknowledge_Enable;
	I2C_InitStruct.I2C_Broadcast = I2C_Broadcast_Disable;
	I2C_InitStruct.I2C_OwnAddress = 0x00;
	I2C_InitStruct.I2C_Prescaler = 640;
	I2C_Init(I2Cx,&I2C_InitStruct);   
}

/**
* @brief I2C驱动函数
* @param 无
* @retval 无
*/
void I2C_Driver(void)
{
	I2C_GPIO_Config();
	I2C_Mode_Config(I2Cn);
}

/**
* @brief I2C写函数
* @param pBuffer：需要写入的数据
* @param WriteAddr：从机地址
* @param NumByteToWrite：需要写入的数据长度
* @retval 无
*/
void I2C_EE_Write(u8* pBuffer, u32 WriteAddr,u16 DeviceAddr, u16 data_size)
{
	int i;
/******************等待从机ready***************/		
	I2C_GenerateEvent(I2Cn,I2C_Event_Start,DISABLE);
	I2Cn->CCR |= I2C_CCR_SI | I2C_CCR_ACK;
	I2C_Cmd(I2Cn,DISABLE);
	I2C_Cmd(I2Cn,ENABLE);
	I2C_GenerateEvent(I2Cn,I2C_Event_Start,ENABLE);	
	while(I2C_GetFlagStatus(I2Cn,I2C_FLAG_StartOk)!= SET);
	I2C_SendAddress(I2Cn, DeviceAddr);//器件地址，写
	while(I2C_GetFlagStatus(I2Cn,I2C_FLAG_MASGetAckW)!=SET);
	I2C_SendData(I2Cn,WriteAddr);//发送要写的字地址
	while(I2C_GetFlagStatus(I2Cn,I2C_FLAG_MDSGetAck)!=SET);
	for(i=0;i<data_size;i++)
	{
		I2C_SendData(I2Cn, *pBuffer);
		while(I2C_GetFlagStatus(I2Cn,I2C_FLAG_MDSGetAck) != SET);
		pBuffer++;
	}
	/******************发送停止位***************/
	I2C_GenerateEvent(I2Cn,I2C_Event_Stop,ENABLE);	
}

/**
* @brief I2C读函数
* @param pBuffer：读到的数据
* @param ReadAddr：从机地址
* @param NumByteToRead：读到的数据长度
* @retval 无
*/
void I2C_EE_Read(u8* pBuffer,u16 ReadAddr, u16 DeviceAddr, u16 data_size)
{
	int i;
/******************等待从机ready***************/		
	I2C_GenerateEvent(I2Cn,I2C_Event_Start,ENABLE);
	while(I2C_GetFlagStatus(I2Cn,I2C_FLAG_StartOk)!= SET);
	I2C_SendAddress(I2Cn, DeviceAddr);//器件地址，写
	while(I2C_GetFlagStatus(I2Cn,I2C_FLAG_MASGetAckW)!=SET);
	I2C_SendData(I2Cn,ReadAddr);//发送要读的页地址
	while(I2C_GetFlagStatus(I2Cn,I2C_FLAG_MDSGetAck)!=SET);
	I2C_SendData(I2Cn,ReadAddr);//发送要读的页地址
	while(I2C_GetFlagStatus(I2Cn,I2C_FLAG_MDSGetAck)!=SET);
/******************接收数据***************/		
	I2C_GenerateEvent(I2Cn,I2C_Event_Restart,ENABLE);//发送起始信号
	while(I2C_GetFlagStatus(I2Cn,I2C_FLAG_ReStartOk)!= SET);
	I2C_SendAddress(I2Cn, DeviceAddr|0x01);//器件地址，读
	while(I2C_GetFlagStatus(I2Cn,I2C_FLAG_MASGetAckR)!=SET);
	I2Cn->CR |= I2C_CR_ACK;//使能自动应答功能
	for(i=0;i<data_size;i++)
	{
		if(i == data_size-1)
		{
			I2Cn->CCR = I2C_CCR_SI|I2C_CR_ACK;//清除当前状态并关闭自动应答功能
			while(I2C_GetFlagStatus(I2Cn,I2C_FLAG_MDGSendNack) != SET);
			*pBuffer=I2C_ReceiveData(I2Cn);
		}
		else
		{
			I2Cn->CCR = I2C_CCR_SI;//清除当前状态
			while(I2C_GetFlagStatus(I2Cn,I2C_FLAG_MDGSendAck) != SET);
			*pBuffer=I2C_ReceiveData(I2Cn);
		}
		pBuffer++;
	}
	/******************发送停止位***************/
	I2C_GenerateEvent(I2Cn,I2C_Event_Stop,ENABLE);
}

/**
  * @brief  配置各个外设时钟.
  * @param  无
  * @retval 无
  */
void RCC_Configuration(void)
{  
	/* I2C, UART, GPIO时钟使能 */
	RCC_APBPeriph2ClockCmd(APBPeriph2_UARTx| I2Cn_Periph_CLOCK, ENABLE);
	RCC_AHBPeriphClockCmd(I2C_SDA_CLOCK|I2C_SCL_CLOCK|UART_Periph_GPIOx, ENABLE);
#if defined	RETARGET_UART_CLKSRC
	RCC_Periph2ClockSourceConfig(RETARGET_UART_Periph,RETARGET_UART_CLKSRC);
#endif
}

int main (void)
{  
	int i=0;
	u8 addr=0x0;
	
	RCC_Configuration();
	UART_Driver(9600);
	I2C_Driver();
	printf("I2C start to write\r\n");
	I2C_EE_Write(arry_write, addr,0xA0, sizeof(arry_write));
	printf("I2C写入EEPROM数据：\r\n");
	for(i=0;i<sizeof(arry_write);i++)
	{
		printf(" %d ",arry_write[i]);
	}
	printf("\r\n");
	printf("I2C start to read\r\n");
	I2C_EE_Read(arry_read,addr,0xA0,sizeof(arry_read));
	printf("I2C读取EEPROM数据：\r\n");
	for(i=0;i<sizeof(arry_read);i++)
	{
		printf(" %d ",arry_read[i]);
	}
	printf("\r\n");
	while(1)
	{
		
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


