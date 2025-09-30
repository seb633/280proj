/**
	******************************************************************************
  * @file    PT32x0xx_sdadc.c
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the SDADC firmware
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

#include "PT32x0xx_sdadc.h"

/** @defgroup SDADC
  * @brief SDADC driver modules
  * @{
  */
	
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define SDADC_DataOffsetKey							((u32)0xCD000000)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static double sdadc_max_resolution[1] = {8388608.0};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief	Initializes the SDADCx peripheral according to
  *         the specified parameters in the SDADC_InitTypeDef.
  * @param	SDADCx: where x can be 0 to select the SDADC peripheral.
  * @param	SDADC_InitStruct: pointer to a SDADC_InitTypeDef
  *         structure that contains the configuration information for
  *         the specified SDADC peripheral.
  * @retval	None
  */
void SDADC_Init(SDADC_TypeDef *SDADCx, SDADC_InitTypeDef *SDADC_InitStruct)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_SDADC_ALL_PERIPH(SDADCx));
	assert_param(IS_SDADC_ChannelPositive(SDADC_InitStruct->SDADC_INP));
	assert_param(IS_SDADC_ChannelNegative(SDADC_InitStruct->SDADC_INN));
	assert_param(IS_SDADC_PgaGain(SDADC_PGA0, SDADC_InitStruct->SDADC_Pga0Gain));
	assert_param(IS_SDADC_PgaGain(SDADC_PGA1, SDADC_InitStruct->SDADC_Pga1Gain));
	assert_param(IS_FUNCTIONAL_STATE(SDADC_InitStruct->SDADC_Chop));
	assert_param(IS_SDADC_PgaChopperMode(SDADC_PGA0, SDADC_InitStruct->SDADC_Pga0ChopMode));
	assert_param(IS_SDADC_RVSPS(SDADC_InitStruct->SDADC_ReferencePositive));
	assert_param(IS_SDADC_Prescaler(SDADC_InitStruct->SDADC_Prescaler));
	assert_param(IS_SDADC_ChopClockDivider(SDADC_InitStruct->SDADC_ChopClockDiv));
	
	tmpreg = SDADCx->CFGR2 & ~(SDADC_CFGR2_CHPS | SDADC_CFGR2_CHNS);
	tmpreg |= SDADC_InitStruct->SDADC_INP | SDADC_InitStruct->SDADC_INN;
	SDADCx->CFGR2 = tmpreg;
	
	tmpreg = SDADCx->PGACR & ~(SDADC_PGACR_PGA0E | SDADC_PGACR_PGA1E | SDADC_PGACR_PCM0);
	if(SDADC_InitStruct->SDADC_Pga0Gain != SDADC_PgaGain_Disable)
	{
		tmpreg &= ~SDADC_PGACR_GAIN0;
		tmpreg |= SDADC_PGACR_PGA0E | ((SDADC_InitStruct->SDADC_Pga0Gain - 1) << 1);
	}
	if(SDADC_InitStruct->SDADC_Pga1Gain != SDADC_PgaGain_Disable)
	{
		tmpreg &= ~SDADC_PGACR_GAIN1;
		tmpreg |= SDADC_PGACR_PGA1E | ((SDADC_InitStruct->SDADC_Pga1Gain - 1) << 9);
	}
	SDADCx->PGACR = tmpreg | SDADC_InitStruct->SDADC_Pga0ChopMode;
	
	SDADCx->CR1 &= ~SDADC_CR1_CCS;
	if(SDADC_InitStruct->SDADC_Pga0ChopMode == SDADC_PgaChopMode_ACImpedance)
	{
		SDADCx->CR1 |= SDADC_ChopClock_IQ;
	}
	
	SDADCx->CCR1 &= ~SDADC_CCR1_CHOPE;
	if(SDADC_InitStruct->SDADC_Chop == ENABLE 
		&& SDADC_InitStruct->SDADC_Pga0ChopMode == SDADC_PgaChopMode_Disable)
	{
		SDADCx->CCR1 |= SDADC_CCR1_CHOPE;
	}
	
	tmpreg = SDADCx->CFGR1 & ~SDADC_CFGR1_RVSPS;
	tmpreg |= SDADC_InitStruct->SDADC_ReferencePositive;
	if(SDADC_InitStruct->SDADC_ReferencePositive == SDADC_RVSPS_BGREF)
	{
		tmpreg |= SDADC_CFGR1_BGE;
	}
	SDADCx->CFGR1 = tmpreg;
	
	
	tmpreg = SDADCx->CFGR3 & ~(SDADC_CFGR3_PSC | SDADC_CFGR3_CDIV);
	tmpreg |= SDADC_InitStruct->SDADC_Prescaler | SDADC_InitStruct->SDADC_ChopClockDiv;
	SDADCx->CFGR3 = tmpreg;
}

