/**
  *******************************************************************************
  * @file    PT32x0xx_crc.h
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the CRC firmware library.
	*
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.
	*	Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of the content or the codes within the content.
	*
	*	Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  *******************************************************************************
**/

#ifndef PT32x0xx_CRC_H
#define PT32x0xx_CRC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PT32x0xx.h"

/* Private define ------------------------------------------------------------*/
/** @defgroup CRC_ALL_PERIPH
* @{
*/
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_CRC_ALL_PERIPH(SEL) 									(0)
#else
#define IS_CRC_ALL_PERIPH(SEL) 									((SEL) == CRC)
#endif

/** @defgroup CRC_InitTypeDef
* @{
*/
typedef struct
{
	u32 CRC_Input;												/*!< Specifies the CRC input, This parameter can be a value of @ref CRC_Input */

	u32 CRC_InputBitSequenceReversal;							/*!< Specifies the CRC input Bit Sequence Reversal,
																	This parameter can be a value of @ref CRC_InputBitSequenceReversal */

	u32 CRC_InputByteSequenceReversal;							/*!< Specifies the CRC input Byte Sequence Reversal,
																	This parameter can be a value of @ref CRC_InputByteSequenceReversal */

	u32 CRC_OutputBitSequenceReversal;							/*!< Specifies the CRC Output Bit Sequence Reversal,
																	This parameter can be a value of @ref CRC_OutputBitSequenceReversal */

	u32 CRC_Seed;												/*!< Specifies the CRC Seed, This parameter can be a value of @ref CRC_Seed */

	u32 CRC_Poly;												/*!< Specifies the CRC Poly, This parameter can be a value of @ref CRC_Poly */
} CRC_InitTypeDef;

/** @defgroup CRC_Input
* @{
*/
#define CRC_Input_8b		                     				(CRC_CR_INS & 0x00000000)
#define CRC_Input_16b		                     				(CRC_CR_INS & 0x00000004)
#define IS_CRC_Input(SEL)										(SEL == CRC_Input_8b || SEL == CRC_Input_16b)

/** @defgroup CRC_InputBitSequenceReversal
* @{
*/
#define CRC_InputBitSequenceReversal_Disable		    		(CRC_CR_IBITR & 0x00000000)
#define CRC_InputBitSequenceReversal_Enable		      			(CRC_CR_IBITR & 0x00000008)
#define IS_CRC_InputBitSequenceReversal(SEL)					(SEL == CRC_InputBitSequenceReversal_Disable || \
																	SEL == CRC_InputBitSequenceReversal_Enable)

/** @defgroup CRC_InputByteSequenceReversal
* @{
*/
#define CRC_InputByteSequenceReversal_Disable		    		(CRC_CR_IBYTER & 0x00000000)
#define CRC_InputByteSequenceReversal_Enable		      		(CRC_CR_IBYTER & 0x00000010)
#define IS_CRC_InputByteSequenceReversal(SEL)					(SEL == CRC_InputByteSequenceReversal_Disable || \
																	SEL == CRC_InputByteSequenceReversal_Enable)

/** @defgroup CRC_OutputBitSequenceReversal
* @{
*/
#define CRC_OutputBitSequenceReversal_Disable		    		(CRC_CR_OBITR & 0x00000000)
#define CRC_OutputBitSequenceReversal_Enable		      		(CRC_CR_OBITR & 0x00000020)
#define IS_CRC_OutputBitSequenceReversal(SEL)					(SEL == CRC_OutputBitSequenceReversal_Disable || \
																	SEL == CRC_OutputBitSequenceReversal_Enable)

/** @defgroup CRC_Seed
* @{
*/
#define IS_CRC_Seed(SEL)										((SEL) <= 0xFFFF)

/** @defgroup CRC_Poly
* @{
*/
#define CRC_Poly_CCITT16	            						0x00001021
#define CRC_Poly_IBM16											0x00008005
#define	IS_CRC_Poly(SEL)										((SEL) == CRC_Poly_CCITT16 || (SEL) == CRC_Poly_IBM16)

/*! function declaration and definition*/
void CRC_Init(CRC_TypeDef *CRCx, CRC_InitTypeDef *CRC_InitStruct);
void CRC_StructInit(CRC_InitTypeDef *CRC_InitStruct);
void CRC_Cmd(CRC_TypeDef *CRCx, FunctionalState NewState);
void CRC_ResetDout(CRC_TypeDef *CRCx);
void CRC_SetSeed(CRC_TypeDef *CRCx, u16 Seed);
void CRC_SetPoly(CRC_TypeDef *CRCx, u16 Poly);
u16 CRC_CalculateCRC(CRC_TypeDef *CRCx, u16 CRC_Data);
u16 CRC_CalculateBlockCRC(CRC_TypeDef *CRCx, u16 pBuffer[], u32 BufferLength);
u16 CRC_GetCRC(CRC_TypeDef *CRCx);
#ifdef __cplusplus
}
#endif

#endif /*__PT32x0xx_CRC_H */
