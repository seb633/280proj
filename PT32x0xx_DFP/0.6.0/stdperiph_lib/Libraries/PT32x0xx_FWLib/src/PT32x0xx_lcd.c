/******************************************************************************
  * @file    PT32x0xx_lcd.c
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the LCD firmware library
  *
  ******************************************************************************
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.  
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of
  * the content or the codes within the content.  
  * 
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved 
  *******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "PT32x0xx_lcd.h"

/** @defgroup LCD
  * @brief LCD driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the LCD peripheral according to the specified parameters in the LCD_InitStruct.
  * @note   This function is used to configure the global features of the LCD
  * @param  LCD_InitStruct: pointer to an LCD_InitTypeDef structure that contains
  *         the configuration information for the specified LCD peripheral.
  * @retval None
  */
void LCD_Init(LCD_TypeDef *LCDx, LCD_InitTypeDef *LCD_InitStruct)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_LCD_ALL_PERIPH(LCDx));
	assert_param(IS_LCD_DutyCycle(LCD_InitStruct->LCD_Duty));
	assert_param(IS_LCD_WAVE(LCD_InitStruct->LCD_Wave));
	assert_param(IS_LCD_PageDisplay(LCD_InitStruct->LCD_PageDisplayMode));
	assert_param(IS_LCD_ScreenControl(LCD_InitStruct->LCD_ScreenControlMode));
	assert_param(IS_LCD_DriveVoltage(LCD_InitStruct->LCD_DriveVoltageType));
	assert_param(IS_LCD_DriveMode(LCD_InitStruct->LCD_DriveMode));
	assert_param(IS_LCD_TotalResistance(LCD_InitStruct->LCD_TotalResistance));
	
	tmpreg = LCDx->CR2 & ~(LCD_CR2_DUTY | LCD_CR2_WAVE | LCD_CR2_PDM | LCD_CR2_SCM |\
							LCD_CR2_TYPE | LCD_CR2_CSEL | LCD_CR2_RSEL);
	tmpreg |= LCD_InitStruct->LCD_Duty |
				LCD_InitStruct->LCD_Wave |
				LCD_InitStruct->LCD_PageDisplayMode |
				LCD_InitStruct->LCD_ScreenControlMode |
				LCD_InitStruct->LCD_DriveVoltageType |
				LCD_InitStruct->LCD_DriveMode |
				LCD_InitStruct->LCD_TotalResistance;
	/* Write to CR2 register*/
	LCDx->CR2 = tmpreg;
}

/**
  * @brief  Fills each LCD_InitStruct member with its default value.
  * @note   This function is used to initialize the global features of the LCD
  * @param  LCD_InitStruct: pointer to an LCD_InitTypeDef structure which will be initialized.
  * @retval None
  */
void LCD_StructInit(LCD_InitTypeDef *LCD_InitStruct)
{
	/* Set the default configuration */
	LCD_InitStruct->LCD_Duty = LCD_Duty_4;		                       
	LCD_InitStruct->LCD_Wave = LCD_Wave_A;			                   
	LCD_InitStruct->LCD_PageDisplayMode = LCD_PageDisplayMode_Fixed;	
	LCD_InitStruct->LCD_ScreenControlMode = LCD_ScreenControlMode_ON;  
	LCD_InitStruct->LCD_DriveVoltageType = LCD_DriveVoltageType_R;		
	LCD_InitStruct->LCD_DriveMode = LCD_DriveMode_1;		           
	LCD_InitStruct->LCD_TotalResistance = LCD_TotalResistance_360k;   
}

/**
  * @brief  Enable or disable the LCD peripheral.
  * @param  NewState: new state of the LCD peripheral.
  *        	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void LCD_Cmd(LCD_TypeDef *LCDx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_LCD_ALL_PERIPH(LCDx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState == ENABLE)
	{
		/* Enable the LCD */
		LCDx->CR1 |= LCD_CR1_EN;
	}
	else
	{
		/* Disable the LCD */
		LCDx->CR1 &= (~LCD_CR1_EN);
	}

	while(LCDx->CR1 & LCD_CR1_BSY);
}

/**
  * @brief  Enable or disable the LCD Data Update
  * @param  NewState: new state of the LCD Data Update
  *        	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void LCD_UpdataCmd(LCD_TypeDef *LCDx, FunctionalState NewState)
{
	assert_param(IS_LCD_ALL_PERIPH(LCDx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	while(LCDx->CR1 & LCD_CR1_BSY);
	/* Set the update bit */
	LCDx->CR1 |= LCD_CR1_DU;
	while(LCDx->CR1 & LCD_CR1_DU);
}

/**
  * @brief  Get the status of the register busy flag
  * @retval The new state of BSY
  */