/**
  * @brief	Fills each SDADC_InitStruct member with its default value.
  * @param	SDADC_InitStruct: pointer to a SDADC_InitTypeDef structure which will be initialized.
  * @retval	None
  */
void SDADC_StructInit(SDADC_InitTypeDef *SDADC_InitStruct)
{
	SDADC_InitStruct->SDADC_INP = SDADC_INP_Vssa;
	SDADC_InitStruct->SDADC_INN = SDADC_INN_Vssa;
	SDADC_InitStruct->SDADC_Pga0Gain = SDADC_PgaGain_x1;
	SDADC_InitStruct->SDADC_Pga1Gain = SDADC_PgaGain_x1;
	SDADC_InitStruct->SDADC_Chop = DISABLE;
	SDADC_InitStruct->SDADC_Pga0ChopMode = SDADC_PgaChopMode_Disable;
	SDADC_InitStruct->SDADC_ReferencePositive = SDADC_RVSPS_BGREF;
	SDADC_InitStruct->SDADC_Prescaler = 46;
	SDADC_InitStruct->SDADC_ChopClockDiv = SDADC_ChopClock_Div8;	
}

/**
  * @brief	Enable or disable the specified SDADC peripheral.
  * @param	SDADCx: where x can be 0 to select the SDADC peripheral.
  * @param	NewState: new state of the SDADCx peripheral.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval	None
  */
void SDADC_Cmd(SDADC_TypeDef *SDADCx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_SDADC_ALL_PERIPH(SDADCx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the selected SDADC peripheral */
		SDADCx->CR1 |= SDADC_CR1_EN;
	}
	else
	{
		/* Disable the selected SDADC peripheral */
		SDADCx->CR1 &= ~SDADC_CR1_EN;
	}
}

/**
  * @brief	Configure the working mode for the specified SDADC peripheral.
  * @param	SDADCx: where x can be 0 to select the SDADC peripheral.
  * @param	Mode: specifies the SDADC working mode.
  *			This parameter can be one of the following values:
  *			@arg SDADC_WorkingMode_Normal: select the normal working mode
  *			@arg SDADC_WorkingMode_Calibration: select the calibration working mode
  * @retval None
  */
void SDADC_WorkModeConfig(SDADC_TypeDef *SDADCx, u32 Mode)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_SDADC_ALL_PERIPH(SDADCx));
	assert_param(IS_SDADC_WorkingMode(Mode));

	tmpreg = SDADCx->CR1 & ~SDADC_CR1_MODE;
	tmpreg |= Mode;
	SDADCx->CR1 = tmpreg;
}

/**
  * @brief	Configure the chopper clock source for the specified SDADC peripheral.
  * @param	SDADCx: where x can be 0 to select the SDADC peripheral.
  * @param	Source: specifies the SDADC's chopper clock source.
  *			This parameter can be one of the following values:
  *			@arg SDADC_ChopClock_Chop: clock source from SDADC
  *			@arg SDADC_ChopClock_IQ: clock source from DDS IQ demodulation signal
  * @retval None
  */
void SDADC_ChopperClockConfig(SDADC_TypeDef *SDADCx, u32 Source)
{
	u32 tmpreg = 0;
	
	/* Check the parameters */
	assert_param(IS_SDADC_ALL_PERIPH(SDADCx));
	assert_param(IS_SDADC_ChopClock(Source));

	tmpreg = SDADCx->CR1 & ~SDADC_CR1_CCS;
	tmpreg |= Source;
	SDADCx->CR1 = tmpreg;
}

