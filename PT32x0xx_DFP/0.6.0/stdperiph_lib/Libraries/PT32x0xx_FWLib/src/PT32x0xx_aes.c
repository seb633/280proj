/**
	******************************************************************************
  * @file    PT32x0xx_aes.c
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the AES firmware
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

/* Includes ------------------------------------------------------------------*/
#include "PT32x0xx_aes.h"

/** @defgroup AES
  * @brief AES driver modules
  * @{
  */
	
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief 	Initializes the AESx peripheral according to
  *         the specified parameters in the AES_InitStruct.
  * @param 	AESx:AES Peripheral
  * @param 	AES_InitStruct: AES_InitTypeDef type structure
  * @retval None
  */
void AES_Init(AES_TypeDef *AESx, AES_InitTypeDef *AES_InitStruct)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_AES_ALL_PERIPH(AESx));
	assert_param(IS_AES_OperatingMode(AES_InitStruct->AES_OperatingMode));
	assert_param(IS_AES_EncryptionMethod(AES_InitStruct->AES_EncryptionMethod));
	assert_param(IS_AES_KeySize(AES_InitStruct->AES_KeySize));
	/* Write to tmpreg */
	tmpreg = AES->CR & ~(AES_CR_MODE | AES_CR_CMS | AES_CR_KSIZE);
	tmpreg |= (u32)((AES_InitStruct->AES_OperatingMode) | \
						(AES_InitStruct->AES_EncryptionMethod) | \
						(AES_InitStruct->AES_KeySize));
	AESx->CR = tmpreg;
}

/**
  * @brief 	Fills each AES_InitStruct member with its default value.
  * @param 	AES_InitStruct: pointer to a AES_InitStruct structure which will be initialized.
  * @retval None
  */
void AES_StructInit(AES_InitTypeDef *AES_InitStruct)
{
	AES_InitStruct->AES_OperatingMode = AES_Mode_Decryption;
	AES_InitStruct->AES_EncryptionMethod = AES_EncryptionMethodECB;
	AES_InitStruct->AES_KeySize = AES_KeySize128;
}

/**
  * @brief 	Start computation of AES.
  * @param 	AESx: AES Peripheral.
  * @param	NewState: new state of the AESx peripheral.
  *			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void AES_StartComputation(AES_TypeDef *AESx)
{
	/* Check the parameters */
	assert_param(IS_AES_ALL_PERIPH(AESx));
	
	AESx->CR |= AES_CR_START;
}

/**
  * @brief Configure the operating mode for the specified AES peripheral.
  * @param AESx: AES Peripheral.
  * @param	Mode: specifies the AES operating mode.
  *			This parameter can be one of the following values:
  *			@arg AES_Mode_Decrypt
  *			@arg AES_Mode_Encryption
  *			@arg AES_Mode_KeyGenration
  * @retval None
  */
void AES_OperatingMode(AES_TypeDef *AESx, u32 Mode)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_AES_ALL_PERIPH(AESx));
	assert_param(IS_AES_OperatingMode(Mode));

	tmpreg = AESx->CR & ~AES_CR_MODE;
	tmpreg |= Mode;
	AESx->CR = tmpreg;
}

/**
  * @brief 	Configure the chaining mode for the specified AES peripheral.
  * @param 	AESx: AES Peripheral.
  * @param	Method: specifies the AES chaining mode.
  *			This parameter can be one of the following values:
  *			@arg AES_EncryptionMethodECB
  *			@arg AES_EncryptionMethodCBC
  * @retval None
  */
void AES_EncryptionMethodConfig(AES_TypeDef *AESx, u32 Method)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_AES_ALL_PERIPH(AESx));
	assert_param(IS_AES_EncryptionMethod(Method));

	tmpreg = AESx->CR & ~AES_CR_CMS;
	tmpreg |= Method;
	AESx->CR = tmpreg;
}

/**
  * @brief 	Configure the key size for the specified AES peripheral.
  * @param 	AESx: AES Peripheral
  * @param	KeySize: specifies the AES key size.
  *			This parameter can be one of the following values:
  *			@arg AES_KeySize128: 128 bit key length.
  *			@arg AES_KeySize192: 192 bit key length.
  *			@arg AES_KeySize256: 256 bit key length.
  * @retval None
  */
void AES_KeySizeConfig(AES_TypeDef *AESx, u32 KeySize)
{
	u32 tmpreg;
	/* Check the parameters */
	assert_param(IS_AES_ALL_PERIPH(AESx));
	assert_param(IS_AES_KeySize(KeySize));

	tmpreg = AESx->CR & ~AES_CR_KSIZE;
	tmpreg |= KeySize;
	AESx->CR = tmpreg;
}


/**
  * @brief 	Enable or disable the specified AES interrupts.
  * @param 	AESx: AES Peripheral.
  * @param	AES_IT: specifies the AES interrupt sources to be enabled or disabled. 
  *			This parameter can be one of the following values:
  *			@arg AES_IT_CC: computation completed interrupt.
  * @retval None
  */
