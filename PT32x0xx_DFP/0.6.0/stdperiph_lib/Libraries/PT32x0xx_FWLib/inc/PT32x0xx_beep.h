/**
	****************************************************************************
  * @file    PT32x0xx_beep.h
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief    This file contains all the functions prototypes for the BEEP firmware library
  *
  ******************************************************************************
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of
  * the content or the codes within the content.
  *
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  *****************************************************************************
**/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PT32x0xx_BEEP_H
#define PT32x0xx_BEEP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "PT32x0xx.h"

/** @addtogroup BEEP
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/**
  * @brief  BEEP Initstructure definition
  */
  
/** @defgroup BEEP_ALL_PERIPH
* @{
*/
#if defined(PT32G031x) || defined(PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_BEEP_ALL_PERIPH(SEL)      				(0)
#else
#define IS_BEEP_ALL_PERIPH(SEL)      				((SEL) == BEEP)
#endif

typedef struct
{
	u32 BEEP_PrescalerValue;    		/*!< Specifies the beep prescaler value.
											This parameter can be a number between 1 and 256 */

	u32 BEEP_HinghSink;      			/*!< Specifies the beep high sink current enable.*/

} Beep_InitTypeDef;

/* Exported constants --------------------------------------------------------*/

/** @defgroup IS_BEEP_PSC
  * @{
  */
#define IS_BEEP_Prescaler(SEL) 		 				((SEL) > 0 && (SEL) <= 256)

/** @defgroup IS_BEEP_HighSink
  * @{
  */
#define BEEP_HinghSink_Disable						(BEEP_CR_HSCE & 0x00000000)
#define BEEP_HinghSink_Enable						(BEEP_CR_HSCE & 0x00010000)
#define IS_BEEP_HighSink(SEL) 		 				((SEL) == BEEP_HinghSink_Disable || (SEL) == BEEP_HinghSink_Enable)

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/
void Beep_Init(BEEP_TypeDef *BEEPx, Beep_InitTypeDef *Beep_InitStruct);
void Beep_Cmd(BEEP_TypeDef *BEEPx, FunctionalState NewState);
void Beep_HighSinkConfig(BEEP_TypeDef *BEEPx, FunctionalState NewState);


#ifdef __cplusplus
}
#endif

#endif /*__PT32x0xx_BEEP_H */