/**
  * @brief  Enable or disable DMA tansfer for the specified SDADC peripheral.
  * @param  SDADCx: where x can be 0 to select the SDADC peripheral.
  * @param  NewState: new state of the SDADCx peripheral.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SDADC_DMACmd(SDADC_TypeDef *SDADCx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_SDADC_ALL_PERIPH(SDADCx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Set the DMAE bit to Enable the SDADC DMA transition*/
		SDADCx->CR2 |= SDADC_CR2_DMAE;
	}
	else
	{
		/* Clear the DMAE bit to Disable the SDADC DMA transition*/
		SDADCx->CR2 &= ~SDADC_CR2_DMAE;
	}
}

/**
  * @brief	Find the CIC filter data maximum output resolution factor for the specified SDADC OSR.
  * @param	OSR: specifies the SDADC's CIC filter oversampling ratio.
  *			This parameter must be a number between 32 to 65536.
  * @retval the CIC filter data maximum output resolution factor.
  */
u8 SDADC_FindFiterDRF(u32 OSR)
{
	static u16 sdadc_osr[] = {
		74,     // 2
		258,    // 3
		456,    // 4
		645,    // 5
		812,    // 6
		1024,   // 7
		1290,   // 8
		1625,   // 9
		2048,   // 10
		2580,   // 11
		3178,   // 12
		3896,   // 13
		4887,   // 14
		6337,   // 15
		8192,   // 16
		10321,  // 17
		13003,  // 18
		16070,  // 19
		20355,  // 20
		26007,  // 21
		32528,  // 22
		41285,  // 23
		51387   // 24
	};
	u8 right;
	u8 mid;
	u8 fdrf;
	
	assert_param(IS_SDADC_FilterOSR(OSR));
	
	right = sizeof(sdadc_osr)/2 - 1;
	fdrf = 0;
	while(fdrf <= right)
	{
		mid = (fdrf + right)/2;
		if(OSR <= sdadc_osr[mid])
		{
			if(mid == 0 || OSR > sdadc_osr[mid - 1])
			{
				fdrf = mid;
				break;
			}
			right = mid;
		}
		else if(OSR > sdadc_osr[mid])
		{
			if(mid == right || OSR <= sdadc_osr[mid + 1])
			{
				fdrf = mid + 1;
				break;
			}
			fdrf = mid + 1;
		}
	}
	
	return (fdrf + 2);
}

/**
  * @brief	Configure the CIC filter oversampling ratio for the specified SDADC peripheral.
  * @param	SDADCx: where x can be 0 to select the SDADC peripheral.
  * @param	OSR: specifies the SDADC's CIC filter oversampling ratio.
  *			This parameter must be a number between 32 to 65536.
  * @retval None
  */
void SDADC_FilterOSRConfig(SDADC_TypeDef *SDADCx, u32 OSR)
{
	u32 tmpreg;
	u32 fdrf;
	
	/* Check the parameters */
	assert_param(IS_SDADC_ALL_PERIPH(SDADCx));
	assert_param(IS_SDADC_FilterOSR(OSR));

	fdrf = SDADC_FindFiterDRF(OSR);
	
	tmpreg = SDADCx->CR4 & ~(SDADC_CR4_FOSR | SDADC_CR4_FDRF);
	tmpreg |= OSR | (fdrf << 24);
	SDADCx->CR4 = tmpreg;
	
	if(SDADCx == SDADC0)
	{
		sdadc_max_resolution[0] = (double)OSR;
		sdadc_max_resolution[0] = sdadc_max_resolution[0] * sdadc_max_resolution[0] * sdadc_max_resolution[0];
		sdadc_max_resolution[0] /= (double)(1 << fdrf);
	}
}

/**
  * @brief	Configure the reference voltage source positive selection for the specified SDADC peripheral.
  * @param	ADCx: where x can be 0 to select the SDADC peripheral.
  * @param	Source: specifies the reference voltage source positive selection.
  *			This parameter can be one of the following values:
  * 		@arg SDADC_RVSPS_BGREF: set BGREF as the reference voltage source positive selection
  *			@arg SDADC_RVSPS_VREFBUF: set VREFBUF as the reference voltage source positive selection
  * @retval None
  */
