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


#define number_c	1000


u16 ADC_Timeout = 0;
u16 ADC_Data = 0;

/**
* @brief ����UART�ĸ�������
* @param ��
* @retval ��
*/
void UART_GPIO_Config(void)
{
	/* ����UART�ܽŵĸ��ù��� */
	GPIO_DigitalRemapConfig(UART_TX_PORT, UART_TX_PIN, UART_TX_AFIO, ENABLE);	
	GPIO_DigitalRemapConfig(UART_RX_PORT, UART_RX_PIN, UART_RX_AFIO, ENABLE);	
}

/**
* @brief ����UART�Ĺ���ģʽ
* @param ��
* @retval ��
*/
void UART_Mode_Config(void)
{
	UART_InitTypeDef UART_InitStruct;

	/*��ʼ��UART*/
	UART_InitStruct.UART_BaudRate = 9600;
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
void UART_Driver(void)
{
	UART_GPIO_Config();
	UART_Mode_Config();
}

/**
* @brief �ȴ�ADC����
* @param ��
* @retval ��
*/
void ADC_Wait_Status(u32 ADC_FLAG)
{
	ADC_Timeout = number_c;
	while(ADC_GetFlagStatus(ADCn,ADC_FLAG) == RESET)
	{
		if((ADC_Timeout--) == 0)
		{
			break;
		}
	}
}

/**
* @brief ����ADCͨ����GPIOģ�⹦��
* @param ��
* @retval ��
*/
void ADC_GPIO_Config(void)
{
	GPIO_AnalogRemapConfig(ADCn_PIN_AFIO,ADCn_PIN, ENABLE);
}

/**
* @brief ADC��������
* @param ��
* @retval ��
*/
void ADC_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStruct;
	
	/*ADC��ʼ��*/
	ADC_InitStruct.ADC_Channel = ADCn_Channelx;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Single;
	ADC_InitStruct.ADC_Prescaler = 30;
	ADC_InitStruct.ADC_RVSPS = ADC_RVSPS_VDDA;
	ADC_InitStruct.ADC_SampleTime = 31;
	ADC_InitStruct.ADC_ChannelSetupTime = 30;
	ADC_Init(ADCn,&ADC_InitStruct);
	/*�����������ת��*/
	ADC_RegularTriggerSource(ADCn,ADC_RegularTriggerSource_Software);
	/*ADCʹ��*/
	ADC_Cmd(ADCn,ENABLE);
	ADC_Wait_Status(ADC_FLAG_RDY);
}

/**
* @brief �����ʱ����
* @param ��
* @retval ��
*/
void Software_Delay(u32 t)
{
	while(t--);
}

/**
* @brief ADC��������
* @param ��
* @retval ��
*/
void ADC_Driver(void)
{
	ADC_GPIO_Config();
	ADC_Mode_Config();
}

/**
  * @brief  ���ø�������ʱ��.
  * @param  ��
  * @retval ��
  */
void RCC_Configuration(void)
{  
	/* ADC, UART, GPIOʱ��ʹ�� */
	RCC_APBPeriph2ClockCmd(APBPeriph2_UARTx, ENABLE);
	RCC_APBPeriph3ClockCmd(ADCn_Periph_CLOCK, ENABLE);
	RCC_AHBPeriphClockCmd(ADCn_PIN_CLOCK|UART_Periph_GPIOx, ENABLE);
#if defined	RETARGET_UART_CLKSRC
	RCC_Periph2ClockSourceConfig(RETARGET_UART_Periph,RETARGET_UART_CLKSRC);
#endif
}

int main (void)
{
	RCC_Configuration();
	UART_Driver();
	printf("ADC����ת����ʼ\r\n");
	ADC_Driver();
	while(1)
	{
		ADC_StartOfRegularConversion(ADCn);
		while(ADC_GetFlagStatus(ADCn,ADC_FLAG_EOC) == 0);
		printf("%d\r\n",ADC_GetConversionValue(ADCn));	
		Software_Delay(1000000);
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


