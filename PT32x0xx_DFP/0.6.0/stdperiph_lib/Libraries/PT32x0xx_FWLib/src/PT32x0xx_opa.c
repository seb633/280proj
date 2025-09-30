/**
	******************************************************************************
  * @file    PT32x0xx_opa.c
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the OPA firmware
  *          library
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of
  * the content or the codes within the content.
  *
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  *****************************************************************************
**/

#include "PT32x0xx_opa.h"

/** @defgroup OPA
  * @brief OPA driver modules
  * @{
  */
  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define OPA_TrimKey										(0x000AE0000)
#define OPA_FactoryKey									(0x000C50000)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief 	Enable or disables the specified OPA peripheral.
  * @param 	OPAx: where x can be 0 to 3 to select the OPA peripheral.
  * @param 	NewState: new state of the OPAx peripheral.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void OPA_Cmd(OPA_TypeDef *OPAx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_OPA_ALL_PERIPH(OPAx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the selected OPA peripheral */
		OPAx->CR1 |= OPA_CR1_EN;
	}
	else
	{
		/* Disable the selected OPA peripheral */
		OPAx->CR1 &= ~OPA_CR1_EN;
	}
}

/**
  * @brief 	Configure the PGA mode common mode voltage signal for the specified OPA peripheral.
  * @param 	OPAx: where x can be 0 to 3 to select the OPA peripheral.
  * @param  VALUE: the mode of the PGA VCM.
  *        	This parameter can be one of the following values:
  * 		@arg PGA_VCM_VDDA: select the 1/2 VDDA as signal.
  * 		@arg PGA_VCM_BG1V0: select the BG1V0 as signal.
  * 		@arg PGA_VCM_External: select the External input as signal.
  * @retval None
  */
void OPA_PGAVcmConfig(OPA_TypeDef *OPAx, u32 Value)
{
	u32 tmpreg;
	
	assert_param(IS_OPA_ALL_PERIPH(OPAx));
	assert_param(IS_PGA_VCMConfig(Value));
	
	tmpreg = OPAx->CR1 & ~OPA_CR1_VCMS;
	tmpreg |= Value;
	OPAx->CR1 = tmpreg;
}

/**
  * @brief 	Configure the power mode for the specified OPA peripheral.
  * @param 	OPAx: where x can be 0 to 3 to select the OPA peripheral.
  * @param  Mode: OPA power mode.
  *       	This parameter can be one of the following values:
  * 		@arg OPA_PowerMode_LowerPower: select the LowerPower mode.
  * 		@arg OPA_PowerMode_Normal: select the Normal mode.
  * 		@arg OPA_PowerMode_HighSpeed: select the HighSpeed mode.
  * @retval None
  */
void OPA_PowerModeConfig(OPA_TypeDef *OPAx, u32 Mode)
{
	u32 tmpreg;
	
	assert_param(IS_OPA_ALL_PERIPH(OPAx));
	assert_param(IS_OPA_PowerMode(Mode));
	
	tmpreg = OPAx->CR1 & ~OPA_CR1_PM;
	tmpreg |= Mode;
	OPAx->CR1 = tmpreg;
}

/**
  * @brief 	Configure the PGA programmable amplifier gain value for the specified OPA peripheral.
  * @param 	OPAx: where x can be 0 to 3 to select the OPA peripheral.
  * @param 	gain: specifies the OPA's PGA mode gain.
  *			This parameter can be one of the following values:
  *			@arg OPA_PgaGain_Disable: Disable the specified OPA' PGA mode.
  *			@arg OPA_PgaGain_x1: PGA programmable amplifier gain value is 1.
  *			@arg OPA_PgaGain_x2: PGA programmable amplifier gain value is 2.
  *			@arg OPA_PgaGain_x5: PGA programmable amplifier gain value is 5.
  *			@arg OPA_PgaGain_x10: PGA programmable amplifier gain value is 10.
  * @retval None
  */
