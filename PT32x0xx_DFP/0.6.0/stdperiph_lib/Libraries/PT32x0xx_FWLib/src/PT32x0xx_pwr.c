/**
	****************************************************************************
  * @file    PT32x0xx_pwr.c
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file provides firmware functions to manage Power Controller (PWR) peripheral
  ******************************************************************************
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.  
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of
  * the content or the codes within the content.  
  * 
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved 
  ***************************************************************************
**/

#include "PT32x0xx_pwr.h"

/** @defgroup PWR
  * @brief PWR driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define PWR_Init_CR1_KEY1              				((u32)0x00005800)
#define PWR_Init_CR1_KEY2              				((u32)0x0000C600)

#define PWR_VREBUF_BG2v0_Key_User					((u32)0x0000AC00)
#define PWR_VREBUF_BG2v0_Key_Manufactory			((u32)0x0000C600)

#define PWR_SDADC0_PowerVoltage_Key					((u32)0x0000CC00)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Enable or disable the Power Voltage Detector(PVD).
  * @param  NewState: new state of the PVD.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void PWR_PVDCmd(FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the PVD */
		PWR->CR1 |= PWR_CR1_PVDE;
	}
	else
	{
		/* Disable the PVD */
		PWR->CR1 &= ~PWR_CR1_PVDE;
	}
}

/**
  * @brief  Configure the voltage threshold detected for the Power Voltage Detector(PVD).
  * @param  PWR_PVDLevel: specifies the PVD detection level
  *			This parameter can be one of the following values:
  * 		@arg PWR_PVDLevel_0: set Level_0 as PVD detection level
  * 		@arg PWR_PVDLevel_1: set Level_1 as PVD detection level
  * 		@arg PWR_PVDLevel_2: set Level_2 as PVD detection level
  * 		@arg PWR_PVDLevel_3: set Level_3 as PVD detection level
  * 		@arg PWR_PVDLevel_4: set Level_4 as PVD detection level
  * 		@arg PWR_PVDLevel_5: set Level_5 as PVD detection level
  * 		@arg PWR_PVDLevel_6: set Level_6 as PVD detection level
  * 		@arg PWR_PVDLevel_7: set Level_7 as PVD detection level
  * @note   Refer to the electrical characteristics of your device datasheet for
  *         more details about the voltage threshold corresponding to each
  *         detection level.
  * @retval None
  */
void PWR_PVDLevelConfig(u32 PWR_PVDLevel)
{
	u32 tmpreg1, tmpreg2;
	
	/* Check the parameters */
	assert_param(IS_PWR_PVDLevel(PWR_PVDLevel));
	
	tmpreg1 = PWR->CR1 & ~PWR_CR1_KEY;
	tmpreg1 |= PWR_Init_CR1_KEY1;
	PWR->CR1 = tmpreg1;
	
	tmpreg2 = PWR->CR1 & ~PWR_CR1_PLS;
	/* Set PLS bits according to PWR_PVDLevel value */
	tmpreg2 |= PWR_PVDLevel;
	/* Write to CR1 register*/
	PWR->CR1 = tmpreg2;
}

/**
  * @brief	Reset the voltage threshold detected for the Power Voltage Detector(PVD).
  * @note   PLS bit reset to factory default values.
  * @retval None
  */
void PWR_PVDLevelReset(void)
{
	u32 tmpreg;

	tmpreg = PWR->CR1 & ~PWR_CR1_KEY;
	/* Reset to factory mode */
	tmpreg |= PWR_Init_CR1_KEY2;
	/* Write to CR1 register*/
	PWR->CR1 = tmpreg;	
}

/**
  * @brief  Configure the FLASH wake up sleep mode.
  * @param  FlashWakeUp_Mode: FLASH wake-up selection.
  *			This parameter can be one of the following values:
  *			@arg PWR_FlashWakeUp_Normal: select normal wake-up mode.
  *			@arg PWR_FlashWakeUp_Fast: select choose fast wake-up. 
  * @retval None
  */
void PWR_FlashWakeUpMode(u32 FlashWakeUpMode)
{
	u32 tmpreg;
	
	assert_param(IS_PWR_FlashWakeUp_Mode(FlashWakeUpMode));
	/* Get the PWR->CR2 value and Clear FWUS bits */
	tmpreg = PWR->CR2 & ~PWR_CR2_FWUS;
	/* Set FWUS bit according to FlashWakeUpMode value */
	tmpreg |= FlashWakeUpMode;
	/* Write to CR2 register*/
	PWR->CR2 = tmpreg;
}

/**
  * @brief	Check whether the specified PWR flag is set or not.
  * @param	PWR_FLAG: specifies the flag to check. 
  *			This parameter can be one of the following values:
  *			@arg PWR_FLAG_LPRDY: LPR ready flag
  * @retval FlagStatus: The new state of PWR's FLAG (SET or RESET).
  */