void SDADC_RVSPSConfig(SDADC_TypeDef *SDADCx, u32 Source)
{
	u32 tmpreg;
	
	assert_param(IS_SDADC_ALL_PERIPH(SDADCx));
	assert_param(IS_SDADC_RVSPS(Source));
	
	/* Get the SDADCx CFGR1 value and Clear RVSPS bits */
	tmpreg = SDADCx->CFGR1 & ~SDADC_CFGR1_RVSPS;
	/* Set RVSPS bit according to Source value */
	tmpreg |= Source;
	/* Write to CFGR1 register*/
	SDADCx->CFGR1 = tmpreg;
}

/**
  * @brief  Enable or disable BGREF for the specified SDADC peripheral.
  * @param  SDADCx: where x can be 0 to select the SDADC peripheral.
  * @param  NewState: new state of the SDADCx peripheral.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SDADC_BGREFCmd(SDADC_TypeDef *SDADCx, FunctionalState NewState)
{
	assert_param(IS_SDADC_ALL_PERIPH(SDADCx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		SDADCx->CFGR1 |= SDADC_CFGR1_BGE;
	}
	else
	{
		SDADCx->CFGR1 &= ~SDADC_CFGR1_BGE;
	}
}

/**
  * @brief  Enable or disable temperature sensor for the specified SDADC peripheral.
  * @param  SDADCx: where x can be 0 to select the SDADC peripheral.
  * @param  NewState: new state of the SDADCx peripheral.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SDADC_TempSensorCmd(SDADC_TypeDef *SDADCx, FunctionalState NewState)
{
	assert_param(IS_SDADC_ALL_PERIPH(SDADCx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		SDADCx->CFGR2 |= SDADC_CFGR2_TSE;
	}
	else
	{
		SDADCx->CFGR2 &= ~SDADC_CFGR2_TSE;
	}
}

/**
  * @brief	Configure the number of conversions to be Discarded for the specified SDADC peripheral.
  * @param	SDADCx: where x can be 0 to select the SDADC peripheral.
  * @param	value: specifies the number of conversions to be Discarded.
  *			This parameter must be a number between 0 to 7.
  * @retval None
  */
void SDADC_ConversionDiscardedConfig(SDADC_TypeDef *SDADCx, u32 value)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_SDADC_ALL_PERIPH(SDADCx));
	assert_param(IS_SDADC_ConversionDiscarded(value));

	tmpreg = SDADCx->CFGR2 & ~SDADC_CFGR2_DISC;
	tmpreg |= (value << 12);
	SDADCx->CFGR2 = tmpreg;
}

