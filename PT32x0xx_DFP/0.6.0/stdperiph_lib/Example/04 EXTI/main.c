/******************************************************************************
  * @Example PT32x0xx_main
  * @author  Application development team
  * @version V1.0.0
  * @date    2023/11/07
  * @brief   ����USER_KEY,�ı����Ƶ�����
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



u8 KET_IntFlag = 0;

/**
* @brief ����LED��GPIO����
* @param ��
* @retval ��
*/
void LED_Driver(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;				//����һ��GPIO_InitTypeDef���͵Ľṹ��
	GPIO_InitStructure.GPIO_Pin = LED_BLUE_PIN;			//ѡ��Ҫ���Ƶ�GPIO����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OutPP;		//��������ģʽΪͨ���������
	GPIO_InitStructure.GPIO_Pull = GPIO_Pull_Down;		//������������
	GPIO_Init(LED_BLUE_PORT, &GPIO_InitStructure);		//���ÿ⺯������ʼ��GPIO
	
	GPIO_SetBits(LED_BLUE_PORT, LED_BLUE_PIN);			//����GPIO״̬
}

/**
* @brief ���ð����ж�
* @param ��
* @retval ��
*/
void KEY_Int_Driver(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;					//����һ��NVIC_InitTypeDef���͵Ľṹ��
	GPIO_InitTypeDef GPIO_InitStructure;				//����һ��GPIO_InitTypeDef���͵Ľṹ��
	
	/*��ʼ��GPIO*/
	GPIO_InitStructure.GPIO_Pin = USER_KEY_PIN;			//ѡ��Ҫ���Ƶ�GPIO����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_In;		//��������ģʽΪ��������
	GPIO_InitStructure.GPIO_Pull = GPIO_Pull_Up;		//������������
	GPIO_Init(GPIOB, &GPIO_InitStructure);				//���ÿ⺯������ʼ��GPIO
	
	/*�˿��жϴ�������ѡ�������غ��½���*/
	EXTI_TriggerTypeConfig(USER_KEY_EXTI,USER_KEY_PIN, EXTI_Trigger_RisingFalling);
	EXTI_ITConfig(USER_KEY_EXTI,USER_KEY_PIN,ENABLE);

	/*����NVIC������*/
	NVIC_InitStruct.NVIC_IRQChannel = USER_KEY_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x00;
	NVIC_Init(&NVIC_InitStruct);
}

/**
  * @brief  ���ø�������ʱ��.
  * @param  ��
  * @retval ��
  */
void RCC_Configuration(void)
{  
	/* GPIO ʱ��ʹ�� */
	RCC_AHBPeriphClockCmd(LED_BLUE_GPIO_CLOCK|USER_KEY_GPIO_CLOCK, ENABLE);
}

int main (void)
{
	RCC_Configuration();
	LED_Driver();
	KEY_Int_Driver();
	while(1)
	{
		if(KET_IntFlag)
		{
			GPIO_ReverseBits(LED_BLUE_PORT, LED_BLUE_PIN);
			KET_IntFlag = 0;
		}
	}
}

/**
* @brief KEY�жϷ�����
* @param None
* @retval None
*/
void USER_KEY_Handler(void)
{
	if(EXTI_GetITStatus(USER_KEY_EXTI,USER_KEY_PIN)!=RESET)
	{
		EXTI_ClearFlag(USER_KEY_EXTI,USER_KEY_PIN);	//����жϱ�־λ;
		KET_IntFlag = 1;
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


