/**
	*****************************************************************************
	* @file    PT32x0xx_ifmc.c
	* @author  Application development team
	* @version V1.0.0
	* @date    2025/2/13
	* @brief   This file provides firmware functions to manage the following
	*          functionalities of the IFMC peripheral:
	*          + Flash Erase and Read\Write Function
	*          + Interrupts and flags management
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

/* Includes ------------------------------------------------------------------*/
#include "PT32x0xx_ifmc.h"

/** @defgroup IFMC
  * @brief IFMC driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define IFMC_AR_UNLOCK_KEY						IFMC_KR1_KEY_MAINCODE
#define IFMC_CR2_UNLOCK_KEY 					IFMC_KR2_KEY_UNLOCK
#define IFMC_CR2_LOCK_KEY 						IFMC_KR2_UKEY_LOCK

#define IFMC_PSCR_UKEY_UNLOCK          			((u32)0x00AC0000)
#define IFMC_PSCR_UKEY_LOCK          			((u32)0x00C60000)

#define IFMC_KR1_UKEY_UNLOCK          			((u32)0x3B6A0000)
#define IFMC_KR1_UKEY_LOCK          			((u32)0xEA2D0000)
#define IFMC_KR1_KEY_MAINCODE					((u32)0x0000ADEB)
#define IFMC_KR1_KEY_USERDATA					((u32)0x000063D2)

#define IFMC_KR2_UKEY_UNLOCK          			((u32)0xB75C0000)
#define IFMC_KR2_UKEY_LOCK          			((u32)0xEA2D0000)
#define IFMC_KR2_KEY_UNLOCK						((u32)0x0000D3A5)

#define IFMC_OPTCR_RDP_UNPROTECTED				((u32)0x000000AA)
#define IFMC_OPTCR_RDP_PROTECTED				((u32)0x000000CC)
#define IFMC_OPTCR_KEY_VALUE					((u32)0x15EC1CCA)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Returns the IFMC Status.
  * @param  None
  * @retval IFMC Status: The returned value can be:
  * 		@arg IFMC_KeyError: 	Key error status
  * 		@arg IFMC_AddrError: 	Address error status
  * 		@arg IFMC_CommandError: Command error status
  * 		@arg IFMC_Complete:  	Complete status
  */
IFMC_Status IFMC_GetStatus(void)
{
	u32 sr;
	IFMC_Status status = IFMC_Complete;
	
	sr = IFMC->SR1;
	if((sr & IFMC_SR1_KERR) == IFMC_SR1_KERR)
	{
		status = IFMC_KeyError;
	}
	else if((sr & IFMC_SR1_AERR) == IFMC_SR1_AERR)
	{
		status = IFMC_AddrError;
	}
	else if((sr & IFMC_SR1_CERR) == IFMC_SR1_CERR)
	{
		status = IFMC_CommandError;
	}
	else if((sr & IFMC_SR1_WC) == IFMC_SR1_WC || (sr & IFMC_SR1_PEC) == IFMC_SR1_PEC || (sr & IFMC_SR1_MEC) == IFMC_SR1_MEC)
	{
		status = IFMC_Complete;
	}

	//clear SR1 all status flag;
	IFMC->SR1 = sr;
	
	/* Return the IFMC Status */
	return status;
}

/**
  * @brief  Sets the IFMC latency value.
  * @param  Latency: specifies the IFMC Latency value.
  *         This parameter must be a number between 0 to 7.
  * @retval None
  */
void IFMC_SetLatency(u32 Latency)
{
	u32 tmpreg;
	
	/* Check the parameters */
	assert_param(IS_IFMC_Latency(Latency));
	
	tmpreg = IFMC->CR1 & ~IFMC_CR1_WAIT;
	tmpreg |= Latency;
	IFMC->CR1 = tmpreg;
}

#if defined (PT32x002x) || defined (PT32G031x) || defined (PT32x012x) || defined (PT32x060x) || defined (PT32x063x) || defined (PT32x066x) || defined (PTM160x) || defined (PTM280x)
/**
  * @brief  Programs a word at a specified address.
  * @param  address: specifies the address to be programmed.
  * @param  data: specifies the word data to be programmed.
  * @retval IFMC_Status: The returned value can be: IFMC_CommandError,
  *         IFMC_KeyError, IFMC_AddrError or IFMC_Complete.
  */
