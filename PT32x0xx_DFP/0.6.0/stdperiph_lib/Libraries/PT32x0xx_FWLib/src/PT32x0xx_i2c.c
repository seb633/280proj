/**
	******************************************************************************
  * @file    PT32Sx0xx_i2c.c
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the I2C firmware
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
#include "PT32x0xx_i2c.h"

/** @defgroup I2C
  * @brief I2C driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the I2Cx peripheral according to the specified
  *         parameters in the I2C_InitStruct.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @param  I2C_InitStruct: pointer to a I2C_InitTypeDef structure that contains
  *         the configuration information for the specified I2C peripheral.
  * @retval None
  */
void I2C_Init(I2C_TypeDef *I2Cx, I2C_InitTypeDef *I2C_InitStruct)
{
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(IS_I2C_AddressValue(I2C_InitStruct->I2C_OwnAddress));
	assert_param(IS_I2C_Acknowledge(I2C_InitStruct->I2C_Acknowledge));
	assert_param(IS_I2C_PrescalerValue(I2C_InitStruct->I2C_Prescaler));
	assert_param(IS_I2C_Broadcast(I2C_InitStruct->I2C_Broadcast));
	/* Configure I2Cx:	I2C addr and Broadcast response control	*/
	/* Set ADDR bit according to I2C_OwnAddress */
	/* Set BC bits according to I2C_Broadcast value */ 
	I2Cx->OAR = ((I2C_InitStruct->I2C_OwnAddress) << 0x01) | I2C_InitStruct->I2C_Broadcast;

	/* Clear I2Cx CR register ACK/PSC bits*/
	I2Cx->CCR = I2C_CCR_ACK | I2C_CCR_PSC;
	/* Configure I2Cx:	Acknowledge enable and Prescaler */
	/* Set ACK bit according to I2C_Acknowledge */
	/* Set PSC bits according to I2C_Prescaler value */ 
	I2Cx->CR = I2C_InitStruct->I2C_Acknowledge | (((I2C_InitStruct->I2C_Prescaler / 4) - 1) << 16);
}

/**
  * @brief  Fills each I2C_InitStruct member with its default value.
  * @param  I2C_InitStruct: pointer to an I2C_InitTypeDef structure which will be initialized.
  * @retval None
  */
void I2C_StructInit(I2C_InitTypeDef *I2C_InitStruct)
{
	I2C_InitStruct->I2C_Prescaler = 192;
	I2C_InitStruct->I2C_Broadcast = I2C_Broadcast_Disable;
	I2C_InitStruct->I2C_OwnAddress = 0;
	I2C_InitStruct->I2C_Acknowledge = I2C_Acknowledge_Disable;
}

/**
  * @brief  Enable or disable the specified I2C peripheral.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @param  NewState: new state of the I2Cx peripheral.
  *       	This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2C_Cmd(I2C_TypeDef *I2Cx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the selected I2Cx peripheral */
		I2Cx->CR = I2C_CR_EN;
	}
	else
	{
		/* Disable the selected I2Cx peripheral */
		I2Cx->CCR = I2C_CCR_EN;
	}
}

/**
  * @brief  Generates I2Cx communication event.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @param  Event: the types of I2Cx's event.
	*     	This parameter can be one of the following values:
	*      	@arg I2C_Event_Stop: I2C generate stop
	*      	@arg I2C_Event_Start: I2C generates start
	*     	@arg I2C_Event_Restart: I2C generates restart
  * @param  NewState: new state of the I2C Event generation.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2C_GenerateEvent(I2C_TypeDef *I2Cx, u8 Event, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(IS_I2C_Event(Event));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	/* Generate a STOP event */
	if(Event == I2C_Event_Stop)
	{
		if(NewState != DISABLE)
		{
			/* Generate a STOP condition */
			I2Cx->CR = I2C_CR_STOP;
			I2Cx->CCR = I2C_CCR_SI;
		}
		else
		{
			/* Disable the STOP condition generation */
			I2Cx->CCR = I2C_CCR_SI | I2C_CCR_STOP;
		}
	}
	/* Generate a START event */
	else if(Event == I2C_Event_Start)
	{
		if(NewState != DISABLE)
		{
			/* Generate a START condition */
			I2Cx->CR = I2C_CR_START;
		}
		else
		{
			/* Disable the START condition generation */
			I2Cx->CCR = I2C_CCR_START;
		}
	}
	/* Generate a ReSTART event */
	else
	{
		if(NewState != DISABLE)
		{
			/* Generate a START condition */
			I2Cx->CR = I2C_CR_START;
			I2Cx->CCR = I2C_CCR_SI;
		}
		else
		{
			/* Disable the START condition generation */
			I2Cx->CCR = I2C_CCR_START;
		}
	}
}

/**
  * @brief  Enables or disable the acknowledge for the specified I2C peripheral.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @param  NewState: new state of the I2Cx peripheral.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2C_AcknowledgeConfig(I2C_TypeDef *I2Cx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if(NewState != DISABLE)
	{
		/* Enable the selected I2Cx peripheral */
		I2Cx->CR = I2C_CR_ACK;
	}
	else
	{
		/* Disable the selected I2Cx peripheral */
		I2Cx->CCR = I2C_CCR_ACK;
	}
}

/**
  * @brief  Set I2Cx high level detect coefficient of time in master mode.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @param  Coefficient: specifies the high level detection coefficient of time.
	*     	This parameter can be a number between 5 and 255.
  * @retval None
  */
