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
u16 ADC_Timeout = 0;
u16 ADC_Data[6];
u32 interrupt1 = 0,interrupt2 = 0;



#if defined (PT32G031x) || defined (PTM280x)

/**
* @brief 配置UART的复用引脚
* @param 无
* @retval 无
*/
void UART_GPIO_Config(void)
{ 
	GPIO_DigitalRemapConfig(UART_TX_PORT, UART_TX_PIN, UART_TX_AFIO,ENABLE);	//UART_TX数字功能复用
	GPIO_DigitalRemapConfig(UART_RX_PORT, UART_RX_PIN, UART_RX_AFIO,ENABLE);	//UART_RX数字功能复用
}

/**
* @brief 配置UART的工作模式
* @param 无
* @retval 无
*/
void UART_Mode_Config(u32 Baudrate)
{
	UART_InitTypeDef UART_InitStruct;

	/*初始化UART*/
	UART_InitStruct.UART_BaudRate = Baudrate;
	UART_InitStruct.UART_DataLength = UART_DataLength_8;
	UART_InitStruct.UART_StopBitLength = UART_StopBitLength_1;
	UART_InitStruct.UART_ParityMode = UART_Parity_None;
	UART_InitStruct.UART_Receiver = UART_Receiver_Enable;
	UART_InitStruct.UART_SampleRate = RETARGET_UART_SampleRate;
	UART_Init(RETARGET_UART_PORT, &UART_InitStruct);
	/*使能UART*/
	UART_Cmd(RETARGET_UART_PORT, ENABLE);
}

/**
* @brief UART驱动函数
* @param 无
* @retval 无
*/

void UART_Driver(u32 Baudrate)
{
	UART_Mode_Config(Baudrate);
	UART_GPIO_Config();
}

/**
* @brief TriggerTimer驱动函数
* @param 无
* @retval 无
*/
void TriggerTimer_Driver(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;//定义一个TIM_TimeBaseInitTypeDef类型的结构体
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	TIM_TimeBaseInitStruct.TIM_Prescaler = 64000-1;
	TIM_TimeBaseInitStruct.TIM_AutoReloadValue = 2000-1;					   
	TIM_TimeBaseInitStruct.TIM_Direction = TIM_Direction_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_CenterAlignedMode = TIM_CenterAlignedMode_Disable;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);//初始化TIM1
	
	TIM_OCInitStruct.TIM_Channel = TIM_Channel_1;//输出比较通道设置为通道1
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;//输出比较模式选择为PWM1
	TIM_OCInitStruct.TIM_UpOCValue = 1000;//向上输出比较值设置 
	TIM_OCInitStruct.TIM_OCOutput = TIM_OCOutput_Enable;//使能输出比较功能
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Low;//设置输出比较空闲电平为低电平
	TIM_OCInitStruct.TIM_OCNOutput = TIM_OCNOutput_Enable;//使能互补输出比较功能
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Low;//设置互补输出比较空闲电平为低电平
	TIM_OCInit(TIM1,&TIM_OCInitStruct);//初始化主定时器输出比较功能	
	TIM_SetDeadTime(TIM1,2);
	TIM_ADCTrigger(TIM1,TIM_ADCTrigger_OC1UE,TIM_ScanMode_Regular,ENABLE);   //ARR匹配触发常规ADC转换
	TIM_ADCTrigger(TIM1,TIM_ADCTrigger_UOAE,TIM_ScanMode_Inject,ENABLE);	//OC1匹配触发注入ADC转换
	TIM_Cmd(TIM1,ENABLE);//使能TIM1	
}

