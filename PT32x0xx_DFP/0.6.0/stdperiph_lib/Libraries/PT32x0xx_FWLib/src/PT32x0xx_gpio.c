/**
	*******************************************************************************
	* @file    PT32x0xx_gpio.c
	* @author  Application development team
	* @version V1.0.0
	* @date    2025/2/13
	* @brief   This file contains all the functions prototypes for the GPIO firmware
	*          library
	*
	********************************************************************************
	* @attention
	*
	* The firmware provided is for guidance only. The codes are offered for the convenience of the customers.  
	* Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of
	* the content or the codes within the content.  
	* 
	* Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved 
	*******************************************************************************
**/

#include "PT32x0xx_gpio.h"

/** @defgroup	GPIO
  * @brief		GPIO driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief 	Initialize the GPIOx peripheral according to the specified parameters
  *        	in the GPIO_InitStruct.
  * @param 	GPIOx: where x can be (A, B, C, D, E or F) to select the GPIO peripheral.
  * @param 	GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure that contains
  *        	the configuration information for the specified GPIO peripheral.
  * @retval None
  */
void GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_InitStruct)
{
	u32 currentpin;
	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_Pin(GPIO_InitStruct->GPIO_Pin));
	assert_param(IS_GPIO_Mode(GPIO_InitStruct->GPIO_Mode));
	assert_param(IS_GPIO_Pull(GPIO_InitStruct->GPIO_Pull));
	
	/*-------------------------- Configure the port pins -----------------------*/
	currentpin = GPIO_InitStruct->GPIO_Pin;
	
	switch(GPIO_InitStruct->GPIO_Pull)
	{
		case GPIO_Pull_NoPull :
			GPIOx->PDCR = currentpin;
			GPIOx->PUCR = currentpin;
			break;
		case GPIO_Pull_Up :
			GPIOx->PDCR = currentpin;
			GPIOx->PUSR = currentpin;
			break;
		case GPIO_Pull_Down :
			GPIOx->PDSR = currentpin;
			GPIOx->PUCR = currentpin;
			break;
		case GPIO_Pull_UpDown:
			GPIOx->PDSR = currentpin;
			GPIOx->PUSR = currentpin;
			break;	
	}
	
	if(GPIO_InitStruct->GPIO_Mode == GPIO_Mode_OutPP || GPIO_InitStruct->GPIO_Mode == GPIO_Mode_OutOD)
	{
		/* Output mode configuration */
		GPIOx->OESR = currentpin;

		if(GPIO_InitStruct->GPIO_Mode == GPIO_Mode_OutOD)
		{
			GPIOx->ODSR = currentpin;
		}
		else
		{
			GPIOx->ODCR = currentpin;
		}
	}
	else if(GPIO_InitStruct->GPIO_Mode == GPIO_Mode_In)
	{
		/* input mode configuration */
		GPIOx->OECR = currentpin;
	}
}

/**
  * @brief	Fill each GPIO_InitStruct member with its default value.
  * @param	GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure which will be initialized.
  * @retval	None
  */
void GPIO_StructInit(GPIO_InitTypeDef *GPIO_InitStruct)
{
	/* Initialize the port pin select*/
	GPIO_InitStruct->GPIO_Pin  = GPIO_Pin_0;
	/* Initialize the gpio mode */
	GPIO_InitStruct->GPIO_Mode = GPIO_Mode_In;
	/* Initialize the Pull-Up and Pull-Down configration*/
	GPIO_InitStruct->GPIO_Pull = GPIO_Pull_NoPull;
}

/**
  * @brief	Read data of the specified GPIO peripheral.
  * @param	GPIOx: where x can be (A, B, C, D, E or F) to select the GPIO peripheral.
  * @retval	GPIO input data port value
  */
u16 GPIO_ReadData(GPIO_TypeDef *GPIOx)
{
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	/* Return  GPIO input data port value */
	return ((u16)GPIOx->DR);
}

/**
  * @brief  Read the specified input port pin.
  * @param  GPIOx: where x can be (A, B, C, D, E or F) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bit to read.
  * 		@arg This parameter can be GPIO_Pin_x where x can be(0..15) or GPIO_Pin_All
  * @retval The input port pin value.
  */
u8 GPIO_ReadDataBit(GPIO_TypeDef *GPIOx, u32 GPIO_Pin)
{
	u8 bitstatus;
	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_Pin(GPIO_Pin));

	if((GPIOx->DR & GPIO_Pin) != (u32)RESET)
	{
		bitstatus = (u8)SET;
	}
	else
	{
		bitstatus = (u8)RESET;
	}

	return bitstatus;
}

/**
  * @brief  Reverse the selected data port bits.
  * @param  GPIOx: where x can be (A, B, C, D, E or F) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bits to be written.
  * 		@arg This parameter can be GPIO_Pin_x where x can be(0~15) or GPIO_Pin_All
  * @retval None
  */
void GPIO_ReverseBits(GPIO_TypeDef *GPIOx, u32 GPIO_Pin)
{
	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_Pin(GPIO_Pin));
	
	GPIOx->DR ^= GPIO_Pin;
}

/**
  * @brief  Set the selected data port bits.
  * @param  GPIOx: where x can be (A, B, C, D, E or F) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bits to be written.
  * 		@arg This parameter can be GPIO_Pin_x where x can be(0..15) or GPIO_Pin_All
  * @retval None
  */
