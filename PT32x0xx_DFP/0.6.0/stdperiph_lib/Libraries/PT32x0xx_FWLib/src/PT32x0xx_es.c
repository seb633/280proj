/**
	******************************************************************************
	* @file    PT32x0xx_es.c
	* @author  Application development team
	* @version V1.0.0
	* @date    2025/2/13
	* @brief   This file contains all the functions prototypes for the ES firmware
	*          library
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

#include "PT32x0xx_es.h"

/** @defgroup ES
  * @brief ES driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief Get the Chip Information
  * @retval ES->CIR
  */
u32 ES_GetChipInfo(void)
{
	return ES->CIR;
}

/**
  * @brief Get the Chip ID
  * @param CID_Number: the value can be 0,1,2
  * @retval CID
  */
u32 ES_GetCID(u8 CID_Number)
{
	/* Check the parameters */
	assert_param(IS_ES_CIDNumber(CID_Number));
	
	return ES->CIDR[CID_Number];
}

/**
  * @brief ES Get User-Defined Information
  * @param UDI_Number: the value can be 0,1,2
  * @retval ES->UDI
  */
u32 ES_GetUserDefinedInfo(u8 UDI_Number)
{
	/* Check the parameters */
	assert_param(IS_ES_UDINumber(UDI_Number));
	
	return ES->UDIR[UDI_Number];
}

/**
  * @brief ES Set User-Defined Information
  * @param UDI_Number: the value can be 0,1,2
  * @param UDI: the UDI value
  */
void ES_SetUserDefinedInfo(u8 UDI_Number, u32 UDI)
{
	/* Check the parameters */
	assert_param(IS_ES_UDINumber(UDI_Number));

	ES->UDIR[UDI_Number] = ES_UDIR_KEY_VALUE;
	ES->UDIR[UDI_Number] = UDI;
}
/**
  * @}
  */