FlagStatus PWR_GetFlagStatus(u32 PWR_FLAG)
{
	FlagStatus bitstatus;

	/* Check the parameters */
	assert_param(IS_PWR_FLAG(PWR_FLAG));

	/* Check the status of the specified PWR flag */
	if((PWR->SR & PWR_FLAG) != (u32)RESET)
	{
		bitstatus = SET;
	}
	else
	{
		bitstatus = RESET;
	}

	return  bitstatus;
}

/**
  * @brief	Enable or disable the VREFBUF.
  * @param	mode: specifies the VREFBUF working mode. 
  *			This parameter can be one of the following values:
  *			@arg PWR_VREFBUF_Disable: Disable the VREFBUF
  *			@arg PWR_VREFBUF_IntVREF: Enable the VREFBUF, it's voltage from internal reference voltage(BG2v0 or BGREF)
  *			@arg PWR_VREFBUF_ExtVREF: Enable the VREFBUF, it's voltage from external reference voltage(VREF+)
  *			@arg PWR_VREFBUF_IntVREF_ToPin: Enable the VREFBUF, its voltage from internal reference voltage(BG2v0 or BGREF) and outputs it to VREF+ pin
  * @retval None
  */
void PWR_VREFBUF_Config(u32 Mode)
{
	u32 tmpreg;

	/* Check the parameters */
	assert_param(IS_PWR_VREFBUF_Mode(Mode));

	tmpreg = PWR->VBUFCR & ~PWR_VBUFCR_MODE;
	tmpreg |= Mode;
	PWR->VBUFCR = tmpreg;
}

/**
  * @brief	Configure internal reference voltage selection for the VREFBUF.
  * @param	sel: specifies the VREFBUF's internal reference voltage selection. 
  *			This parameter can be one of the following values:
  *			@arg PWR_VREFBUF_IntVREF_BG2v0: BG2v0 is selected for the VREFBUF's internal reference voltage
  *			@arg PWR_VREFBUF_IntVREF_BGREF: BGREF is selected for the VREFBUF's internal reference voltage
  * @retval None
  */
void PWR_VREFBUF_InternalRefVoltageConfig(u32 Sel)
{
	u32 tmpreg;

	/* Check the parameters */
	assert_param(IS_PWR_VREFBUF_IntVREF(Sel));

	tmpreg = PWR->VBUFCR & ~PWR_VBUFCR_IRVS;
	tmpreg |= Sel;
	PWR->VBUFCR = tmpreg;
}

/**
  * @brief	Configure the BG2v0 offset trimming value for the VREFBUF.
  * @param	Value: specifies the BG2v0 offset trimming value.
  *			This parameter must be a number between 0x00 to 0x3F.
  * @retval None
  */
void PWR_VREFBUF_BG2v0TrimValueConfig(u32 Value)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_PWR_VREFBUF_TrimValue(Value));
	
	tmpreg = PWR->VBUFCR & ~PWR_VBUFCR_TRIM;
	tmpreg |= (Value << 16) | PWR_VREBUF_BG2v0_Key_User;
	PWR->VBUFCR = tmpreg;
}

#if defined (PT32x060x) || defined (PT32x063x) || defined (PT32x066x)
/**
  * @brief Get the VREFBUF's BG2v0 offset trimming value
  * @param None
  * @retval trimming value
  */
u8 PWR_VREFBUF_GetBG2v0TrimValue(void)
{
	return ((PWR->VBUFCR & PWR_VBUFCR_TRIM) >> 16);
}

/**
  * @brief  Check whether the specified BGTF flag is set or not in VREFBUF.
  * @param  None
  * @retval FlagStatus: The new state of BGTF FLAG (SET or RESET).
  */
FlagStatus PWR_VREFBUF_GetBGTFFlagStatus(void)
{
	FlagStatus bitstatus;

	/* Check the status of the specified BGREF offset trimming value source flag */
	if((PWR->VBUFCR & PWR_VBUFCR_BGTF) != 0)
	{
		bitstatus = SET;
	}
	else
	{
		bitstatus = RESET;
	}
	
	return  bitstatus;
}

/**
  * @brief  Enable or disable the SDADC0's power.
  * @param  NewState: new state of the SDADC0's power.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void PWR_SDADC0_PowerCmd(FunctionalState NewState)
{
	u32 tmpreg;	
	/* Check the parameters */
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	tmpreg = PWR->SDADC0PCR & ~PWR_SDADC0PCR_EN;
	
	if(NewState != DISABLE)
	{
		tmpreg |= PWR_SDADC0PCR_EN | PWR_SDADC0_PowerVoltage_Key;
	}
	PWR->SDADC0PCR = tmpreg;
	if(NewState != DISABLE)
	{
		while(!(PWR->SDADC0PCR & PWR_SDADC0PCR_RDY));
	}	
}
#endif

