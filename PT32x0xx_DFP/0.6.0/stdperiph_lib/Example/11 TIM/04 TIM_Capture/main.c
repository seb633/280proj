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



u16 capture_value = 0;
u8 flag = 0;

#if defined (PT32G031x) || defined (PTM280x)

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
* @brief ���ø���TIM���벶��ͨ������
* @param ��
* @retval ��
*/
void CaptureTIM_GPIO_Config(void)
{
	/* ����TIM�ܽŵĸ��ù��� */
	GPIO_DigitalRemapConfig(CaptureTIM_CH_AFIO, CaptureTIM_CH_PIN, CaptureTIM_CH_AFx, ENABLE);
}

/**
* @brief ����TIM������ʽ
* @param ��
* @retval ��
*/
void CaptureTIM_Mode_Config(TIM_TypeDef *TIMx)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;//����һ��TIM_TimeBaseInitTypeDef���͵Ľṹ��
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	TIM_TimeBaseInitStruct.TIM_Prescaler = GetClockFreq(CLKSRC_HCLK)/1000-1;			//����ʱ��Ϊ1ms
	TIM_TimeBaseInitStruct.TIM_AutoReloadValue = 0xFFFF;						   		//����Ϊ���65535ms
	TIM_TimeBaseInitStruct.TIM_Direction = TIM_Direction_Up;					 		//���ϼ���
	TIM_TimeBaseInitStruct.TIM_CenterAlignedMode = TIM_CenterAlignedMode_Disable; 		//���������ļ���ģʽ
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStruct);									//��ʼ��CaptureTIM

	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;							//����Ϊͨ��1
	TIM_ICInitStruct.TIM_ICRiseCapture = TIM_ICRiseCapture_Disable;		 	//��ʹ�����������ز���
	TIM_ICInitStruct.TIM_ICFallCapture = TIM_ICFallCapture_Enable;		 	//ʹ�������½��ز���
	TIM_ICInitStruct.TIM_ICResetCounter = TIM_ICResetCounter_Enable;   		//���ò�������ü�����
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPrescaler_None;					//����Ϊ�޷�Ƶ
	TIM_ICInitStruct.TIM_ICFilter = TIM_ICFilter_4;							//�����˲�
	TIM_ICInitStruct.TIM_ICSource = TIM_ICSource_ICS1;						//ͨ��1����Դѡ��ΪICS1
	TIM_ICInit(TIMx,&TIM_ICInitStruct);									//��ʼ��������
	TIM_ITConfig(TIMx,TIM_IT_IC1I,ENABLE);                         		//ʹ��ͨ��1�����ж�
	
	NVIC_InitStruct.NVIC_IRQChannel = CaptureTIM_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIMx, ENABLE);													//����CaptureTIM
}

/**
* @brief ����TIM����
* @param ��
* @retval ��
*/
void CaptureTIM_Driver()
{
	CaptureTIM_GPIO_Config();
	CaptureTIM_Mode_Config(CaptureTIM);
}

/**
  * @brief  ���ø�������ʱ��.
  * @param  ��
  * @retval ��
  */
void RCC_Configuration(void)
{  
	/* CaptureTIM, UART, GPIO ʱ��ʹ�� */
	RCC_AHBPeriphClockCmd(CaptureTIM_CH_CLOCK|UART_Periph_GPIOx, ENABLE);
	RCC_APBPeriph1ClockCmd(CaptureTIM_Periph_CLOCK, ENABLE);
	RCC_APBPeriph2ClockCmd(APBPeriph2_UARTx, ENABLE);
#if defined	RETARGET_UART_CLKSRC
	RCC_Periph2ClockSourceConfig(RETARGET_UART_Periph,RETARGET_UART_CLKSRC);
#endif
}

int main (void)
{
	RCC_Configuration();
	UART_Driver(9600);
	CaptureTIM_Driver();
	printf("����ͨ��������������\r\n");
	while(1)
	{
		if(flag == 1)
		{
			flag =0 ;
			printf("PWM��������%dms\r\n",capture_value);
		}
	}
}

void CaptureTIM_Handler(void)
{
	if(TIM_GetFlagStatus(CaptureTIM,TIM_FLAG_IC1F)!=RESET)
	{
		capture_value = TIM_GetICxValue(CaptureTIM,TIM_Channel_1);
		TIM_ClearFlag(CaptureTIM,TIM_FLAG_IC1F);
		flag = 1;
	}
}

