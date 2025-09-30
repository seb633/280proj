/******************************************************************************
  * @Example PT32x0xx_main
  * @author  Ӧ�ÿ����Ŷ�
  * @version V1.0.0
  * @date    2023/11/07
  * @brief   LED������
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



/**
  * @brief  ���ø���BreathTimer����Ƚ�ͨ������
  * @param  ��
  * @retval ��
*/
void BreathTimer_GPIO_Config(void)
{
	GPIO_DigitalRemapConfig(BreathLed_AFIO, BreathLed_PIN, BreathLed_AFx, ENABLE);
}

/**
* @brief ����BreathTimer������ʽ
* @param ��
* @retval ��
*/
void BreathTimer_Mode_Config(TIM_TypeDef *TIMx)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;//����һ��TIM_TimeBaseInitTypeDef���͵Ľṹ��
	TIM_OCInitTypeDef TIM_OCInitStruct;//����һ��TIM_OCInitTypeDeff���͵Ľṹ��
	
	TIM_TimeBaseInitStruct.TIM_Prescaler = GetClockFreq(CLKSRC_PCLK)/500000-1;										   	
	TIM_TimeBaseInitStruct.TIM_AutoReloadValue = 1000-1;						   			
	TIM_TimeBaseInitStruct.TIM_Direction = TIM_Direction_Up;					 			//���ϼ���
	TIM_TimeBaseInitStruct.TIM_CenterAlignedMode = TIM_CenterAlignedMode_Disable; 			//���������ļ���ģʽ
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStruct);									 	//��ʼ��BreathTimer
	
	TIM_OCInitStruct.TIM_Channel = BreathTimer_CHx;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_UpOCValue = 0;
	TIM_OCInitStruct.TIM_OCOutput = TIM_OCOutput_Enable;
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Low;
	TIM_OCInitStruct.TIM_OCNOutput = TIM_OCNOutput_Disable;
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Low;
	TIM_OCInit(TIMx,&TIM_OCInitStruct);
	TIM_Cmd(TIMx, ENABLE);					//����BreathTimer
}

/**
  * @brief  ���ø�������ʱ��.
  * @param  ��
  * @retval ��
  */
void RCC_Configuration(void)
{  
	/* BreathTimer, GPIO clocks enable */
	RCC_AHBPeriphClockCmd(BreathLed_GPIO_CLOCK, ENABLE);
	RCC_APBPeriph1ClockCmd(BreathTimer_Periph_CLOCK, ENABLE);
}

/**
  * @brief 	����BreathTimer����
  * @param 	��
  * @retval ��
*/
void BreathTimer_Driver(TIM_TypeDef *TIMx)
{
	BreathTimer_GPIO_Config();
	BreathTimer_Mode_Config(TIMx);
}

int main (void)
{	
	RCC_Configuration();
	BreathTimer_Driver(BreathTimer);
	while(1)
	{
		for(u16 i=0;i<999;i+=1)
		{
			while(!TIM_GetFlagStatus(BreathTimer,TIM_FLAG_ARF));
			TIM_ClearFlag(BreathTimer,TIM_FLAG_ARF);
			TIM_SetOCxValue(BreathTimer,BreathTimer_CHx,i);			
		}
		for(u16 i=0;i<999;i+=1)
		{
			while(!TIM_GetFlagStatus(BreathTimer,TIM_FLAG_ARF));
			TIM_ClearFlag(BreathTimer,TIM_FLAG_ARF);
			TIM_SetOCxValue(BreathTimer,BreathTimer_CHx,999-i);
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
	printf("Wrong parameters value: file %s on line %ld\r\n", file, line);
	/* Infinite loop */
	while (1)
	{
	}
}
#endif


