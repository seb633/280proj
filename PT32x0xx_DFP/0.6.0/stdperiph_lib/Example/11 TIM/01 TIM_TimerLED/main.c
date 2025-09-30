/******************************************************************************
  * @Example PT32x0xx_main
  * @author  Ӧ�ÿ����Ŷ�
  * @version V1.0.0
  * @date    2023/11/07
  * @brief   ����ÿ��1����˸һ��
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




u8 TIM_IT_FLAG = 0;

/**
  * @brief 	LED��������
  * @param 	��
  * @retval ��
*/
void LED_Driver(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;				//����һ��GPIO_InitTypeDef���͵Ľṹ��
	GPIO_InitStructure.GPIO_Pin = LED_BLUE_PIN;			//ѡ��Ҫ���Ƶ�GPIO����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OutPP;		//��������ģʽΪͨ���������
	GPIO_InitStructure.GPIO_Pull = GPIO_Pull_Up;		//��������
	GPIO_Init(LED_BLUE_PORT, &GPIO_InitStructure);		//���ÿ⺯������ʼ��GPIO
		
	GPIO_SetBits(LED_BLUE_PORT, LED_BLUE_PIN);			//����GPIO״̬
}

/**
  * @brief 	����TIMER_LED������ʽ
  * @param 	��
  * @retval ��
*/
void TIMER_LED_Diver(TIM_TypeDef *TIMx)
{
	NVIC_InitTypeDef NVIC_InitStruct;							//����һ��NVIC_InitTypeDef���͵Ľṹ��
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;			//����һ��NVIC_InitTypeDef���͵Ľṹ��
	
	TIM_TimeBaseInitStruct.TIM_Prescaler = GetClockFreq(CLKSRC_PCLK)/10000-1;				
	TIM_TimeBaseInitStruct.TIM_AutoReloadValue = 10000-1;		//0.01MHZ/10000=1HZ
	TIM_TimeBaseInitStruct.TIM_Direction = TIM_Direction_Up;	//���ϼ���
	TIM_TimeBaseInitStruct.TIM_CenterAlignedMode = TIM_CenterAlignedMode_Disable;
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStruct);			//��ʼ��TIM21
	
	TIM_ITConfig(TIMx,TIM_IT_ARI,ENABLE);						//��ʱ�жϳ�ʼ��
	
	NVIC_InitStruct.NVIC_IRQChannel = TimerLed_IRQn;			//��ʱ�ж�Դ����
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x00;			//�ж����ȼ�����
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			//ʹ��NVIC������
	NVIC_Init(&NVIC_InitStruct);							//��ʼ��NVIC	
	
	TIM_Cmd(TIMx, ENABLE);									//����TimerLed
}

/**
  * @brief  ���ø�������ʱ��.
  * @param  ��
  * @retval ��
  */
void RCC_Configuration(void)
{  
	/* TimerLed, GPIO clocks enable */
	RCC_AHBPeriphClockCmd(LED_BLUE_GPIO_CLOCK, ENABLE);
	RCC_APBPeriph1ClockCmd(TimerLed_Periph_CLOCK, ENABLE);
}


int main (void)
{
	RCC_Configuration();
	LED_Driver();
	TIMER_LED_Diver(TimerLed);
	while(1)
	{
		if(TIM_IT_FLAG)	//��ȡ�жϱ�־�Ƿ�����
		{
			GPIO_ResetBits(LED_BLUE_PORT,LED_BLUE_PIN);
		}
		else
		{
			GPIO_SetBits(LED_BLUE_PORT,LED_BLUE_PIN);
		} 
	}
}

void TimerLed_Handler(void)
{
	if(TIM_GetFlagStatus(TimerLed,TIM_FLAG_ARF) != RESET)
	{
		TIM_ClearFlag(TimerLed,TIM_FLAG_ARF);
		TIM_IT_FLAG = ~TIM_IT_FLAG;
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
	printf("Wrong parameters value: file %s on line %ld\r\n", file, line);
	/* Infinite loop */
	while (1)
	{
	}
}
#endif


