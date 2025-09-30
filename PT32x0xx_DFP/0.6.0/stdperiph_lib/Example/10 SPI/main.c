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

/*信息输出*/
#define FLASH_DEBUG_ON         1

#define FLASH_INFO(fmt,arg...)           printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)          do{\
                                          if(FLASH_DEBUG_ON)\
                                          printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

/* WIP(busy)标志，FLASH内部正在写入 */
#define WIP_Flag                  0x01
#define Dummy_Byte                0xFF
										  
/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((u32)0x1000)
#define SPIT_LONG_TIMEOUT         ((u32)(10 * SPIT_FLAG_TIMEOUT))

static __IO u32  SPITimeout = SPIT_LONG_TIMEOUT; 
static u16 SPI_TIMEOUT_UserCallback(u8 errorCode);
u8 SPI_FLASH_SendByte(SPI_TypeDef* SPIx,u8 byte);
										  
/**
  * @brief  读取FLASH ID
  * @param 	无
  * @retval FLASH ID
  */
u32 SPI_FLASH_ReadID(SPI_TypeDef* SPIx)
{
	u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

	/* 开始通讯：CS低电平 */   
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_Low);

	/* 发送JEDEC指令，读取ID */
	SPI_FLASH_SendByte(SPIx,W25X_JedecDeviceID);

	/* 读取一个字节数据 */
	Temp0 = SPI_FLASH_SendByte(SPIx,Dummy_Byte);

	/* 读取一个字节数据 */
	Temp1 = SPI_FLASH_SendByte(SPIx,Dummy_Byte);

	/* 读取一个字节数据 */
	Temp2 = SPI_FLASH_SendByte(SPIx,Dummy_Byte);

	/* 停止通讯：CS高电平 */
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_High);

	/*把数据组合起来，作为函数的返回值*/
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

	return Temp;
}

 /**
  * @brief  向FLASH发送 写使能 命令
  * @param  none
  * @retval none
  */
void SPI_FLASH_WriteEnable(SPI_TypeDef* SPIx)
{
	/* 通讯开始：CS低 */
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_Low);

	/* 发送写使能命令*/
	SPI_FLASH_SendByte(SPIx ,W25X_WriteEnable);

	/*通讯结束：CS高 */
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_High);
}

 /**
  * @brief  等待WIP(BUSY)标志被置0，即等待到FLASH内部数据写入完毕
  * @param  none
  * @retval none
  */
void SPI_FLASH_WaitForWriteEnd(SPI_TypeDef* SPIx)
{
	u8 FLASH_Status = 0;

	/* 选择 FLASH: CS 低 */
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_Low);

	/* 发送 读状态寄存器 命令 */
	SPI_FLASH_SendByte(SPIx ,W25X_ReadStatusReg);

	/* 若FLASH忙碌，则等待 */
	do
	{
		/* 读取FLASH芯片的状态寄存器 */
	FLASH_Status = SPI_FLASH_SendByte(SPIx ,Dummy_Byte);	 
	}
	while ((FLASH_Status & WIP_Flag) == SET);  /* 正在写入标志 */

	/* 停止信号  FLASH: CS 高 */
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_High);
}

 /**
  * @brief  使用SPI发送一个字节的数据
  * @param  byte：要发送的数据
  * @retval 返回接收到的数据
  */
u8 SPI_FLASH_SendByte(SPI_TypeDef* SPIx,u8 byte)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* 等待发送缓冲区为空，TXE事件 */
	while (SPI_GetFlagStatus(SPIx, SPI_FLAG_TXE) == RESET)
	{
		if((SPITimeout--) == 0) 
			return SPI_TIMEOUT_UserCallback(0);
	}

	/* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
	SPI_SendData(SPIx, byte);

	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* 等待接收缓冲区非空，RXNE事件 */
	while (SPI_GetFlagStatus(SPIx, SPI_FLAG_BSY) == SET)
	{
		if((SPITimeout--) == 0) 
			return SPI_TIMEOUT_UserCallback(1);
	}

	/* 读取数据寄存器，获取接收缓冲区数据 */
	return SPI_ReceiveData(SPIx);
}

/**
  * @brief  等待超时回调函数
  * @param  None.
  * @retval None.
  */
static  u16 SPI_TIMEOUT_UserCallback(u8 errorCode)
{
	/* 等待超时后的处理,输出错误信息 */
	FLASH_ERROR("SPI 等待超时!errorCode = %d",errorCode);
	return 0;
}								  
										  
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
* @brief 配置SPI的复用引脚
* @param 无
* @retval 无
*/
void SPI_GPIO_Config(void)
{
	GPIO_DigitalRemapConfig(SPI_CS_AFIO, SPI_CS_PIN, SPI_CS_AFx, ENABLE);		//SPI_CS数字功能复用
	GPIO_DigitalRemapConfig(SPI_SCK_AFIO, SPI_SCK_PIN, SPI_SCK_AFx, ENABLE);	//SPI_SCK数字功能复用
	GPIO_DigitalRemapConfig(SPI_MOSI_AFIO, SPI_MOSI_PIN, SPI_MOSI_AFx, ENABLE);	//SPI_MOSI数字功能复用
	GPIO_DigitalRemapConfig(SPI_MISO_AFIO, SPI_MISO_PIN, SPI_MISO_AFx, ENABLE);	//SPI_MISO数字功能复用   
}

