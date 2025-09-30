/**
  ******************************************************************************
  * @file    PT32x0xx_usart.h
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the USART firmware
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

#ifndef PT32x0xx_USART_H
#define PT32x0xx_USART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PT32x0xx.h"

/** @defgroup USART_Sync_InitTypeDef
* @{
*/
typedef struct
{
	u32 USART_MasterSlaveMode;				/*!< Specifies the Master or slave mode, This parameter can be a value of @ref USART_Sync_MasterSlaveMode */
	
	u32 USART_BaudRate; 					/*!< Specifies the Baud Rate, This parameter can be a even number between 2 and 254 */

	u32 USART_FrameFormat;					/*!< Specifies the Frame Format. This parameter can be a value of @ref USART_Sync_FrameFormat */
	
	u32 USART_ClockPolarity;              	/*!< Specifies the serial clock Polarity, This parameter can be a value of @ref USART_Sync_ClockPolarity */

	u32 USART_ClockPhase;                	/*!< Specifies the clock active edge, This parameter can be a value of @ref USART_Sync_ClockPhase */
    
	u32 USART_Receiver;						/*!< Specifies the receive enable. This parameter can be a value of @ref USART_Receiver */
} USART_Sync_InitTypeDef;

/** @defgroup USART_Async_InitTypeDef
* @{
*/
typedef struct
{
	u32 USART_BaudRate;						/*!< This member configures the USART communication baud rate. @note USART_Async_BaudRate shouldn't over 3Mbps. */

	u32 USART_SampleRate;					/*!< This member configures the USART communication SampleRate.*/

	u32 USART_DataLength;					/*!< Specifies the Data Length. This parameter can be a value of @ref USART_Async_DataLength */
	
	u32 USART_StopBitLength;				/*!< Specifies the Stop Bit Length, This parameter can be a value of @ref USART_Async_StopBitLength */

	u32 USART_ParityMode;					/*!< Specifies the parity mode. This parameter can be a value of @ref USART_Async_ParityMode */
    
	u32 USART_Receiver;						/*!< Specifies the receive enable. This parameter can be a value of @ref USART_Receiver */
} USART_Async_InitTypeDef;

/** @defgroup USART_ALL_PERIPH
* @{
*/
#if defined(PT32G031x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined(PTM280x)
#define IS_USART_ALL_PERIPH(SEL)					(0)
#elif defined(PT32x012x) || defined (PTM160x)
#define IS_USART_ALL_PERIPH(SEL)					((SEL) == USART0)
#else
#define IS_USART_ALL_PERIPH(SEL)					((SEL) == USART0)
#endif

/** @defgroup USART_Receiver
* @{
*/
#define USART_Receiver_Disable						(USART_CR1_RE & 0x00000000)
#define USART_Receiver_Enable						(USART_CR1_RE & 0x00000002)
#define IS_USART_Receiver(SEL)						((SEL) == USART_Receiver_Disable || (SEL) == USART_Receiver_Enable)

/** @defgroup USART_RXDMAE
* @{
*/
#define USART_RXDMAE_Disable						(USART_CR1_RXDMAE & 0x00000000)
#define USART_RXDMAE_Enable							(USART_CR1_RXDMAE & 0x00000004)
#define IS_USART_RXDMAE(SEL)						((SEL) == USART_RXDMAE_Disable || (SEL) == USART_RXDMAE_Enable)

/** @defgroup USART_TXDMAE
* @{
*/
#define USART_TXDMAE_Disable						(USART_CR1_TXDMAE & 0x00000000)
#define USART_TXDMAE_Enable							(USART_CR1_TXDMAE & 0x00000008)
#define IS_USART_TXDMAE(SEL)						((SEL) == USART_TXDMAE_Disable || (SEL) == USART_TXDMAE_Enable)

/** @defgroup USART_Sync_ClockPolarity
* @{
*/
#define USART_ClockPolarity_Low						(USART_CR1_CPOL & 0x00000000)
#define USART_ClockPolarity_High					(USART_CR1_CPOL & 0x00000010)
#define IS_USART_Sync_ClockPolarity(SEL)			((SEL) == USART_ClockPolarity_Low || (SEL) == USART_ClockPolarity_High)

/** @defgroup USART_Sync_ClockPhase
* @{
*/
#define USART_ClockPhase_1Edge						(USART_CR1_CPHA & 0x00000000)
#define USART_ClockPhase_2Edge						(USART_CR1_CPHA & 0x00000020)
#define IS_USART_Sync_ClockPhase(SEL)				((SEL) == USART_ClockPhase_1Edge || (SEL) == USART_ClockPhase_2Edge)