void I2C_HighLevelDetectConfig(I2C_TypeDef *I2Cx, u8 Coefficient)
{
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(IS_I2C_HighLevelDetectTimeCoefficient(Coefficient));
	/* Write to I2Cx CCR in order to clear I2Cx CR HCT bits */
	I2Cx->CCR = I2C_CCR_HCT;
	/* Write to I2Cx CR */
	I2Cx->CR = ((u32)Coefficient) << 8;
}

/**
  * @brief  Check whether the specified I2Cx flag is set or not.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @param  I2C_FLAG: specifies the flag to check.
  *          This parameter can be one of the following values:
  *               @arg I2C_FLAG_Stop 			    	总线上出现一个停止信号
  *               @arg I2C_FLAG_StartOk 			  	起始信号发送完毕
  *               @arg I2C_FLAG_ReStartOk 				重复起始信号发送完毕
  *               @arg I2C_FLAG_MASGetAckW 	    		主机， 地址字发送完毕， 收到 ACK
  *               @arg I2C_FLAG_MASGetNackW 			主机， 地址字发送完毕， 收到 NACK
  *               @arg I2C_FLAG_MDSGetAck 		  		主机， 数据字发送完毕， 收到 ACK
  *               @arg I2C_FLAG_MDSGetNack 				主机， 数据字发送完毕， 收到 NACK
  *               @arg I2C_FLAG_MArbitrationlost  		主机， 总线仲裁失败
  *               @arg I2C_FLAG_MASGetAckR 				主机， 地址字发送完毕， 收到 ACK
  *               @arg I2C_FLAG_MASGetNackR 			主机， 地址字发送完毕， 收到 NACK
  *               @arg I2C_FLAG_MDGSendAck 				主机， 数据字接收完毕， 回送 ACK
  *               @arg I2C_FLAG_MDGSendNack 			主机， 数据字接收完毕， 回送 NACK
  *
  *               @arg I2C_FLAG_SAGSendAckW 			从机， 地址字接收完毕， 回送 ACK
  *               @arg I2C_FLAG_SALAGSendAckW 			从机，总线仲裁失败转化的从机， 地址字接收完毕， 回送 ACK
  *               @arg I2C_FLAG_SBCAGSendAck 			从机，广播地址字接收完毕， 回送 ACK
  *               @arg I2C_FLAG_SALBCAGSendAck 			从机，总线仲裁失败转化的从机，广播地址字接收完毕， 回送 ACK
  *               @arg I2C_FLAG_SDGSendAck 				从机， 数据字接收完毕， 回送 ACK
  *               @arg I2C_FLAG_SDGSendNack 			从机， 数据字接收完毕， 回送 NACK
  *               @arg I2C_FLAG_SBCDGSendAck 			从机， 广播数据字接收完毕， 回送 ACK
  *               @arg I2C_FLAG_SBCDGSendNack 			从机， 广播数据字接收完毕， 回送 NACK
  *               @arg I2C_FLAG_SDGGSRS 				从机， 数据字接收完毕， 接收到停止信号或重复起始信号
  *               @arg I2C_FLAG_SAGSendAckR 			从机， 地址字接收完毕， 回送 ACK
  *               @arg I2C_FLAG_SALAGSendAckR 			从机， 主机总线仲裁失败转化的从机,地址字接收完毕， 回送 ACK
  *               @arg I2C_FLAG_SDSReadAck 				从机， 数据字发送完毕， 收到 ACK
  *               @arg I2C_FLAG_SDSReadNack 			从机， 数据字发送完毕， 收到 NACK
  *               @arg I2C_FLAG_SDSSAGSRS 				从机， 数据字接收完毕， 回送 ACK 后、接收到停止信号或重复起始信号
  *               @arg I2C_FLAG_IDLE 				    总线空闲
  * @retval The new state of I2C_FLAG (SET or RESET).
  */
FlagStatus I2C_GetFlagStatus(I2C_TypeDef *I2Cx, u16 I2C_FLAG)
{
	FlagStatus bitstatus;
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(IS_I2C_StatusFlag(I2C_FLAG));

	if(I2Cx->SR == I2C_FLAG)
	{
		bitstatus = SET;
	}
	else
	{
		bitstatus = RESET;
	}

	return bitstatus;
}

/**
  * @brief  Send a data byte for the I2Cx peripheral.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @param  Data: Byte to be transmitted..
  * @retval None
  */
void I2C_SendData(I2C_TypeDef *I2Cx, u8 Data)
{
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	/* Write in the DR register the data to be sent */
	I2Cx->DR = (u8)Data;
	I2Cx->CCR = I2C_CCR_SI | I2C_CCR_START;
}

/**
  * @brief  Send a data / addr byte for the I2Cx peripheral
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @param  Data: Byte to be transmitted..
  * @retval None
  */
void I2C_SendAddress(I2C_TypeDef *I2Cx, u8 Data)
{
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	/* Write in the DR register the data to be sent */
	I2Cx->DR = (u8)Data;
	I2Cx->CCR = I2C_CCR_SI | I2C_CCR_START;
}

/**
  * @brief  Return the most recent received data by the I2Cx peripheral.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @retval The value of the received data.
  */
u8 I2C_ReceiveData(I2C_TypeDef *I2Cx)
{
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	/* Return the data in the DR register */
	return (u8)I2Cx->DR;
}

/**
  * @}
  */


