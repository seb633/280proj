/**
	******************************************************************************
  * @file    PT32x0xx_rcc.c
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the RCC peripheral:
  *           + Initialization and Configuration
  *           + Interrupts and flags management
  *
  *******************************************************************************
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of
  * the content or the codes within the content.
  *
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "PT32x0xx_rcc.h"

/** @defgroup RCC
  * @brief RCC driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Forces or releases High Speed APB peripheral 1 reset.
  * @param  RCC_APB1Periph: specifies the APB peripheral 1 to reset.
  *			This parameter can be any combination of the following values:
  *			@arg RCC_APBPeriph1_TIM1
  *			@arg RCC_APBPeriph1_TIM2
  *			@arg RCC_APBPeriph1_TIM3
  *			@arg RCC_APBPeriph1_TIM4
  *			@arg RCC_APBPeriph1_TIM5
  *			@arg RCC_APBPeriph1_TIM6
  *			@arg RCC_APBPeriph1_TIM7
  *			@arg RCC_APBPeriph1_TIM8
  *			@arg RCC_APBPeriph1_LPTIM
  *			@arg RCC_APBPeriph1_TIM20
  *			@arg RCC_APBPeriph1_TIM21
  *			@arg RCC_APBPeriph1_TIM22
  * @param  NewState: new state of the specified peripheral reset.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APBPeriph1ResetCmd(u32 RCC_APBPeriph, FunctionalState NewState)
{
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
		return;
#else
	/* Check the parameters */
	assert_param(IS_RCC_APB_PERIPH1(RCC_APBPeriph));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if(NewState != DISABLE)
	{
		RCC->APBRSTR1 |= RCC_APBPeriph;
	}
	else
	{
		RCC->APBRSTR1 &= ~RCC_APBPeriph;
	}
#endif
}

/**
  * @brief  Forces or releases High Speed APB peripheral 2 reset.
  * @param  RCC_APB2Periph: specifies the APB peripheral 2 to reset.
  *			This parameter can be any combination of the following values:
  *			@arg RCC_APBPeriph2_UART0
  *			@arg RCC_APBPeriph2_UART1
  *			@arg RCC_APBPeriph2_UART2
  *			@arg RCC_APBPeriph2_UART3
  *			@arg RCC_APBPeriph2_USART0
  *			@arg RCC_APBPeriph2_LPUART0
  *			@arg RCC_APBPeriph2_LPUART1
  *			@arg RCC_APBPeriph2_IRTIM
  *			@arg RCC_APBPeriph2_SPI0
  *			@arg RCC_APBPeriph2_SPI1
  *			@arg RCC_APBPeriph2_I2C0
  *			@arg RCC_APBPeriph2_I2C1
  *			@arg RCC_APBPeriph2_CAN0
  * @param  NewState: new state of the specified peripheral reset.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APBPeriph2ResetCmd(u32 RCC_APBPeriph, FunctionalState NewState)
{
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
	return;
#else
	/* Check the parameters */
	assert_param(IS_RCC_APB_PERIPH2(RCC_APBPeriph));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if(NewState != DISABLE)
	{
		RCC->APBRSTR2 |= RCC_APBPeriph;
	}
	else
	{
		RCC->APBRSTR2 &= ~RCC_APBPeriph;
	}
#endif
}

/**
  * @brief  Forces or releases High Speed APB peripheral 3 reset.
  * @param  RCC_APB3Periph: specifies the APB peripheral 3 to reset.
  *			This parameter can be any combination of the following values:
  *			@arg RCC_APBPeriph3_ADC0
  *			@arg RCC_APBPeriph3_SDADC0
  *			@arg RCC_APBPeriph3_DAC0
  *			@arg RCC_APBPeriph3_DAC1
  *			@arg RCC_APBPeriph3_CMP0
  *			@arg RCC_APBPeriph3_CMP1
  *			@arg RCC_APBPeriph3_CMP2
  *			@arg RCC_APBPeriph3_CMP3
  *			@arg RCC_APBPeriph3_OPA
  *			@arg RCC_APBPeriph3_LDAC
  *			@arg RCC_APBPeriph3_DDS
  *			@arg RCC_APBPeriph3_LCD
  *			@arg RCC_APBPeriph3_PWM
  *			@arg RCC_APBPeriph3_BEEP
  * @param  NewState: new state of the specified peripheral reset.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APBPeriph3ResetCmd(u32 RCC_APBPeriph, FunctionalState NewState)
{
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
	return;
#else
	/* Check the parameters */
	assert_param(IS_RCC_APB_PERIPH3(RCC_APBPeriph));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		RCC->APBRSTR3 |= RCC_APBPeriph;
	}
	else
	{
		RCC->APBRSTR3 &= ~RCC_APBPeriph;
	}
#endif
}

/**
  * @brief  Forces or releases High Speed APB peripheral 4 reset.
  * @param  RCC_APB4Periph: specifies the APB peripheral 4 to reset.
  *			This parameter can be any combination of the following values:
  *			@arg RCC_APBPeriph4_IWDG
  *			@arg RCC_APBPeriph4_WWDG
  *			@arg RCC_APBPeriph4_RTC
  *			@arg RCC_APBPeriph4_AES
  *			@arg RCC_APBPeriph4_CRC
  *			@arg RCC_APBPeriph4_IFMC
  * @param  NewState: new state of the specified peripheral reset.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APBPeriph4ResetCmd(u32 RCC_APBPeriph, FunctionalState NewState)
{
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
	return;
#else
	/* Check the parameters */
	assert_param(IS_RCC_APB_PERIPH4(RCC_APBPeriph));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if(NewState != DISABLE)
	{
		RCC->APBRSTR4 |= RCC_APBPeriph;
	}
	else
	{
		RCC->APBRSTR4 &= ~RCC_APBPeriph;
	}
