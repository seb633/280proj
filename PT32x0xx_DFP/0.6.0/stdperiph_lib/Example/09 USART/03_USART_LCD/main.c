/******************************************************************************
  * @Example PT32x0xx_main
  * @author  应用开发团队
  * @version V1.0.0
  * @date    2023/11/07
  * @brief   Main program body
  *          
  ******************************************************************************
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

#define SPIT_FLAG_TIMEOUT         	((u32)0x1000)

#define FLASH_CS_PORT 				GPIOB
#define FLASH_CS_PIN 				GPIO_Pin_4

#define FLASH_CS_HIGH() 			GPIO_SetBits(FLASH_CS_PORT, FLASH_CS_PIN)
#define FLASH_CS_LOW() 				GPIO_ResetBits(FLASH_CS_PORT, FLASH_CS_PIN)

//----------------------------------------------------------------

#define LCD_RS_PORT 				GPIOB
#define LCD_RS_PIN 					GPIO_Pin_11

#define LCD_RES_PORT 				GPIOB
#define LCD_RES_PIN 				GPIO_Pin_12

#define LCD_CS_PORT 				GPIOB
#define LCD_CS_PIN 					GPIO_Pin_5

#define LCD_BKLED_PORT 				GPIOB
#define LCD_BKLED_PIN 				GPIO_Pin_10

#define LCD_RS_HIGH() 				GPIO_SetBits(LCD_RS_PORT, LCD_RS_PIN)
#define LCD_RS_LOW() 				GPIO_ResetBits(LCD_RS_PORT, LCD_RS_PIN)

#define LCD_RES_HIGH() 				GPIO_SetBits(LCD_RES_PORT, LCD_RES_PIN)
#define LCD_RES_LOW() 				GPIO_ResetBits(LCD_RES_PORT, LCD_RES_PIN)

#define LCD_CS_HIGH() 				GPIO_SetBits(LCD_CS_PORT, LCD_CS_PIN)
#define LCD_CS_LOW() 				GPIO_ResetBits(LCD_CS_PORT, LCD_CS_PIN)

#define LCD_BKLED_ON() 				GPIO_SetBits(LCD_BKLED_PORT, LCD_BKLED_PIN)
#define LCD_BKLED_OFF() 			GPIO_ResetBits(LCD_BKLED_PORT, LCD_BKLED_PIN)

//----------------------------------------------------------------

#define ST7735_GREENTAB160x80
#define COLSTART 24
#define ROWSTART 0

#define TFT_INIT_DELAY 0x80

#define TFT_NOP 0x00
#define TFT_SWRST 0x01

#define TFT_CASET 0x2A
#define TFT_PASET 0x2B
#define TFT_RAMWR 0x2C

#define TFT_RAMRD 0x2E
#define TFT_IDXRD 0x00

#define TFT_MADCTL 0x36
#define TFT_MAD_MY 0x80
#define TFT_MAD_MX 0x40
#define TFT_MAD_MV 0x20
#define TFT_MAD_ML 0x10
#define TFT_MAD_BGR 0x08
#define TFT_MAD_MH 0x04
#define TFT_MAD_RGB 0x00

#define TFT_INVOFF 0x20
#define TFT_INVON 0x21

// ST7735 specific commands used in init
#define ST7735_NOP 0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID 0x04
#define ST7735_RDDST 0x09

#define ST7735_SLPIN 0x10
#define ST7735_SLPOUT 0x11
#define ST7735_PTLON 0x12
#define ST7735_NORON 0x13

#define ST7735_INVOFF 0x20
#define ST7735_INVON 0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON 0x29
#define ST7735_CASET 0x2A
#define ST7735_RASET 0x2B
#define ST7735_RAMWR 0x2C
#define ST7735_RAMRD 0x2E

#define ST7735_PTLAR 0x30
#define ST7735_VSCRDEF 0x33
#define ST7735_COLMOD 0x3A
#define ST7735_MADCTL 0x36
#define ST7735_VSCRSADD 0x37

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR 0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1 0xC0
#define ST7735_PWCTR2 0xC1
#define ST7735_PWCTR3 0xC2
#define ST7735_PWCTR4 0xC3
#define ST7735_PWCTR5 0xC4
#define ST7735_VMCTR1 0xC5

#define ST7735_RDID1 0xDA
#define ST7735_RDID2 0xDB
#define ST7735_RDID3 0xDC
#define ST7735_RDID4 0xDD

#define ST7735_PWCTR6 0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

#define ST77XX_MADCTL_MY 0x80
#define ST77XX_MADCTL_MX 0x40
#define ST77XX_MADCTL_MV 0x20
#define ST77XX_MADCTL_ML 0x10
#define ST77XX_MADCTL_RGB 0x00
#define ST77XX_MADCTL_BGR 0x08

//240*320
#define POWER_CONTROL_A	0xCB
#define POWER_CONTROL_B_1	0xCF
#define POWER_CONTROL_B_2	0xEF
#define DRIVER_TIMING_CONTROL_A	0xE8
#define DRIVER_TIMING_CONTROL_B	0xEA
#define POWER_ON_SEQUENCE_CONTROL	0xED
#define PUMP_RATIO_CONTROL	0xF7
#define POWER_CONTROL_VRH	0xC0
#define POWER_CONTROL_SAP_BT	0xC1
#define VCM_CONTROL	0xC5
#define VCM_CONTROL_2	0xC7
#define MEMORY_ACCESS_CONTROL	0x36
#define PIXEL_FORMAT	0x3A
#define FRAME_RATIO_CONTROL	0xB1
#define DISPLAY_FUNCTION_CONTROL_1	0xB6
#define DISPLAY_FUNCTION_CONTROL_2	0x30
#define GAMMA_FUNCTION_DISABLE	0xF2
#define GAMMA_CURVE_SELECTED	0x26
#define POSITIVE_GAMMA_CORRECTION	0xE0
#define NEGATIVE_GAMMA_CORRECTION	0xE1
#define EXIT_SLEEP	0x11
#define TURN_ON_DISPLAY	0x29
#define SWRESET 0x01

#define LCD_HOR_RES 240
#define LCD_VER_RES 320

/*The LCD needs a bunch of command/argument values to be initialized. They are stored in this struct. */
typedef struct
{
	u8 cmd;
	u8 data[16];
	u8 databytes; // No of data in data; bit 7 = delay after set; 0xFF = end of cmds.
} lcd_init_cmd_t;

