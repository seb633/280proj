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

/* WIP(busy)标志，FLASH内部正在写入 */
#define WIP_Flag                  		0x01
#define Dummy_Byte               		0xFF

/*等待超时时间*/
#define USART_FLAG_TIMEOUT         		((u32)0x1000)
#define USART_LONG_TIMEOUT         		((u32)(10 * USART_FLAG_TIMEOUT))

/*信息输出*/
#define FLASH_DEBUG_ON         			1

#define FLASH_INFO(fmt,arg...)          printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)         printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)         do{\
                                          if(FLASH_DEBUG_ON)\
                                          printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

/* CS线输出 */
#define CS_High							GPIO_SetBits(USART_CS_PORT, USART_CS_PIN);	
#define CS_Low							GPIO_ResetBits(USART_CS_PORT, USART_CS_PIN);
										  
/* USART read/write flash define*/
#define sFLASH_ID              		0xEF4017    //W25Q64
#define USART_FLASH_PageSize            256
#define USART_FLASH_PerWritePageSize    256

/*命令定义-开头*******************************/
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
  * @brief  读取FLASH ID
  * @param 	无
  * @retval FLASH ID
  */
u32 USART_FLASH_ReadID(USART_TypeDef* USARTx)
{
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

	/* 开始通讯：CS低电平 */   
	CS_Low;

	/* 发送JEDEC指令，读取ID */
	USART_FLASH_SendByte(USARTx,W25X_JedecDeviceID);

	/* 读取一个字节数据 */
	Temp0 = USART_FLASH_SendByte(USARTx,Dummy_Byte);

	/* 读取一个字节数据 */
	Temp1 = USART_FLASH_SendByte(USARTx,Dummy_Byte);

	/* 读取一个字节数据 */
	Temp2 = USART_FLASH_SendByte(USARTx,Dummy_Byte);

	/* 停止通讯：CS高电平 */
	CS_High;

	/*把数据组合起来，作为函数的返回值*/
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

	return Temp;
}


 /**
  * @brief  向FLASH发送 写使能 命令
  * @param  none
  * @retval none
  */
void USART_FLASH_WriteEnable(USART_TypeDef* USARTx)
{
	/* 通讯开始：CS低 */
	CS_Low;

	/* 发送写使能命令*/
	USART_FLASH_SendByte(USARTx ,W25X_WriteEnable);

	/*通讯结束：CS高 */
	CS_High;
}

 /**
  * @brief  等待WIP(BUSY)标志被置0，即等待到FLASH内部数据写入完毕
  * @param  none
  * @retval none
  */
void USART_FLASH_WaitForWriteEnd(USART_TypeDef* USARTx)
{
	u8 FLASH_Status = 0;

	/* 选择 FLASH: CS 低 */
	CS_Low;

	/* 发送 读状态寄存器 命令 */
	USART_FLASH_SendByte(USARTx ,W25X_ReadStatusReg);

	/* 若FLASH忙碌，则等待 */
	do
	{
		/* 读取FLASH芯片的状态寄存器 */
		FLASH_Status = USART_FLASH_SendByte(USARTx ,Dummy_Byte);	 
	}
	while ((FLASH_Status & WIP_Flag) == SET);  /* 正在写入标志 */

	/* 停止信号  FLASH: CS 高 */
	CS_High;
}

 /**
  * @brief  使用USART发送一个字节的数据
  * @param  byte：要发送的数据
  * @retval 返回接收到的数据
  */
u8 USART_FLASH_SendByte(USART_TypeDef* USARTx,u8 byte)
{
	 USARTTimeout = USART_FLAG_TIMEOUT;
	/* 等待发送缓冲区为空，TXE事件 */
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
	{
		if((USARTTimeout--) == 0) 
			return USART_TIMEOUT_UserCallback(0);
	}

	/* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
	USART_Sync_SendData(USARTx, byte);

	USARTTimeout = USART_FLAG_TIMEOUT;
	/* 等待接收缓冲区非空，RXNE事件 */
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXC) == RESET)
	{
		if((USARTTimeout--) == 0) 
			return USART_TIMEOUT_UserCallback(1);
	}

	/* 读取数据寄存器，获取接收缓冲区数据 */
	return USART_Sync_ReceiveData(USARTx);
}