#endif
}

/**
  * @brief	Forces or releases AHB peripheral reset.
  * @param  RCC_AHBPeriph: specifies the AHB peripheral to reset.
  *			This parameter can be any combination of the following values:
  *			@arg RCC_AHBPeriph_GPIOA
  *			@arg RCC_AHBPeriph_GPIOB
  *			@arg RCC_AHBPeriph_GPIOC
  *			@arg RCC_AHBPeriph_GPIOD
  *			@arg RCC_AHBPeriph_GPIOE
  *			@arg RCC_AHBPeriph_GPIOF
  *			@arg RCC_AHBPeriph_DMA0
  *			@arg RCC_AHBPeriph_ALU
  * @param  NewState: new state of the specified peripheral reset.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_AHBPeriphResetCmd(u32 RCC_AHBPeriph, FunctionalState NewState)
{
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
	return;
#else
	/* Check the parameters */
	assert_param(IS_RCC_AHBRST_PERIPH(RCC_AHBPeriph));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		RCC->AHBRSTR |= RCC_AHBPeriph;
	}
	else
	{
		RCC->AHBRSTR &= ~RCC_AHBPeriph;
	}
#endif
}

/**
  * @brief	Check whether the specific reset flag is set or not.
  * @param	RCC_FLAG: specify the reset flag.
  *			This parameter can be any combination of the following values:
  *        	@arg RCC_FLAG_POR: Power reset flag
  *        	@arg RCC_FLAG_PVD: PVD reset flag
  *        	@arg RCC_FLAG_PIN: Pin reset flag
  *        	@arg RCC_FLAG_PLVD: Power low voltage detection reset flag
  *        	@arg RCC_FLAG_SFR: Software reset flag
  *        	@arg RCC_FLAG_LOCKUP: LOCKUP reset flag
  *        	@arg RCC_FLAG_IWDG: Indepent watch dog reset flag
  *        	@arg RCC_FLAG_WWDG: window watch dog reset flag
  *        	@arg RCC_FLAG_HSE: HSE clock fail flag
  *        	@arg RCC_FLAG_PLL: PLL clock fail flag
  *        	@arg RCC_FLAG_RELOAD: Configuration word reset
  *        	@arg RCC_FLAG_CPU: CPU reset
* @retval	FlagStatus: the new state of reset flag(SET or RESET)
  */
FlagStatus RCC_GetResetFlagStatus(u32 RCC_FLAG)
{
	/* Check the parameters */
	assert_param(IS_RCC_ResetStatusFlag(RCC_FLAG));

	if((RCC->RSR & RCC_FLAG) != RESET)
	{
		/* Return the flag status */
		return SET;
	}
	else
	{
		/* Return the flag status */
		return RESET;
	}
}

/**
  * @brief	Clear the specific global reset flag.
  * @param	RCC_FLAG: specify the reset flag.
  *        	This parameter can be any combination of the following values:
  *        	@arg RCC_FLAG_SFR: Software reset flag
  *        	@arg RCC_FLAG_IWDG: Indepent watch dog reset flag
  *        	@arg RCC_FLAG_HSE: HSE clock fail flag
  *        	@arg RCC_FLAG_PLL: PLL clock fail flag
  *        	@arg RCC_FLAG_POR: Power reset flag
  *        	@arg RCC_FLAG_PIN: Pin reset flag
  *        	@arg RCC_FLAG_PVD: PVD low voltage reset flag
  *       	@arg RCC_FLAG_PLVD: Power low voltage detection reset flag
  *        	@arg RCC_FLAG_LOCKUP: LOCKUP reset flag
  *        	@arg RCC_FLAG_RELOAD: Configuration word reset
  *        	@arg RCC_FLAG_CPU: CPU reset
  *        	@arg RCC_FLAG_WWDG: WWDG reset
  *        	@arg RCC_FLAG_ALL: ALL reset
  * @retval None
  */
void RCC_ClearResetFlag(u32 RCC_FLAG)
{
	/* Check the parameters */
	assert_param(IS_RCC_ResetStatusFlag(RCC_FLAG));
	/* Set bits to clear the reset flags */
	RCC->RSR = RCC_FLAG;
}

/**
  * @brief  Enable or disables the specified Reset Source.
  * @param  ResetSource: specifies the Reset source to be enabled or disabled.
  *        	This parameter can be any combination of the following values:
  *        	@arg RCC_ResetEnable_PVD: PVD reset enable
  *        	@arg RCC_ResetEnable_PLVD: Power low voltage detection reset enable
  *        	@arg RCC_ResetEnable_LOCKUPE: LOCKUP reset enable
  *        	@arg RCC_ResetEnable_IWDG: IWDG reset enable
  *        	@arg RCC_ResetEnable_HSE: HSE fail reset enable
  *        	@arg RCC_ResetEnable_PLL: PLL fail reset enable
  * @param  NewState: new state of the specified Reset Source
  *       	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_ResetConfig(u32 ResetSource, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_RCC_ResetEnable(ResetSource));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if(NewState != DISABLE)
	{
		RCC->RCR |= ResetSource;
	}
	else
	{
		RCC->RCR &= ~ResetSource;
	}
}

/**
  * @brief	Generate advanced software reset.
  * @param	RCC_AdvancedSoftwareResetSource: Advanceed software reset source selection.
  *       	This parameter can be one of the following values:
  *			@arg RCC_AdvancedSoftwareReset_RELOAD: Generate advanced software reset by configuration word.
  *         @arg RCC_AdvancedSoftwareReset_CPU: Generate advanced software reset by CPU.
  * @retval	None
  */