/**
  * @brief	Configure the channel positive and negative selection and for the specified SDADC peripheral.
  * @param	SDADCx: where x can be 0 to select the SDADC peripheral.
  * @param	INP: specifies the channel positive selection.
  *			This parameter can be one of the following values:
  *			@arg SDADC_Channel_P0: SDADC channel 0 positive selected
  *			@arg SDADC_Channel_P1: SDADC channel 1 positive selected
  *			@arg SDADC_Channel_P2: SDADC channel 2 positive selected
  *			@arg SDADC_Channel_P3: SDADC channel 3 positive selected
  *			@arg SDADC_Channel_P4: SDADC channel 4 positive selected
  *			@arg SDADC_Channel_P5: SDADC channel 5 positive selected
  *			@arg SDADC_Channel_P6: SDADC channel 6 positive selected
  *			@arg SDADC_Channel_P7: SDADC channel 7 positive selected
  *			@arg SDADC_Channel_P8: SDADC channel 8 positive selected
  *			@arg SDADC_Channel_P9: SDADC channel 9 positive selected
  *			@arg SDADC_Channel_P10: SDADC channel 10 positive selected
  *			@arg SDADC_Channel_P11: SDADC channel 11 positive selected
  *			@arg SDADC_Channel_P12: SDADC channel 12 positive selected
  *			@arg SDADC_Channel_P13: SDADC channel 13 positive selected
  *			@arg SDADC_Channel_P14: SDADC channel 14 positive selected
  *			@arg SDADC_Channel_P15: SDADC channel 15 positive selected
  *			@arg SDADC_Channel_P16: SDADC channel 16 positive selected
  *			@arg SDADC_Channel_P17: SDADC channel 17 positive selected
  *			@arg SDADC_Channel_P18: SDADC channel 18 positive selected
  *			@arg SDADC_Channel_P19: SDADC channel 19 positive selected
  *			@arg SDADC_Channel_P20: SDADC channel 20 positive selected
  *			@arg SDADC_Channel_P21: SDADC channel 21 positive selected
  *			@arg SDADC_Channel_P22: SDADC channel 22 positive selected
  *			@arg SDADC_Channel_P23: SDADC channel 23 positive selected
  *			@arg SDADC_Channel_P24: SDADC channel 24 positive selected
  *			@arg SDADC_Channel_P25: SDADC channel 25 positive selected
  *			@arg SDADC_Channel_P52: SDADC channel 52 positive(DAC1_OUT) selected
  *			@arg SDADC_Channel_P53: SDADC channel 53 positive(DAC0_OUT) selected
  *			@arg SDADC_Channel_P54: SDADC channel 54 positive(BGREF) selected
  *			@arg SDADC_Channel_P58: SDADC channel 58 positive(VDDA/6) selected
  *			@arg SDADC_Channel_P59: SDADC channel 59 positive(VSSA) selected
  *			@arg SDADC_Channel_P60: SDADC channel 60 positive(Temp Sensor Positive) selected
  * @param	INN: specifies the channel negative selection.
  *			This parameter can be one of the following values:
  *			@arg SDADC_Channel_N0: SDADC channel 0 negative selected
  *			@arg SDADC_Channel_N1: SDADC channel 1 negative selected
  *			@arg SDADC_Channel_N2: SDADC channel 2 negative selected
  *			@arg SDADC_Channel_N3: SDADC channel 3 negative selected
  *			@arg SDADC_Channel_N4: SDADC channel 4 negative selected
  *			@arg SDADC_Channel_N5: SDADC channel 5 negative selected
  *			@arg SDADC_Channel_N6: SDADC channel 6 negative selected
  *			@arg SDADC_Channel_N7: SDADC channel 7 negative selected
  *			@arg SDADC_Channel_N8: SDADC channel 8 negative selected
  *			@arg SDADC_Channel_N9: SDADC channel 9 negative selected
  *			@arg SDADC_Channel_N10: SDADC channel 10 negative selected
  *			@arg SDADC_Channel_N11: SDADC channel 11 negative selected
  *			@arg SDADC_Channel_N12: SDADC channel 12 negative selected
  *			@arg SDADC_Channel_N13: SDADC channel 13 negative selected
  *			@arg SDADC_Channel_N14: SDADC channel 14 negative selected
  *			@arg SDADC_Channel_N15: SDADC channel 15 negative selected
  *			@arg SDADC_Channel_N16: SDADC channel 16 negative selected
  *			@arg SDADC_Channel_N17: SDADC channel 17 negative selected
  *			@arg SDADC_Channel_N18: SDADC channel 18 negative selected
  *			@arg SDADC_Channel_N19: SDADC channel 19 negative selected
  *			@arg SDADC_Channel_N20: SDADC channel 20 negative selected
  *			@arg SDADC_Channel_N21: SDADC channel 21 negative selected
  *			@arg SDADC_Channel_N22: SDADC channel 22 negative selected
  *			@arg SDADC_Channel_N23: SDADC channel 23 negative selected
  *			@arg SDADC_Channel_N24: SDADC channel 24 negative selected
  *			@arg SDADC_Channel_N25: SDADC channel 25 negative selected
  *			@arg SDADC_Channel_N56: SDADC channel 55 negative(BG1v0) selected
  *			@arg SDADC_Channel_N56: SDADC channel 56 negative selected
  *			@arg SDADC_Channel_N59: SDADC channel 59 negative(VSSA) selected
  *			@arg SDADC_Channel_N60: SDADC channel 60 negative(Temp Sensor Negative) selected
  *			@arg SDADC_Channel_N61: SDADC channel 61 negative selected
  *			@arg SDADC_Channel_N62: SDADC channel 62 negative selected
  *			@arg SDADC_Channel_N63: SDADC channel 63 negative selected  
  * @retval None
  */
