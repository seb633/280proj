/******************************************************************************
  * @Example PT32x0xx_main
  * @author  Ӧ�ÿ����Ŷ�
  * @version V1.0.0
  * @date    2023/11/07
  * @brief   Systick��ʱ1s��˸һ���̵�
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

u32 TimingDelay;


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
* @brief LED��������
* @param ��
* @retval ��
*/
void LED_Driver(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;				//����һ��GPIO_InitTypeDef���͵Ľṹ��
	GPIO_InitStructure.GPIO_Pin = LED_GREEN_PIN;		//ѡ��Ҫ���Ƶ�GPIO����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OutPP;		//��������ģʽΪͨ���������
	GPIO_InitStructure.GPIO_Pull = GPIO_Pull_Up;		//����ģʽ
	GPIO_Init(LED_GREEN_PORT, &GPIO_InitStructure);		//���ÿ⺯������ʼ��GPIO
		
	GPIO_SetBits(LED_GREEN_PORT, LED_GREEN_PIN);		//����GPIO״̬
}

/**
* @brief SysTick��������
* @param ��
* @retval ��
*/
void Systick_Driver(void)
{
	u32 HCLK = 0;
	HCLK = GetClockFreq(CLKSRC_HCLK);		//���HCLKʱ��
	if (SysTick_Config(HCLK / 100000))		//10us�ж�һ��
	{ 
		while (1);
	}
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;//�ر�SYSTIC������  
}

/**
* @brief ��ȡ���ĳ�����SysTick�жϺ��� SysTick_Handler()����
* @param ��
* @retval ��
*/
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}

/**
* @brief ���ú��뼶��ʱʱ��(10usΪ����)
* @param ��
* @retval ��
*/
void Delay_ms(u32 nTime)
{ 
	TimingDelay = nTime*100;	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//ʹ��SYSTIC������  
	while(TimingDelay != 0);
}

/**
  * @brief  ���ø�������ʱ��.
  * @param  ��
  * @retval ��
  */
void RCC_Configuration(void)
{  
	/* GPIOʱ��ʹ�� */
	RCC_AHBPeriphClockCmd(UART_Periph_GPIOx, ENABLE);
}

int main (void)
{
	RCC_Configuration();
	LED_Driver();
	Systick_Driver();
	while(1)
	{
		GPIO_ReverseBits(LED_GREEN_PORT,LED_GREEN_PIN);
		Delay_ms(1000);
	}
}

/**
* @brief SysTick�жϷ�����
* @param None
* @retval None
*/
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
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