typedef struct
{
	u16 x1;
	u16 y1;
	u16 x2;
	u16 y2;
} disp_area_t;

u16 tick=0;
u32 burst_end_flag = 0;
u8 ili9341_portrait_mode = 0;

void Delay(u32 t)
{
	while(t--);
}

void Delay_Ms(u32 t)
{
	u32 i=0;
	tick=0;
	SysTick_Config(GetClockFreq(CLKSRC_SYSCLK)/1000);
	for(i=0;i<t;i++)
	{
		while(!(tick));
		tick=0;
	}
	SysTick->CTRL=0;
}

void ili9341_send_cmd(u8 cmd)
{
	LCD_CS_LOW();
	LCD_RS_LOW();
	USART0->DR = cmd;
	while (!(USART0->SR & 0x800));
	LCD_CS_HIGH();
}

void ili9341_send_data(void *data, u16 length)
{
	LCD_CS_LOW();
	LCD_RS_HIGH();
	for (u16 i = 0; i < length; i++)
	{
		USART0->DR = ((u8 *)data)[i];
		while (!(USART0->SR & 0x800));
	}
	LCD_CS_HIGH();
}

void FLASH_GPIO_Config(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = FLASH_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OutPP;
	GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NoPull;
	GPIO_Init(FLASH_CS_PORT, &GPIO_InitStructure);

	FLASH_CS_HIGH();
}

/**
  * @brief  等待超时回调函数
  * @param  None.
  * @retval None.
  */
u16 SPI_TIMEOUT_UserCallback(u8 errorCode)
{
  /* 等待超时后的处理,输出错误信息 */
//  FLASH_ERROR("SPI 等待超时!errorCode = %d",errorCode);
  return 0;
}

u8 FLASH_SendByte(USART_TypeDef* USARTx,u8 byte)
{
	u16 SPITimeout;
	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* 等待发送缓冲区为空，TXE事件 */
	while (!(USARTx->SR&USART_SR_TXE))
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
	}

	/* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
	USARTx->DR=byte;

	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* 等待接收缓冲区非空，RXNE事件 */
	while (!(USARTx->SR&USART_SR_RXNE))
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
	}
	Delay(50);
	/* 读取数据寄存器，获取接收缓冲区数据 */
	return USARTx->DR;
}

void FLASH_Open(u32 address)
{
	FLASH_CS_LOW();
	FLASH_SendByte(USART0,0x3);
	FLASH_SendByte(USART0,(address&0xFF0000)>>16);
	FLASH_SendByte(USART0,(address&0xFF00)>>8);
	FLASH_SendByte(USART0,address&0xFF);

		// There is a level conflict here. It is suggested to add a resistor to the MOSI pin of the MCU.
	GPIO_DigitalRemapConfig(AFIOB,GPIO_Pin_7,AFIO_AF_4,DISABLE);	
//	GPIO_DigitalRemapConfig(AFIOA,GPIO_Pin_6,AFIO_AF_4,DISABLE);		

//	while (USART0->SR & 0x01) // Empty SPI reveive FIFO buffer
//		USART0->DR;
}