void AES_ITConfig(AES_TypeDef *AESx, u32 AES_IT, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_AES_ALL_PERIPH(AESx));
	assert_param(IS_AES_IT(AES_IT));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the selected AES interrupts */
		AESx->IER |= AES_IER_CC;
	}
	else
	{
		/* Enable the selected AES interrupts */
		AESx->IER &= (~AES_IER_CC);
	}
}

/**
  * @brief	Check whether the specified AES flag is set or not.
  * @param	AESx: where x can be 0 to select the AES peripheral.
  * @param	AES_FLAG: specifies the flag to check. 
  *			This parameter can be one of the following values:
  *			@arg AES_FLAG_CC: computation completed flag.
  * @retval FlagStatus: The new state of AES FLAG (SET or RESET).
  */
FlagStatus AES_GetFlagStatus(AES_TypeDef *AESx, u32 AES_FLAG)
{
	FlagStatus bitstatus = RESET;

	/* Check the parameters */
	assert_param(IS_AES_ALL_PERIPH(AESx));
	assert_param(IS_AES_FLAG(AES_FLAG));

	/* Check the status of the specified AES flag */
	if((AESx->SR & AES_FLAG) != (u32)RESET)
	{
		/* AES_FLAG is set */
		bitstatus = SET;
	}
	else
	{
		/* AES_FLAG is reset */
		bitstatus = RESET;
	}

	/* Return the AES_FLAG status */
	return  bitstatus;
}

/**
  * @brief 	Set AES key.
  * @param 	AESx: AES Peripheral
  * @param 	Keys: Keys Value can be a number between 0x00 and 0xFFFFFFFF.
  * @param	KeySize: specifies the AES key size.
  *			This parameter can be one of the following values:
  *			@arg AES_KeySize128: 128 bit key length.
  *			@arg AES_KeySize192: 192 bit key length.
  *			@arg AES_KeySize256: 256 bit key length.
  * @retval None
  */
void AES_SetKey(AES_TypeDef *AESx, u32 *Keys, u32 KeySize)
{
	u8 i, len;
	/* Check the parameters */
	assert_param(IS_AES_ALL_PERIPH(AESx));
	assert_param(IS_AES_KeySize(KeySize));

	if(KeySize == AES_KeySize128)
	{
		len = 4;
	}
	else if(KeySize == AES_KeySize192)
	{
		len = 6;
	}
	else
	{
		len = 8;
	} 
 
	for(i = 0; i < len; i++)
	{
		AES->KR[i] = Keys[i];
	}
}

/**
  * @brief 	Get AES key.
  * @param 	AESx: AES Peripheral.
  * @param 	Keys: Keys Value can be a number between 0x00 and 0xFFFFFFFF.
  * @param	KeySize: specifies the AES key size.
  *			This parameter can be one of the following values:
  *			@arg AES_KeySize128: 128 bit key length.
  *			@arg AES_KeySize192: 192 bit key length.
  *			@arg AES_KeySize256: 256 bit key length.
  * @retval None
  */
void AES_GetKey(AES_TypeDef *AESx, u32 *Keys, u32 KeySize)
{
	u8 i,len;
	/* Check the parameters */
	assert_param(IS_AES_ALL_PERIPH(AESx));
	assert_param(IS_AES_KeySize(KeySize));

	if(KeySize == AES_KeySize128)
	{
		len = 4;
	}
	else if(KeySize == AES_KeySize192)
	{
		len = 6;
	}
	else
	{
		len = 8;
	} 
 
	for(i = 0;i < len; i++)
	{
		Keys[i] = AES->KR[i];
	}
}

/**
  * @brief 	Set AES InitializationVectorInput.
  * @param 	AESx: AES Peripheral.
  * @param 	IV[4]: Din Value can be a number between 0x00 and 0xFFFFFFFF.
  * @retval None
  */
void AES_InitializationVectorInput(AES_TypeDef *AESx, u32 IV[4])
{
	u8 i;
	/* Check the parameters */
	assert_param(IS_AES_ALL_PERIPH(AESx));
	for(i=0;i<4;i++)
	{
		AES->IVR[i] = IV[i];
	}
}

/**
  * @brief  Set AES DataInput.
  * @param 	AESx: AES Peripheral.
  * @param 	Din[4]: Din Value can be a number between 0x00 and 0xFFFFFFFF.
  * @retval None
  */
void AES_DataInput(AES_TypeDef *AESx, u32 Din[4])
{
	u8 i;
	/* Check the parameters */
	assert_param(IS_AES_ALL_PERIPH(AESx));
	for(i=0;i<4;i++)
	{
		AES->DINR[i] = Din[i];
	}
}

/**
  * @brief 	Get AES DataOutput.
  * @param 	AESx: AEC Peripheral.
  * @retval None
  */
void AES_GetDataOutput(AES_TypeDef *AESx, u32 *data)
{
	u8 i;
	/* Check the parameters */
	assert_param(IS_AES_ALL_PERIPH(AESx));
	for(i=0;i<4;i++)
	{
		data[i] = AES->DOUTR[i];
	}
}

/**
  * @}
  */
