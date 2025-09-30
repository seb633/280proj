/**
	******************************************************************************
  * @file    PT32Sx0xx_i2c.h
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the I2C firmware
  *          library
  ********************************************************************************
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of
  * the content or the codes within the content.
  *
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  *********************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PT32x0xx_I2C_H
#define PT32x0xx_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "PT32x0xx.h"

/* Private define ------------------------------------------------------------*/

/** @addtogroup I2C
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/**
  * @brief  I2C Init structure definition
  */

typedef struct
{
	u32 I2C_Prescaler;					/*!< Specifies the baud rate prescaler control.This parameter can be a number between 20 and 4096
												 @note I2C BaundRate = PCLK's Frequency / I2C_Prescaler
												 @note I2C BaundRate can't over 1Mbps */

	u32 I2C_Broadcast;					/*!< Specifies the Broadcast response control.This parameter can be a value of @ref I2C_broadcast_state */

	u32 I2C_OwnAddress;					/*!< Specifies the device own address.This parameter can be a 7-bit address */

	u32 I2C_Acknowledge;   		   	   	/*!< Enables or disables the acknowledgement.This parameter can be a value of @ref I2C_acknowledgement_state */

} I2C_InitTypeDef;

/* Exported constants --------------------------------------------------------*/

/** @defgroup IS_I2C_ALL_PERIPH
  * @{
  */
#if defined (PT32x002x) || defined(PT32G031x) || defined (PT32x012x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM160x) || defined (PTM280x)
#define IS_I2C_ALL_PERIPH(SEL)    					((SEL) == I2C0)
#else
#define IS_I2C_ALL_PERIPH(SEL)    					((SEL) == I2C0 || (SEL) == I2C1)
#endif
/**
  * @}
  */

/** @defgroup I2C_acknowledge_state
  * @{
  */
#define I2C_Acknowledge_Enable			 			(I2C_CR_ACK & 0x00000001)
#define I2C_Acknowledge_Disable						(I2C_CR_ACK & 0x00000000)
#define IS_I2C_Acknowledge(SEL)						((SEL) == I2C_Acknowledge_Enable || (SEL) == I2C_Acknowledge_Disable)

/** @defgroup IS_I2C_HighLevelDetectTimeCoefficient
  * @{
  */
#define IS_I2C_HighLevelDetectTimeCoefficient(SEL)	((SEL) >= 5  && (SEL) <= 255)

/** @defgroup IS_I2C_BaudRateValue
  * @{
  */
#define IS_I2C_PrescalerValue(SEL)					((SEL) >= 20 && (SEL) <= 4096 && ((SEL) & 0x03) == 0)

/** @defgroup I2C_FLAG
  * @{
  */
