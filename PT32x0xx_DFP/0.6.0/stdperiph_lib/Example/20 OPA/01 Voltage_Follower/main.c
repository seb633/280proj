/******************************************************************************
  * @Example PT32x0xx_main
  * @author  Application development team
  * @version V1.0.0
  * @date    2023/11/07
  * @brief   OPA跟随器,输出端与N端短接
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


/**
* @brief OPA引脚模式功能使能
* @param 无
* @retval 无
*/
void OPA_GPIO_Config(void)
{
	GPIO_AnalogRemapConfig(OPAn_Px_AFIO,OPAn_Px_PIN,ENABLE);    
	GPIO_AnalogRemapConfig(OPAn_Nx_AFIO,OPAn_Nx_PIN,ENABLE);    
	GPIO_AnalogRemapConfig(OPAn_OUT_AFIO,OPAn_OUT_PIN,ENABLE);  
}

/**
  * @brief  配置各个外设时钟.
  * @param  无
  * @retval 无
  */
void RCC_Configuration(void)
{  
	/* GPIO 时钟使能 */
	RCC_AHBPeriphClockCmd(OPAn_Px_CLOCK|OPAn_Nx_CLOCK|OPAn_OUT_CLOCK, ENABLE);
	/* OPA时钟使能 */
	RCC_APBPeriph3ClockCmd(RCC_APBPeriph3_OPA,ENABLE);
}

int main (void)
{
	RCC_Configuration();

	OPA_GPIO_Config();

	OPA_Cmd(OPAn,ENABLE);

    while(1);
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