/** @defgroup USART_WordLengthAndParity
* @{
*/
#define USART_WordLengthAndParity_7DE				(USART_CR1_WP & 0x00000000)
#define USART_WordLengthAndParity_7DO				(USART_CR1_WP & 0x00000010)
#define USART_WordLengthAndParity_8D				(USART_CR1_WP & 0x00000020)
#define USART_WordLengthAndParity_8DW				(USART_CR1_WP & 0x00000030)
#define USART_WordLengthAndParity_8DE				(USART_CR1_WP & 0x00000040)
#define USART_WordLengthAndParity_8DO				(USART_CR1_WP & 0x00000050)
#define USART_WordLengthAndParity_9D				(USART_CR1_WP & 0x00000060)
#if defined(PT32x012x) || defined (PTM160x)
#define IS_USART_Aync_WordLengthAndParity(SEL)		(((SEL) & ~USART_CR1_WP) == 0 && (SEL) != USART_WordLengthAndParity_8DW && \
														(SEL) <= USART_WordLengthAndParity_9D)
#else
#define IS_USART_Aync_WordLengthAndParity(SEL)		(((SEL) & ~USART_CR1_WP) == 0 && (SEL) <= USART_WordLengthAndParity_9D)
#endif

/** @defgroup USART_Mode
* @{
*/
#define USART_ModeAsync								(USART_CR1_MODE & 0x00000000)
#define USART_ModeSync								(USART_CR1_MODE & 0x00000080)
#define IS_USART_Mode(SEL)							((SEL) == USART_ModeAsync || (SEL) == USART_ModeSync)

/** @defgroup USART_StopBitLength
* @{
*/
#define USART_StopBitLength_1						(USART_CR1_STOP & 0x00000000)
#define USART_StopBitLength_2						(USART_CR1_STOP & 0x00000100)
#define USART_StopBitLength_0P5						(USART_CR1_STOP & 0x00000200)
#define USART_StopBitLength_1P5						(USART_CR1_STOP & 0x00000300)
#if defined(PT32x012x) || defined (PTM160x)
#define IS_USART_Async_StopBitLength(SEL)	 		((SEL) == USART_StopBitLength_1 || (SEL) == USART_StopBitLength_2)
#else
#define IS_USART_Async_StopBitLength(SEL)			(((SEL) & ~USART_CR1_STOP) == 0 && (SEL) <= USART_StopBitLength_1P5)
#endif

/** @defgroup USART_DMATrigger
* @{
*/
#define USART_DMA_Trigger0							(USART_CR1_DMATC & 0x00000000)
#define USART_DMA_Trigger1							(USART_CR1_DMATC & 0x00001000)
#define USART_DMA_Trigger2							(USART_CR1_DMATC & 0x00002000)
#if defined(PT32x012x) || defined (PTM160x)
#define IS_USART_DMATriggerControl(SEL)				(0)
#else
#define IS_USART_DMATriggerControl(SEL)				(((SEL) & ~USART_CR1_DMATC) == 0 && (SEL) <= USART_DMA_Trigger2)
#endif

/** @defgroup USART_Async_PolarityControl
* @{
*/
#define USART_PolarityControl_RXP					(USART_CR1_RXP & 0x00010000)
#define USART_PolarityControl_TXP					(USART_CR1_TXP & 0x00020000)
#define IS_USART_Async_PolarityControl(SEL)			((SEL) == USART_PolarityControl_RXP || (SEL) == USART_PolarityControl_TXP)

/** @defgroup USART_Async_SingleLineDirection
* @{
*/
#define USART_SingleLineDirection_Rx				(USART_CR1_SLDS & 0x00000000)
#define USART_SingleLineDirection_Tx				(USART_CR1_SLDS & 0x00200000)
#define IS_USART_Async_SingleLineDirection(SEL)		((SEL) == USART_SingleLineDirection_Rx || (SEL) == USART_SingleLineDirection_Tx)

/** @defgroup USART_Sync_MasterSlaveMode
* @{
*/
#define USART_MasterSlaveMode_Master				(USART_CR1_MSM & 0x00000000)
#define USART_MasterSlaveMode_Slave					(USART_CR1_MSM & 0x01000000)
#define IS_USART_Sync_MasterSlaveMode(SEL)			((SEL) == USART_MasterSlaveMode_Master || (SEL) == USART_MasterSlaveMode_Slave)
	
/** @defgroup USART_Sync_FrameFormat
* @{
*/
#define USART_FrameFormat_MSB						(USART_CR1_FF & 0x00000000)
#define USART_FrameFormat_LSB						(USART_CR1_FF & 0x02000000)
#define IS_USART_Sync_FrameFormat(SEL)				((SEL) == USART_FrameFormat_MSB || (SEL) == USART_FrameFormat_LSB)
	
/** @defgroup USART loopback mode
* @{
*/
#define USART_LoopBackMode_Disable					(USART_CR1_LBM & 0x00000000)
#define USART_LoopBackMode_Enable					(USART_CR1_LBM & 0x80000000)
#define IS_USART_LoopBackMode(SEL)					((SEL) == USART_LoopBackMode_Disable || (SEL) == USART_LoopBackMode_Enable)