IFMC_Status IFMC_ProgramWord(u32 Address, u32 Data)
{
	IFMC_Status status;

	/* Check the parameters */
	assert_param(IS_IFMC_PageAddress(Address));
	
	IFMC->KR1 = IFMC_KR1_UKEY_UNLOCK;
	while(IFMC->KR1 & IFMC_KR1_LOCK);
	
	IFMC->KR1 = IFMC_AR_UNLOCK_KEY;
	IFMC->AR = Address;

	IFMC->KR2 = IFMC_KR2_UKEY_UNLOCK;
	while(IFMC->KR2 & IFMC_KR2_LOCK2);
	
	IFMC->KR2 = IFMC_CR2_UNLOCK_KEY;
	while(IFMC->KR2 & IFMC_KR2_LOCK1);

	IFMC->DR1 = Data;
	IFMC->CR2 = IFMC_CR2_PG;

	status = IFMC_GetStatus();

	return status;
}

/**
  * @brief  Programs words at a specified address.
  * @param  address: specifies the address to be programmed.
  * @param  data: specifies the datas to be programmed.
  * @param  size: the data length.
  * @retval IFMC_Status: The returned value can be: IFMC_CommandError,
  *         IFMC_KeyError, IFMC_AddrError or IFMC_Complete.
  */
IFMC_Status IFMC_ProgramWords(u32 Address, u32 Data[], u32 Size)
{
	IFMC_Status status;
	u32 i;

	/* Check the parameters */
	assert_param(IS_IFMC_PageAddress(Address));

	if(Data == NULL || Size == 0)
	{
		return IFMC_Complete;
	}

	//address auto increment enble;
	IFMC->CR1 |= IFMC_CR1_AINC;

	IFMC->KR1 = IFMC_KR1_UKEY_UNLOCK;
	while(IFMC->KR1 & IFMC_KR1_LOCK);
	
	IFMC->KR1 = IFMC_AR_UNLOCK_KEY;
	IFMC->AR = Address;

	IFMC->KR2 = IFMC_KR2_UKEY_UNLOCK;
	while(IFMC->KR2 & IFMC_KR2_LOCK2);
	
	IFMC->KR2 = IFMC_CR2_UNLOCK_KEY;
	while(IFMC->KR2 & IFMC_KR2_LOCK1);

	for(i = 0; i < Size; i++)
	{
		IFMC->DR1 = Data[i];
		IFMC->CR2 = IFMC_CR2_PG;
		
		status = IFMC_GetStatus();
		if(status != IFMC_Complete)
		{
			break;
		}
	}

	IFMC->KR2 = IFMC_CR2_LOCK_KEY;
	while((IFMC->KR2 & IFMC_KR2_LOCK1) == 0);

	//address auto increment disable;
	IFMC->CR1 &= ~IFMC_CR1_AINC;

	return status;
}
#else
/**
  * @brief  Programs a dword at a specified address.
  * @param  address: specifies the address to be programmed.
  * @param  data: specifies the dword data to be programmed.
  * @retval IFMC_Status: The returned value can be: IFMC_CommandError,
  *         IFMC_KeyError, IFMC_AddrError or IFMC_Complete.
  */
IFMC_Status IFMC_ProgramDWord(u32 Address, u64 Data)
{
	IFMC_Status status;

	IFMC->KR1 = IFMC_KR1_UKEY_UNLOCK;
	while(IFMC->KR1 & IFMC_KR1_LOCK);
	
	IFMC->KR1 = IFMC_AR_UNLOCK_KEY;
	IFMC->AR = Address;

	IFMC->KR2 = IFMC_KR2_UKEY_UNLOCK;
	while(IFMC->KR2 & IFMC_KR2_LOCK2);
	
	IFMC->KR2 = IFMC_CR2_UNLOCK_KEY;
	while(IFMC->KR2 & IFMC_KR2_LOCK1);

	IFMC->DR1 = (u32)(Data & 0xFFFFFFFF);
	IFMC->DR2 = (u32)(Data >> 32);
	IFMC->CR2 = IFMC_CR2_PG;

	status = IFMC_GetStatus();

	return status;
}

/**
  * @brief  Programs dwords at a specified address.
  * @param  address: specifies the address to be programmed.
  * @param  data: specifies the datas to be programmed.
  * @param  size: the data length.
  * @retval IFMC_Status: The returned value can be: IFMC_CommandError,
  *         IFMC_KeyError, IFMC_AddrError or IFMC_Complete.
  */