void OPA_PGAGainConfig(OPA_TypeDef *OPAx, u32 Gain)
{
	u32 tmpreg;
	
	assert_param(IS_OPA_ALL_PERIPH(OPAx));
	assert_param(IS_OPA_PgaGain(Gain));
	
	OPAx->CR1 &= ~OPA_CR1_PGA;
	if(Gain != OPA_PgaGain_Disable)
	{
		tmpreg = OPAx->CR1 & ~OPA_CR1_GAIN;
		tmpreg |= OPA_PGA_Enable | ((Gain - 1) << 12);
		OPAx->CR1 = tmpreg;
	}
}

/**
  * @brief 	Enable or disable positive inputs for the specified OPA peripheral.
  * @param 	OPAx: where x can be 0 to 3 to select the OPA peripheral.
  * @param 	inputs: specifies the OPA's positive inputs.
  *			This parameter can be one or more of the following values:
  *			@arg OPA_PositiveInput_0: Enable or disable OPA' positive input 0(OPAx_P0).
  *			@arg OPA_PositiveInput_1: Enable or disable OPA' positive input 1(OPAx_P1).
  *			@arg OPA_PositiveInput_2: Enable or disable OPA' positive input 2(OPAx_P2).
  * @param 	NewState: new state of the OPA's positive inputs.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void OPA_PositiveInputsConfig(OPA_TypeDef *OPAx, u32 Inputs, FunctionalState NewState)
{
	assert_param(IS_OPA_ALL_PERIPH(OPAx));
	assert_param(IS_OPA_PositiveInput(Inputs));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if(NewState != DISABLE)
	{
		OPAx->CR1 |= Inputs;
	}
	else
	{
		OPAx->CR1 &= ~Inputs;
	}
}

/**
  * @brief 	Enable or disable negative inputs for the specified OPA peripheral.
  * @param 	OPAx: where x can be 0 to 3 to select the OPA peripheral.
  * @param 	inputs: specifies the OPA's negative inputs.
  *			This parameter can be one or more of the following values:
  *			@arg OPA_NegativeInput_0: Enable or disable OPA' negative input 0(OPAx_N0).
  *			@arg OPA_NegativeInput_1: Enable or disable OPA' negative input 1(OPAx_N1).
  *			@arg OPA_NegativeInput_2: Enable or disable OPA' negative input 2(OPAx_N2).
  *			@arg OPA_NegativeInput_3: Enable or disable OPA' negative input 3(OPAx_N3).
  * @param 	NewState: new state of the OPA's negative inputs.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void OPA_NegativeInputsConfig(OPA_TypeDef *OPAx, u32 Inputs, FunctionalState NewState)
{
	assert_param(IS_OPA_ALL_PERIPH(OPAx));
	assert_param(IS_OPA_NegativeInput(Inputs));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if(NewState != DISABLE)
	{
		OPAx->CR1 |= Inputs;
	}
	else
	{
		OPAx->CR1 &= ~Inputs;
	}
}

/**
  * @brief 	Enable or disable outputs for the specified OPA peripheral.
  * @param 	OPAx: where x can be 0 to 3 to select the OPA peripheral.
  * @param 	outputs: specifies the OPA's output.
  *			This parameter can be one or more of the following values:
  *			@arg OPA_Output_0: Enable or disable OPA' output 0(OPAx_OUT0).
  *			@arg OPA_Output_1: Enable or disable OPA' output 1(OPAx_OUT1).
  *			@arg OPA_Output_2: Enable or disable OPA' output 2(OPAx_OUT2).
  *			@arg OPA_Output_3: Enable or disable OPA' output 3(OPAx_OUT3).
  * @param 	NewState: new state of the OPA's outputs.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void OPA_OutputsConfig(OPA_TypeDef *OPAx, u32 Outputs, FunctionalState NewState)
{
	assert_param(IS_OPA_ALL_PERIPH(OPAx));
	assert_param(IS_OPA_Output(Outputs));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if(NewState != DISABLE)
	{
		OPAx->CR1 |= Outputs;
	}
	else
	{
		OPAx->CR1 &= ~Outputs;
	}
}

/**
  * @brief 	Enable or disable virtual ground for the specified OPA peripheral.
  * @param 	OPAx: where x can be 0 to 3 to select the OPA peripheral.
  * @param 	NewState: new state of the OPA's outputs.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void OPA_VGndCmd(OPA_TypeDef *OPAx, FunctionalState NewState)
{
	assert_param(IS_OPA_ALL_PERIPH(OPAx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if(NewState != DISABLE)
	{
		OPAx->CR1 |= OPA_CR1_VGE;
	}
	else
	{
		OPAx->CR1 &= ~OPA_CR1_VGE;
	}
}

/**
  * @brief 	Configure the calibration mode for the specified OPA peripheral.
  * @param 	OPAx: where x can be 0 to 3 to select the OPA peripheral.
  *			Mode: OPA calibration mode.
  *			This parameter can be one of the following values:
  *			@arg OPA_CalibrationMode_Disable: Disable the specified OPA' calibration mode.
  *			@arg OPA_CalibrationMode_Manual: Enable the manual calibration mode for the specified OPA.
  *			@arg OPA_CalibrationMode_Auto: Enable the auto calibration mode for the specified OPA.
  * @retval None
  */
