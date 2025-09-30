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

u8 PVD_IntFlag=0;

/**
* @brief ����LED��GPIO����
* @param ��
* @retval ��
*/
void LED_Driver(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//����һ��GPIO_InitTypeDef���͵Ľṹ��
	GPIO_InitStructure.GPIO_Pin = LED_RED_PIN;	//ѡ��Ҫ���Ƶ�GPIO����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OutPP;		//��������ģʽΪͨ���������
	GPIO_InitStructure.GPIO_Pull = GPIO_Pull_Up;	//������������
	GPIO_Init(LED_RED_PORT, &GPIO_InitStructure);	//���ÿ⺯������ʼ��GPIO
	GPIO_SetBits(LED_RED_PORT, LED_RED_PIN);			//���ÿ⺯��������GPIO״̬
}

/**
* @brief PVD�ж���������
* @param ��
* @retval ��
*/
void PVD_Int_Driver(void)
{
	RCC_ClearResetFlag(RCC_FLAG_POR);
	RCC_ClearResetFlag(RCC_FLAG_PVD);
	/* ��ֹPVD��λ */
	RCC_ResetConfig(RCC_ResetEnable_PVD,DISABLE);	
	PWR_PVDLevelConfig(PWR_PVDLevel_5);
	NVIC_EnableIRQ(PVD_IRQn);
	PWR_PVDCmd(ENABLE);
}

/**
  * @brief  ���ø�������ʱ��.
  * @param  ��
  * @retval ��
  */
void RCC_Configuration(void)
{  
	/* GPIOʱ��ʹ�� */
	RCC_AHBPeriphClockCmd(LED_RED_GPIO_CLOCK, ENABLE);
}

int main (void)
{
	RCC_Configuration();
	LED_Driver();
	PVD_Int_Driver();
	while(1)
	{
		if(PVD_IntFlag == 1)
		{
			/*����PVD�ж�*/
			GPIO_ResetBits(LED_RED_PORT, LED_RED_PIN);
		}
		else
		{
			/*δ����PVD�ж�*/
			GPIO_SetBits(LED_RED_PORT, LED_RED_PIN);
		}
	}
}

/**
* @brief PVD�жϷ�����
* @param None
* @retval None
*/
void PVD_Handler(void)
{
	PVD_IntFlag = 1;
	PWR_PVDCmd(DISABLE);
}

#elif defined (PT32x012x) || defined (PT32x002x)

u8 PVD_IntFlag = 0;

/**
* @brief ����LED��GPIO����
* @param ��
* @retval ��
*/
void LED_Driver(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//����һ��GPIO_InitTypeDef���͵Ľṹ��
	GPIO_InitStructure.GPIO_Pin = LED_RED_PIN;	//ѡ��Ҫ���Ƶ�GPIO����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OutPP;		//��������ģʽΪͨ���������
	GPIO_InitStructure.GPIO_Pull = GPIO_Pull_Up;	//������������
	GPIO_Init(LED_RED_PORT, &GPIO_InitStructure);	//���ÿ⺯������ʼ��GPIO
	GPIO_SetBits(LED_RED_PORT, LED_RED_PIN);			//���ÿ⺯��������GPIO״̬
}

/**
* @brief PVD�ж���������
* @param ��
* @retval ��
*/
void PVD_Int_Driver(void)
{
	RCC_ClearResetFlag(RCC_FLAG_POR);
	RCC_ClearResetFlag(RCC_FLAG_PVD);
	/* ��ֹPVD��λ */
	RCC_ResetConfig(RCC_ResetEnable_PVD,DISABLE);	
	PWR_PVDLevelConfig(PWR_PVDLevel_6);
	NVIC_EnableIRQ(PVD_IRQn);
}

/**
  * @brief  ���ø�������ʱ��.
  * @param  ��
  * @retval ��
  */
void RCC_Configuration(void)
{  
	/* GPIOA, GPIOB ʱ��ʹ�� */
	RCC_AHBPeriphClockCmd(LED_RED_GPIO_CLOCK, ENABLE);
}

int main (void)
{
	RCC_Configuration();
	LED_Driver();
	PVD_Int_Driver();
	while(1)
	{
		if(PVD_IntFlag == 1)
		{
			/*����PVD�ж�*/
			GPIO_ResetBits(LED_RED_PORT, LED_RED_PIN);
		}
		else
		{
			/*δ����PVD�ж�*/
			GPIO_SetBits(LED_RED_PORT, LED_RED_PIN);
		}
	}
}

/**
* @brief PVD�жϷ�����
* @param None
* @retval None
*/
void PVD_Handler(void)
{
	PVD_IntFlag = 1;
	NVIC_DisableIRQ(PVD_IRQn);
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


