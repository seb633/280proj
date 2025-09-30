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
* @brief 配置TIM的复用引脚
* @param 无
* @retval 无
*/
void TIM_GPIO_Config(void)
{
	/* 配置TIM-CH1管脚的复用功能 */
	GPIO_DigitalRemapConfig(BreakTIM_CH_AFIO, BreakTIM_CH_PIN, BreakTIM_CH_AFx, ENABLE);
	/* 配置TIM-BKIN管脚的复用功能 */
	GPIO_DigitalRemapConfig(BreakTIM_BKIN_AFIO, BreakTIM_BKIN_PIN, BreakTIM_BKIN_AFx, ENABLE);
	/* 配置TIM-BKIN内部上拉 */
	GPIO_PullUpConfig(BreakTIM_BKIN_GPIO,BreakTIM_BKIN_PIN,ENABLE);
}

/**
* @brief 配置TIM工作方式
* @param 无
* @retval 无
*/
void TIM_Mode_Config(TIM_TypeDef *TIMx)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;//定义一个TIM_TimeBaseInitTypeDef类型的结构体
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_BKICRInitTypeDef BKICRInit;
	
	TIM_TimeBaseInitStruct.TIM_Prescaler = 64-1;
	TIM_TimeBaseInitStruct.TIM_AutoReloadValue = 1000-1;					   
	TIM_TimeBaseInitStruct.TIM_Direction = TIM_Direction_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_CenterAlignedMode = TIM_CenterAlignedMode_Disable;//不开启中心计数模式
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStruct);//初始化TIM

	TIM_OCInitStruct.TIM_Channel = TIM_Channel_1;//输出比较通道设置为通道1
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;//输出比较模式选择为PWM1
	TIM_OCInitStruct.TIM_UpOCValue = 500;//输出比较值设置为500 此时占空比为50%
	TIM_OCInitStruct.TIM_OCOutput = TIM_OCOutput_Enable;//使能输出比较功能
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Low;//设置输出比较空闲电平为低电平
	TIM_OCInitStruct.TIM_OCNOutput = TIM_OCNOutput_Enable;//使能互补输出比较功能
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Low;//设置互补输出比较空闲电平为低电平
	TIM_OCInit(TIMx,&TIM_OCInitStruct);//初始化主定时器输出比较功能		
	
	BKICRInit.TIM_Break = TIM_Break_Enable;//使能刹车功能
	BKICRInit.TIM_BreakInputControl = TIM_BreakInput_OutputIdle;//刹车时不强制关闭定时器
	BKICRInit.TIM_BreakPolarity = TIM_BreakPolarity_Low;//刹车输入源低电平有效
	BKICRInit.TIM_BreakSource = TIM_BreakSource_TIMxBKIN|TIM_BreakSource_Software;//刹车输入源选择外部管脚与软件输入
	BKICRInit.TIM_Breakfilter = TIM_Breakfilter_4;
	TIM_BKICRInit(TIMx,&BKICRInit);	//初始化刹车/死区功能
	TIM_SetDeadTime(TIMx,10);  	//配置死区时间为10个计数
	TIM_Cmd(TIMx,ENABLE);//使能TIM
}

/**
* @brief 配置TIM函数
* @param 无
* @retval 无
*/
void TIM_Driver(void)
{
	TIM_GPIO_Config();
	TIM_Mode_Config(BreakTIM);
}

/**
* @brief 配置按键函数
* @param 无
* @retval 无
*/
void KEY_Driver(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//定义一个GPIO_InitTypeDef类型的结构体
	/*初始化GPIO*/
	GPIO_InitStructure.GPIO_Pin = USER_KEY_PIN;				//选择要控制的GPIO引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_In;			//设置引脚模式为数字输入
	GPIO_InitStructure.GPIO_Pull = GPIO_Pull_Up;			//设置上拉电阻
	GPIO_Init(USER_KEY_PORT, &GPIO_InitStructure);			//调用库函数，初始化GPIO
}

/**
  * @brief  配置各个外设时钟.
  * @param  无
  * @retval 无
  */
void RCC_Configuration(void)
{  
	/* TIM, GPIO时钟使能 */
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