/** @defgroup USART_Sync_BaudRate
* @{
*/
#define IS_USART_Sync_BaudRate(SEL)					(((SEL) & 0x01) == 0 && ((SEL) & 0xFFFF0000) == 0)
	
/** @defgroup USART_Async_BaudRate
* @{
*/
#define IS_USART_Async_BaudRate(SEL)				((SEL) <= 3000000)


/** @defgroup USARTAsync_SampleRate
* @{
*/
#define USART_SampleRate_1X							(USART_BRR_SR & 0x00000000)
#define USART_SampleRate_2X							(USART_BRR_SR & 0x00010000)
#define USART_SampleRate_4X							(USART_BRR_SR & 0x00020000)
#define USART_SampleRate_8X							(USART_BRR_SR & 0x00030000)
#define USART_SampleRate_16X						(USART_BRR_SR & 0x00040000)
#define USART_SampleRate_32X						(USART_BRR_SR & 0x00050000)
#define IS_USART_Async_SampleRate(SEL)				(((SEL) & ~USART_BRR_SR) == 0 && ((SEL) & USART_BRR_SR) <= USART_SampleRate_32X)

/** @defgroup USART_IT
* @{
*/
#define USART_IT_RXNE								USART_IER_RXNE
#define USART_IT_RXHF								USART_IER_RXHF
#define USART_IT_RXF								USART_IER_RXF
#define USART_IT_RXTO								USART_IER_RXTO
#define USART_IT_IDLE								USART_IER_IDLE
#define USART_IT_PE									USART_IER_PE
#define USART_IT_FE									USART_IER_FE
#define USART_IT_OVR								USART_IER_OVR
#define USART_IT_TXE								USART_IER_TXE
#define USART_IT_TXHE								USART_IER_TXHE
#define USART_IT_TXF								USART_IER_TXF
#define USART_IT_TXC								USART_IER_TXC
#define USART_IT_SCKTC								USART_IER_SCKTC
#if defined(PT32x012x) || defined (PTM160x)
#define USART_IT_ALL								(USART_IER_RXNE | USART_IER_IDLE | USART_IER_PE | USART_IER_FE | USART_IER_TXE | USART_IER_TXC | USART_IT_SCKTC)
#else
#define USART_IT_ALL								((u32)0x00010FFF)
#endif
#define IS_USART_IT(SEL)							(((SEL) & ~USART_IT_ALL) == 0 && (SEL) != 0x00)

/** @defgroup USART_FLAG
* @{
*/
#define USART_FLAG_RXNE								USART_SR_RXNE
#define USART_FLAG_RXHF								USART_SR_RXHF
#define USART_FLAG_RXF								USART_SR_RXF
#define USART_FLAG_RXTO								USART_SR_RXTO
#define USART_FLAG_IDLE								USART_SR_IDLE
#define USART_FLAG_PE								USART_SR_PE
#define USART_FLAG_FE								USART_SR_FE
#define USART_FLAG_OVR								USART_SR_OVR
#define USART_FLAG_TXE								USART_SR_TXE
#define USART_FLAG_TXHE								USART_SR_TXHE
#define USART_FLAG_TXF								USART_SR_TXF
#define USART_FLAG_TXC								USART_SR_TXC
#define USART_FLAG_SCKTC							USART_SR_SCKTC
#if defined(PT32x012x) || defined (PTM160x)
#define USART_FLAG_ALL								(USART_SR_RXNE | USART_SR_IDLE | USART_SR_PE | USART_SR_FE | USART_SR_TXE | USART_SR_TXC | USART_SR_SCKTC)
#else
#define USART_FLAG_ALL								((u32)0x00010FFF)
#endif
#define IS_USART_FLAG(SEL)							(((SEL) & ~USART_FLAG_ALL) == 0 && (SEL) != 0x00)

/** @defgroup USART_Sync_Data
* @{
*/
#define IS_USART_Sync_Data(SEL)						((SEL) <= 0xFF)

/** @defgroup USART_Async_Data
* @{
*/
#define IS_USART_Async_Data(SEL)					((SEL) <= 0x1FF)

/** @defgroup USART_Sync_SCKCount
* @{
*/
#define IS_USART_Sync_SCKCount(SEL)					((SEL) >= 1 && (SEL) <= 0x1000000)

/** @defgroup USART_Async_FrameIntervalTime
* @{
*/
#if defined(PT32x012x) || defined (PTM160x)
#define IS_USART_Async_FrameIntervalTime(SEL)		(0)
#else
#define IS_USART_Async_FrameIntervalTime(SEL)		((SEL) <= 0xFF)
#endif