void GPIO_SetBits(GPIO_TypeDef *GPIOx, u32 GPIO_Pin)
{
	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_Pin(GPIO_Pin));
	
	GPIOx->BSR = GPIO_Pin;
}

/**
  * @brief  Reset the selected data port bits.
  * @param  GPIOx: where x can be (A, B, C, D, E or F) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bits to be written.
  * 		@arg This parameter can be GPIO_Pin_x where x can be(0..15) or GPIO_Pin_All
  * @retval None
  */
void GPIO_ResetBits(GPIO_TypeDef *GPIOx, u32 GPIO_Pin)
{
	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_Pin(GPIO_Pin));
	
	GPIOx->BRR = GPIO_Pin;
}

/**
  * @brief  Enable or Disable GPIO_Pin Pull_Up function.
  * @param  GPIOx: where x can be (A, B, C, D, E or F) to select the AFIO peripheral.
  * @param  GPIO_Pin: specifies the port bit to be written.
  * 		@arg This parameter can be GPIO_Pin_x where x can be(0..15) or GPIO_Pin_All
  * @param  NewState: new state of the port pin Pull_Up function.
  * 		@arg This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void GPIO_PullUpConfig(GPIO_TypeDef *GPIOx, u32 GPIO_Pin, FunctionalState NewState)
{
	  /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_Pin(GPIO_Pin));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if(NewState != DISABLE)
    {
		GPIOx->PDCR = GPIO_Pin;
        GPIOx->PUSR = GPIO_Pin;
    }
    else
    {
        GPIOx->PUCR = GPIO_Pin;
    }
}

/**
  * @brief  Enable or Disable GPIO_Pin Pull_Down function.
  * @param  GPIOx: where x can be (A, B, C, D, E or F) to select the AFIO peripheral.
  * @param  GPIO_Pin: specifies the port bit to be written.
  * 		@arg This parameter can be GPIO_Pin_x where x can be(0..15) or GPIO_Pin_All
  * @param  NewState: new state of the port pin Pull_Down function.
  * 		@arg This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void GPIO_PullDownConfig(GPIO_TypeDef *GPIOx, u32 GPIO_Pin, FunctionalState NewState)
{
	/* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_Pin(GPIO_Pin));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if(NewState != DISABLE)
    {
		GPIOx->PUCR = GPIO_Pin;
        GPIOx->PDSR = GPIO_Pin;
    }
    else
    {
        GPIOx->PDCR = GPIO_Pin;
    }
}

/**
  * @brief  Enable or Disable Schmitt function.
  * @param  GPIOx: where x can be (A, B, C, D, E or F) to select the AFIO peripheral.
  * @param  GPIO_Pin: specifies the port bit to be written.
  * 		@arg This parameter can be GPIO_Pin_x where x can be(0..15) or GPIO_Pin_All
  * @param  NewState: new state of the port pin schmitt function.
  * 		@arg This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void GPIO_SchmittConfig(GPIO_TypeDef *GPIOx, u32 GPIO_Pin, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_Pin(GPIO_Pin));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		GPIOx->SCSR = GPIO_Pin;
	}
	else
	{
		GPIOx->SCCR = GPIO_Pin;
	}
}

/**
  * @brief  Enable or Disable Specified Digital Pin Remapping
  * @param  AFIOx: where x can be (A, B, C, D, E or F) to select the AFIO peripheral.
  * @param  GPIO_Pin: specifies the pin for the Alternate function.
  * 		@arg This parameter can be GPIO_Pin_x where x can be(0..15) or GPIO_Pin_All
  * @param  AFIO_AF: selects the pin to used as Digtal Alternate function.
  * 		@arg This parameter can be AFIO_AF_x where x can be(0..6)
  * @param  NewState: new state of the port pin remapping.
  * 		@arg This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void GPIO_DigitalRemapConfig(AFIO_TypeDef *AFIOx, u16 GPIO_Pin, u8 AFIO_AF, FunctionalState NewState)
{
	u8 temp = 0;
	/* Check the parameters */
	assert_param(IS_AFIO_ALL_PERIPH(AFIOx));
	assert_param(IS_GPIO_Pin(GPIO_Pin));
	assert_param(IS_AFIO_AF(AFIO_AF));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		AFIOx->AFCR = GPIO_Pin;

		while(GPIO_Pin)
		{
			if((GPIO_Pin & 0x01) == 1)
			{
				AFIOx->AFSR[temp >> 3] |= AFIO_AF << (temp & 0x7) * 4;
			}

			GPIO_Pin = GPIO_Pin >> 1;
			temp++;
		}
	}
	else
	{
		AFIOx->AFCR = GPIO_Pin;
	}
}

/**
  * @brief  Enable or Disable Specified Analog Pin Remapping
  * @param  AFIOx: where x can be (A, B, C, D, E or F) to select the AFIO peripheral.
  * @param  GPIO_Pin: specifies the port bit to be written.
  * 		@arg This parameter can be GPIO_Pin_x where x can be(0..15) or GPIO_Pin_All
  * @param  NewState: new state of the port pin remapping.
  * 		@arg This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void GPIO_AnalogRemapConfig(AFIO_TypeDef *AFIOx, u32 GPIO_Pin, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_AFIO_ALL_PERIPH(AFIOx));
	assert_param(IS_GPIO_Pin(GPIO_Pin));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		AFIOx->ANASR = GPIO_Pin;
	}
	else
	{
		AFIOx->ANACR = GPIO_Pin;
	}
}