void SDADC_ChannelConfig(SDADC_TypeDef *SDADCx, u32 INP, u32 INN)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_SDADC_ALL_PERIPH(SDADCx));
	assert_param(IS_SDADC_ChannelPositive(INP));
	assert_param(IS_SDADC_ChannelNegative(INN));
	
	/* Get the SDADC CFGR2 value and Clear the SDADC channel selection bits */
	tmpreg = SDADCx->CFGR2 & ~(SDADC_CFGR2_CHPS | SDADC_CFGR2_CHNS);
	tmpreg |= INP | INN;
	/* Configure the SDADC channel */
	SDADCx->CFGR2 = tmpreg;
}

/**
  * @brief	Configure the prescaler for the specified SDADC peripheral.
  * @param	SDADCx: where x can be 0 to select the SDADC peripheral.
  * @param	Division: specifies the prescaler.
  *			This parameter can be a number between 1 and 127.
  * @retval None
  */
void SDADC_PrescalerConfig(SDADC_TypeDef *SDADCx, u32 Division)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_SDADC_ALL_PERIPH(SDADCx));
	assert_param(IS_SDADC_Prescaler(Division));
	
	/* Get the ADCx CFGR3 value and Clear PSC bits */
	tmpreg = SDADCx->CFGR3 & ~SDADC_CFGR3_PSC;
	/* Set PSC bit according to Division value */
	tmpreg |= Division;
	/* Write to CFGR3 register*/
	SDADCx->CFGR3 = tmpreg;
}

/**
  * @brief	Configure the chopper clock divider for the specified SDADC peripheral.
  * @param	SDADCx: where x can be 0 to select the SDADC peripheral.
  * @param	Division: specifies the divider for chopper clock.
  *			This parameter can be one of the following values:
  *			@arg SDADC_ChopClock_Div8: SDADC Clock divided by 8 selected
  *			@arg SDADC_ChopClock_Div16: SDADC Clock divided by 16 selected
  *			@arg SDADC_ChopClock_Div32: SDADC Clock divided by 32 selected
  *			@arg SDADC_ChopClock_Div64: SDADC Clock divided by 64 selected
  *			@arg SDADC_ChopClock_Div128: SDADC Clock divided by 128 selected
  *			@arg SDADC_ChopClock_Div256: SDADC SDADC Clock divided by 256 selected
  *			@arg SDADC_ChopClock_Div512: SDADC Clock divided by 512 selected
  *			@arg SDADC_ChopClock_Div1024: SDADC Clock divided by 1024 selected
  * @retval None
  */
void SDADC_ChopClockDividerConfig(SDADC_TypeDef *SDADCx, u32 Division)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_SDADC_ALL_PERIPH(SDADCx));
	assert_param(IS_SDADC_ChopClockDivider(Division));
	
	/* Get the ADCx CFGR3 value and Clear CDIV bits */
	tmpreg = SDADCx->CFGR3 & ~SDADC_CFGR3_CDIV;
	/* Set CDIV bit according to Division value */
	tmpreg |= Division;
	/* Write to CFGR3 register*/
	SDADCx->CFGR3 = tmpreg;
}

/**
  * @brief	Enable or disable the specified SDADC interrupts.
  * @param	SDADCx: where x can be 0 to select the SDADC peripheral.
  * @param	SDADC_IT: specifies the SDADC interrupt sources to be enabled or disabled. 
  *			This parameter can be one of the following values:
  *			@arg SDADC_IT_EOC: regular channel end of conversion interrupt
  * @param	NewState: new state of the specified SDADC interrupts.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval	None
  */
