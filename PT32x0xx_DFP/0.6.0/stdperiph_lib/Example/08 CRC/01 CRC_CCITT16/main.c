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

#if defined (PT32G031x) || defined (PTM280x)

u8 NORMAL_DATA_BUF[256]; //输入待加密的数据缓冲区
u8 err=0;
u16 s;

const u16 crc_ccitt_tab[256] = {
  0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
  0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
  0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
  0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
  0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
  0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
  0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
  0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
  0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
  0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
  0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
  0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
  0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
  0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
  0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
  0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
  0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
  0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
  0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
  0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
  0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
  0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
  0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
  0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
  0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
  0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
  0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
  0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
  0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
  0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
  0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
  0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

u16 crc_ccitt_fast(u16 crc, u8* buffer, u32 length)
{
  while (length--)
    crc = crc_ccitt_tab[(crc >> 8 ^ *buffer++) & 0xFF] ^ (crc << 8);
  
  return crc;
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
* @brief 初始化CRC模块的工作及其工作方式
* @param 无
* @retval 无
*/
void CRC_Driver(void)
{
	CRC_InitTypeDef CRC_InitStructure;					//定义一个CRC_InitTypeDef类型的结构体
	
	CRC_StructInit(&CRC_InitStructure);					//使用默认值初始化CRC_InitStructure
	
	CRC_InitStructure.CRC_InputByteSequenceReversal=CRC_InputByteSequenceReversal_Enable;		//输入字节序列反转 只对16bit模式有效
	CRC_InitStructure.CRC_InputBitSequenceReversal = CRC_InputBitSequenceReversal_Disable;		//输入位序列反转
	CRC_InitStructure.CRC_OutputBitSequenceReversal = CRC_OutputBitSequenceReversal_Disable;		//输出位序列反转
	CRC_InitStructure.CRC_Input = CRC_Input_16b;		//16 位数据宽度
	CRC_InitStructure.CRC_Poly = CRC_Poly_CCITT16;		//设置多项式系数
	CRC_InitStructure.CRC_Seed = 0;						//CRC初始值
	CRC_Init(CRC,&CRC_InitStructure);						//初始化CRC
	
	CRC_Cmd(CRC,ENABLE);									//CRC使能
	CRC_ResetDout(CRC);									//清除CRC数据输出寄存器
}

/**
  * @brief  配置各个外设时钟.
  * @param  无
  * @retval 无
  */
void RCC_Configuration(void)
{  
	/* CRC, UART, GPIO 时钟使能 */
	RCC_AHBPeriphClockCmd(UART_Periph_GPIOx, ENABLE);
	RCC_APBPeriph2ClockCmd(APBPeriph2_UARTx,ENABLE); 
	RCC_APBPeriph4ClockCmd(RCC_APBPeriph4_CRC,ENABLE); 
}


int main (void)
{
	u32 length,i=0;
	u8 *buf8;
	u16 *buf16;
	u16 sum1, sum2,CRC_Result;	
	CRC_Result = 0;										//CRC初始值  
	
	RCC_Configuration();
	UART_Driver(9600);	
	CRC_Driver();	
	
	buf8 = (u8*)NORMAL_DATA_BUF;   
	buf16 = (u16*)NORMAL_DATA_BUF; 
	
	for (i=0; i<256; i++) 
	{
		*buf8++ = (u8)i;    
	} 
	/* 软件CRC计算 */
	buf8 = (u8*)NORMAL_DATA_BUF;	
	buf16 = (u16*)NORMAL_DATA_BUF;
	
	length = 256;		
	sum1 = crc_ccitt_fast(CRC_Result, buf8, length);	//获取CRC值

	/* 硬件CRC计算 */
	length /= 2;
	while (length--)
	{ 
		s=*buf16;
		CRC->DINR=*buf16++;
	}	
	sum2 = (u16)CRC_GetCRC(CRC);							//获取CRC值
	

	/* 2次计算结果比较 */
	if(sum1 != sum2)
	{
		err++;
		printf("CRC error\r\n");   
	}		
	else if(sum1 == sum2)
	{
		printf("软件计算CRC结果与硬件计算CRC结果相同\r\n软件计算CRC结果:%d\t硬件计算CRC结果:%d\r\n",sum1,sum2);
	}  
	
	while (1)
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