/**
* @brief 配置SPI的工作模式
* @param 无
* @retval 无
*/
void SPI_MasterMode_Config(SPI_TypeDef* SPIx)
{
	SPI_InitTypeDef SPI_InitStructure;//定义一个SPI_InitTypeDef类型的结构体
	SPI_StructInit(&SPI_InitStructure);//使用默认值初始化SPI_InitStructure

	SPI_InitStructure.SPI_MasterSlaveMode = SPI_MasterSlaveMode_Master;//配置为主机模式
	SPI_InitStructure.SPI_ClockPolarity = SPI_ClockPolarity_High;//时钟极性，空闲状态时，SCK保持高电平
	SPI_InitStructure.SPI_ClockPhase = SPI_ClockPhase_2Edge;//时钟相位，数据采样从第二个时钟边沿开始
	SPI_InitStructure.SPI_CSS = SPI_CSS_SoftwareControl;//CS软件控制，默认初始值为高电平
	SPI_InitStructure.SPI_Prescaler = 100;//SPI时钟分频系数
	SPI_InitStructure.SPI_BaudRate = 64; //SPI波特率预分频 f(sck)=f(pclk)/(SPI_Prescaler*(1+SPI_Prescaler))
	SPI_Init(SPIx, &SPI_InitStructure);//初始化SPI
	SPI_Cmd(SPIx, ENABLE);//使能SPI
}

/**
* @brief SPI驱动函数
* @param 无
* @retval 无
*/
void SPI_Driver(SPI_TypeDef* SPIx)
{
	SPI_GPIO_Config();
	SPI_MasterMode_Config(SPIx);
}

/**
* @brief Nor Flash读取器件ID函数
* @param 无
* @retval 无
*/
u32 FLASH_ReadID(SPI_TypeDef* SPIx)
{
	u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
	/* 开始通讯：CS低电平 */   
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_Low);
	/* 发送JEDEC指令，读取ID */
	SPI_FLASH_SendByte(SPIx,W25X_JedecDeviceID);
	/* 读取一个字节数据 */
	Temp0 = SPI_FLASH_SendByte(SPIx,0x00);
	/* 读取一个字节数据 */
	Temp1 = SPI_FLASH_SendByte(SPIx,0x00);
	/* 读取一个字节数据 */
	Temp2 = SPI_FLASH_SendByte(SPIx,0x00);
	/* 停止通讯：CS高电平 */
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_High);

	/*把数据组合起来，作为函数的返回值*/
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
	return Temp;
}

/**
* @brief Nor Flash读取单字节函数
* @param 无
* @retval 无
*/
void Flash_ReadByte(SPI_TypeDef* SPIx,u32 addr)
{
	u8 data[10]={0},i;
	/* 开始通讯：CS低电平 */   
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_Low);
	/* 发送指令，读取数据 */
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
* @brief Nor Flash块擦除函数
* @param 无
* @retval 无
*/
void Flash_SectorErase(SPI_TypeDef* SPIx,u32 addr)
{
	//执行写使能
	SPI_FLASH_WriteEnable(SPIx);
	//进行扇区擦除
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_Low);
	SPI_FLASH_SendByte(SPIx,W25X_SectorErase);
	SPI_FLASH_SendByte(SPIx,(addr&0xFF0000)>>16);
	SPI_FLASH_SendByte(SPIx,(addr&0xFF00)>>8);
	SPI_FLASH_SendByte(SPIx,addr&0xFF);
	SPI_SoftwareControlCSSConfig(SPIx,SPI_SoftwareControlCSS_High);
	SPI_FLASH_WaitForWriteEnd(SPIx);
}

/**
* @brief Nor Flash单字节写入函数
* @param 无
* @retval 无
*/
void Flash_WriteByte(SPI_TypeDef* SPIx,u32 addr)
{
	u8 i,data[10]={0};
	//执行写使能
	SPI_FLASH_WriteEnable(SPIx);
	//进行数据写入
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
* @brief Nor Flash写使能函数
* @param 无
* @retval 无
*/
void FLASH_ReadWrite(SPI_TypeDef* SPIx,u32 addr)
{
	Flash_SectorErase(SPIx,addr);
	Flash_ReadByte(SPIx,addr);
	Flash_WriteByte(SPIx,addr);
	Flash_ReadByte(SPIx,addr);
}

/**
  * @brief  配置各个外设时钟.
  * @param  无
  * @retval 无
  */
void RCC_Configuration(void)
{  
	/* UART0, GPIO 时钟使能 */
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
	printf("正在读取Flash信息\r\n");
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