void RCC_AdvancedSoftwareReset(u8 RCC_AdvancedSoftwareResetSource)
{
	assert_param(IS_RCC_AdvancedSoftwareReset(RCC_AdvancedSoftwareResetSource));

	if(RCC_AdvancedSoftwareResetSource == RCC_AdvancedSoftwareReset_RELOAD)
	{
		RCC->ASRCR = RCC_KEY_RELOAD;
	}
	else if(RCC_AdvancedSoftwareResetSource == RCC_AdvancedSoftwareReset_CPU)
	{
		RCC->ASRCR = RCC_KEY_CPU;
	}
}

/**
  * @brief  Enable or disables the APB peripheral 1 clock.
  * @param  RCC_APBPeriph1: specifies the APB peripheral 1 to gates its clock.
  *			This parameter can be any combination of the following values:
  *			@arg RCC_APBPeriph1_TIM1
  *			@arg RCC_APBPeriph1_TIM2
  *			@arg RCC_APBPeriph1_TIM3
  *			@arg RCC_APBPeriph1_TIM4
  *			@arg RCC_APBPeriph1_TIM5
  *			@arg RCC_APBPeriph1_TIM6
  *			@arg RCC_APBPeriph1_TIM7
  *			@arg RCC_APBPeriph1_TIM8
  *			@arg RCC_APBPeriph1_LPTIM
  *			@arg RCC_APBPeriph1_TIM20
  *			@arg RCC_APBPeriph1_TIM21
  *			@arg RCC_APBPeriph1_TIM22
  * @param  NewState: new state of the specified peripheral clock.
  *   		This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APBPeriph1ClockCmd(u32 RCC_APBPeriph, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_RCC_APB_PERIPH1(RCC_APBPeriph));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		RCC->APBENR1 |= RCC_APBPeriph;
	}
	else
	{
		RCC->APBENR1 &= ~RCC_APBPeriph;
	}
}

/**
  * @brief  Enable or disables the APB peripheral 2 clock.
  * @param  RCC_APBPeriph2: specifies the APB peripheral 2 to gates its clock.
  *			This parameter can be any combination of the following values:
  *			@arg RCC_APBPeriph2_UART0
  *			@arg RCC_APBPeriph2_UART1
  *			@arg RCC_APBPeriph2_UART2
  *			@arg RCC_APBPeriph2_UART3
  *			@arg RCC_APBPeriph2_USART0
  *			@arg RCC_APBPeriph2_LPUART0
  *			@arg RCC_APBPeriph2_LPUART1
  *			@arg RCC_APBPeriph2_IRTIM
  *			@arg RCC_APBPeriph2_SPI0
  *			@arg RCC_APBPeriph2_SPI1
  *			@arg RCC_APBPeriph2_I2C0
  *			@arg RCC_APBPeriph2_I2C1
  *			@arg RCC_APBPeriph2_CAN0
  * @param  NewState: new state of the specified peripheral clock.
  *   		This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APBPeriph2ClockCmd(u32 RCC_APBPeriph, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_RCC_APB_PERIPH2(RCC_APBPeriph));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if(NewState != DISABLE)
	{
		RCC->APBENR2 |= RCC_APBPeriph;
	}
	else
	{
		RCC->APBENR2 &= ~RCC_APBPeriph;
	}
}

/**
  * @brief  Enable or disables the APB peripheral 3 clock.
  * @param  RCC_APB3Periph: specifies the APB peripheral 3 to gates its clock.
  *			This parameter can be any combination of the following values:
  *			@arg RCC_APBPeriph3_ADC0
  *			@arg RCC_APBPeriph3_SDADC0
  *			@arg RCC_APBPeriph3_DAC0
  *			@arg RCC_APBPeriph3_DAC1
  *			@arg RCC_APBPeriph3_CMP0
  *			@arg RCC_APBPeriph3_CMP1
  *			@arg RCC_APBPeriph3_CMP2
  *			@arg RCC_APBPeriph3_CMP3
  *			@arg RCC_APBPeriph3_OPA
  *			@arg RCC_APBPeriph3_LDAC
  *			@arg RCC_APBPeriph3_DDS
  *			@arg RCC_APBPeriph3_LCD
  *			@arg RCC_APBPeriph3_PWM
  *			@arg RCC_APBPeriph3_BEEP
  * @param  NewState: new state of the specified peripheral clock.
  *   		This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APBPeriph3ClockCmd(u32 RCC_APBPeriph, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_RCC_APB_PERIPH3(RCC_APBPeriph));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if(NewState != DISABLE)
	{
		RCC->APBENR3 |= RCC_APBPeriph;
	}
	else
	{
		RCC->APBENR3 &= ~RCC_APBPeriph;
	}
}

/**
  * @brief  Enable or disables the APB peripheral 4 clock.
  * @param  RCC_APB4Periph: specifies the APB peripheral 4 to gates its clock.
  *			This parameter can be any combination of the following values:
  *			@arg RCC_APBPeriph4_IWDG
  *			@arg RCC_APBPeriph4_WWDG
  *			@arg RCC_APBPeriph4_RTC
  *			@arg RCC_APBPeriph4_AES
  *			@arg RCC_APBPeriph4_CRC
  *			@arg RCC_APBPeriph4_IFMC
  * @param  NewState: new state of the specified peripheral clock.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APBPeriph4ClockCmd(u32 RCC_APBPeriph, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_RCC_APB_PERIPH4(RCC_APBPeriph));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if(NewState != DISABLE)
	{
		RCC->APBENR4 |= RCC_APBPeriph;
	}
	else
	{
		RCC->APBENR4 &= ~RCC_APBPeriph;
	}
}

/**
  * @brief  Enable or disables the AHB peripheral clock.
  * @param  RCC_AHBPeriph: specifies the AHB peripheral to gates its clock.
  *			This parameter can be any combination of the following values:
  *			@arg RCC_AHBPeriph_GPIOA
  *			@arg RCC_AHBPeriph_GPIOB
  *			@arg RCC_AHBPeriph_GPIOC
  *			@arg RCC_AHBPeriph_GPIOD
  *			@arg RCC_AHBPeriph_GPIOE
  *			@arg RCC_AHBPeriph_GPIOF
  *			@arg RCC_AHBPeriph_DMA0
  *			@arg RCC_AHBPeriph_ALU
  * @param  NewState: new state of the specified peripheral clock.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_AHBPeriphClockCmd(u32 RCC_AHBPeriph, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_RCC_AHB_PERIPH(RCC_AHBPeriph));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		RCC->AHBENR |= RCC_AHBPeriph;
	}
	else
	{
		RCC->AHBENR &= ~RCC_AHBPeriph;
	}
}

/**
  * @brief  Selects the system clock source.
  * @param  SystemClock: the selection of clock source.
  *			This parameter can be one of the following values:
  *         @arg RCC_SYSCLK_HSI: Select HSI clock as the system clock.
  *         @arg RCC_SYSCLK_HSE: Select HSE clock as the system clock.
  *         @arg RCC_SYSCLK_PLL: Select PLL clock as the system clock.
  * @retval None
  */
