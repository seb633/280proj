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



u8 arry_write[8] = {0x01,0x00,0x00,0x00};
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
 * @brief
 * @param ms:
 * @retval None
 */
void CMix_Hardware_Delay_ms(uint32_t ms)
{
    uint32_t i, j;

    for (i = 0; i < ms; i++)
    {
        for (j = 0; j < 8000; j++)
        {
            __NOP();
        }
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
* @brief 配置I2C的复用引脚
* @param 无
* @retval 无
*/
void I2C_GPIO_Config(void)
{
	/* ????????? + ??(I2C????),??????? */
	GPIO_InitTypeDef GPIO_InitStruct;

	/* SDA: PA12,SCL: PA11(? PT32x0xx_config.h ???)*/
	GPIO_InitStruct.GPIO_Pin  = I2C_SDA_PIN | I2C_SCL_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OutOD;   /* ???? */
	GPIO_InitStruct.GPIO_Pull = GPIO_Pull_Up;      /* ??(?????????) */
	GPIO_Init(GPIOA, &GPIO_InitStruct);
//	GPIO_SetBits(GPIOA, I2C_SDA_PIN);
//	GPIO_SetBits(GPIOA, I2C_SCL_PIN);
	/* ??I2C??????? */
	GPIO_DigitalRemapConfig(I2C_SDA_AFIO, I2C_SDA_PIN, I2C_SDA_AFx, ENABLE); /* I2C0 SDA */
	GPIO_DigitalRemapConfig(I2C_SCL_AFIO, I2C_SCL_PIN, I2C_SCL_AFx, ENABLE); /* I2C0 SCL */
}

/**
* @brief 配置I2C的工作模式
* @param 无
* @retval 无
*/
void I2C_Mode_Config(I2C_TypeDef *I2Cx)
{  
	I2C_InitTypeDef I2C_InitStruct;
	
	I2C_InitStruct.I2C_Acknowledge = I2C_Acknowledge_Disable;
	I2C_InitStruct.I2C_Broadcast = I2C_Broadcast_Enable;
	I2C_InitStruct.I2C_OwnAddress = 0x00;
	I2C_InitStruct.I2C_Prescaler = 4096;
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
	static  int count;
	int i;
/******************等待从机ready***************/		
	I2C_GenerateEvent(I2Cn,I2C_Event_Start,DISABLE);
	I2Cn->CCR |= I2C_CCR_SI | I2C_CCR_ACK;
	I2C_Cmd(I2Cn,DISABLE);
	I2C_Cmd(I2Cn,ENABLE);
	I2C_GenerateEvent(I2Cn,I2C_Event_Start,ENABLE);	
	while(I2C_GetFlagStatus(I2Cn,I2C_FLAG_StartOk)!= SET);
	I2C_SendAddress(I2Cn, DeviceAddr);//器件地址，写
	while(I2C_GetFlagStatus(I2Cn,I2C_FLAG_MASGetAckW)!=SET)
	{
			count ++;
			if(count >64000)
			{
				I2C_GenerateEvent(I2Cn,I2C_Event_Stop,ENABLE);	
				count = 0;
				return;
			}
	};
	I2C_SendData(I2Cn,*pBuffer);//发送要写的字地址
	pBuffer++;
	while(I2C_GetFlagStatus(I2Cn,I2C_FLAG_MDSGetAck)!=SET)
	{
			count ++;
			if(count >64000)
			{
				I2C_GenerateEvent(I2Cn,I2C_Event_Stop,ENABLE);	
				count = 0;
				return;
			}
	};
	for(i=0;(i<data_size-1);i++)
	{
		I2C_SendData(I2Cn, *pBuffer);
		while(I2C_GetFlagStatus(I2Cn,I2C_FLAG_MDSGetAck) != SET)
		{
			count ++;
			if(count >640000)
			{
				I2C_GenerateEvent(I2Cn,I2C_Event_Stop,ENABLE);	
				count = 0;
				return;
			}
		};
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
	static uint32_t Read_Count;
/******************等待从机ready***************/		
	I2C_GenerateEvent(I2Cn,I2C_Event_Start,ENABLE);
		I2Cn->CCR |= I2C_CCR_SI | I2C_CCR_ACK;
	I2C_Cmd(I2Cn,DISABLE);
	I2C_Cmd(I2Cn,ENABLE);
/******************接收数据***************/		
//	I2C_GenerateEvent(I2Cn,I2C_Event_Restart,ENABLE);//发送起始信号
	while(I2C_GetFlagStatus(I2Cn,I2C_FLAG_StartOk)!= SET)
				{
			Read_Count ++;
			if(Read_Count >64000)
			{
				I2C_GenerateEvent(I2Cn,I2C_Event_Stop,ENABLE);	
				Read_Count = 0;
				return;
			}
	};
	I2C_SendAddress(I2Cn, DeviceAddr|0x01);//器件地址，读
	while(I2C_GetFlagStatus(I2Cn,I2C_FLAG_MASGetAckR)!=SET)
				{
			Read_Count ++;
			if(Read_Count >64000)
			{
				I2C_GenerateEvent(I2Cn,I2C_Event_Stop,ENABLE);	
				Read_Count = 0;
				return;
			}
	};
	I2Cn->CR |= I2C_CR_ACK;//使能自动应答功能
	for(i=0;i<data_size;i++)
	{
		if(i == data_size-1)
		{
			I2Cn->CCR = I2C_CCR_SI|I2C_CR_ACK;//清除当前状态并关闭自动应答功能
			while(I2C_GetFlagStatus(I2Cn,I2C_FLAG_MDGSendNack) != SET)
						{
			Read_Count ++;
			if(Read_Count >64000)
			{
				I2C_GenerateEvent(I2Cn,I2C_Event_Stop,ENABLE);	
				Read_Count = 0;
				return;
			}
	};
			*pBuffer=I2C_ReceiveData(I2Cn);
		}
		else
		{
			I2Cn->CCR = I2C_CCR_SI;//清除当前状态
			while(I2C_GetFlagStatus(I2Cn,I2C_FLAG_MDGSendAck) != SET)
						{
			Read_Count ++;
			if(Read_Count >64000)
			{
				I2C_GenerateEvent(I2Cn,I2C_Event_Stop,ENABLE);	
				Read_Count = 0;
				return;
			}
	};
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
	static uint16_t duty, prd;
	static uint8_t onoff, dead;
	
	RCC_Configuration();
	UART_Driver(9600);
	I2C_Driver();
	CMix_Hardware_Delay_ms(1000);
	printf("I2C start to write\r\n");
	while(1)
	{
			duty = 20;
			onoff = 1;
			prd = 160;
			dead = 0;//1-16
			arry_write[0] = 0x06;
			arry_write[1] = onoff;
			arry_write[2] = (duty>>8)&0xFF;
			arry_write[3] = (duty & 0xFF);
			arry_write[4] = (prd>>8)&0xFF;
			arry_write[5] = (prd & 0xFF);
			arry_write[6] = dead;
			arry_write[7] = 0;
				for(int i = 0; i<7;i++)
			{
				arry_write[7] += arry_write[i];
			}
			arry_write[7] = ~(arry_write[7]);
			arry_write[7] += 1; 
			
			I2C_EE_Write(arry_write, addr,0xA2, 8);
		
//			CMix_Hardware_Delay_ms(500);
//			arry_write[0] = 0x01;
//			arry_write[1] = 0x00;
//			arry_write[2] = 0x50;
//			arry_write[3] = (~(arry_write[0]+arry_write[1]+arry_write[2]))+1;

//			I2C_EE_Write(arry_write, addr,0xA0, 4);
//		
//			arry_write[0] = 0x02;
//			arry_write[1] = 0x01;
//			arry_write[2] = 0x04;
//			arry_write[3] = (~(arry_write[0]+arry_write[1]+arry_write[2]))+1;
//			CMix_Hardware_Delay_ms(500);
//			I2C_EE_Write(arry_write, addr,0xA0, 4);
//			CMix_Hardware_Delay_ms(20);
//			arry_write[0] = 0x03;
//			I2C_EE_Write(arry_write, addr,0xA0, 1);
//			I2C_EE_Read(arry_read,addr,0xA0,/*sizeof(arry_read)*/1);
			CMix_Hardware_Delay_ms(50);
	}
	I2C_EE_Write(arry_write, addr,0xA0, sizeof(arry_write));
	//printf("I2C写入EEPROM数据：\r\n");
	for(i=0;i<sizeof(arry_write);i++)
	{
		printf(" %d ",arry_write[i]);
	}
	printf("\r\n");
	printf("I2C start to read\r\n");
	I2C_EE_Read(arry_read,addr,0xA0,sizeof(arry_read));
	//printf("I2C读取EEPROM数据：\r\n");
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
	printf("Wrong parameters value: file %s on line %lu\r\n", file, line);
	/* Infinite loop */
	while (1)
	{
	}
}
#endif