/**
* @brief 等待ADC启动
* @param 无
* @retval 无
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
* @brief ADC驱动函数
* @param 无
* @retval 无
*/
void ADC_Driver(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	/*ADC_IN0模拟功能使能*/
	GPIO_AnalogRemapConfig(ADCn_PIN_AFIO,ADCn_PIN, ENABLE);
	
	/*ADC_EOS JEOS中断使能*/
	NVIC_InitStruct.NVIC_IRQChannel = ADCn_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
	ADC_ITConfig(ADCn,ADC_IT_EOS|ADC_IT_JEOS,ENABLE);
	/*ADC初始化*/
	ADC_InitStruct.ADC_Channel = ADC_Channel_62;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Single;
	ADC_InitStruct.ADC_Prescaler = 5;
	ADC_InitStruct.ADC_RVSPS = ADC_RVSPS_VDDA;
	ADC_InitStruct.ADC_SampleTime = 10;
	ADC_InitStruct.ADC_ChannelSetupTime = 100;
	ADC_Init(ADCn,&ADC_InitStruct);
	ADC_RegularTriggerSource(ADCn,ADC_RegularTriggerSource_Timer);
	ADC_RegularTimerTriggerSource(ADCn,ADC_RegularTimerTriggerSource_TIM1);
	/*常规扫描使能*/
	ADC_RegularScanCmd(ADCn,ENABLE);
	ADC_RSCNTConfig(ADCn,3);
	ADC_ScanChannelConfig(ADCn,0,ADC_Channel_59);   //扫描通道0转换GND
	ADC_ScanChannelConfig(ADCn,1,ADC_Channel_62);   //扫描通道0转换BG1V0
	ADC_ScanChannelConfig(ADCn,2,ADCn_Channelx);   //扫描通道0转换BG1V2
	/*注入扫描使能*/
	ADC_InjectedScanCmd(ADCn,ENABLE);
	ADC_JSCNTConfig(ADCn,2);
	ADC_InjectedTriggerSource(ADCn,ADC_InjectedTriggerSource_Timer);
	ADC_InjectedTimerTriggerSource(ADCn,ADC_InjectedTimerTriggerSource_TIM1);
	ADC_InjectScanChannelConfig(ADCn,0,ADCn_Channelx);   //扫描通道0转换
	ADC_InjectScanChannelConfig(ADCn,1,ADC_Channel_62);   //扫描通道1转换BG1V0
	ADC_InjectScanChannelConfig(ADCn,2,ADC_Channel_59);   //扫描通道2转换GND	
	ADC_ConversionControlConfig(ADCn,ADC_ConversionControl_4);
	ADC_SingleConversionCountControl(ADCn,0);
	/*ADC使能*/
	ADC_Cmd(ADCn,ENABLE);
	ADC_Wait_Status(ADC_FLAG_RDY);
}

/**
* @brief 软件延时函数
* @param 无
* @retval 无
*/
void Software_Delay(u32 t)
{
	while(t--);
}

/**
  * @brief  配置各个外设时钟.
  * @param  无
  * @retval 无
  */
void RCC_Configuration(void)
{  
	/* TIM1, ADC, UART, GPIO 时钟使能 */
	RCC_APBPeriph1ClockCmd(RCC_APBPeriph1_TIM1, ENABLE);
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
	UART_Driver(9600);

	printf("ADC常规扫描和注入扫描开始\r\n");
	ADC_Driver();
	TriggerTimer_Driver();

	while(1)
	{
		if(interrupt1 ==1 && interrupt2==1)
		{
			printf("\r\n常规：\r\n");
			printf("%d\r\n",ADC_Data[0]);
			printf("%d\r\n",ADC_Data[1]);
			printf("%d\r\n",ADC_Data[2]);
			
			printf("\r\n注入：\r\n");
			printf("%d\r\n",ADC_Data[3]);
			printf("%d\r\n",ADC_Data[4]);
			printf("%d\r\n",ADC_Data[5]);
			interrupt1 = 0;
			interrupt2 = 0;
		}
	}	
}


/**
* @brief ADC中断服务函数
* @param None
* @retval None
*/
void ADCn_Handler(void)
{
	if(ADC_GetFlagStatus(ADCn,ADC_FLAG_EOS))
	{
		interrupt1++;
		ADC_Data[0] = ADC_GetRegularScanConversionValue(ADCn,0);
		ADC_Data[1] = ADC_GetRegularScanConversionValue(ADCn,1);
		ADC_Data[2] = ADC_GetRegularScanConversionValue(ADCn,2);
	}
	else if(ADC_GetFlagStatus(ADCn,ADC_FLAG_JEOS))
	{
		interrupt2++;
		ADC_Data[3] = ADC_GetInjectScanConversionValue(ADCn,0);
		ADC_Data[4] = ADC_GetInjectScanConversionValue(ADCn,1);
		ADC_Data[5] = ADC_GetInjectScanConversionValue(ADCn,2);		
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