void RCC_SetSystemClock(u32 SystemClock)
{
	u32 tmp;
	/* Check the parameters */
	assert_param(IS_RCC_SYSCLK(SystemClock));
	/* Get the RCC CFGR value and Clear SCW bits */
	tmp = RCC->CFGR & ~RCC_CFGR_SCW;
	/* Set SCW bit according to SystemClock value */
	tmp |= SystemClock;
	/* Write to CFGR register*/
	RCC->CFGR = tmp;
}

/**
  * @brief  Get the selection of system clock.
  * @retval System Clock Selection
  *         @arg RCC_SYSCLK_HSI: Select HSI clock as the system clock.
  *         @arg RCC_SYSCLK_HSE: Select HSE clock as the system clock.
  *         @arg RCC_SYSCLK_PLL: Select PLL clock as the system clock.
  */
u8 RCC_GetSystemClockSelection(void)
{
	u8 SystemClockSelection;
	SystemClockSelection = (RCC->CFGR & RCC_CFGR_SCW);
	return SystemClockSelection;
}

/**
  * @brief  Selects system clock after wake-up.
  * @param	Clock: specified the system clock after wake-up.
  *			This parameter can be one of the following values:
  *         @arg RCC_SYSCLK_AfterWakeUp_HSI: Set HSI clock as system clock after wake-up.
  *         @arg RCC_SYSCLK_AfterWakeUp_PrevClock: Set the clock before MCU sleep as system clock after wake-up.
  * @retval None
  */
void RCC_SetSystemClockAfterWakeUp(u32 Clock)
{
	/* Check the parameters */
	assert_param(IS_RCC_SystemClockAfterWakeUp(Clock));

	if(Clock != RCC_SYSCLK_AfterWakeUp_HSI)
	{
		/* Set the SCWK Bit */
		RCC->CFGR |= RCC_CFGR_SCWK;
	}
	else
	{
		/* Reset the SCWK Bit */
		RCC->CFGR &= ~RCC_CFGR_SCWK;
	}
}

/**
  * @brief  Configure the HCLK Clock Prescaler.
  * @param  Prescaler: specifies the Prescaler value
  *			This parameter can be a number between 0 and 31.
  * @retval None
  */
void RCC_HCLKSetPrescaler(u32 Prescaler)
{
	u32 tmp;
	/* Check the parameters */
	assert_param(IS_RCC_HCLKPrescaler(Prescaler));
	/* Get the RCC CFGR value and Clear HPRE bits */
	tmp = RCC->CFGR & ~RCC_CFGR_HPRE;
	/* Set HPRE bit according to Prescaler value */
	tmp |= (Prescaler << 16);
	/* Write to CFGR register*/
	RCC->CFGR = tmp;
}

/**
  * @brief  Configure the PCLK Clock Prescaler.
  * @param  Prescaler: specifies the Prescaler value.
  *			This parameter can be a number between 0 and 31.
  * @retval None
  */
void RCC_PCLKSetPrescaler(u32 Prescaler)
{
	u32 tmp;
	/* Check the parameters */
	assert_param(IS_RCC_PCLKPrescaler(Prescaler));
	/* Get the RCC CFGR value and Clear PPRE bits */
	tmp = RCC->CFGR & ~RCC_CFGR_PPRE;
	/* Set PPRE bit according to Prescaler value */
	tmp |= (Prescaler << 24);
	/* Write to CFGR register*/
	RCC->CFGR = tmp;
}

/**
  * @brief  Selects the clock source to output on MCO pin.
  * @param  source: the selection of Clock Output Source.
  *         @arg RCC_ClockOutput_HSI: Select HSI clock as the clock output source.
  *         @arg RCC_ClockOutput_HSE: Select HSE clock as the clock output source.
  *         @arg RCC_ClockOutput_PLL: Select PLL clock as the clock output source.
  *         @arg RCC_ClockOutput_LSI: Select LSI clock as the clock output source.
  *         @arg RCC_ClockOutput_SYSCLK: Select System clock as the clock output source.
  *         @arg RCC_ClockOutput_LSE: Select LSE clock as the clock output source.
  *         @arg RCC_ClockOutput_None: None clock output.
  * @retval None
  */