void USART_SendClk(USART_TypeDef* USARTx, u32 length)
{
	u32 lengthtemp=0,lengthcnt=0,i=0;
	lengthcnt=length&0xFF000000;
	lengthtemp=length&0xFFFFFF;
	for(i=0;i<lengthcnt;i++)
	{
		USART0->CR3=0xFFFFFF;
		while (!burst_end_flag);
		burst_end_flag=0;
	}
	USART0->CR3=lengthtemp;
	while (!(burst_end_flag));	
	burst_end_flag=0;
}

void LCD_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = LCD_RS_PIN | LCD_RES_PIN | LCD_BKLED_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OutPP;
	GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NoPull;
	GPIO_Init(LCD_RS_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LCD_CS_PIN;
	GPIO_Init(LCD_CS_PORT, &GPIO_InitStructure);

	LCD_CS_HIGH();
	LCD_RES_HIGH();
	LCD_BKLED_OFF();
}

//----------------------------------------------------------------

void LCD_SPI_Config(void)
{
//	GPIO_DigitalRemapConfig(AFIOA,GPIO_Pin_6,AFIO_AF_4,ENABLE);				//MOSI	
	GPIO_DigitalRemapConfig(AFIOB,GPIO_Pin_7,AFIO_AF_4,ENABLE);				//MOSI
//	GPIO_DigitalRemapConfig(AFIOB,GPIO_Pin_8,AFIO_AF_4,ENABLE);				//MISO
	GPIO_DigitalRemapConfig(AFIOB,GPIO_Pin_9,AFIO_AF_4,ENABLE);				//SCK

	USART0->BRR=2;
	USART0->CR1=0x0000083;	//mode 00 rxen spien
	USART0->IER = 0x10000;
	NVIC_EnableIRQ(USART0_IRQn);	
}

void ili9341_set_orientation(u8 orientation)
{
	// const char *orientation_str[] = {
	//     "PORTRAIT", "PORTRAIT_INVERTED", "LANDSCAPE", "LANDSCAPE_INVERTED"
	// };

	// ESP_LOGD(TAG, "Display orientation: %s", orientation_str[orientation]);

	/*
		Portrait:  0xC8 = ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST77XX_MADCTL_BGR
		Landscape: 0xA8 = ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST77XX_MADCTL_BGR
		Remark: "inverted" is ignored here
	*/
	const u8 data[] = {0x88, 0xE8, 0xC8, 0xA8};

	// ESP_LOGD(TAG, "0x36 command value: 0x%02X", data[orientation]);

	if (orientation < 2)
		ili9341_portrait_mode = 1;
	else
		ili9341_portrait_mode = 0;

	ili9341_send_cmd(ST7735_MADCTL);
	ili9341_send_data((void *)&data[orientation & 0x03], 1);
}

void LCD_ILI9341_Init(void)
{
	const lcd_init_cmd_t init_cmds[] = {
		{POWER_CONTROL_A , {0x39, 0x2C, 0x00,0x34,0x02}, 5},				   
		{POWER_CONTROL_B_1, {0x00, 0xC1, 0x30}, 3},				   
		{POWER_CONTROL_B_2, {0x03, 0x80, 0x02}, 3}, 
		{DRIVER_TIMING_CONTROL_A, {0x85,0x00,0x78}, 3},								   
		{DRIVER_TIMING_CONTROL_B, {0x00, 0x00}, 2},					   
		{POWER_ON_SEQUENCE_CONTROL, {0x64,0x03,0x12,0x81}, 4},								 
		{PUMP_RATIO_CONTROL, {0x20}, 1},						   
		{POWER_CONTROL_VRH, {0x23}, 1},						
		{POWER_CONTROL_SAP_BT, {0x10}, 1},						
		{VCM_CONTROL, {0x3E,0x28}, 2},			
		{VCM_CONTROL_2, {0x86}, 1},			
		{MEMORY_ACCESS_CONTROL, {0x48}, 1}, 
		{PIXEL_FORMAT, {0x55}, 1},																								
		{FRAME_RATIO_CONTROL, {0x00,0X13}, 2}, // 16 args, no delay:x
		{DISPLAY_FUNCTION_CONTROL_1, {0x08,0x82,0x27,0x00}, 4}, 
		{DISPLAY_FUNCTION_CONTROL_2, {0x00,0x00,0x01,0x3F}, 4},																					
		{GAMMA_FUNCTION_DISABLE, {0}, 1},	
		{GAMMA_CURVE_SELECTED,{0x1},1},
		{POSITIVE_GAMMA_CORRECTION,{0x0F,0x31,0x2B,0x0C,0x0E,0x08,0x4E,0xF1,0x37,0x07,0x10,0x03,0x0E,0x09,0x00},15},
		{NEGATIVE_GAMMA_CORRECTION,{0x00,0x0E,0x14,0x03,0x11,0x07,0x31,0xC1,0x48,0x08,0x0F,0x0C,0x31,0x36,0x0F},15},
		{EXIT_SLEEP, {0}, 0},
		{TURN_ON_DISPLAY, {0}, 0},		
		{0, {0}, 0xff}
	};

	LCD_RES_LOW();
	Delay_Ms(30);
	LCD_RES_HIGH();
	Delay_Ms(30);
	ili9341_send_cmd(SWRESET);
	Delay_Ms(500);	
	// Send all the commands
	u16 cmd = 0;
	while (init_cmds[cmd].databytes != 0xff)
	{
		ili9341_send_cmd(init_cmds[cmd].cmd);
		ili9341_send_data((void *)(init_cmds[cmd].data), init_cmds[cmd].databytes & 0x1F);
		if (init_cmds[cmd].databytes & 0x80)
			Delay_Ms(100);
		cmd++;
	}

#define CONFIG_DISPLAY_ORIENTATION 		0

#if (CONFIG_DISPLAY_ORIENTATION == 0) || (CONFIG_DISPLAY_ORIENTATION == 1)
	ili9341_portrait_mode = 1;
#else
	ili9341_portrait_mode = 0;
#endif

	ili9341_set_orientation(CONFIG_DISPLAY_ORIENTATION);

	LCD_BKLED_ON();
}

