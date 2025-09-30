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

const u16 crc16_ibm_table[256] = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

u16 crc16_ibm_fast(const u8 *data, u32 length)
{
    u16 crc = 0xFFFF;    //CRC初始值
	
    while(length--)
	{
        crc = (crc >> 8) ^ crc16_ibm_table[(crc ^ *data++) & 0xFF];
    }
	
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
	
	CRC_InitStructure.CRC_InputBitSequenceReversal = CRC_InputBitSequenceReversal_Enable;		//输入位序列反转
	CRC_InitStructure.CRC_OutputBitSequenceReversal = CRC_OutputBitSequenceReversal_Enable;		//输出位序列反转
	CRC_InitStructure.CRC_Input = CRC_Input_16b;		//16 位数据宽度
	CRC_InitStructure.CRC_Poly = CRC_Poly_IBM16;		//设置多项式系数
	CRC_InitStructure.CRC_Seed = 0xFFFF;						//CRC初始值
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
	u16 sum1, sum2;	
	
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
	sum1 = crc16_ibm_fast(buf8, length);	//获取CRC值

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