void RCC_SetMCOSource(u32 Source)
{
	u32 tmp;
	/* Check the parameters */
	assert_param(IS_RCC_ClockOutputSource(Source));
	/* Get the RCC MCOR value and Clear COSRC bits */
	tmp = RCC->MCOR & ~RCC_MCOR_COSRC;
	/* Set COSRC bit according to Source value */
	tmp |= Source;
	/* Write to MCOR register*/
	RCC->MCOR = tmp;
}

/**
  * @brief  Configure the prescaler value for clock output pin.
  * @param  PrescaleSelcection: the selection of clock output prescaler.
  *         @arg RCC_ClockOutputPrescale_None : None Prescale the clock output frequency.
  *         @arg RCC_ClockOutputPrescale_2: Prescale the clock output frequency into 1/2.
  *         @arg RCC_ClockOutputPrescale_4:	Prescale the clock output frequency into 1/4.
  *         @arg RCC_ClockOutputPrescale_8: Prescale the clock output frequency into 1/8.
  *         @arg RCC_ClockOutputPrescale_16: Prescale the clock output frequency into 1/16.
  * @retval None
  */
void RCC_MCOSetPrescaler(u32 PrescaleSelcection)
{
	u32 tmp;
	/* Check the parameters */
	assert_param(IS_RCC_ClockOutputPrescale(PrescaleSelcection));
	/* Get the RCC MCOR value and Clear COPRE bits */
	tmp = RCC->MCOR & ~RCC_MCOR_COPRE;
	/* Set COPRE bit according to PrescaleSelcection value */
	tmp |= PrescaleSelcection;
	/* Write to MCOR register*/
	RCC->MCOR = tmp;
}

/**
  * @brief  Configure peripheral clock source for peripheral 1.
  * @param  periph: the selection of peripheral 1.
  *         @arg RCC_Periph1_IWDG : select IWDG.
  *         @arg RCC_Periph1_TIM4 : select TIM4.
  *         @arg RCC_Periph1_RTC : select RTC.
  *         @arg RCC_Periph1_LCD : select LCD.
  *         @arg RCC_Periph1_BEEP : select BEEP.
  *         @arg RCC_Periph1_LPTIM : select LPTIM.
  * @param  source: clock source of the selected peripheral.
  *         @arg RCC_IWDG_CLKSRC_LSI : clock source LSI of IWDG.
  *         @arg RCC_IWDG_CLKSRC_LSE : clock source LSE of IWDG.
  *         @arg RCC_TIM4_CLKSRC_LSI : clock source LSI of TIM4.
  *         @arg RCC_TIM4_CLKSRC_LSE : clock source LSE of TIM4.
  *         @arg RCC_RTC_CLKSRC_LSI : clock source LSI of RTC.
  *         @arg RCC_RTC_CLKSRC_LSE : clock source LSE of RTC.
  *         @arg RCC_LCD_CLKSRC_LSI : clock source LSI of LCD.
  *         @arg RCC_LCD_CLKSRC_LSE : clock source LSE of LCD.
  *         @arg RCC_BEEP_CLKSRC_LSI : clock source LSI of BEEP.
  *         @arg RCC_BEEP_CLKSRC_LSE : clock source LSE of BEEP.
  *         @arg RCC_LPTIM_CLKSRC_PCLK : clock source PCLK of LPTIM.
  *         @arg RCC_LPTIM_CLKSRC_HSI : clock source HSI of LPTIM.
  *         @arg RCC_LPTIM_CLKSRC_SYSCLK : clock source SYSCLK of LPTIM.
  *         @arg RCC_LPTIM_CLKSRC_LSE : clock source LSI of LPTIM.
  *         @arg RCC_LPTIM_CLKSRC_LSE : clock source LSE of LPTIM.
  * @retval None
  */
void RCC_Periph1ClockSourceConfig(u32 Periph, u32 Source)
{
	u32 tmp;

	assert_param(IS_RCC_Periph1(Periph));
	
	switch(Periph)
	{
		case RCC_Periph1_IWDG:
			assert_param(IS_RCC_IWDG_CLKSRC(Source));
			tmp = RCC->PCSCR1 & ~RCC_PCSCR1_IWDG;
			break;
		case RCC_Periph1_TIM4:
			assert_param(IS_RCC_TIM4_CLKSRC(Source));
			tmp = RCC->PCSCR1 & ~RCC_PCSCR1_TIM4;
			break;
		case RCC_Periph1_RTC:
			assert_param(IS_RCC_RTC_CLKSRC(Source));
			tmp = RCC->PCSCR1 & ~RCC_PCSCR1_RTC;
			break;
		case RCC_Periph1_LCD:
			assert_param(IS_RCC_LCD_CLKSRC(Source));
			tmp = RCC->PCSCR1 & ~RCC_PCSCR1_LCD;
			break;
		case RCC_Periph1_BEEP:
			assert_param(IS_RCC_BEEP_CLKSRC(Source));
			tmp = RCC->PCSCR1 & ~RCC_PCSCR1_BEEP;
			break;
		case RCC_Periph1_LPTIM:
			assert_param(IS_RCC_LPTIM_CLKSRC(Source));
			tmp = RCC->PCSCR1 & ~RCC_PCSCR1_LPTIM;
			break;
		default:
			return ;
	}
	
	tmp |= Source;
	RCC->PCSCR1 = tmp;
}