IFMC_Status IFMC_ProgramDWords(u32 Address, u64 Data[], u32 Size)
{
	IFMC_Status status;
	u32 i;

	/* Check the parameters */
	assert_param(IS_IFMC_PageAddress(Address));

	if(Data == NULL || Size == 0)
	{
		return IFMC_Complete;
	}

	//Address auto increment enble;
	IFMC->CR1 |= IFMC_CR1_AINC;

	IFMC->KR1 = IFMC_KR1_UKEY_UNLOCK;
	while(IFMC->KR1 & IFMC_KR1_LOCK);
	
	IFMC->KR1 = IFMC_AR_UNLOCK_KEY;
	IFMC->AR = Address;

	IFMC->KR2 = IFMC_KR2_UKEY_UNLOCK;
	while(IFMC->KR2 & IFMC_KR2_LOCK2);
	
	IFMC->KR2 = IFMC_CR2_UNLOCK_KEY;
	while(IFMC->KR2 & IFMC_KR2_LOCK1);

	for(i = 0; i < Size; i++)
	{
		IFMC->DR1 = (u32)(Data[i] & 0xFFFFFFFF);
		IFMC->DR2 = (u32)(Data[i] >> 32);
		IFMC->CR2 = IFMC_CR2_PG;
		
		status = IFMC_GetStatus();
		if(status != IFMC_Complete)
		{
			break;
		}
	}

	IFMC->KR2 = IFMC_CR2_LOCK_KEY;
	while((IFMC->KR2 & IFMC_KR2_LOCK1) == 0);

	//address auto increment disable;
	IFMC->CR1 &= ~IFMC_CR1_AINC;

	return status;
}
#endif

/**
  * @brief  Erases a specified page in program memory.
  * @param  address: The page address in program memory to be erased.
  * @note   A Page is erased in the Program memory only if the address to load
  *         is the start address of a page (multiple of 512 bytes).
  * @retval IFMC_Status: The returned value can be: IFMC_CommandError,
  *         IFMC_KeyError, IFMC_AddrError or IFMC_Complete.
  */
IFMC_Status IFMC_ErasePage(u32 Address)
{
	IFMC_Status status;
	
	/* Check the parameters */
	assert_param(IS_IFMC_PageAddress(Address));

	IFMC->KR1 = IFMC_KR1_UKEY_UNLOCK;
	while(IFMC->KR1 & IFMC_KR1_LOCK);
	
	IFMC->KR1 = IFMC_AR_UNLOCK_KEY;
	IFMC->AR = Address;
	
	IFMC->KR2 = IFMC_KR2_UKEY_UNLOCK;
	while(IFMC->KR2 & IFMC_KR2_LOCK2);
	
	IFMC->KR2 = IFMC_CR2_UNLOCK_KEY;
	while(IFMC->KR2 & IFMC_KR2_LOCK1);
	
	IFMC->CR2 = IFMC_CR2_PER;

	status = IFMC_GetStatus();
		
	return status;
}

/**
  * @brief  IFMC Erase chip.
  * @param  None
  * @retval IFMC_Status: The returned value can be: IFMC_CommandError,
  *         IFMC_KeyError, IFMC_AddrError or IFMC_Complete.
  */
IFMC_Status IFMC_EraseChip(void)
{
	IFMC_Status status;

	IFMC->KR1 = IFMC_KR1_UKEY_UNLOCK;
	while(IFMC->KR1 & IFMC_KR1_LOCK);
	
	IFMC->KR1 = IFMC_AR_UNLOCK_KEY;
	IFMC->AR = 0;
	
	IFMC->KR2 = IFMC_KR2_UKEY_UNLOCK;
	while(IFMC->KR2 & IFMC_KR2_LOCK2);
	
	IFMC->KR2 = IFMC_CR2_UNLOCK_KEY;
	while(IFMC->KR2 & IFMC_KR2_LOCK1);
	
	IFMC->CR2 = IFMC_CR2_MER;

	status = IFMC_GetStatus();

	return status;
}