/**
  * @brief  Enable or disable the SDADC0's external LPF capacitor
  * @param  NewState: new state of the SDADC0's power.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void PWR_SDADC0_PowerCapModeConfig(FunctionalState NewState)
{
	u32 tmpreg;

	tmpreg = PWR->SDADC0PCR & ~PWR_SDADC0PCR_PCMS;
	if(NewState != DISABLE)
	{
		tmpreg |= PWR_SDADC0PCR_PCMS | PWR_SDADC0_PowerVoltage_Key;
	}
	PWR->SDADC0PCR = tmpreg;
}

/**
  * @brief	Configure power voltage selection for the SDADC0.
  * @param	voltage: specifies the SDADC0's power voltage. 
  *			This parameter can be one of the following values:
  *			@arg PWR_SDADC0_PowerVoltage_2p2V: 2.2V is selected for the SDADC0's power voltage
  *			@arg PWR_SDADC0_PowerVoltage_2p4V: 2.4V is selected for the SDADC0's power voltage(default)
  *			@arg PWR_SDADC0_PowerVoltage_2p7V: 2.7V is selected for the SDADC0's power voltage
  *			@arg PWR_SDADC0_PowerVoltage_3p0V: 3.0V is selected for the SDADC0's power voltage
  *			@arg PWR_SDADC0_PowerVoltage_3p3V: 3.3V is selected for the SDADC0's power voltage
  *			@arg PWR_SDADC0_PowerVoltage_3p6V: 3.6V is selected for the SDADC0's power voltage
  *			@arg PWR_SDADC0_PowerVoltage_4p2V: 4.2V is selected for the SDADC0's power voltage
  *			@arg PWR_SDADC0_PowerVoltage_4p5V: 4.5V is selected for the SDADC0's power voltage
  * @retval None
  */
void PWR_SDADC0_PowerVoltageConfig(u32 Voltage)
{
	u32 tmpreg;

	/* Check the parameters */
	assert_param(IS_PWR_SDADC0_PowerVoltage(Voltage));

	tmpreg = PWR->SDADC0PCR & ~PWR_SDADC0PCR_PVS;
	tmpreg |= Voltage | PWR_SDADC0_PowerVoltage_Key;
	PWR->SDADC0PCR = tmpreg;
}

/**
  * @brief  Enters Sleep mode.
  * @note   In Sleep mode, all I/O pins keep the same state as in Run mode.
  * @param  PWR_SLEEPEntry: specifies if SLEEP mode in entered with WFI or WFE instruction.
  *			This parameter can be one of the following values:
  *			@arg PWR_SleepEntry_WFI: enter SLEEP mode with WFI instruction
  *			@arg PWR_SleepEntry_WFE: enter SLEEP mode with WFE instruction
  * @retval None
  */
void PWR_EnterSleepMode(u8 PWR_SleepEntry)
{
	/* Check the parameters */
	assert_param(IS_PWR_SleepEntry(PWR_SleepEntry));
	/* Clear SLEEPDEEP bit of Cortex-M0 System Control Register */
	SCB->SCR &= (u32)~((u32)SCB_SCR_SLEEPDEEP_Msk);

	/* Select SLEEP mode entry */
	if(PWR_SleepEntry == PWR_SleepEntry_WFI)
	{
		/* Request Wait For Interrupt */
		__WFI();
	}
	else
	{
		/* Request Wait For Event */
		__SEV();
		__WFE();
		__WFE();
	}
}

/**
  * @brief  Enters DeepSleep mode.
  * @note   In DeepSleep mode, all I/O pins keep the same state as in Run mode.
  *			- The registers of the Core-M0, the memory information is still saved.
  * 		@param  PWR_DeepSleepEntry: specifies if DeepSlee mode in entered with WFI or WFE instruction.
  *			This parameter can be one of the following values:
  *			@arg PWR_DeepSleepEntry_WFI: enter DEEPSLEEP mode with WFI instruction
  *			@arg PWR_DeepSleepEntry_WFE: enter DEEPSLEEP mode with WFE instruction
  * @retval None
  */
void PWR_EnterDeepSleepMode(u8 PWR_DeepSleepEntry)
{
	/* Check the parameters */
	assert_param(IS_PWR_DeepSleepEntry(PWR_DeepSleepEntry));
	/* Set SLEEPDEEP bit of Cortex-M0 System Control Register */
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

	/* Select DEEPSLEEP mode entry */
	if(PWR_DeepSleepEntry == PWR_DeepSleepEntry_WFI)
	{
		/* Request Wait For Interrupt */
		__WFI();
	}
	else
	{
		/* Request Wait For Event */
		__WFE();	// Clear the event register (if any)
		__WFE();	// Enter Deepsleep when the event register is 0.
	}
}