/**
  * @brief  Configure peripheral clock source for peripheral 2.
  * @param  periph: the selection of peripheral 2.
  *         @arg RCC_Periph2_LPUART0: select LPUART0.
  *         @arg RCC_Periph2_LPUART1: select LPUART1.
  * @param  source: clock source of the selected peripheral.
  *         @arg RCC_LPUART0_CLKSRC_PCLK: clock source PCLK of LPUART0.
  *         @arg RCC_LPUART0_CLKSRC_HSI: clock source HSI of LPUART0.
  *         @arg RCC_LPUART0_CLKSRC_HCLK: clock source HCLK of LPUART0.
  *         @arg RCC_LPUART0_CLKSRC_LSE: clock source LSI of LPUART0.
  *         @arg RCC_LPUART0_CLKSRC_LSE: clock source LSE of LPUART0.
  *         @arg RCC_LPUART1_CLKSRC_PCLK: clock source PCLK of LPUART1.
  *         @arg RCC_LPUART1_CLKSRC_HSI: clock source HSI of LPUART1.
  *         @arg RCC_LPUART1_CLKSRC_HCLK: clock source HCLK of LPUART1.
  *         @arg RCC_LPUART1_CLKSRC_LSE: clock source LSI of LPUART1.
  *         @arg RCC_LPUART1_CLKSRC_LSE: clock source LSE of LPUART1.
  * @retval None
  */
void RCC_Periph2ClockSourceConfig(u32 Periph, u32 Source)
{
	u32 tmp;
	/* Check the parameters */
	assert_param(IS_RCC_Periph2(Periph));
	if(Periph == RCC_Periph2_LPUART0)
	{
		assert_param(IS_RCC_LPUART0_CLKSRC(Source));
		tmp = RCC->PCSCR2 & ~RCC_PCSCR2_LPUART0;
	}
	else if(Periph == RCC_Periph2_LPUART1)
	{
		assert_param(IS_RCC_LPUART1_CLKSRC(Source));
		tmp = RCC->PCSCR2 & ~RCC_PCSCR2_LPUART1;
	}
	else
	{
		return ;
	}
	
	tmp |= Source;
	RCC->PCSCR2 = tmp;
}

/**
  * @brief  Enable or disable the specified clock source
  * @param  ClockSource: specifies the clock source to be enabled or disabled.
  *         This parameter can be one of the following values:
  *         @arg RCC_ClockSource_HSI: Specify the new state of Internal High Crystal.
  *         @arg RCC_ClockSource_HSE: Specify the new state of Extern High Crystal.
  *         @arg RCC_ClockSource_HSEExternal: Specify the new state of Extern High Clock.
  *         @arg RCC_ClockSource_PLL: Specify the new state of Phase Locked Loop.
  *         @arg RCC_ClockSource_LSI: Specify the new state of Internal Low Crystal.
  *         @arg RCC_ClockSource_LSE: Specify the new state of Extern Low Crystal.
  *         @arg RCC_ClockSource_LSEExternal: Specify the new state of Extern Low Clock.
  * @param  NewState: new state of the specified Reset Source.
  *			This parameter can be: ENABLE or DISABLE.
  * @return None
  */
void RCC_ClockSourceConfig(u32 ClockSource, FunctionalState NewState)
{
	u32 tmp;
	
	/* Check the parameters */
	assert_param(IS_RCC_ClockSource(ClockSource));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		if(ClockSource == RCC_ClockSource_LSI)
		{
			RCC->LSICR |= RCC_LSICR_EN;
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
			while((RCC->LSICR & RCC_LSICR_RDY) != RCC_LSICR_RDY);
#endif
		}
		else if(ClockSource == RCC_ClockSource_HSI)
		{
			RCC->HSICR |= RCC_HSICR_EN;
		}
		else if(ClockSource == RCC_ClockSource_HSE)
		{
			tmp = RCC->HSECR & ~RCC_HSECR_ECE;
			tmp |= RCC_HSECR_EN;
			RCC->HSECR = tmp;
			while((RCC->HSECR & RCC_HSECR_RDY) != RCC_HSECR_RDY);
		}
		else if(ClockSource == RCC_ClockSource_HSEExternal)
		{
			tmp = RCC->HSECR & ~RCC_HSECR_EN;
			tmp |= RCC_HSECR_ECE;
			RCC->HSECR = tmp;
			while((RCC->HSECR & RCC_HSECR_RDY) != RCC_HSECR_RDY);
		}
		else if(ClockSource == RCC_ClockSource_LSE)
		{
			tmp = RCC->LSECR & ~RCC_LSECR_ECE;
			tmp |= RCC_LSECR_EN;
			RCC->LSECR = tmp;
			while((RCC->LSECR & RCC_LSECR_RDY) != RCC_LSECR_RDY);
		}
		else if(ClockSource == RCC_ClockSource_LSEExternal)
		{
			tmp = RCC->LSECR & ~RCC_LSECR_EN;
			tmp |= RCC_LSECR_ECE;
			RCC->LSECR = tmp;
			while((RCC->LSECR & RCC_LSECR_RDY) != RCC_LSECR_RDY);
		}
		else if(ClockSource == RCC_ClockSource_PLL)
		{
			RCC->PLLCFGR1 |= RCC_PLLCFGR1_EN;
			while((RCC->PLLCFGR1 & RCC_PLLCFGR1_RDY) != RCC_PLLCFGR1_RDY);
		}
	}
	else
	{
		if(ClockSource == RCC_ClockSource_LSI)
		{
			RCC->LSICR &= ~RCC_LSICR_EN;
		}
		else if(ClockSource == RCC_ClockSource_HSI)
		{
			RCC->HSICR = ~RCC_HSICR_EN;
		}
		else if(ClockSource == RCC_ClockSource_HSE)
		{
			RCC->HSECR &= ~RCC_HSECR_EN;
		}
		else if(ClockSource == RCC_ClockSource_HSEExternal)
		{
			RCC->HSECR &= ~RCC_HSECR_ECE;
		}
		else if(ClockSource == RCC_ClockSource_LSE)
		{
			RCC->LSECR &= ~RCC_LSECR_EN;
		}
		else if(ClockSource == RCC_ClockSource_LSEExternal)
		{
			RCC->LSECR &= ~RCC_LSECR_ECE;
		}
		else if(ClockSource == RCC_ClockSource_PLL)
		{
			RCC->PLLCFGR1 &= ~RCC_PLLCFGR1_EN;
		}
	}
}

