/**
  ******************************************************************************
  * @file    PT32x0xx_AES.h
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the aes firmware
  *
  ******************************************************************************
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of
  * the content or the codes within the content.
  *
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PT32x0xx_AES_H
#define __PT32x0xx_AES_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
/* Includes ------------------------------------------------------------------*/
#include "PT32x0xx.h"
	
/* Exported types ------------------------------------------------------------*/

/**
  * @brief	AES Init structure definition
  */
typedef struct
{
	u32 AES_OperatingMode;				    		/*!< Specifies the operating mode selection.
																			This parameter can be a value of @ref IS_AES_OperatingMode */

	u32 AES_EncryptionMethod;						/*!< Specifies the encryption method.
																			 This parameter can be a value of @ref IS_AES_EncryptionMethod */

	u32 AES_KeySize;						    	/*!< Specifies the Key size selection.
																			 This parameter can be a value of @ref IS_AES_KeySize */

}AES_InitTypeDef;


/** @defgroup AES_ALL_PERIPH
* @{
*/
#if defined (PT32G031x)  || defined(PT32x012x) || defined(PT32x060x) || defined (PT32x066x) || defined (PTM160x) || defined (PTM280x)
#define IS_AES_ALL_PERIPH(AESx)               		(0)
#else
#define IS_AES_ALL_PERIPH(AESx)   					((AESx) == AES)
#endif

/** @defgroup AES_OperatingMode
* @{
*/
#define AES_Mode_Decryption			                (AES_CR_MODE & 0x00000000)
#define AES_Mode_Encryption	                 		(AES_CR_MODE & 0x00000010)
#define AES_Mode_KeyGenration	                 	(AES_CR_MODE & 0x00000030)
#define IS_AES_OperatingMode(Mode)            		((Mode) == AES_Mode_Decryption ||\
														(Mode) == AES_Mode_Encryption ||\
														(Mode) == AES_Mode_KeyGenration)

/** @defgroup AES_EncryptionMethod
* @{
*/
#define AES_EncryptionMethodECB						(AES_CR_CMS & 0x00000000)
#define AES_EncryptionMethodCBC						(AES_CR_CMS & 0x00000100)
#define IS_AES_EncryptionMethod(Method)     	  	((Method) == AES_EncryptionMethodECB ||\
														(Method) == AES_EncryptionMethodCBC)

/** @defgroup AES_KeySize
* @{
*/
#define AES_KeySize128								(AES_CR_KSIZE & 0x00000000)
#define AES_KeySize192								(AES_CR_KSIZE & 0x00001000)
#define AES_KeySize256								(AES_CR_KSIZE & 0x00002000)
#if defined (PT32x063x)
#define IS_AES_KeySize(KeySize)						((KeySize) == AES_KeySize128)
#else
#define IS_AES_KeySize(KeySize)						((KeySize) == AES_KeySize128 ||\
														(KeySize) == AES_KeySize192 ||\
														(KeySize) == AES_KeySize256)
#endif
/** @defgroup AES_FLAG
* @{
*/
#define AES_FLAG_CC                           		(AES_SR_CC & 0x00000001)
#define IS_AES_FLAG(AES_FLAG)                 		((AES_FLAG) == AES_FLAG_CC)

/** @defgroup AES_Interrupt
* @{
*/
#define AES_IT_CC                             		(AES_IER_CC & 0x00000001)
#define IS_AES_IT(AES_IT)                     		((AES_IT) == AES_IT_CC)

void AES_Init(AES_TypeDef *AESx, AES_InitTypeDef *AES_InitStruct);
void AES_StructInit(AES_InitTypeDef *AES_InitStruct);
void AES_StartComputation(AES_TypeDef *AESx);
void AES_OperatingMode(AES_TypeDef *AESx, u32 SEL);
void AES_EncryptionMethodConfig(AES_TypeDef *AESx, u32 SEL);
void AES_KeySizeConfig(AES_TypeDef *AESx, u32 KeySize);
void AES_ITConfig(AES_TypeDef *AESx, u32 AES_IT, FunctionalState NewState);
FlagStatus AES_GetFlagStatus(AES_TypeDef *AESx, u32 AES_FLAG);
void AES_SetKey(AES_TypeDef *AESx, u32 *Keys, u32 KeySize);
void AES_GetKey(AES_TypeDef *AESx, u32 *Keys, u32 KeySize);
void AES_DataInput(AES_TypeDef *AESx, u32 Din[4]);
void AES_InitializationVectorInput(AES_TypeDef *AESx, u32 IV[4]);
void AES_GetDataOutput(AES_TypeDef *AESx, u32 *data);
#ifdef __cplusplus
}
#endif // _AES_H_

#endif /* __PT32x0xx_AES_H */

/**
  * @}
  */

/**
  * @}
  */

/************************(C) COPYRIGHT PENGPAI Microelectronics*****END OF FILE****/
