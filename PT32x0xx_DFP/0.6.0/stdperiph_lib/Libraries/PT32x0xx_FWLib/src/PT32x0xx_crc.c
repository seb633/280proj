/**
	*******************************************************************************
  * @file    PT32x0xx_crc.c
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file provides firmware functions to manage the following
	*          functionalities of the CRC peripheral:
	*           + Initialization and Configuration
	*
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.  
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of
  * the content or the codes within the content.  
  * 
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved 
  *******************************************************************************
**/

#include "PT32x0xx_crc.h"

/** @defgroup CRC
  * @brief CRC driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief Initializes the CRC peripheral according to the specified parameters
  *        	in the CRC_InitStruct.
  * @param CRC_InitStruct:pointer to an CRC_InitTypeDef structure that contains
  *         the configuration information for the specified CRC peripheral.
  * @retval None
  */
void CRC_Init(CRC_TypeDef *CRCx, CRC_InitTypeDef *CRC_InitStruct)
{
	u32 tmpreg = 0;
	/* Check the parameters */
	assert_param(IS_CRC_ALL_PERIPH(CRCx));
	assert_param(IS_CRC_Input(CRC_InitStruct->CRC_Input));
	assert_param(IS_CRC_InputBitSequenceReversal(CRC_InitStruct->CRC_InputBitSequenceReversal));
	assert_param(IS_CRC_InputByteSequenceReversal(CRC_InitStruct->CRC_InputByteSequenceReversal));
	assert_param(IS_CRC_OutputBitSequenceReversal(CRC_InitStruct->CRC_OutputBitSequenceReversal));
	assert_param(IS_CRC_Seed(CRC_InitStruct->CRC_Seed));
	assert_param(IS_CRC_Poly(CRC_InitStruct->CRC_Poly));
	
	/* Get the CRC CR value, and Clear IBITR/IBYTER/OBITR bits*/
	tmpreg = CRCx->CR & ~(CRC_CR_INS | CRC_CR_IBITR | CRC_CR_IBYTER | CRC_CR_OBITR);
	/* Configure CRC:	input selection,input bit sequence reversal,input byte sequence reversal and output bit sequence reversal	*/
	/* Set DIN bit according to CRC_Input value */
	/* Set IBITR bits according to CRC_InputBitSequenceReversal value */ 
	/* Set IBYTER bits according to CRC_InputByteSequenceReversal value */
	/* Set OBITR bit according to CRC_OutputBitSequenceReversal value */
	tmpreg |= ((CRC_InitStruct->CRC_Input) |\
				(CRC_InitStruct->CRC_InputBitSequenceReversal) |\
				(CRC_InitStruct->CRC_InputByteSequenceReversal) |\
				(CRC_InitStruct->CRC_OutputBitSequenceReversal));
	/* Write to CR register */
	CRCx->CR = tmpreg;
	
	/* Write to SEEDR register */
	CRCx->SEEDR = CRC_InitStruct->CRC_Seed;
	
	/* Write to POLYR register */
	CRCx->POLYR = CRC_InitStruct->CRC_Poly;
}

/**
  * @brief  Fills each CRC_InitStruct member with its default value.
  * @param  CRC_InitStruct: pointer to an CRC_InitTypeDef structure which will be initialized.
  * @retval None
  */
void CRC_StructInit(CRC_InitTypeDef *CRC_InitStruct)
{
	/* Initialize the CRC data input width */
	CRC_InitStruct->CRC_Input = CRC_Input_16b;
	/* Initialize the CRC data input bit reverse function */
	CRC_InitStruct->CRC_InputBitSequenceReversal = CRC_InputBitSequenceReversal_Disable;
	/* Initialize the CRC data output bit reverse function */
	CRC_InitStruct->CRC_OutputBitSequenceReversal = CRC_OutputBitSequenceReversal_Disable;
	/* Initialize the CRC data input byte reverse function */
	CRC_InitStruct->CRC_InputByteSequenceReversal = CRC_InputByteSequenceReversal_Enable;
	/* Initialize the CRC POLY */
	CRC_InitStruct->CRC_Poly = CRC_Poly_CCITT16;
	/* Initialize the CRC SEED */
	CRC_InitStruct->CRC_Seed = 0;
}