FlagStatus LCD_GetBusyStatus(LCD_TypeDef *LCDx)
{
	FlagStatus bitstatus;
	
	assert_param(IS_LCD_ALL_PERIPH(LCDx));

	if((LCDx->CR1 & LCD_CR1_BSY) != RESET)
	{
		bitstatus = SET;
	}
	else
	{
		bitstatus = RESET;
	}

	return bitstatus;
}

/**
  * @brief  Configure the page display mode for the specified LCD peripheral.
  * @param  Dispaly_Mode: the selection of page display mode to configure.
  *        	This parameter can be one of the following values:
  *        	@arg LCD_PageDisplayMode_Fixed: set the page display mode as fixed mode.
  *        	@arg LCD_PageDisplayMode_Round: set the page display mode as round mode.
  *        	@arg LCD_PageDisplayMode_RoundOFF: set the page display mode as round off mode.
  *       	@arg LCD_PageDisplayMode_Continue: set the page display mode as continue mode.
  * @retval None
  */
void LCD_PageDisplayModeConfig(LCD_TypeDef *LCDx, u32 DispalyMode)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_LCD_ALL_PERIPH(LCDx));
	assert_param(IS_LCD_PageDisplay(DispalyMode));
	/* Get the LCDx->CR2 value and Clear PDM bits */
	tmpreg = LCDx->CR2 & ~LCD_CR2_PDM;
	/* Set PDM bit according to DispalyMode value */
	tmpreg |= DispalyMode;
	/* Write to CR2 register*/
	LCDx->CR2 = tmpreg;
}

/**
  * @brief  Set the LCD screen control mode.
  * @param  mode: specifies the LCD screen control mode.
  *        	This parameter can be any combination of the following values:
  *        	@arg LCD_ScreenControlMode_None: Normal display mode
  *        	@arg LCD_ScreenControlMode_Blink: Flicker display mode
  *        	@arg LCD_ScreenControlMode_ON: Full light display mode
  *        	@arg LCD_ScreenControlMode_OFF: All out display mode
  * @retval None
  */
void LCD_ScreenControlModeConfig(LCD_TypeDef *LCDx, u32 Mode)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_LCD_ALL_PERIPH(LCDx));
	assert_param(IS_LCD_ScreenControl(Mode));
	/* Get the LCDx->CR2 value and Clear SCM bits */
	tmpreg = LCDx->CR2 & ~LCD_CR2_SCM;
	/* Set SCM bit according to Mode value */
	tmpreg |= Mode;
	/* Write to CR2 register*/
	LCDx->CR2 = tmpreg ;
}

/**
  * @brief  Enable or disable the LCD grayscale control.
  * @param  NewState: new state of the LCD grayscale control.
  *        	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void LCD_GrayscaleCmd(LCD_TypeDef *LCDx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_LCD_ALL_PERIPH(LCDx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState == ENABLE)
	{
		/* Enable the grayscale control */
		LCDx->GCR |= LCD_GCR_EN;
	}
	else
	{
		/* Disable the grayscale control */
		LCDx->GCR &= ~LCD_GCR_EN;
	}
}

/**
  * @brief  Configure for the LCD Grey Value
  * @param  Gray_Rate: the LCD Grey Value to configure.
  *        	@arg IS_LCD_Grayscale(x)  The value of x that satisfies the condition
  * @retval None
  */
void LCD_GrayscaleConfig(LCD_TypeDef *LCDx, u16 Grayscale)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_LCD_ALL_PERIPH(LCDx));
	assert_param(IS_LCD_Grayscale(Grayscale));
	
	tmpreg = LCDx->GCR & ~LCD_GCR_GDR;
	tmpreg |= Grayscale;
	LCDx->GCR = tmpreg;
}

/**
  * @brief  Configure for the LCD Prescaler Value
  * @param  Prescaler: the LCD Prescaler Value to configure.
  * @retval NONE
  */
void LCD_SetPrescaler(LCD_TypeDef *LCDx, u32 Prescaler)
{
	/* Check the parameters */
	assert_param(IS_LCD_ALL_PERIPH(LCDx));
	assert_param(IS_LCD_Prescaler(Prescaler));
	/* Write to PSCR register*/
	LCDx->PSCR = Prescaler;
}

/**
  * @brief  Configure the frame frequency for the specified LCD peripheral.
  * @param  Duty: the selection of duty to configure.
  *        	This parameter can be one of the following values:
  *        	@arg LCD_Duty_2: set the duty as 1/2.
  *        	@arg LCD_Duty_4: set the duty as 1/4.
  *        	@arg LCD_Duty_6: set the duty as 1/6.
  *        	@arg LCD_Duty_8: set the duty as 1/8.
  * @param  Duty: the selection of frequency to configure.
  *       	This parameter can be a number between 25 and 120.
  * @retval LCD prescaler.
  */