/**
  * @brief  Configure frequency for the LSI clock.
  * @param  freq: the LSI's frequency.
  *			This parameter can be one of the following values:
  *         @arg RCC_LSIFreq_38400Hz: Specify the LSI's frequency is 38.4kHz.
  *         @arg RCC_LSIFreq_32768Hz: Specify the LSI's frequency is 32.768kHz.
  *	@return None
  */
void RCC_LSIFreqConfig(u32 freq)
{
	u32 tmp;
	
	/* Check the parameters */
	assert_param(IS_RCC_LSIFreq(freq));

	tmp = RCC->LSICR & ~RCC_LSICR_FREQS;
	tmp |= freq;
	RCC->LSICR = tmp;
}

/**
  * @brief  Enable or disable HSE automatic control of oscillation amplitude.
  * @param  NewState: new state of automatic amplitude control.
  *			This parameter can be: ENABLE or DISABLE.
  * @return None
  */
void RCC_HSEAmplitudeConfig(FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if(NewState != DISABLE)
	{
		/* Set the AAC bit */
		RCC->HSECR |= RCC_HSECR_AAC;
	}
	else
	{
		/* Reset the AAC bit */
		RCC->HSECR &= ~RCC_HSECR_AAC;
	}
}

/**
  * @brief  Configure frequency range for HSE.
  * @param  Range: specifies the frequency range of HSE.
  *         This parameter can be one of the following values:
  *         @arg RCC_HSEFreqRange_4_10MHz: Configure the HSE frequency range from 4 to 10MHz.
  *         @arg RCC_HSEFreqRange_10_25MHz: Configure the HSE frequency range from 10 to 25MHz.
  * @return None
  */
void RCC_HSEFreqRangeConfig(u32 Range)
{
	u32 tmp;
	/* Check the parameters */
	assert_param(IS_RCC_HSEFreqRange(Range));
	/* Get the RCC HSECR value and Clear FREQ bits */
	tmp = RCC->HSECR & ~RCC_HSECR_FREQ;
	/* Set FREQ bit according to Range value */
	tmp |= Range;
	/* Write to HSECR register*/
	RCC->HSECR = tmp;
}

/**
  * @brief  Enable or disable HSE fail detecting.
  * @param  NewState: new state of the specified Reset Source.
  *			This parameter can be: ENABLE or DISABLE.
  * @return None
  */
void RCC_HSEFailDetectConfig(FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if(NewState != DISABLE)
	{
		/* Set the FDE bit */
		RCC->HSECR |= RCC_HSECR_FDE;
	}
	else
	{
		/* Reset the FDE bit */
		RCC->HSECR &= ~RCC_HSECR_FDE;
	}
}

/**
  * @brief  Enable or disable low pass filter bypass for the specified clock.
  * @param  ClockSource: specifies the clock low pass filter bypass to be enabled or disabled.
  *         This parameter can be one of the following values:
  *         @arg RCC_CLKF_HSE : Specify the Extern High Crystal.
  *         @arg RCC_CLKF_LSE : Specify the Extern Low Crystal.
  *         @arg RCC_CLKF_PLL : Specify the PLL.
  * @param  NewState: new state of the specified Reset Source.
  *			This parameter can be: ENABLE or DISABLE.
  * @return None
  */
void RCC_ClockFilterBypassConfig(u32 ClockSource, FunctionalState NewState)
{
	__IO u32 *ptr;
	/* Check the parameters */
	assert_param(IS_RCC_ClockFilterSource(ClockSource));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	switch(ClockSource)
	{
		case RCC_CLKF_HSE:
			ptr = &RCC->HSECR;
			break;
		case RCC_CLKF_LSE:
			ptr = &RCC->LSECR;
			break;
		case RCC_CLKF_PLL:
			ptr = &RCC->PLLCFGR1;
			break;
		default:
			return ;
	}

	if(NewState != DISABLE)
	{
		*ptr |= RCC_CLKF_LPFB;
	}
	else
	{
		*ptr &= ~RCC_CLKF_LPFB;
	}
}

/**
  * @brief  Configure low pass filter argument value for the HSE clock.
  * @param  FilterCfg: the osc low pass filter argument value.
  *			This parameter can be one of the following values:
  *         @arg RCC_HSE_Filter_4ns: Specify low pass filter argument value is 4ns.
  *         @arg RCC_HSE_Filter_6ns: Specify low pass filter argument value is 6ns.
  *         @arg RCC_HSE_Filter_8ns: Specify low pass filter argument value is 8ns.
  *         @arg RCC_HSE_Filter_10ns: Specify low pass filter argument value is 10ns.
  *	@return None
  */
void RCC_HSEFilterConfig(u32 FilterCfg)
{
	u32 tmp;
	/* Check the parameters */
	assert_param(IS_RCC_HSEFilterCfg(FilterCfg));

	tmp = RCC->HSECR & ~RCC_HSECR_LPF;
	tmp |= FilterCfg;
	RCC->HSECR = tmp;
}

/**
  * @brief  Configure low pass filter argument value for the LSE clock.
  * @param  FilterCfg: the osc low pass filter argument value.
  *			This parameter can be one of the following values:
  *         @arg RCC_LSE_Filter_9ns: Specify low pass filter argument value is 4ns.
  *         @arg RCC_LSE_Filter_11ns: Specify low pass filter argument value is 6ns.
  *         @arg RCC_LSE_Filter_15ns: Specify low pass filter argument value is 8ns.
  *         @arg RCC_LSE_Filter_20ns: Specify low pass filter argument value is 10ns.
  *	@return None
  */
