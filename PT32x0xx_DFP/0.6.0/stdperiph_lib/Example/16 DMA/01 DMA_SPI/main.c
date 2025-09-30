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

u16 SPIn_VAL[10] = {0};
u16 SPIn_TX_Buf[10] = {0,1,2,3,4,5,6,7,8,9};

#if defined (PT32G031x) || defined (PTM280x)
/**
* @brief 软件延时函数
* @param 无
* @retval 无
*/
void Software_Delay(void)
{
	u8 i, j;
	for(i=0; i<200; i++)
		for(j=0; j<200; j++);
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
* @brief 配置DMA工作模式
* @param 无
* @retval 无
*/
void DMA_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStruct;
	DMA_StructInit(&DMA_InitStruct);
	
	DMA_InitStruct.DMA_SourceBaseAddress = (u32)&SPIn->DR;		//源地址
	DMA_InitStruct.DMA_DestinationBaseAddress = (u32)SPIn_VAL;	//目的地址
	DMA_InitStruct.DMA_NumberOfData = 10;//要传输数据的数量
	DMA_InitStruct.DMA_SourceDataSize = DMA_SourceDataSize_Half;//数据源至DMA控制器的传输数据宽度
	DMA_InitStruct.DMA_DestinationDataSize = DMA_DestinationDataSize_Half;	//DMA控制器至数据目的地的传输数据宽度
	DMA_InitStruct.DMA_SourceAddressIncrement = DMA_SourceAddressIncrement_Disable;	//DMA源地址递增不使能
	DMA_InitStruct.DMA_DestinationAddressIncrement = DMA_DestinationAddressIncrement_Enable;	//DMA目的地址递增
	DMA_InitStruct.DMA_Direction = DMA_Direction_PeripheralToMemory;//传输方向
	DMA_InitStruct.DMA_CircularMode = DMA_CircularMode_Disable;	//不开启循环模式
	DMA_InitStruct.DMA_ChannelPriority = DMA_ChannelPriority_0;
	DMA_PeripheralConfig(DMAn,DMAn_CHNUM,DMA_CH_SPIn_RX);	//DMA通道0关联外设SPIn_RX
	DMA_Init(DMAn_Channeln,&DMA_InitStruct);	//初始化DMA
	DMA_Cmd(DMAn_Channeln,ENABLE);	//使能
}

/**
* @brief SPI驱动函数
* @param 无
* @retval 无
*/
void SPI_Driver(SPI_TypeDef *SPIx)
{
	SPI_InitTypeDef SPI_InitStructure;						 
	SPI_StructInit(&SPI_InitStructure);//使用默认值初始化SPI_InitStructure

	/* SPI 模式配置 */
	SPI_InitStructure.SPI_MasterSlaveMode = SPI_MasterSlaveMode_Master;//配置为主机模式
	SPI_InitStructure.SPI_ClockPolarity = SPI_ClockPolarity_High;//时钟极性，空闲状态时，SCK保持高电平
	SPI_InitStructure.SPI_ClockPhase = SPI_ClockPhase_2Edge;//时钟相位，数据采样从第二个时钟边沿开始
	SPI_InitStructure.SPI_CSS = SPI_CSS_SoftwareControl;//CS软件控制，默认初始值为高电平
	SPI_InitStructure.SPI_Prescaler = 1;//SPI时钟分频系数
	SPI_InitStructure.SPI_BaudRate = 16;//SPI波特率预分频 f(sck)=f(pclk)/(SPI_Prescaler*(1+SPI_Prescaler))
	
	SPI_Init(SPIx, &SPI_InitStructure);//初始化SPI
	SPI_SoftwareControlCSSConfig(SPIx, SPI_SoftwareControlCSS_High);//软件片选信号控制：CS输出高电平
//	SPI_TransferDMACmd(SPIx,ENABLE);

	SPI_ReceiveDMACmd(SPIx,ENABLE);//使能SPI接收DMA功能
	SPIx->CR1 |= SPI_CR1_LBM;
	SPI_Cmd(SPIx, ENABLE);//使能SPI
}

/**
  * @brief  配置各个外设时钟.
  * @param  无
  * @retval 无
  */
void RCC_Configuration(void)
{  
	/* UART, GPIO, DMA 时钟使能 */
	RCC_AHBPeriphClockCmd(UART_Periph_GPIOx|DMAn_Periph_CLOCK, ENABLE);
	RCC_APBPeriph2ClockCmd(APBPeriph2_UARTx|SPIn_Periph_CLOCK,ENABLE); 
#if defined	RETARGET_UART_CLKSRC
	RCC_Periph2ClockSourceConfig(RETARGET_UART_Periph,RETARGET_UART_CLKSRC);
#endif
}

int main (void)
{
	u8 i = 0;
	u8 flag = 0;
  	/*UART配置*/
	RCC_Configuration();
	UART_Driver(9600);
	/*SPI配置*/
	SPI_Driver(SPIn);
	/*DMA配置*/
	DMA_Mode_Config();
	/*SPI发送*/
	for(i=0;i<10;i++)
	{
		SPI_SendData(SPIn,SPIn_TX_Buf[i]);
		Software_Delay();
	}
	while(1)
	{
		if((DMA_GetNumberOfData(DMAn_Channeln) == 0)&&(flag == 0))
		{
			for(i=0;i<10;i++)
			{
				printf("SPIn_VAL[%d] = %d\r\n",i,SPIn_VAL[i]);
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