void LCD_SetFrameRate(LCD_TypeDef *LCDx, u32 Duty, u32 FrameRate)
{
	u32 psc, clk, tmp, tmp_duty;
	/* Check the parameters */
	assert_param(IS_LCD_ALL_PERIPH(LCDx));
	assert_param(IS_LCD_DutyCycle(Duty));
	assert_param(IS_LCD_FrameRate(FrameRate));

	tmp_duty = (Duty + 1) * 2;
	tmp = 2 * FrameRate * tmp_duty;
	
	if((RCC->PCSCR2 & RCC_PCSCR1_LCD) == RCC_LCD_CLKSRC_LSI)
	{
		clk = GetClockFreq(CLKSRC_LSI) - tmp;
	}
	else
	{
		clk = GetClockFreq(CLKSRC_LSE) - tmp;
	}

	if(Duty == LCD_Duty_2 || Duty == LCD_Duty_4)
	{
		tmp *= 8;                        //psc=(LSI-2*Freq*Duty)/(16*Freq*Duty)
	}
	else
	{
		tmp *= 4;                        //psc=(LSI-2*Freq*Duty)/(8*Freq*Duty)
	}

	psc = (clk + tmp/2) / tmp;

	if(psc > 31)
	{
		psc = 31;
	}

	LCDx->PSCR = psc;
}

/**
  * @brief  Configure for the LCD Blink Time Value
  * @param  Blink Time: the LCD Blink Time Value to configure.
  * @retval NONE
  */
void LCD_SetBlinkTime(LCD_TypeDef *LCDx, u32 Time)
{
	/* Check the parameters */
	assert_param(IS_LCD_ALL_PERIPH(LCDx));
	assert_param(IS_LCD_BlinkTime(Time));
	
	/* Write to BTCR register*/
	LCDx->BTCR = Time;
}

/**
  * @brief  Configure for the LCD Blinking Lighting Time Value
  * @param  Blinking Lighting Time: the LCD Blinking Lighting Time Value to configure.
  * @retval NONE
  */
void LCD_SetLightingTime(LCD_TypeDef *LCDx, u32 Time)
{
	/* Check the parameters */
	assert_param(IS_LCD_ALL_PERIPH(LCDx));
	assert_param(IS_LCD_LightTime(Time));
	
	/* Write to BONCR register*/
	LCDx->BONCR = Time;
}

/**
  * @brief  Configure for the LCDx Pin Enable
  * @param  LCDx PIN ENABLE: the LCDx PIN to configure.
  * @param  NewState: new state of the USARTx peripheral.
  *        	This parameter can be: ENABLE or DISABLE.
  * @retval NONE
  */
void LCD_PinConfig(LCD_TypeDef *LCDx, u64 LCD_Pin, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_LCD_ALL_PERIPH(LCDx));
	assert_param(IS_LCD_PIN(LCD_Pin));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		LCDx->PCR1 |= (u32)(LCD_Pin & LCD_PCR1_PE);
		LCDx->PCR2 |= (u32)((LCD_Pin >> 32) & LCD_PCR2_PE);
	}
	else
	{
		LCDx->PCR1 &= ~((u32)(LCD_Pin & LCD_PCR1_PE));
		LCDx->PCR2 &= ~((u32)((LCD_Pin >> 32) & LCD_PCR2_PE));
	}
}

/**
  * @brief  Configure for the LCDx Pin Enable
  * @param  LCDx PIN: select the LCDx PIN to configure.
  * @retval NONE
  */
void LCD_PinDriverConfig(LCD_TypeDef *LCDx, u64 LCD_Pin, u8 Driver)
{
	/* Check the parameters */
	assert_param(IS_LCD_ALL_PERIPH(LCDx));
	assert_param(IS_LCD_PIN(LCD_Pin));
	assert_param(IS_LCD_DRIVER(Driver));

	if(Driver == LCD_DRIVER_COM)
	{
		LCDx->PDCR1 |= (u32)(LCD_Pin & LCD_PDCR1_PDC);
		LCDx->PDCR2 |= (u32)((LCD_Pin >> 32) & LCD_PDCR2_PDC);
	}

	else
	{
		LCDx->PDCR1 &= ~((u32)(LCD_Pin & LCD_PDCR1_PDC));
		LCDx->PDCR2 &= ~((u32)((LCD_Pin >> 32) & LCD_PDCR2_PDC));
	}
}

/**
  * @brief  Configure for the LCDx Page display time
  * @param  Display_Time: the LCDx Display_Time to configure.
  * @retval NONE
  */
void LCD_SetPageDisplayTime(LCD_TypeDef *LCDx, u16 DisplayTime)
{
	/* Check the parameters */
	assert_param(IS_LCD_ALL_PERIPH(LCDx));
	assert_param(IS_LCD_DisplayTime(DisplayTime));
	
	/* Write to PDTR register*/
	LCDx->PDTR = DisplayTime;
}