#elif defined (PT32x002x) || defined (PT32x012x) 

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
* @brief ���ø���TIM���벶��ͨ������
* @param ��
* @retval ��
*/
void CaptureTIM_GPIO_Config(void)
{
	/* ����TIM�ܽŵĸ��ù��� */
	GPIO_DigitalRemapConfig(CaptureTIM_CH_AFIO, CaptureTIM_CH_PIN, CaptureTIM_CH_AFx, ENABLE);
}

/**
* @brief ����TIM������ʽ
* @param ��
* @retval ��
*/
void CaptureTIM_Mode_Config(TIM_TypeDef *TIMx)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;//����һ��TIM_TimeBaseInitTypeDef���͵Ľṹ��
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	TIM_TimeBaseInitStruct.TIM_Prescaler = GetClockFreq(CLKSRC_HCLK)/1000-1;			//����ʱ��Ϊ1ms
	TIM_TimeBaseInitStruct.TIM_AutoReloadValue = 0xFFFF;						   		//����Ϊ���65535ms
	TIM_TimeBaseInitStruct.TIM_Direction = TIM_Direction_Up;					 		//���ϼ���
	TIM_TimeBaseInitStruct.TIM_CenterAlignedMode = TIM_CenterAlignedMode_Disable; 		//���������ļ���ģʽ
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStruct);									//��ʼ��CaptureTIM

	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;							//����Ϊͨ��1
	TIM_ICInitStruct.TIM_ICRiseCapture = TIM_ICRiseCapture_Disable;		 	//��ʹ�����������ز���
	TIM_ICInitStruct.TIM_ICFallCapture = TIM_ICFallCapture_Enable;		 	//ʹ�������½��ز���
	TIM_ICInitStruct.TIM_ICResetCounter = TIM_ICResetCounter_Enable;   		//���ò�������ü�����
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPrescaler_None;					//����Ϊ�޷�Ƶ
	TIM_ICInitStruct.TIM_ICFilter = TIM_ICFilter_8;							//�����˲�
	TIM_ICInitStruct.TIM_ICSource = TIM_ICSource_ICS1;						//ͨ��1����Դѡ��ΪICS1
	TIM_ICInit(TIMx,&TIM_ICInitStruct);									//��ʼ��������
	TIM_ITConfig(TIMx,TIM_IT_IC1I,ENABLE);                         		//ʹ��ͨ��1�����ж�
	
	NVIC_InitStruct.NVIC_IRQChannel = CaptureTIM_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIMx, ENABLE);													//����CaptureTIM
}

/**
* @brief ����TIM����
* @param ��
* @retval ��
*/
void CaptureTIM_Driver()
{
	CaptureTIM_GPIO_Config();
	CaptureTIM_Mode_Config(CaptureTIM);
}

/**
  * @brief  ���ø�������ʱ��.
  * @param  ��
  * @retval ��
  */
void RCC_Configuration(void)
{  
	/* CaptureTIM, UART, GPIO ʱ��ʹ�� */
	RCC_AHBPeriphClockCmd(CaptureTIM_CH_CLOCK|UART_Periph_GPIOx, ENABLE);
	RCC_APBPeriph1ClockCmd(CaptureTIM_Periph_CLOCK, ENABLE);
	RCC_APBPeriph2ClockCmd(APBPeriph2_UARTx, ENABLE);
#if defined	RETARGET_UART_CLKSRC
	RCC_Periph2ClockSourceConfig(RETARGET_UART_Periph,RETARGET_UART_CLKSRC);
#endif
}

int main (void)
{
	RCC_Configuration();
	UART_Driver(9600);
	CaptureTIM_Driver();
	printf("����ͨ��������������\r\n");
	while(1)
	{
		if(flag == 1)
		{
			flag =0 ;
			printf("PWM��������%dms\r\n",capture_value);
		}
	}
}

void CaptureTIM_Handler(void)
{
	if(TIM_GetFlagStatus(CaptureTIM,TIM_FLAG_IC1F)!=RESET)
	{
		capture_value = TIM_GetICxValue(CaptureTIM,TIM_Channel_1);
		flag = 1;
		TIM_UpdateEvent(CaptureTIM);
		TIM_ClearFlag(CaptureTIM,TIM_FLAG_IC1F);
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