/**
  * @brief  Enable or disable the specified IFMC interrupts.
  * @param  ITFlags: specifies the IFMC interrupt sources to be enabled or
  *         disabled.
  *         This parameter can be any combination of the following values:
  * 		@arg IFMC_IT_WCIE: Write operation complete interrupt
  * 		@arg IFMC_IT_PECIE: Page erase operation complete interrupt
  * 		@arg IFMC_IT_MECIE: Chip erase operation complete interrupt
  * 		@arg IFMC_IT_CERIE: Operation command error interrupt
  * 		@arg IFMC_IT_AERIE: Address error interrupt
  * 		@arg IFMC_IT_KERIE: Key error interrupt
  * @param  NewState: new state of the IFMC peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void IFMC_ITConfig(u32 IFMC_IT, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_IFMC_IT(IFMC_IT));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the interrupt sources */
		IFMC->IER |= IFMC_IT;
	}
	else
	{
		/* Disable the interrupt sources */
		IFMC->IER &= ~IFMC_IT;
	}
}

/**
  * @brief  Check whether the specified IFMC flag is set or not.
  * @param  Flags: specifies the IFMC flag to check.
  *         This parameter can be one of the following values:
  *			@arg IFMC_FLAG_WC: Write operation complete flag
  *			@arg IFMC_FLAG_PEC: Page erase operation complete flag
  *			@arg IFMC_FLAG_MEC: Chip erase operation complete flag
  *			@arg IFMC_FLAG_CERR: Operation command error flag
  *			@arg IFMC_FLAG_AERR: Address error flag
  *			@arg IFMC_FLAG_KERR: Key error flag
  * @retval The new state of IFMC_FLAG (SET or RESET).
  */
FlagStatus IFMC_GetFlagStatus(u32 IFMC_FLAG)
{
	FlagStatus bitstatus = RESET;
	
	/* Check the parameters */
	assert_param(IS_IFMC_FLAG(IFMC_FLAG));

	if((IFMC->SR1 & IFMC_FLAG) != (u32)RESET)
	{
		bitstatus = SET;
	}
	else
	{
		bitstatus = RESET;
	}

	/* Return the new state of IFMC_FLAG (SET or RESET) */
	return bitstatus;
}

/**
  * @brief  Clear the IFMC's pending flags.
  * @param  Flags: specifies the IFMC flags to clear.
  *         This parameter can be any combination of the following values:
  *			@arg IFMC_FLAG_WC: Write operation complete flag
  *			@arg IFMC_FLAG_PEC: Page erase operation complete flag
  *			@arg IFMC_FLAG_CEC: Chip erase operation complete flag
  *			@arg IFMC_FLAG_CERR: Operation command error flag
  *			@arg IFMC_FLAG_KERR: Key error flag
  *			@arg IFMC_FLAG_AERR: Address error flag
  *			@arg IFMC_FLAG_ALL: All flags above
  * @retval None
  */
void IFMC_ClearFlag(u32 IFMC_FLAG)
{
	/* Check the parameters */
	assert_param(IS_IFMC_FLAG(IFMC_FLAG));
	
	/* Clear the flags */
	IFMC->SR1 = IFMC_FLAG;
}

/**
  * @brief  Returns the ReadProtect Status.
  * @param  None
  * @retval The new state of ReadProtect Status (SET or RESET).
  */
ProtectStatus IFMC_GetReadProtectStatus(void)
{
	if((IFMC->OPTCR & IFMC_OPTCR_RDP) == IFMC_OPTCR_RDP_PROTECTED)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/**
  * @brief  Enable or disable the specified IFMC read protect
  * @param  NewState: new state of the IFMC peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void IFMC_SetReadProtect(FunctionalState NewState)
{
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	IFMC->OPTCR = IFMC_OPTCR_KEY_VALUE;
	if(NewState != DISABLE)
	{
		IFMC->OPTCR = IFMC_OPTCR_RDP_PROTECTED;
	}
	else
	{
		IFMC->OPTCR = IFMC_OPTCR_RDP_UNPROTECTED;
	}
}

/**
  * @brief  Read a word at a specified address.
  * @param  address: specifies the address to be read.
  * @retval The word at a specified address.
  */
u32 IFMC_ReadWord(u32 Address)
{
	return *(__IO u32 *)Address;
}

/**
  * @brief  Read a half word at a specified address.
  * @param  address: specifies the address to be read.
  * @retval The half word at a specified address.
  */
u16 IFMC_ReadHalfWord(u32 Address)
{
	return *(__IO u16 *)Address;
}

/**
  * @brief 	Read a Byte at a specified address.
  * @param  address: specifies the address to be read.
  * @retval The byte at a specified address.
  */
u8 IFMC_ReadByte(u32 Address)
{
	return *(__IO u8 *)Address;
}