/**
  * @brief  等待超时回调函数
  * @param  None.
  * @retval None.
  */
static u16 USART_TIMEOUT_UserCallback(u8 errorCode)
{
	/* 等待超时后的处理,输出错误信息 */
	FLASH_ERROR("USART 等待超时!errorCode = %d",errorCode);
	return 0;
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
* @brief 配置SPI的复用引脚
* @param 无
* @retval 无
*/
void USART_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_DigitalRemapConfig(USART_MOSI_AFIO, USART_MOSI_PIN, USART_TX_AFx,ENABLE);	//MOSI数字功能复用
	GPIO_DigitalRemapConfig(USART_MISO_AFIO, USART_MISO_PIN, USART_RX_AFx,ENABLE);	//MISO数字功能复用
	GPIO_DigitalRemapConfig(USART_SCK_AFIO, USART_SCK_PIN, USART_SCK_AFx,ENABLE);	//SCK数字功能复用
	//CS推挽输出
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OutPP;
	GPIO_InitStruct.GPIO_Pin = USART_CS_PIN;
	GPIO_InitStruct.GPIO_Pull = GPIO_Pull_NoPull;
	GPIO_Init(USART_CS_PORT, &GPIO_InitStruct);
	CS_High;
}

/**
* @brief 配置SPI的工作模式
* @param 无
* @retval 无
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
	USART_Cmd(USARTx, ENABLE);//使能USARTx
}

/**
* @brief USARTx同步模式驱动函数
* @param 无
* @retval 无
*/
void USART_Driver(USART_TypeDef* USARTx)
{
	USART_GPIO_Config();
	USART_MasterMode_Config(USARTx);
}

/**
* @brief Nor Flash读取单字节函数
* @param 无
* @retval 无
*/
void Flash_ReadByte(USART_TypeDef* USARTx,u32 addr)
{
	u8 data[10]={0},i;
	/* 开始通讯：CS低电平 */   
	CS_Low;
	/* 发送指令，读取数据 */
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
* @brief Nor Flash块擦除函数
* @param 无
* @retval 无
*/
void Flash_SectorErase(USART_TypeDef* USARTx,u32 addr)
{
	//执行写使能
	USART_FLASH_WriteEnable(USARTx);
	//进行扇区擦除
	CS_Low;
	USART_FLASH_SendByte(USARTx,W25X_SectorErase);
	USART_FLASH_SendByte(USARTx,(addr&0xFF0000)>>16);
	USART_FLASH_SendByte(USARTx,(addr&0xFF00)>>8);
	USART_FLASH_SendByte(USARTx,addr&0xFF);
	CS_High;
	USART_FLASH_WaitForWriteEnd(USARTx);
}

/**
* @brief Nor Flash单字节写入函数
* @param 无
* @retval 无
*/
void Flash_WriteByte(USART_TypeDef* USARTx,u32 addr)
{
	u8 i,data[10]={0};
	//执行写使能
	USART_FLASH_WriteEnable(USARTx);
	//进行数据写入
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
* @brief Nor Flash写使能函数
* @param 无
* @retval 无
*/
void FLASH_ReadWrite(USART_TypeDef* USARTx,u32 addr)
{
	Flash_SectorErase(USARTx,addr);
	Flash_ReadByte(USARTx,addr);
	Flash_WriteByte(USARTx,addr);
	Flash_ReadByte(USARTx,addr);
}


/**
  * @brief  配置各个外设时钟.
  * @param  无
  * @retval 无
  */
void RCC_Configuration(void)
{  
	/* USART, RETARGET_UART, GPIO 时钟使能 */
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
	printf("正在读取Flash信息\r\n");
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