void SDADC_ITConfig(SDADC_TypeDef *SDADCx, u32 SDADC_IT, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_SDADC_ALL_PERIPH(SDADCx));
	assert_param(IS_SDADC_IT(SDADC_IT));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if(NewState != DISABLE)
	{
		/* Enable the selected SDADC interrupts */
		SDADCx->IER |= SDADC_IT;
	}
	else
	{
		/* Disable the selected SDADC interrupts */
		SDADCx->IER &= ~SDADC_IT;
	}
}

/**
  * @brief	Check whether the specified SDADC flag is set or not.
  * @param	SDADCx: where x can be 0 to select the SDADC peripheral.
  * @param	SDADC_FLAG: specifies the flag to check. 
  *			This parameter can be one of the following values:
  *			@arg SDADC_FLAG_EOC: regular channel end of conversion flag
  * @retval FlagStatus: The new state of SDADC FLAG (SET or RESET).
  */
FlagStatus SDADC_GetFlagStatus(SDADC_TypeDef *SDADCx, u32 SDADC_FLAG)
{
	FlagStatus bitstatus;

	/* Check the parameters */
	assert_param(IS_SDADC_ALL_PERIPH(SDADCx));
	assert_param(IS_SDADC_FLAG(SDADC_FLAG));

	/* Check the status of the specified ADC flag */
	if((SDADCx->SR & SDADC_FLAG) != (u32)RESET)
	{
		/* SDADC_FLAG is set */
		bitstatus = SET;
	}
	else
	{
		/* SDADC_FLAG is reset */
		bitstatus = RESET;
	}

	/* Return the SDADC_FLAG status */
	return  bitstatus;
}

/**
  * @brief	Configure the gain for the specified SDADC's PGA.
  * @param	SDADCx: where x can be 0 to select the SDADC peripheral.
  * @param	PGAx: specifies the PGA of SDADC.
  *			This parameter can be one of the following values:
  * 		@arg SDADC_PGA0: set the gain of SDADC's PGA0
  *			@arg SDADC_PGA1: set the gain of SDADC's PGA1
  * @param	gain: specifies the PGA's gain.
  *			This parameter can be one of the following values:
  * 		@arg SDADC_PgaGain_Disable: Disable the specified SDADC' PGA
  *			@arg SDADC_PgaGain_x1: PGA's gain value is 1
  *			@arg SDADC_PgaGain_x5p05: PGA's gain value is 5.05
  *			@arg SDADC_PgaGain_x8p72: PGA's gain value is 8.72
  *			@arg SDADC_PgaGain_x16: PGA's gain value is 16
  *			@arg SDADC_PgaGain_x24: PGA's gain value is 24
  *			@arg SDADC_PgaGain_x32: PGA's gain value is 32
  *			@arg SDADC_PgaGain_x64: PGA's gain value is 64
  *			@arg SDADC_PgaGain_x0p25: PGA's gain value is 0.25
  * @retval None
  */
void SDADC_PgaGainConfig(SDADC_TypeDef *SDADCx, u32 PGAx, u32 gain)
{
	u32 tmpreg;
	
	assert_param(IS_SDADC_ALL_PERIPH(SDADCx));
	assert_param(IS_SDADC_PGA(PGAx));
	assert_param(IS_SDADC_PgaGain(PGAx, gain));
	
	if(gain == SDADC_PgaGain_Disable)
	{
		if(PGAx == SDADC_PGA0)
		{
			SDADCx->PGACR &= ~SDADC_PGACR_PGA0E;
		}
		else
		{
			SDADCx->PGACR &= ~SDADC_PGACR_PGA1E;
		}
	}
	else
	{
		tmpreg = SDADCx->PGACR;
		if(PGAx == SDADC_PGA0)
		{
			tmpreg &= ~SDADC_PGACR_GAIN0;
			tmpreg |= SDADC_PGACR_PGA0E | ((gain - 1) << 1);
		}
		else
		{
			tmpreg &= ~SDADC_PGACR_GAIN1;
			tmpreg |= SDADC_PGACR_PGA1E | ((gain - 1) << 9);
		}
		SDADCx->PGACR = tmpreg;
	}
}