void RCC_LSEFilterConfig(u32 FilterCfg)
{
	u32 tmp;
	/* Check the parameters */
	assert_param(IS_RCC_LSEFilterCfg(FilterCfg));

	tmp = RCC->LSECR & ~RCC_LSECR_LPF;
	tmp |= FilterCfg;
	RCC->LSECR = tmp;
}

/**
  * @brief  Configure low pass filter argument value for the PLL clock.
  * @param  FilterCfg: the osc low pass filter argument value.
  *			This parameter can be one of the following values:
  *         @arg RCC_PLL_Filter_2ns: Specify low pass filter argument value is 4ns.
  *         @arg RCC_PLL_Filter_4ns: Specify low pass filter argument value is 6ns.
  *         @arg RCC_PLL_Filter_6ns: Specify low pass filter argument value is 8ns.
  *         @arg RCC_PLL_Filter_8ns: Specify low pass filter argument value is 10ns.
  *	@return None
  */
void RCC_PLLFilterConfig(u32 FilterCfg)
{
	u32 tmp;
	/* Check the parameters */
	assert_param(IS_RCC_PLLFilterCfg(FilterCfg));

	tmp = RCC->PLLCFGR1 & ~RCC_PLLCFGR1_LPF;
	tmp |= FilterCfg;
	RCC->PLLCFGR1 = tmp;
}

/**
  * @brief  Configure the internal feedback resistance for HSE.
  * @param  FDR: the HSE internal feedback resistance selection.
  *			This parameter can be one of the following values:
  *         @arg RCC_HSE_FDR_1100k: Specify the HSE internal feedback resistance is 1100k.
  *         @arg RCC_HSE_FDR_800k: Specify the HSE internal feedback resistance is 800k.
  *         @arg RCC_HSE_FDR_500k: Specify the HSE internal feedback resistance is 500k.
  *         @arg RCC_HSE_FDR_200k: Specify the HSE internal feedback resistance is 200k.
  * @return None
  */
void RCC_HSEFeedbackResistanceConfig(u32 FDR)
{
	u32 tmp;
	/* Check the parameters */
	assert_param(IS_RCC_HSE_FDR(FDR));
	/* Get the RCC HSECR value and Clear FDR bits */
	tmp = RCC->HSECR & ~RCC_HSECR_FDR;
	/* Set FDR bit according to FDR value */
	tmp |= FDR;
	/* Write to HSECR register*/
	RCC->HSECR = tmp;
}

/**
  * @brief  Configure the internal feedback resistance for LSE.
  * @param  FDR: the LSE internal feedback resistance selection.
  *			This parameter can be one of the following values:
  *         @arg RCC_LSE_FDR_8M: Specify the LSE internal feedback resistance is 8M.
  *         @arg RCC_LSE_FDR_6M: Specify the LSE internal feedback resistance is 6M.
  *         @arg RCC_LSE_FDR_5M: Specify the LSE internal feedback resistance is 5M.
  *         @arg RCC_LSE_FDR_3M: Specify the LSE internal feedback resistance is 3M.
  * @return None
  */
void RCC_LSEFeedbackResistanceConfig(u32 FDR)
{
	u32 tmp;
	/* Check the parameters */
	assert_param(IS_RCC_LSE_FDR(FDR));
	/* Get the RCC LSECR value and Clear FDR bits */
	tmp = RCC->LSECR & ~RCC_LSECR_FDR;
	/* Set FDR bit according to FDR value */
	tmp |= FDR;
	/* Write to LSECR register*/
	RCC->LSECR = tmp;
}

/**
  * @brief  Configure clock source output delay time for the PLL.
  * @param  delay: select PLL clock source output delay time.
  *         This parameter can be one of the following values:
  *         @arg RCC_PLLSRC_DELAY_256: PLL clock source output delay 256 PLLSRC's clocks.
  *         @arg RCC_PLLSRC_DELAY_512: PLL clock source output delay 512 PLLSRC's clocks.
  * @retval None
  */
void RCC_PLLClockSourceDelayConfig(u32 Delay)
{
	u32 tmp;
	/* Check the parameters */
	assert_param(IS_RCC_PLLSRC_DELAY(Delay));
	/* Get the RCC PLLCFGR1 value and Clear DELAY bits */
	tmp = RCC->PLLCFGR1 & ~RCC_PLLCFGR1_DELAY;
	/* Set DELAY bit according to Delay value */
	tmp |= Delay;
	/* Write to PLLCFGR1 register*/
	RCC->PLLCFGR1 = tmp;
}

/**
  * @brief  Select the entry clock source for the PLL.
  * @param  PLLClockSource: select PLL entry clock source.
  *         This parameter can be one of the following values:
  *         @arg RCC_PLLClockSource_HSI: Select Internal High Crystal as the PLL entry clock source.
  *         @arg RCC_PLLClockSource_HSE: Select Extern High Crystal as the PLL entry clock source.
  * @retval None
  */
void RCC_PLLClockSourceConfig(u32 PLLClockSource)
{
	u32 tmp;
	/* Check the parameters */
	assert_param(IS_RCC_PLLClockSource(PLLClockSource));
	/* Get the RCC PLLCFGR1 value and Clear SRC bits */
	tmp = RCC->PLLCFGR1 & ~RCC_PLLCFGR1_SRC;
	/* Set SRC bit according to PLLClockSource value */
	tmp |= PLLClockSource;
	/* Write to PLLCFGR1 register*/
	RCC->PLLCFGR1 = tmp;
}