void ili9341_flush(const disp_area_t *area, u32 address)
{
	u8 data[4];

	/*Column addresses*/
	ili9341_send_cmd(0x2A);
	data[0] = (area->x1 >> 8) & 0xFF;
	data[1] = (area->x1 & 0xFF) ;
	data[2] = (area->x2 >> 8) & 0xFF;
	data[3] = (area->x2 & 0xFF);
	ili9341_send_data(data, 4);

	/*Page addresses*/
	ili9341_send_cmd(0x2B);
	data[0] = (area->y1 >> 8) & 0xFF;
	data[1] = (area->y1 & 0xFF);
	data[2] = (area->y2 >> 8) & 0xFF;
	data[3] = (area->y2 & 0xFF);
	ili9341_send_data(data, 4);

	/*Memory write*/
	ili9341_send_cmd(0x2C);

	// FLASH_Open(page * LCD_HOR_RES * LCD_VER_RES * 2);
	FLASH_Open(address);
	LCD_CS_LOW();
	LCD_RS_HIGH();

	u32 total = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1);

	// for (u32 i = 0; i < (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1) * 2; i++)
//	while (!(USART0->SR & 0x100));
//	for (u32 i = 0; i < total*2; i++)
//	{
//		USART0->DR = 0xFF;
//		while (!(USART0->SR & 0x800));
//	}
	USART0->SR=0x800;
	USART_SendClk(USART0,total*2);

	FLASH_CS_HIGH(); // FLASH_Close();
	LCD_CS_HIGH();
//	GPIO_DigitalRemapConfig(AFIOA,GPIO_Pin_6,AFIO_AF_4,ENABLE);	 // Enable MOSI pin function	
	GPIO_DigitalRemapConfig(AFIOB,GPIO_Pin_7,AFIO_AF_4,ENABLE);	 // Enable MOSI pin function
}



int app_main(void)
{
#define DEMO_MODE 0

#if DEMO_MODE == 0
//	const u8 div[4] = {1,
//							2,
//							5,
//							100};

	while (1)
	{
		for (u8 time = 0; time < 4; time++)
		{
			for (u8 i = 0; i < 101; i++)
			{
				disp_area_t area = {0,
									0,
									239,
									319};

				ili9341_flush(&area, 0 + 240 * 320 * i * 2);

//				Delay_Ms(50 / div[time]);
			}
//			Delay_Ms(1500);
		}
	}
#else
	disp_area_t area = {0,
						0,
						79,
						159};

	ili9341_flush(&area, 776160);

	Delay_Ms(5000);

	return 0;
#endif
}



int main(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE); 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE); 
	RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_USART0,ENABLE);
	FLASH_GPIO_Config();
	LCD_GPIO_Config();
	LCD_SPI_Config();
	LCD_ILI9341_Init();

	app_main();

	while (1)
	{
	}
}

/**
* @brief USART0中断服务函数
* @param None
* @retval None
*/
void USART0_Handler(void)
{
	extern u32 burst_end_flag;
	burst_end_flag = 1;
	USART0->SR=0x10000;		
}

/**
* @brief SysTick中断服务函数
* @param None
* @retval None
*/
void SysTick_Handler(void)
{
	extern u16 tick;
	tick++;
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