/**
  * @brief  Configure for the LCDx Scroll display max page
  * @param  Max_Page: the LCDx Scroll display max page to configure.
  * @retval NONE
  */
void LCD_SetScrollDisplayPage(LCD_TypeDef *LCDx, u8 MaxPage)
{
	/* Check the parameters */
	assert_param(IS_LCD_ALL_PERIPH(LCDx));
	assert_param(IS_LCD_ScrollDisplayPage(MaxPage));
	
	/* Write to SDPR register*/
	LCDx->SDPR = MaxPage;
}

/**
  * @brief  Configure for the LCDx Scroll display max page
  * @param  Max_Page: the LCDx Scroll display max page to configure.
  * @retval NONE
  */
void LCD_PageNumberConfig(LCD_TypeDef *LCDx, u8 PageNumber)
{
	/* Check the parameters */
	assert_param(IS_LCD_ALL_PERIPH(LCDx));
	assert_param(IS_LCD_PageNumber(PageNumber));
	
	/* Write to PNR register*/
	LCDx->PNR = PageNumber;
}

/**
  * @brief  Configure the reference voltage as user mode for the specified LCD peripheral.
  * @param  Value: the selection of frequency to configure.
  *       	This parameter can be a number between 0 and 15.
  * @retval None
  */
void LCD_VrefUserModeConfig(LCD_TypeDef *LCDx, u8 Value)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_LCD_ALL_PERIPH(LCDx));
	assert_param(IS_LCD_VrefUserValue(Value));

	tmpreg = LCDx->VREFCR & ~(LCD_VREFCR_RS | LCD_VREFCR_KEY);
	tmpreg |= Value | LCD_VrefKey_User;
	/* Write to VREFCR register*/
	LCDx->VREFCR = tmpreg;
}

/**
  * @brief  Configure the reference voltage as factory mode for the specified LCD peripheral.
  * @param  None
  * @retval None
  */
void LCD_VrefFactoryModeConfig(LCD_TypeDef *LCDx)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_LCD_ALL_PERIPH(LCDx));
	
	tmpreg = LCDx->VREFCR & ~LCD_VREFCR_KEY;
	/* Reset to factory mode */
	tmpreg |= LCD_VrefKey_Factory;
	/* Write to VREFCR register*/
	LCDx->VREFCR = tmpreg;
}

/**
  * @brief	Clears the LCD Page Buff for specifies the Page to clear.
  * @param  PageNum
  *        	@arg IS_LCD_Page(x)  The value of x that satisfies the condition
  * @retval None
  */
void LCD_ClearPage(LCD_TypeDef *LCDx, u32 PageNum)
{
	u8 j;
	/* Check the parameters */
	assert_param(IS_LCD_ALL_PERIPH(LCDx));
	assert_param(IS_LCD_Page(PageNum));

	for(j = 0; j < LCD_PAGE_REGS; j++)
	{
		LCDRAM[PageNum][j] = 0;
	}
}

/**
  * @brief  Clear the LCD all pages buff.
  * @param  None
  * @retval None
  */
void LCD_ClearAllPages(LCD_TypeDef *LCDx)
{
	u8 i, j;
	/* Check the parameters */
	assert_param(IS_LCD_ALL_PERIPH(LCDx));
	for(i = 0; i < LCD_MAX_PAGES; i++)
	{
		for(j = 0; j < LCD_PAGE_REGS; j++)
		{
			LCDRAM[i][j] = 0;
		}
	}
}

/**
  * @brief	Set the LCD Page Buff for specifies the Page to set.
  * @param  LCD_PinSource
  *        	@arg IS_PinSource(x)  The value of x that satisfies the condition
  * @param  LCD_COM
  *        	@arg IS_LCD_COM(x)  The value of x that satisfies the condition
  * @param  NewState: new state of the USARTx peripheral.
  *       	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void LCD_SetDisplayCOM(LCD_TypeDef *LCDx, u32 LCD_PinSource, u32 COM, FunctionalState NewState)
{
	u8 i = 0;

	/* Check the parameters */
	assert_param(IS_LCD_ALL_PERIPH(LCDx));
	assert_param(IS_LCD_PinSource(LCD_PinSource));
	assert_param(IS_LCD_COM(COM));

	if(NewState == ENABLE)
	{
		for(i = 0; i < LCD_MAX_PAGES; i++)
		{
			LCDRAM[i][LCD_PinSource] |= COM;
		}
	}
	else
	{
		for(i = 0; i < LCD_MAX_PAGES; i++)
		{
			LCDRAM[i][LCD_PinSource] &= ~COM;
		}
	}
}
