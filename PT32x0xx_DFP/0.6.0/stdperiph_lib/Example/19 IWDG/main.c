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
* @brief IWDG��������
* @param ��
* @retval ��
*/
void IWDG_Driver(u32 reloadvalue)
{	
	IWDG_LockCmd(IWDG,IWDG_LockKey_Unlock);//����IWDG�Ĵ���
	IWDG_SetReload(IWDG, reloadvalue);//������װ�ؼĴ���ֵ
	IWDG_ReloadCounter(IWDG);//ι��(���ؼ�����)
	RCC_ResetConfig(RCC_ResetEnable_IWDG, ENABLE);//���Ź���λʹ��
	IWDG_Cmd(IWDG, ENABLE);//ʹ��IWDG
	IWDG_LockCmd(IWDG, IWDG_LockKey_Lock);	//��סIWDG�Ĵ���
}

/**
* @brief IWDGι������
* @param ��
* @retval ��
*/
void IWDG_Feed(void)
{
	IWDG_LockCmd(IWDG, IWDG_LockKey_Unlock);//����IWDG�Ĵ���
	IWDG_ReloadCounter(IWDG);//ι��(���ؼ�����)
	IWDG_LockCmd(IWDG, IWDG_LockKey_Lock);//��סIWDG�Ĵ���	
}

/**
  * @brief  ���ø�������ʱ��.
  * @param  ��
  * @retval ��
  */
void RCC_Configuration(void)
{  
	/* UART0, IWDG, GPIO ʱ��ʹ�� */
    RCC_AHBPeriphClockCmd(UART_Periph_GPIOx, ENABLE);
	RCC_APBPeriph2ClockCmd(APBPeriph2_UARTx, ENABLE);
	RCC_APBPeriph4ClockCmd(RCC_APBPeriph4_IWDG, ENABLE);	
#if defined	RETARGET_UART_CLKSRC
	RCC_Periph2ClockSourceConfig(RETARGET_UART_Periph,RETARGET_UART_CLKSRC);
#endif	
}

int main (void)
{
	RCC_Configuration();
	UART_Driver(9600);
	IWDG_Driver(32678);	//timeout=1s	
	if(RCC_GetResetFlagStatus(RCC_FLAG_IWDG)!=RESET)//�ж��Ƿ����������Ź���λ
	{
		RCC_ClearResetFlag(RCC_FLAG_IWDG);//�����־λ
		printf("�������Ź�������λ������Ϣ�ɳ���λ����ӡ\r\n");
	}
	while(1)
	{
//		IWDG_Feed();//ι��
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