/**
  * @brief	Configure the working mode for the specified PGA's chopper.
  * @param	SDADCx: where x can be 0 to select the SDADC peripheral.
  * @param	PGAx: specifies the PGA of SDADC.
  *			This parameter can be one of the following values:
  * 		@arg SDADC_PGA0: set the gain of SDADC's PGA0
  *			@arg SDADC_PGA1: set the gain of SDADC's PGA1
  * @param	Mode: specifies the SDADC working mode.
  *			This parameter can be one of the following values:
  *			@arg SDADC_PgaChopMode_Disable: Disable the PGA's chopper
  *			@arg SDADC_PgaChopMode_ACImpedance: the PGA's chopper for AC impedance
  *			@arg SDADC_PgaChopMode_Enable: Enable the PGA's chopper
  * @retval None
  */
void SDADC_PgaChopperWorkModeConfig(SDADC_TypeDef *SDADCx, u32 PGAx, u32 Mode)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_SDADC_ALL_PERIPH(SDADCx));
	assert_param(IS_SDADC_PGA(PGAx));
	assert_param(IS_SDADC_PgaChopperMode(PGAx, Mode));

	tmpreg = SDADCx->PGACR & ~SDADC_PGACR_PCM0;
	tmpreg |= Mode;
	SDADCx->PGACR = tmpreg;
	
	if(Mode != SDADC_PgaChopMode_Disable)
	{
		SDADC_ChopperCmd(SDADCx, DISABLE);
		if(Mode == SDADC_PgaChopMode_Enable)
		{
			SDADC_ChopperClockConfig(SDADCx, SDADC_ChopClock_Chop);
		}
		else
		{
			SDADC_ChopperClockConfig(SDADCx, SDADC_ChopClock_IQ);
		}
	}
}

/**
  * @brief	Returns the last SDADCx conversion result data for channel.
  * @param	SDADCx: where x can be 0 to select the SDADC peripher.
  * @retval SDADCx conversion data
  */
s32 SDADC_GetConversionValue(SDADC_TypeDef *SDADCx)
{
	s32 data;
	
	/* Check the parameters */
	assert_param(IS_SDADC_ALL_PERIPH(SDADCx));

	data = SDADCx->DR << 8;
	/* Return the selected SDADC conversion value */
	return (data >> 8);
}

/**
  * @brief	Configure the data offset the specified SDADC peripheral.
  * @param	SDADCx: where x can be 0 to select the SDADC peripheral.
  * @param	offset: specifies the SDADC data offset.
  *			This parameter can be a number between 0 and 0x00FFFFFF.
  * @retval None
  */
void SDADC_DataOffsetConfig(SDADC_TypeDef *SDADCx, u32 offset)
{
	/* Check the parameters */
	assert_param(IS_SDADC_ALL_PERIPH(SDADCx));
	assert_param(IS_SDADC_DataOffsetValue(offset));
	
	SDADCx->DOFR = SDADC_DataOffsetKey | offset;
}

/**
  * @brief	Enable or disable the specified SDADC's chopper.
  * @param	SDADCx: where x can be 0 to select the SDADC peripheral.
  * @param	NewState: new state of the SDADC's chopper.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval	None
  */
void SDADC_ChopperCmd(SDADC_TypeDef *SDADCx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_SDADC_ALL_PERIPH(SDADCx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	SDADCx->CCR1 &= ~SDADC_CCR1_CHOPE;
	if(NewState != DISABLE)
	{
		/* Enable the selected SDADC peripheral */
		SDADCx->CCR1 |= SDADC_CCR1_CHOPE;
	}
}

/**
  * @brief	Get the SDADC's data maximum resolution.
  * @param	SDADCx: where x can be 0 to select the SDADC peripheral.
  * @retval	Data maximum resolution
  */
double SDADC_GetDataMaxResolution(SDADC_TypeDef *SDADCx)
{
	/* Check the parameters */
	assert_param(IS_SDADC_ALL_PERIPH(SDADCx));
	
	if(SDADCx == SDADC0)
	{
		return sdadc_max_resolution[0];
	}
	else
	{
		return 1.0;
	}
}
