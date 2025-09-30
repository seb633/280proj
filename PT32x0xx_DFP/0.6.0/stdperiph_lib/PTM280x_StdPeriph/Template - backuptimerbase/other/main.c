/******************************************************************************
 * @Example PT32x0xx_main
 * @author  Application development team
 * @version V1.0.0
 * @date    2023/11/07
 * @brief   Main program body - 双向DCDC控制器 (寄存器级配置)
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
#include "dcdc_control.h"
#include "adc_monitor.h"
#include "fault_detect.h"
#include "comm.h"

#if defined(PT32G031x) || defined(PTM280x)

#define UART_RECV_MAX_SIZE 256

typedef __PACKED_STRUCT UartRecvBuffType
{
	u16 write;
	u16 read;
	u8 buf[UART_RECV_MAX_SIZE];
}
UartRecvBuffType;

u16 ReceiveData = 0;
u8 flag = 0;
UartRecvBuffType UartRecvBuff;

static void print_hex(u8 *pBuf, u16 size)
{
	u16 i;

	for (i = 0; i < size; i++)
	{
		printf("%02x ", pBuf[i]);
	}
	printf("\n");
}

void UartBuffInit(void)
{
	UartRecvBuff.write = 0;
	UartRecvBuff.read = 0;
}

void UartRecvHandle(u8 ch)
{
	if (UartRecvBuff.read == (UartRecvBuff.write + 1) || (UartRecvBuff.write == (UART_RECV_MAX_SIZE - 1) && UartRecvBuff.read == 0))
	{
		return;
	}

	UartRecvBuff.buf[UartRecvBuff.write++] = ch;
	if (UartRecvBuff.write >= UART_RECV_MAX_SIZE)
	{
		UartRecvBuff.write = 0;
	}
}

u16 UartReadBytes(u8 *pBuf, u16 size)
{
	u16 readSize;

	if (size == 0)
	{
		return 0;
	}

	for (readSize = 0; readSize < size; readSize++)
	{
		if (UartRecvBuff.write != UartRecvBuff.read)
		{
			pBuf[readSize] = UartRecvBuff.buf[UartRecvBuff.read++];
			if (UartRecvBuff.read >= UART_RECV_MAX_SIZE)
			{
				UartRecvBuff.read = 0;
			}
		}
		else
		{
			break;
		}
	}

	return readSize;
}

/**
 * @brief 配置UART的复用引脚 - 寄存器级 (已在GPIO_AFIO_Configuration中实现)
 * @param 无
 * @retval 无
 */
void UART_GPIO_Config(void)
{
	// GPIO和AFIO配置已在GPIO_AFIO_Configuration()中完成
	// PA15 -> UART0_TX (AF5), PB2 -> UART0_RX (AF5)
}

/**
 * @brief 配置UART的工作模式 - 寄存器级配置
 * @param 无
 * @retval 无
 */
void UART_Mode_Config(u32 Baudrate)
{
	// UART0 寄存器级配置
	// 计算波特率：BRR = PCLK / (采样率 * 波特率)
	// 假设PCLK=48MHz, 采样率=8 (RETARGET_UART_SampleRate)
	u32 brr_val = 48000000 / (8 * Baudrate);
	UART0->BRR = brr_val;
	
	// UART控制寄存器配置：8N1格式
	UART0->CR = UART_CR_RE |     // 接收使能
	            UART_CR_EN;      // UART使能 (注意：没有独立的TE位)
	
	// 中断使能：接收中断
	UART0->IER = UART_IER_RXNE; // 接收缓冲区非空中断
	
	// NVIC中断配置
	NVIC_SetPriority(UART0_IRQn, 0);
	NVIC_EnableIRQ(UART0_IRQn);
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
 * @brief  配置各个外设时钟 - 寄存器级配置
 * @param  无
 * @retval 无
 */
void RCC_Configuration(void)
{
	/* 启用外设时钟 - 直接写寄存器 */
	// TIM1 (APBENR1), UART0 (APBENR2), ADC0/CMP0/LDAC (APBENR3), GPIO (AHBENR)
	RCC->APBENR1 |= RCC_APBENR1_TIM1;           // TIM1 时钟
	RCC->APBENR2 |= RCC_APBENR2_UART0;          // UART0 时钟
	RCC->APBENR3 |= (RCC_APBENR3_ADC0 | RCC_APBENR3_CMP0 | RCC_APBENR3_LDAC); // ADC0, CMP0, LDAC 时钟
	RCC->AHBENR |= (RCC_AHBENR_GPIOA | RCC_AHBENR_GPIOB); // GPIO A/B 时钟
	
#if defined RETARGET_UART_CLKSRC
	RCC_Periph2ClockSourceConfig(RETARGET_UART_Periph, RETARGET_UART_CLKSRC);
#endif
}

/**
 * @brief  配置GPIO和AFIO - 寄存器级配置
 * @param  无
 * @retval 无
 */
void GPIO_AFIO_Configuration(void)
{
	/* TIM1 PWM 引脚配置 */
	// PA5 -> TIM1_CH1 (AF1), PA7 -> TIM1_CH1N (AF1)
	AFIOA->AFSR[0] |= (AFIO_AF_1 << (5 * 4)) | (AFIO_AF_1 << (7 * 4)); // PA5, PA7 设为 AF1
	
	/* TIM1_BKIN 引脚配置 */
	// PB12 -> TIM1_BKIN (AF3) - 需要使用AFSR[1]因为是高位引脚
	AFIOB->AFSR[1] |= (AFIO_AF_3 << ((12 - 8) * 4)); // PB12 设为 AF3
	
	/* UART0 引脚配置 */
	// PA15 -> UART0_TX (AF5) - 需要使用AFSR[1]因为是高位引脚
	AFIOA->AFSR[1] |= (AFIO_AF_5 << ((15 - 8) * 4)); // PA15 设为 AF5
	AFIOB->AFSR[0] |= (AFIO_AF_5 << (2 * 4));  // PB2 设为 AF5
	
	/* ADC 模拟引脚配置 */
	// 假设使用 PA3(ADC_IN3) 和 PA4(ADC_IN4) 作为 Vin/Vout 采样
	AFIOA->ANASR |= (1 << 3) | (1 << 4); // PA3, PA4 设为模拟模式
}

int main(void)
{
	u8 buf[16]; // = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	u16 size;

	RCC_Configuration();
	GPIO_AFIO_Configuration();
	UART_Driver(9600);
	// printf("澎湃微欢迎使用！\r\n");
	UartBuffInit();
	SystemInit();
	fun_DCDC_Init();
	fun_ADC_Init();
	fun_Comm_Init();

	// Main control loop
	while (1)
	{
		if ((size = UartReadBytes(buf, 16)) > 0)
		{
			print_hex(buf, size);
		}
		fun_ADC_Update();      // 1) Sample ADCs (updates cached Vm/Im)
		fun_Fault_Check();     // 2) Fault detection (OV/UV/OC)
		fun_DCDC_ModeUpdate(); // 3) Decide/keep BUCK or BOOST mode
		fun_DCDC_ControlLoop(); // 4) PI control + protections + PWM apply
		fun_Comm_Process();    // 5) UART command parsing and status reply
	}
}

/**
 * @brief UART中断服务函数 - 寄存器级
 * @param None
 * @retval None
 */
void RETARGET_UART_Handler(void)
{
	if (UART0->SR & UART_SR_RXNE) // 检查接收数据寄存器非空标志
	{
		ReceiveData = UART0->DR; // 读取数据寄存器
		UartRecvHandle(ReceiveData);
	}
}

#endif

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 */
void assert_failed(u8 *file, u32 line)
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
