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
u16 ADC_Timeout = 0, flag = 0;
u16 ADC_Data = 0;



/**
* @brief ����UART�ĸ�������
* @param ��
* @retval ��
*/
void UART_GPIO_Config(void)
{ 
	GPIO_DigitalRemapConfig(UART_TX_PORT, UART_TX_PIN, UART_TX_AFIO,ENABLE);	//UART_TX���ֹ��ܸ���
	GPIO_DigitalRemapConfig(UART_RX_PORT, UART_RX_PIN, UART_RX_AFIO,ENABLE);	//UART_RX���ֹ��ܸ���
}

/**
* @brief ����UART�Ĺ���ģʽ
* @param ��
* @retval ��
*/
void UART_Mode_Config(u32 Baudrate)
{
	UART_InitTypeDef UART_InitStruct;

	/*��ʼ��UART*/
	UART_InitStruct.UART_BaudRate = Baudrate;
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

void UART_Driver(u32 Baudrate)
{
	UART_Mode_Config(Baudrate);
	UART_GPIO_Config();
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
* @brief ����ADC�Ĺ���ģʽ
* @param ��
* @retval ��
*/
void ADC_GPIO_Config(void)
{
	/*ADC_INxģ�⹦��ʹ��*/
	GPIO_AnalogRemapConfig(ADCn_PIN_AFIO,ADCn_PIN, ENABLE);
}

/**
* @brief ����ADC�Ĺ���ģʽ
* @param ��
* @retval ��
*/
void ADC_Mode_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	/*ADC_EOC�ж�ʹ��*/
	NVIC_InitStruct.NVIC_IRQChannel = ADCn_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
	ADC_ITConfig(ADCn,ADC_IT_EOC,ENABLE);
	/*ADC��ʼ��*/
	ADC_InitStruct.ADC_Channel = ADCn_Channelx;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Continuous;
	ADC_InitStruct.ADC_Prescaler = 3;
	ADC_InitStruct.ADC_RVSPS = ADC_RVSPS_VDDA;
	ADC_InitStruct.ADC_SampleTime = 31;
	ADC_Init(ADCn,&ADC_InitStruct);
	/*�����������ת��*/
	ADC_RegularTriggerSource(ADCn,ADC_RegularTriggerSource_Software);
	/*ADCʹ��*/
	ADC_Cmd(ADCn,ENABLE);
	ADC_Wait_Status(ADC_FLAG_RDY);
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
* @brief �����ʱ����
* @param ��
* @retval ��
*/
void Software_Delay(u32 t)
{
	while(t--);
}

/**
  * @brief  ���ø�������ʱ��.
  * @param  ��
  * @retval ��
  */
void RCC_Configuration(void)
{  
	/* ADC, UART, GPIO ʱ��ʹ�� */
	RCC_APBPeriph2ClockCmd(APBPeriph2_UARTx, ENABLE);
	RCC_APBPeriph3ClockCmd(ADCn_Periph_CLOCK, ENABLE);
	RCC_AHBPeriphClockCmd(UART_Periph_GPIOx|ADCn_PIN_CLOCK, ENABLE);
#if defined	RETARGET_UART_CLKSRC
	RCC_Periph2ClockSourceConfig(RETARGET_UART_Periph,RETARGET_UART_CLKSRC);
#endif
}

int main (void)
{
	RCC_Configuration();
	UART_Driver(9600);;
	printf("ADC����ת����ʼ\r\n");
	ADC_Driver();
#if !defined (PT32x012x) && !defined (PT32x002x)
	ADC_StartOfRegularConversion(ADCn);
#endif
	while(1)
	{		
		if(flag == 1)
		{
			printf("%d\r\n",ADC_Data);	
			Software_Delay(1000000);
			flag = 0;
		}
	}	
}

/**
* @brief ADC�жϷ�����
* @param None
* @retval None
*/
void ADCn_Handler(void)
{
	if(ADC_GetFlagStatus(ADCn,ADC_FLAG_EOC))
	{
		ADC_Data = ADC_GetConversionValue(ADCn);
		flag = 1;
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