void OPA_CalibrationModeConfig(OPA_TypeDef *OPAx, u32 Mode)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_OPA_ALL_PERIPH(OPAx));
	assert_param(IS_OPA_CalibrationMode(Mode));
	
	tmpreg = OPAx->CR2 & ~OPA_CR2_CALE;
	tmpreg |= Mode;
	OPAx->CR2 = tmpreg;
}

/**
  * @brief 	Configure the calibration mode for the specified OPA's auto calibration.
  * @param 	OPAx: where x can be 0 to 3 to select the OPA peripheral.
  *			Mode: OPA's auto calibration mode.
  *			This parameter can be one of the following values:
  *			@arg OPA_AutoCalibrationMode_Normal: Normal auto calibration is selected.
  *			@arg OPA_AutoCalibrationMode_HighSpeed: High speed auto calibration is selected.
  * @retval None
  */
void OPA_AutoCalibrationModeConfig(OPA_TypeDef *OPAx, u32 Mode)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_OPA_ALL_PERIPH(OPAx));
	assert_param(IS_OPA_AutoCalibrationMode(Mode));
	
	tmpreg = OPAx->CR2 & ~OPA_CR2_ACMS;
	tmpreg |= Mode;
	OPAx->CR2 = tmpreg;
}

/**
  * @brief 	Configure the offset trimming value for the specified OPA peripheral.
  * @param 	OPAx: where x can be 0 to 3 to select the OPA peripheral.
  * @param  NewState: new state of the OPAx peripheral.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void OPA_OffsetTrimValueConfig(OPA_TypeDef *OPAx, u32 Value)
{
	/* Check the parameters */
	assert_param(IS_OPA_ALL_PERIPH(OPAx));
	assert_param(IS_OPA_OffsetTrimValue(Value));

	OPAx->OTR = Value | OPA_TrimKey;
}

/**
  * @brief 	Reset the offset trimming value to factory mode
  * @param 	OPAx: where x can be 0 to 3 to select the OPA peripheral.
  * @retval None
  */
void OPA_OffsetTrimValueReset(OPA_TypeDef *OPAx)
{
	/* Check the parameters */
	assert_param(IS_OPA_ALL_PERIPH(OPAx));
	
	OPAx->OTR = OPA_FactoryKey;
}

/**
  * @brief  Get OPA calibration output
  * @param  OPAx:OPA Number
  * @retval The value of OPA calibration output.
  */
u8 OPA_GetCalibrationOutput(OPA_TypeDef *OPAx)
{
	/* Check the parameters */
	assert_param(IS_OPA_ALL_PERIPH(OPAx));

	if(OPAx->OTR & OPA_OTR_CAL)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
  * @brief 	Get OPA Offset trimming value source flag
  * @param  OPAx: where x can be 0 to 3 to select the OPA peripheral.
  * @retval The new state of OPA_FLAG (SET or RESET).
  */
FlagStatus OPA_GetOffsetTrimSourceStatus(OPA_TypeDef *OPAx)
{
	FlagStatus bitstatus = RESET;
	/* Check the parameters */
	assert_param(IS_OPA_ALL_PERIPH(OPAx));
	
	/* Check the status of the specified OPA flag */
	if((OPAx->OTR & OPA_OTR_OTF) != (u32)RESET)
	{
		bitstatus = SET;
	}
	else
	{
		bitstatus = RESET;
	}

	return  bitstatus;
}
