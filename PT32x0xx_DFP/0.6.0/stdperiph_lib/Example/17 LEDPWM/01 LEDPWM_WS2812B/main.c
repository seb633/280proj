/******************************************************************************
  * @Example PT32x0xx_main
  * @author  Ӧ�ÿ����Ŷ�
  * @version V1.0.0
  * @date    2023/11/07
  * @brief   USART���ͻ�ӭ���
  *          
  ******************************************************************************
  * @attention	��������Ϊ��������9600��1λֹͣλ������żУ��
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers. 
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of the content or the  codes within the content.
  *
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  *
  *****************************************************************************/
	
#include "PT32x0xx.h"
#include "PT32x0xx_config.h"

#if defined (PT32x002x) || defined (PT32x012x) 

const char pai[64][8]={
{0,0,0,0,0,0,0,0},	{0,0,0,0,0,0,0,0},	{0,0,0,0,1,0,0,0},	{0,0,0,1,0,0,0,0},	{1,1,1,1,1,0,0,0},	{0,0,0,1,0,0,0,0},	{0,0,0,0,1,0,0,0},	{0,0,0,0,0,0,0,0},	{0,0,0,0,0,0,0,0},	{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},	{0,0,0,0,0,0,0,0},	{0,0,0,0,0,0,0,0},	{0,0,0,0,0,1,1,1},	{0,0,1,1,1,0,0,0},	{0,0,0,0,0,0,1,1},	{0,0,0,0,0,0,0,0},	{0,0,0,0,0,0,0,0},	{0,0,0,0,0,0,0,0},	{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},	{0,0,0,0,1,1,1,1},	{0,0,0,0,1,0,0,0},	{0,0,0,1,0,0,0,0},	{0,0,0,0,1,0,0,0},	{0,0,0,1,0,0,0,0},	{1,1,1,1,1,0,0,0},	{0,0,0,1,0,0,0,0},	{0,0,0,0,0,0,0,0},	{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},	{0,0,0,0,0,0,0,0},	
{0,0,0,0,0,0,0,0},	{0,0,0,0,0,0,0,0},	
{0,0,0,0,0,0,0,0},	{0,0,0,0,0,0,0,0},	{0,0,0,0,0,1,0,0},	{0,0,1,1,1,1,1,1},	{1,0,0,0,0,1,0,0},	{0,0,0,0,0,0,0,1},	{1,0,0,0,0,0,0,0},	{0,0,0,0,0,0,0,1},	{0,0,0,0,0,0,0,0},	{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},	{0,0,0,0,0,0,0,0},	{0,0,0,0,0,1,0,0},	{0,0,1,1,1,1,0,0},	{1,1,0,0,0,1,0,0},	{0,0,0,0,0,0,1,0},	{0,1,0,0,0,0,0,0},	{0,0,1,0,0,1,1,1},	{0,0,0,1,1,1,0,0},	{0,0,1,0,0,0,0,0},
{0,0,0,0,0,0,0,0},	{0,0,0,0,0,0,0,0},	{0,0,0,0,0,0,0,0},	{0,0,1,0,0,0,0,0},	{0,0,0,0,0,1,0,0},	{0,0,1,1,1,1,1,1},	{0,0,0,0,0,1,0,0},	{0,0,1,0,0,0,0,0},	{0,0,0,0,0,0,0,0},	{0,0,0,0,0,0,0,0}							
};  //512����


u32 color_cnt = 0;


/**
* @brief �����ʱ����
* @param ��
* @retval ��
*/
void Software_Delay(void)
{
	u8 i, j,x;
	for(i=0; i<200; i++)
		for(j=0; j<200; j++)
			for(x=0; x<20; x++);
}

/**
* @brief ����LEDPWM_OUT�ĸ�������
* @param ��
* @retval ��
*/
void LEDPWM_GPIO_Config(void)
{
	/* ����UART�ܽŵĸ��ù��� */
	GPIO_DigitalRemapConfig(LEDPWM_OUT_AFIO, LEDPWM_OUT_PIN, LEDPWM_OUT_AFx, ENABLE);		
}

/**
* @brief ����LEDPWM�Ĺ���ģʽ
* @param ��
* @retval ��
*/
void LEDPWM_Mode_Config(void)
{
	LEDPWM_InitTypeDef LEDPWMInit;
	NVIC_InitTypeDef NVIC_InitStruct;

	/*��ʼ��LEDPWM*/
	LEDPWM_StructInit(&LEDPWMInit);
	LEDPWMInit.LEDPWM_Code0Duty = 0x0D;  		//291ns�ߵ�ƽ
	LEDPWMInit.LEDPWM_Code1Duty = 0x2E;			//979ns�ߵ�ƽ
	LEDPWMInit.LEDPWM_Cycle0 = 0x3B;  			//60*1/48M=1.25us
	LEDPWMInit.LEDPWM_Cycle1 = 0x3B;  			//60*1/48M=1.25us
	LEDPWM_Init(LEDPWM, &LEDPWMInit);
	/*�����ж�����������NVIC*/
	LEDPWM_ITConfig(LEDPWM,LEDPWM_IT_TXE,ENABLE);
	NVIC_InitStruct.NVIC_IRQChannel = LEDPWM_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	/*ʹ��LEDPWM*/
	LEDPWM_Cmd(LEDPWM, ENABLE);
}

/**
* @brief LEDPWM��������
* @param ��
* @retval ��
*/
void LEDPWM_Driver(void)
{
	LEDPWM_GPIO_Config();
	LEDPWM_Mode_Config();
}

/**
  * @brief  ���ø�������ʱ��.
  * @param  ��
  * @retval ��
  */
void RCC_Configuration(void)
{  
	/* LEDPWM, GPIOʱ��ʹ�� */
	RCC_AHBPeriphClockCmd(LEDPWM_OUT_CLOCK, ENABLE);
	RCC_APBPeriph3ClockCmd(RCC_APBPeriph3_LEDPWM,ENABLE); 
}

int main (void)
{
	RCC_Configuration();
	LEDPWM_Driver();
		
    while(1)
	{		
	}
}

/**
* @brief LEDPWM�жϷ�����
* @param None
* @retval None
*/
void LEDPWM_Handler(void)
{
	static u16 cnt = 0;
	u32 color[3] = {0x00000010,0x1000,0x100000};
	
	/*��λ�Ĵ���ʣ��ռ��24bits*/
	if(LEDPWM_GetFlagStatus(LEDPWM, LEDPWM_FLAG_TXE))
	{
		if(cnt<512)
		{
			/*���24λLEDPWM�ź�*/
			LEDPWM->DR = color[color_cnt]*pai[cnt/8][cnt%8];	
			cnt++;
		}
		else
		{
			//�����λ��
			Software_Delay();
			/*��λ�뷢�����*/			
			if(cnt >= 512)
			{
				cnt = 0;
				/*��ɫ*/
				color_cnt++;	
				if(color_cnt >= 3)	
				{
					color_cnt = 0;
				}
				LEDPWM->DR = color[color_cnt]*pai[cnt/8][cnt%8];		
				cnt++;
			}		
		}
		LEDPWM_ClearFlag(LEDPWM, LEDPWM_FLAG_TXE);
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
	printf("Wrong parameters value: file %s on line %ld\r\n", file, line);
	/* Infinite loop */
	while (1)
	{
	}
}
#endif