#define I2C_FLAG_Stop 			  					(I2C_SR_SR & 0x00)	/* 总线上出现一个停止信号	*/
#define I2C_FLAG_StartOk 							(I2C_SR_SR & 0x01)	/* 起始信号发送完毕 */
#define I2C_FLAG_ReStartOk 							(I2C_SR_SR & 0x02)	/* 重复起始信号发送完毕 */
#define I2C_FLAG_MASGetAckW 						(I2C_SR_SR & 0x03)	/* 主机， 地址字发送完毕， 收到 ACK  */
#define I2C_FLAG_MASGetNackW 						(I2C_SR_SR & 0x04)	/* 主机， 地址字发送完毕， 收到 NACK */
#define I2C_FLAG_MDSGetAck 							(I2C_SR_SR & 0x05)	/* 主机， 数据字发送完毕， 收到 ACK  */
#define I2C_FLAG_MDSGetNack 						(I2C_SR_SR & 0x06)	/* 主机， 数据字发送完毕， 收到 NACK */
#define I2C_FLAG_MArbitrationlost 					(I2C_SR_SR & 0x07)	/* 主机， 总线仲裁失败 */
#define I2C_FLAG_MASGetAckR 						(I2C_SR_SR & 0x08)	/* 主机， 地址字发送完毕， 收到 ACK  */
#define I2C_FLAG_MASGetNackR 						(I2C_SR_SR & 0x09)	/* 主机， 地址字发送完毕， 收到 NACK */
#define I2C_FLAG_MDGSendAck 						(I2C_SR_SR & 0x0A)	/* 主机， 数据字接收完毕， 回送 ACK  */
#define I2C_FLAG_MDGSendNack 						(I2C_SR_SR & 0x0B)	/* 主机， 数据字接收完毕， 回送 NACK */
#define I2C_FLAG_SAGSendAckW 						(I2C_SR_SR & 0x0C)	/* 从机， 地址字接收完毕， 回送 ACK */
#define I2C_FLAG_SALAGSendAckW 						(I2C_SR_SR & 0x0D)	/* 从机， 总线仲裁失败转化的从机， 地址字接收完毕， 回送 ACK */
#define I2C_FLAG_SBCAGSendAck 						(I2C_SR_SR & 0x0E)	/* 从机， 广播地址字接收完毕， 回送 ACK */
#define I2C_FLAG_SALBCAGSendAck 					(I2C_SR_SR & 0x0F)	/* 从机， 总线仲裁失败转化的从机，广播地址字接收完毕， 回送 ACK */
#define I2C_FLAG_SDGSendAck 						(I2C_SR_SR & 0x10)	/* 从机， 数据字接收完毕， 回送 ACK */
#define I2C_FLAG_SDGSendNack 						(I2C_SR_SR & 0x11)	/* 从机， 数据字接收完毕， 回送 NACK */
#define I2C_FLAG_SBCDGSendAck 						(I2C_SR_SR & 0x12)	/* 从机， 广播数据字接收完毕， 回送 ACK */
#define I2C_FLAG_SBCDGSendNack 						(I2C_SR_SR & 0x13)	/* 从机， 广播数据字接收完毕， 回送 NACK */
#define I2C_FLAG_SDGGSRS 							(I2C_SR_SR & 0x14)	/* 从机， 数据字接收完毕， 接收到停止信号或重复起始信号 */
#define I2C_FLAG_SAGSendAckR 						(I2C_SR_SR & 0x15)	/* 从机， 地址字接收完毕， 回送 ACK */
#define I2C_FLAG_SALAGSendAckR 						(I2C_SR_SR & 0x16)	/* 从机， 主机总线仲裁失败转化的从机,地址字接收完毕， 回送 ACK */
#define I2C_FLAG_SDSReadAck 						(I2C_SR_SR & 0x17)	/* 从机， 数据字发送完毕， 收到 ACK */
#define I2C_FLAG_SDSReadNack 						(I2C_SR_SR & 0x18)	/* 从机， 数据字发送完毕， 收到 NACK */
#define I2C_FLAG_SDSSAGSRS 							(I2C_SR_SR & 0x19)	/* 从机， 数据字接收完毕， 回送 ACK 后、接收到停止信号或重复起始信号 */
#define I2C_FLAG_IDLE 								(I2C_SR_SR & 0x1F)	/* 总线空闲*/
#define IS_I2C_StatusFlag(SEL) 		 				(((SEL) & ~I2C_SR_SR) == 0 && (SEL) <= I2C_FLAG_IDLE)

/** @defgroup I2C_broadcast_state
  * @{
  */
#define I2C_Broadcast_Enable			 		 	(I2C_OAR_BC & 0x00000001)
#define I2C_Broadcast_Disable						(I2C_OAR_BC & 0x00000000)
#define IS_I2C_Broadcast(SEL)						((SEL) == I2C_Broadcast_Enable || (SEL) == I2C_Broadcast_Disable)


/** @defgroup IS_I2C_AddressValue
  * @{
  */
#define IS_I2C_AddressValue(SEL)  					((SEL) < 128)


/** @defgroup IS_I2C_Event
  * @{
  */
#define I2C_Event_Stop								0
#define I2C_Event_Start							 	1
#define I2C_Event_Restart						 	2
#define IS_I2C_Event(SEL)  					     	((SEL) <= I2C_Event_Restart)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void I2C_Init(I2C_TypeDef *I2Cx, I2C_InitTypeDef *I2C_InitStruct);
void I2C_StructInit(I2C_InitTypeDef *I2C_InitStruct);
void I2C_Cmd(I2C_TypeDef *I2Cx, FunctionalState NewState);
void I2C_GenerateEvent(I2C_TypeDef *I2Cx, u8 Event, FunctionalState NewState);
void I2C_AcknowledgeConfig(I2C_TypeDef *I2Cx, FunctionalState NewState);
void I2C_HighLevelDetectConfig(I2C_TypeDef *I2Cx, u8 Coefficient);
FlagStatus I2C_GetFlagStatus(I2C_TypeDef *I2Cx, u16 I2C_FLAG);
void I2C_SendData(I2C_TypeDef *I2Cx, u8 Data);
void I2C_SendAddress(I2C_TypeDef *I2Cx, u8 Data);
u8 I2C_ReceiveData(I2C_TypeDef *I2Cx);

/**
  * @}
  */


#ifdef __cplusplus
}
#endif

#endif /*__PT32x0xx_I2C_H */