/**
	* @brief  Enables or disables the specified CRC peripheral.
  * @param  NewState: new state of the CRC peripheral.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CRC_Cmd(CRC_TypeDef *CRCx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_CRC_ALL_PERIPH(CRCx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the selected CRC peripheral */
		CRCx->CR |= CRC_CR_EN;
	}
	else
	{
		/* Disable the selected CRC peripheral */
		CRCx->CR &= ~CRC_CR_EN;
	}
}

/**
  * @brief Reset data output for the specified CRC peripheral.
	* @param None
  * @retval None
  */
void CRC_ResetDout(CRC_TypeDef *CRCx)
{
	/* Check the parameters */
	assert_param(IS_CRC_ALL_PERIPH(CRCx));
	
	CRCx->CR |= CRC_CR_RST;
}

/**
  * @brief Set seed value	for the specified CRC peripheral.
	* @param Seed: the data will be writed into CRC->SEEDR.
	*          This parameter can be a number between 0x0000 and 0xFFFF.
  * @retval None
  */
void CRC_SetSeed(CRC_TypeDef *CRCx, u16 Seed)
{
	/* Check the parameters */
	assert_param(IS_CRC_ALL_PERIPH(CRCx));
	assert_param(IS_CRC_Seed(Seed));
	
	/* Write to SEEDR register */
	CRCx->SEEDR = Seed;	
}

/**
  * @brief Set poly	value	for the specified CRC peripheral.
  * @param Poly: the data will be writed into CRC->POLYR.
  *          This parameter can be one of the following values:
  * 					@arg CRC_Poly_CCITT16: mean G(X) = X^16 + X^12 + X^5 + 1
  * 					@arg CRC_Poly_IBM16: mean G(X) = X^16 + X^15 + X^2 + 1
  * @retval None
  */
void CRC_SetPoly(CRC_TypeDef *CRCx, u16 Poly)
{
	/* Check the parameters */
	assert_param(IS_CRC_ALL_PERIPH(CRCx));
	assert_param(IS_CRC_Poly(Poly));
	
	/* Write to POLYR register */
	CRCx->POLYR = Poly;	
}

/**
  * @brief Computes the 16-bit CRC of a given data(16-bit).
  * @param CRC_Data: the data will be writed into CRC->DINR
  * @retval 16-bit value of the DOUTR register 
  */
u16 CRC_CalculateCRC(CRC_TypeDef *CRCx, u16 CRC_Data)
{
	/* Check the parameters */
	assert_param(IS_CRC_ALL_PERIPH(CRCx));
	
	CRCx->DINR = CRC_Data;
	return CRCx->DOUTR;
}

/**
  * @brief Computes the 16-bit CRC of a given buffer of data(16-bit).
  * @param pBuffer: the data will be writed into CRC->DINR
  * @param BufferLength: the length of pBuffer
  * @retval 16-bit value of the DOUTR register 
  */
u16 CRC_CalculateBlockCRC(CRC_TypeDef *CRCx, u16 pBuffer[], u32 BufferLength)
{
	u32 index = 0;

	/* Check the parameters */
	assert_param(IS_CRC_ALL_PERIPH(CRCx));
	
	for(index = 0; index < BufferLength; index++)
	{
		CRCx->DINR = pBuffer[index];
	}

	return CRCx->DOUTR;
}

/**
  * @brief Get the data output of the specified CRC peripheral.
  * @param None
  * @retval 16-bit value of the DOUTR register 
  */
u16 CRC_GetCRC(CRC_TypeDef *CRCx)
{
	/* Check the parameters */
	assert_param(IS_CRC_ALL_PERIPH(CRCx));
	
	return CRCx->DOUTR;
}

/**
  * @}
  */
