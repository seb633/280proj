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



u8 flag = 1;

/**
* @brief ����TIM�ĸ�������
* @param ��
* @retval ��
*/
void TIM_GPIO_Config(void)
{
	/* ����TIM-CH1�ܽŵĸ��ù��� */
	GPIO_DigitalRemapConfig(BreakTIM_CH_AFIO, BreakTIM_CH_PIN, BreakTIM_CH_AFx, ENABLE);
	/* ����TIM-BKIN�ܽŵĸ��ù��� */
	GPIO_DigitalRemapConfig(BreakTIM_BKIN_AFIO, BreakTIM_BKIN_PIN, BreakTIM_BKIN_AFx, ENABLE);
	/* ����TIM-BKIN�ڲ����� */
	GPIO_PullUpConfig(BreakTIM_BKIN_GPIO,BreakTIM_BKIN_PIN,ENABLE);
}

/**
* @brief ����TIM������ʽ
* @param ��
* @retval ��
*/
void TIM_Mode_Config(TIM_TypeDef *TIMx)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;//����һ��TIM_TimeBaseInitTypeDef���͵Ľṹ��
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_BKICRInitTypeDef BKICRInit;
	
	TIM_TimeBaseInitStruct.TIM_Prescaler = 64-1;
	TIM_TimeBaseInitStruct.TIM_AutoReloadValue = 1000-1;					   
	TIM_TimeBaseInitStruct.TIM_Direction = TIM_Direction_Up;//���ϼ���
	TIM_TimeBaseInitStruct.TIM_CenterAlignedMode = TIM_CenterAlignedMode_Disable;//���������ļ���ģʽ
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStruct);//��ʼ��TIM

	TIM_OCInitStruct.TIM_Channel = TIM_Channel_1;//����Ƚ�ͨ������Ϊͨ��1
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;//����Ƚ�ģʽѡ��ΪPWM1
	TIM_OCInitStruct.TIM_UpOCValue = 500;//����Ƚ�ֵ����Ϊ500 ��ʱռ�ձ�Ϊ50%
	TIM_OCInitStruct.TIM_OCOutput = TIM_OCOutput_Enable;//ʹ������ȽϹ���
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Low;//��������ȽϿ��е�ƽΪ�͵�ƽ
	TIM_OCInitStruct.TIM_OCNOutput = TIM_OCNOutput_Enable;//ʹ�ܻ�������ȽϹ���
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Low;//���û�������ȽϿ��е�ƽΪ�͵�ƽ
	TIM_OCInit(TIMx,&TIM_OCInitStruct);//��ʼ������ʱ������ȽϹ���		
	
	BKICRInit.TIM_Break = TIM_Break_Enable;//ʹ��ɲ������
	BKICRInit.TIM_BreakInputControl = TIM_BreakInput_OutputIdle;//ɲ��ʱ��ǿ�ƹرն�ʱ��
	BKICRInit.TIM_BreakPolarity = TIM_BreakPolarity_Low;//ɲ������Դ�͵�ƽ��Ч
	BKICRInit.TIM_BreakSource = TIM_BreakSource_TIMxBKIN|TIM_BreakSource_Software;//ɲ������Դѡ���ⲿ�ܽ����������
	BKICRInit.TIM_Breakfilter = TIM_Breakfilter_4;
	TIM_BKICRInit(TIMx,&BKICRInit);	//��ʼ��ɲ��/��������
	TIM_SetDeadTime(TIMx,10);  	//��������ʱ��Ϊ10������
	TIM_Cmd(TIMx,ENABLE);//ʹ��TIM
}

/**
* @brief ����TIM����
* @param ��
* @retval ��
*/
void TIM_Driver(void)
{
	TIM_GPIO_Config();
	TIM_Mode_Config(BreakTIM);
}

/**
* @brief ���ð�������
* @param ��
* @retval ��
*/
void KEY_Driver(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//����һ��GPIO_InitTypeDef���͵Ľṹ��
	/*��ʼ��GPIO*/
	GPIO_InitStructure.GPIO_Pin = USER_KEY_PIN;				//ѡ��Ҫ���Ƶ�GPIO����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_In;			//��������ģʽΪ��������
	GPIO_InitStructure.GPIO_Pull = GPIO_Pull_Up;			//������������
	GPIO_Init(USER_KEY_PORT, &GPIO_InitStructure);			//���ÿ⺯������ʼ��GPIO
}

/**
  * @brief  ���ø�������ʱ��.
  * @param  ��
  * @retval ��
  */
void RCC_Configuration(void)
{  
	/* TIM, GPIOʱ��ʹ�� */
	RCC_AHBPeriphClockCmd(BreakTIM_CH_CLOCK|BreakTIM_BKIN_CLOCK|USER_KEY_GPIO_CLOCK, ENABLE);
	RCC_APBPeriph1ClockCmd(BreakTIM_Periph_CLOCK, ENABLE);
}


int main (void)
{
	RCC_Configuration();
	TIM_Driver();
	KEY_Driver();
	while(1)
	{
		if(GPIO_ReadDataBit(USER_KEY_PORT,USER_KEY_PIN)&&(flag == 0))
		{
			TIM_SoftwareBreakCMD(BreakTIM,DISABLE);
			flag = 1;
		}
		if(!GPIO_ReadDataBit(USER_KEY_PORT,USER_KEY_PIN)&&(flag == 1))
		{
			TIM_SoftwareBreakCMD(BreakTIM,ENABLE);
			flag = 0;
		}
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