/** @defgroup USART_Async_TimeOut
* @{
*/
#if defined(PT32x012x) || defined (PTM160x)
#define IS_USART_Async_Timeout(SEL)					(0)
#else
#define IS_USART_Async_Timeout(SEL)					((SEL) <= 0xFF)
#endif

/** @defgroup USART_RESET_KEY
* @{
*/
#define USART_Reset_RXFR    						((u32)0x0000C5D6)
#define USART_Reset_TXFR   							((u32)0x0000A1B2)

/** @defgroup USART_FIFO
* @{
*/
#define USART_FIFO_RX    							0
#define USART_FIFO_TX    							1
#if defined(PT32x012x) || defined (PTM160x)
#define IS_USART_FIFO(SEL)							(0)
#else
#define IS_USART_FIFO(SEL) 				  			((SEL) == USART_FIFO_RX || (SEL) == USART_FIFO_TX)
#endif

/** @defgroup USART_Async_DataLength
* @{
*/
#define USART_DataLength_7							7
#define USART_DataLength_8							8
#define USART_DataLength_9							9
#define IS_USART_Async_DataLength(SEL)				((SEL) >= USART_DataLength_7 && (SEL) <= USART_DataLength_9)

/** @defgroup USART_Async_ParityMode
* @{
*/
#define USART_Parity_None		   					0
#define USART_Parity_Odd							1
#define USART_Parity_Even		    				2
#define IS_USART_Async_ParityMode(SEL)				((SEL) <= USART_Parity_Even)

#define IS_USART_Async_ErrorDataLengthAndParity(len, parity)	(((len) == USART_DataLength_7 && (parity) == USART_Parity_None) || \
																	((len) == USART_DataLength_9 && (parity) != USART_Parity_None))

/*! function declaration and definition*/
void USART_Sync_Init(USART_TypeDef *USARTx, USART_Sync_InitTypeDef *USART_InitStruct);
void USART_Sync_InitStruct(USART_Sync_InitTypeDef *USART_InitStruct);
void USART_Async_Init(USART_TypeDef *USARTx, USART_Async_InitTypeDef *USART_InitStruct);
void USART_Async_StructInit(USART_Async_InitTypeDef *USART_InitStruct);
void USART_Cmd(USART_TypeDef *USARTx, FunctionalState NewState);
void USART_ReceiverCmd(USART_TypeDef *USARTx, FunctionalState NewState);
void USART_ReceiveDMACmd(USART_TypeDef *USARTx, FunctionalState NewState);
void USART_TransferDMACmd(USART_TypeDef *USARTx, FunctionalState NewState);
#if 0
void USART_Sync_ClockPolarityConfig(USART_TypeDef *USARTx, u32 ClockPolarity);
void USART_Sync_ClockPhaseConfig(USART_TypeDef *USARTx, u32 ClockPhase);
#endif
void USART_Async_WordLengthAndParityConfig(USART_TypeDef *USARTx, u32 WordLenthAndParity);
void USART_Async_StopBitLengthConfig(USART_TypeDef *USARTx, u32 StopBitLenth);
void USART_DMATriggerControlConfig(USART_TypeDef *USARTx, u32 DMATiggerControl);
void USART_Async_SingleLineModeCmd(USART_TypeDef *USARTx, FunctionalState NewState);
void USART_Async_SinglelineDirectionConfig(USART_TypeDef *USARTx, u32 Direction);
void USART_LoopbackModeCmd(USART_TypeDef *USARTx, FunctionalState NewState);
void USART_Async_BaudRateConfig(USART_TypeDef *USARTx, u32 BaudRate, u32 SampleRate);
void USART_ITConfig(USART_TypeDef *USARTx, u32 USART_IT, FunctionalState NewState);
FlagStatus USART_GetFlagStatus(USART_TypeDef *USARTx, u32 USART_FLAG);
void USART_ClearFlag(USART_TypeDef *USARTx, u32 USART_FLAG);
ITStatus USART_GetITStatus(USART_TypeDef *USARTx, u32 USART_IT);
void USART_Sync_SendData(USART_TypeDef *USARTx, u16 Data);
u16 USART_Sync_ReceiveData(USART_TypeDef *USARTx);
void USART_Async_SendData(USART_TypeDef *USARTx, u16 Data);
u16 USART_Async_ReceiveData(USART_TypeDef *USARTx);
void USART_Sync_SendSCKClock(USART_TypeDef *USARTx, u32 Count);
void USART_Async_SetFrameIntervalTime(USART_TypeDef *USARTx, u32 FrameIntervalTime);
void USART_Async_SetTimeout(USART_TypeDef *USARTx, u32 Timeout);
void USART_FifoReset(USART_TypeDef *USARTx, u8 FIFO);
#ifdef __cplusplus
}
#endif

#endif /*__PT32x0xx_USART_H */
