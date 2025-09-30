/**
  ******************************************************************************
  * @file    PT32x0xx_uart.h
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the UART firmware
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

#ifndef PT32x0xx_UART_H
#define PT32x0xx_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PT32x0xx.h"

/** @defgroup UART_InitTypeDef
* @{
*/
typedef struct
{
	u32 UART_BaudRate;					/*!< This member configures the UART communication baud rate. @note UART_BaudRate shouldn't over 3Mbps. */

	u32 UART_SampleRate;				/*!< This member configures the UART communication SampleRate.*/

	u32 UART_DataLength;				/*!< Specifies the Data Length. This parameter can be a value of @ref UART_DataLength */

	u32 UART_StopBitLength;				/*!< Specifies the number of stop bits transmitted. This parameter can be a value of @ref UART_StopBitLength */

	u32 UART_ParityMode;				/*!< Specifies the parity mode. This parameter can be a value of @ref UART_ParityMode */

	u32 UART_Receiver;					/*!< Specifies the receive enable. This parameter can be a value of @ref UART_Receiver */
} UART_InitTypeDef;

/** @defgroup UART_SmartCardInitTypeDef
* @{
*/
typedef struct
{
	u32 SmartCard_SampleRate;			/*!< This member configures the Smart Card communication SampleRate.*/

	u32 SmartCard_ParityMode;			/*!< Specifies the parity mode. This parameter can be a value of @ref UART_ParityMode */
	
	u32 SmartCard_Direction;			/*!< Specifies the Data transfer direction. This parameter can be a value of @ref UART_SmartCardDirection */

	u32 SmartCard_Prescaler;			/*!< Specifies the receive enable. This parameter can be a value of @ref UART_SmartCardPrescaler */
} UART_SmartCardInitTypeDef;

/** @defgroup UART_ALL_PERIPH
* @{
*/
#if defined (PT32x002x) || defined(PT32x012x) || defined (PTM160x)
#define IS_UART_ALL_PERIPH(SEL)						((SEL) == LPUART0)
#elif defined(PT32G031x) || defined (PTM280x)
#define	IS_UART_ALL_PERIPH(SEL)      				((SEL) == UART0)
#elif defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x)
#define	IS_UART_ALL_PERIPH(SEL)      				((SEL) == UART0 || (SEL) == UART1 || (SEL) == LPUART0 || (SEL) == LPUART1)
#else
#define	IS_UART_ALL_PERIPH(SEL)      				((SEL) == UART0 || (SEL) == UART1 || (SEL) == UART2 || \
														(SEL) == UART3 || (SEL) == LPUART0 || (SEL) == LPUART1)
#endif

#if defined (PT32x002x) || defined(PT32G031x) || defined(PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_UART_SmartCard_PERIPH(SEL)   			(0)
#else
#define IS_UART_SmartCard_PERIPH(SEL)   			((SEL) == UART0)
#endif

/** @defgroup UART_Receiver
* @{
*/
#define UART_Receiver_Disable						(UART_CR_RE & 0x00000000)
#define UART_Receiver_Enable						(UART_CR_RE & 0x00000002)
#define IS_UART_Receiver(SEL)						((SEL) == UART_Receiver_Disable || (SEL) == UART_Receiver_Enable)

/** @defgroup UART_RXDMAE
* @{
*/
#define UART_RXDMAE_Disable							(UART_CR_RXDMAE & 0x00000000)
#define UART_RXDMAE_Enable							(UART_CR_RXDMAE & 0x00000004)
#if defined (PT32x002x) || defined(PT32x012x) || defined (PTM160x)
#define IS_UART_RXDMAE(SEL)							(0)
#else
#define IS_UART_RXDMAE(SEL)							((SEL) == UART_RXDMAE_Disable || (SEL) == UART_RXDMAE_Enable)
#endif

/** @defgroup UART_TXDMAE
* @{
*/
#define UART_TXDMAE_Disable							(UART_CR_TXDMAE & 0x00000000)
#define UART_TXDMAE_Enable							(UART_CR_TXDMAE & 0x00000008)
#if defined (PT32x002x) || defined(PT32x012x) || defined (PTM160x)
#define IS_UART_TXDMAE(SEL)							(0)
#else
#define IS_UART_TXDMAE(SEL)							((SEL) == UART_TXDMAE_Disable || (SEL) == UART_TXDMAE_Enable)
#endif
                                             
/** @defgroup UART_WordLengthAndParity
* @{
*/
#define UART_WordLengthAndParity_7DE				(UART_CR_WP & 0x00000000)
#define UART_WordLengthAndParity_7DO				(UART_CR_WP & 0x00000010)
#define UART_WordLengthAndParity_8D					(UART_CR_WP & 0x00000020)
#define UART_WordLengthAndParity_8DW				(UART_CR_WP & 0x00000030)
#define UART_WordLengthAndParity_8DE				(UART_CR_WP & 0x00000040)
#define UART_WordLengthAndParity_8DO				(UART_CR_WP & 0x00000050)
#define UART_WordLengthAndParity_9D					(UART_CR_WP & 0x00000060)
#if defined(PT32G031x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM280x)
#define IS_UART_WordLengthAndParity(SEL)	    	((SEL) == UART_WordLengthAndParity_8D  || (SEL) == UART_WordLengthAndParity_8DE || \
														(SEL) == UART_WordLengthAndParity_8DO || (SEL) == UART_WordLengthAndParity_9D)
#elif defined (PT32x002x) || defined(PT32x012x) || defined (PTM160x)
#define IS_UART_WordLengthAndParity(SEL)	    	(((SEL) & ~UART_CR_WP) == 0 && (SEL) != UART_WordLengthAndParity_8DW && \
														(SEL) <= UART_WordLengthAndParity_9D)
#else
#define IS_UART_WordLengthAndParity(SEL)	    	(((SEL) & ~UART_CR_WP) == 0 && (SEL) <= UART_WordLengthAndParity_9D)
#endif

/** @defgroup UART_StopBitLength
* @{
*/
#define UART_StopBitLength_1						(UART_CR_STOP & 0x00000000)
#define UART_StopBitLength_2						(UART_CR_STOP & 0x00000100)
#define UART_StopBitLength_0P5						(UART_CR_STOP & 0x00000200)
#define UART_StopBitLength_1P5						(UART_CR_STOP & 0x00000300)
#if defined (PT32x002x) || defined(PT32G031x) || defined(PT32x012x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM160x) || defined (PTM280x)
#define IS_UART_StopBitLength(SEL)					((SEL) == UART_StopBitLength_1 || (SEL) == UART_StopBitLength_2)
#else
#define IS_UART_StopBitLength(SEL)					(((SEL) & ~UART_CR_STOP) == 0 && (SEL) <= UART_StopBitLength_1P5)
#endif

/** @defgroup UART_DMATrigger
* @{
*/
#define UART_DMA_Trigger0							(UART_CR_DMATC & 0x00000000)
#define UART_DMA_Trigger1							(UART_CR_DMATC & 0x00001000)
#define UART_DMA_Trigger2							(UART_CR_DMATC & 0x00002000)
#if defined (PT32x002x) || defined(PT32G031x) || defined(PT32x012x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM160x) || defined (PTM280x)
#define IS_UART_DMATriggerControl(SEL)				(0)
#else
#define IS_UART_DMATriggerControl(SEL)				(((SEL) & ~UART_CR_DMATC) == 0 && (SEL) <= UART_DMA_Trigger2)
#endif

/** @defgroup UART_PolarityControl
* @{
*/
#define UART_PolarityControl_RXP					(UART_CR_RXP & 0x00010000)
#define UART_PolarityControl_TXP					(UART_CR_TXP & 0x00020000)
#define IS_UART_PolarityControl(SEL)				((SEL) == UART_PolarityControl_RXP || (SEL) == UART_PolarityControl_TXP)
                                           
/** @defgroup UART_SingleLineDirection
* @{
*/
#define UART_SingleLineDirection_Rx					(UART_CR_SLDS & 0x00000000)
#define UART_SingleLineDirection_Tx					(UART_CR_SLDS & 0x00200000)
#define IS_UART_SingleLineDirection(SEL)			((SEL) == UART_SingleLineDirection_Rx || (SEL) == UART_SingleLineDirection_Tx)

/** @defgroup UART_MuteModeWakeupMethod
* @{
*/
#define UART_MuteModeWakeupMethod_Idle				(UART_CR_MMWU & 0x00000000)
#define UART_MuteModeWakeupMethod_Match				(UART_CR_MMWU & 0x01000000)
#if defined (PT32x002x) || defined(PT32G031x) || defined(PT32x012x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM160x) || defined (PTM280x)
#define IS_UART_MuteModeWakeupMethod(SEL)			(0)
#else
#define IS_UART_MuteModeWakeupMethod(SEL)			((SEL) == UART_MuteModeWakeupMethod_Idle || (SEL) == UART_MuteModeWakeupMethod_Match)
#endif

/** @defgroup UART_Wakeup
* @{
*/
#define UART_WakeupMethod_RXNE		          		(UART_CR_LPWU & 0x00000000)
#define UART_WakeupMethod_StartBit		      		(UART_CR_LPWU & 0x04000000)
#if defined (PT32x002x) || defined(PT32G031x) || defined(PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_UART_WakeupMode(SEL)						(0)
#else
#define IS_UART_WakeupMode(SEL)						((SEL) == UART_WakeupMethod_RXNE || (SEL) == UART_WakeupMethod_StartBit)
#endif

/** @defgroup UART loopback mode
* @{
*/
#define UART_LoopBackMode_Disable					(UART_CR_LBM & 0x00000000)
#define UART_LoopBackMode_Enable					(UART_CR_LBM & 0x80000000)
#define IS_UART_LoopBackMode(SEL)					((SEL) == UART_LoopBackMode_Disable || (SEL) == UART_LoopBackMode_Enable)

/** @defgroup UART_SlaveDeviceAddress
* @{
*/
#if defined (PT32x002x) || defined(PT32G031x) || defined(PT32x012x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM160x) || defined (PTM280x)
#define IS_UART_SlaveDeviceAddress(SEL)				(0)
#else
#define IS_UART_SlaveDeviceAddress(SEL)				((SEL) <= 0xFF)
#endif

/** @defgroup UART_SampleRate
* @{
*/
#define UART_SampleRate_1X							(UART_BRR_SR & 0x00000000)
#define UART_SampleRate_2X							(UART_BRR_SR & 0x00010000)
#define UART_SampleRate_4X							(UART_BRR_SR & 0x00020000)
#define UART_SampleRate_8X							(UART_BRR_SR & 0x00030000)
#define UART_SampleRate_16X							(UART_BRR_SR & 0x00040000)
#define UART_SampleRate_32X							(UART_BRR_SR & 0x00050000)
#if defined (PT32G031x) || defined (PTM280x)
#define IS_UART_SampleRate(SEL)						((SEL) == UART_SampleRate_8X || (SEL) == UART_SampleRate_16X)
#elif defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x)
#define IS_UART_SampleRate(SEL)						((SEL) == UART_SampleRate_4X || (SEL) == UART_SampleRate_8X || (SEL) == UART_SampleRate_16X)
#else
#define IS_UART_SampleRate(SEL)						(((SEL) & ~UART_BRR_SR) == 0 && ((SEL) & UART_BRR_SR) <= UART_SampleRate_32X)
#endif

/** @defgroup UART_IT
* @{
*/
#define UART_IT_RXNE								UART_IER_RXNE
#define UART_IT_RXHF								UART_IER_RXHF
#define UART_IT_RXF									UART_IER_RXF
#define UART_IT_RXTO								UART_IER_RXTO
#define UART_IT_IDLE								UART_IER_IDLE
#define UART_IT_PE									UART_IER_PE
#define UART_IT_FE									UART_IER_FE
#define UART_IT_OVR									UART_IER_OVR
#define UART_IT_TXE									UART_IER_TXE
#define UART_IT_TXHE								UART_IER_TXHE
#define UART_IT_TXF									UART_IER_TXF
#define UART_IT_TXC									UART_IER_TXC
#define UART_IT_LPWU								UART_IER_LPWU
#define UART_IT_NACK								UART_IER_NACK
#if defined(PT32G031x) || defined (PTM280x)
#define UART_IT_ALL         						(UART_IT_RXNE | UART_IT_RXTO | UART_IT_IDLE | UART_IT_PE | \
														UART_IT_FE | UART_IT_OVR | UART_IT_TXE | UART_IT_TXC)
#elif defined (PT32x002x) || defined(PT32x012x) || defined (PTM160x)
#define UART_IT_ALL									(UART_IT_RXNE | UART_IT_IDLE | UART_IT_PE | UART_IT_FE | \
														UART_IT_TXE | UART_IT_TXC | UART_IT_LPWU)
#elif defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x)
#define UART_IT_ALL         						(UART_IT_RXNE | UART_IT_IDLE | UART_IT_PE | UART_IT_FE | \
														UART_IT_OVR | UART_IT_TXE | UART_IT_TXC | UART_IT_LPWU | UART_IT_NACK)
#else
#define UART_IT_ALL									((u32)0x00012FFF)
#endif
#define IS_UART_IT(SEL)								(((SEL) & ~UART_IT_ALL) == 0 && (SEL) != 0x00)

/** @defgroup UART_FLAG
* @{
*/
#define UART_FLAG_RXNE								UART_SR_RXNE
#define UART_FLAG_RXHF								UART_SR_RXHF
#define UART_FLAG_RXF								UART_SR_RXF
#define UART_FLAG_RXTO								UART_SR_RXTO
#define UART_FLAG_IDLE								UART_SR_IDLE
#define UART_FLAG_PE								UART_SR_PE
#define UART_FLAG_FE								UART_SR_FE
#define UART_FLAG_OVR								UART_SR_OVR
#define UART_FLAG_TXE								UART_SR_TXE
#define UART_FLAG_TXHE								UART_SR_TXHE
#define UART_FLAG_TXF								UART_SR_TXF
#define UART_FLAG_TXC								UART_SR_TXC
#define UART_FLAG_MUTE								UART_SR_MUTE
#define UART_FLAG_LPWU								UART_SR_LPWU
#define UART_FLAG_NACK								UART_SR_NACK
#if defined(PT32G031x) || defined (PTM280x)
#define UART_FLAG_ALL         						(UART_FLAG_RXNE | UART_FLAG_RXTO | UART_FLAG_IDLE | UART_FLAG_PE | \
														UART_FLAG_FE | UART_FLAG_OVR | UART_FLAG_TXE | UART_FLAG_TXC)
#elif defined (PT32x002x) || defined(PT32x012x) || defined (PTM160x)
#define UART_FLAG_ALL								(UART_FLAG_RXNE | UART_FLAG_IDLE | UART_FLAG_PE | UART_FLAG_FE | \
														UART_FLAG_TXE | UART_FLAG_TXC | UART_FLAG_LPWU)
#elif defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x)
#define UART_FLAG_ALL         						(UART_FLAG_RXNE | UART_FLAG_IDLE | UART_FLAG_PE | UART_FLAG_FE | \
														UART_FLAG_OVR | UART_FLAG_TXE | UART_FLAG_TXC | UART_FLAG_LPWU | UART_FLAG_NACK)
#else
#define UART_FLAG_ALL								((u32)0x00013FFF)
#endif
#define IS_UART_FLAG(SEL)							(((SEL) & ~UART_FLAG_ALL) == 0 && (SEL) != 0x00)

/** @defgroup UART_Data
* @{
*/
#define IS_UART_Data(SEL)							((SEL) <= 0x1FF)

/** @defgroup UART_FrameIntervalTime
* @{
*/
#if defined (PT32x002x) || defined(PT32x012x) || defined (PTM160x)
#define IS_UART_FrameIntervalTime(SEL)				(0)
#else
#define IS_UART_FrameIntervalTime(SEL)				((SEL) <= 0xFF)
#endif

/** @defgroup UART_Timeout
* @{
*/
#if defined (PT32x002x) || defined(PT32x012x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM160x)
#define IS_UART_Timeout(SEL)						(0)
#else
#define IS_UART_Timeout(SEL)						((SEL) <= 0xFF)
#endif

/** @defgroup UART_RESET_KEY
* @{
*/
#define UART_Reset_RXFR    							((u32)0x0000C5D6)
#define UART_Reset_TXFR   							((u32)0x0000A1B2)

/** @defgroup UART_FIFO
* @{
*/
#define UART_FIFO_RX    							0
#define UART_FIFO_TX    							1
#if defined (PT32x002x) || defined(PT32G031x) || defined(PT32x012x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM160x) || defined (PTM280x)
#define IS_UART_FIFO(SEL)							(0)
#else
#define IS_UART_FIFO(SEL) 				  			((SEL) == UART_FIFO_RX || (SEL) == UART_FIFO_TX)
#endif

/** @defgroup UART_SmartCardDirection
* @{
*/
#define UART_SmartCardDir_Rx						(UART_SCCR_DIR & 0x00000000)
#define UART_SmartCardDir_Tx						(UART_SCCR_DIR & 0x00000002)
#define IS_UART_SmartCardDirection(SEL)				((SEL) == UART_SmartCardDir_Rx || (SEL) == UART_SmartCardDir_Tx)

/** @defgroup UART_SmartCardPrescaler
* @{
*/
#define UART_SmartCardPSC_Div2						(UART_SCCR_PSC & 0x00000000)
#define UART_SmartCardPSC_Div4						(UART_SCCR_PSC & 0x00000010)
#define UART_SmartCardPSC_Div8						(UART_SCCR_PSC & 0x00000020)
#define UART_SmartCardPSC_Div16						(UART_SCCR_PSC & 0x00000030)
#define UART_SmartCardPSC_Div32						(UART_SCCR_PSC & 0x00000040)
#define UART_SmartCardPSC_Div64						(UART_SCCR_PSC & 0x00000050)
#define UART_SmartCardPSC_Div128					(UART_SCCR_PSC & 0x00000060)
#if defined (PT32x002x) || defined(PT32G031x) || defined(PT32x012x) || defined (PTM160x) || defined (PTM280x)
#define IS_UART_SmartCardPrescaler(SEL)	 			(0)
#else
#define IS_UART_SmartCardPrescaler(SEL)	 			((SEL) <= UART_SmartCardPSC_Div128)
#endif

/** @defgroup UART_BaudRate
* @{
*/
#define IS_UART_BaudRate(SEL)						((SEL) <= 3000000)

/** @defgroup UART_DataLength
* @{
*/
#define UART_DataLength_7							7
#define UART_DataLength_8							8
#define UART_DataLength_9							9
#if defined(PT32G031x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM280x)
#define IS_UART_DataLength(SEL)						((SEL) == UART_DataLength_8 || (SEL) == UART_DataLength_9)
#else
#define IS_UART_DataLength(SEL)						((SEL) >= UART_DataLength_7 && (SEL) <= UART_DataLength_9)
#endif

/** @defgroup UART_ParityMode
* @{
*/
#define UART_Parity_None							0
#define UART_Parity_Odd								1
#define UART_Parity_Even							2
#define IS_UART_ParityMode(SEL)						((SEL) <= UART_Parity_Even)
#define IS_UART_SmartCardParityMode(SEL)			((SEL) == UART_Parity_Odd || (SEL) == UART_Parity_Even)

#define IS_UART_ErrorDataLengthAndParity(len, parity)	(((len) == UART_DataLength_7 && (parity) == UART_Parity_None) || \
															((len) == UART_DataLength_9 && (parity) != UART_Parity_None))

/*! function declaration and definition*/
void UART_Init(UART_TypeDef *UARTx, UART_InitTypeDef *UART_InitStruct);
void UART_SmartCardInit(UART_TypeDef *UARTx, UART_SmartCardInitTypeDef *UART_SmartCardInitStruct);
void UART_StructInit(UART_InitTypeDef *UART_InitStruct);
void UART_SmartCardStructInit(UART_SmartCardInitTypeDef *UART_SmartCardInitStruct);
void UART_Cmd(UART_TypeDef *UARTx, FunctionalState NewState);
void UART_ReceiverCmd(UART_TypeDef *UARTx, FunctionalState NewState);
void UART_ReceiveDMACmd(UART_TypeDef *UARTx, FunctionalState NewState);
void UART_TransferDMACmd(UART_TypeDef *UARTx, FunctionalState NewState);
void UART_WordLengthAndParityConfig(UART_TypeDef *UARTx, u32 WordLenthAndParity);
void UART_StopBitLengthConfig(UART_TypeDef *UARTx, u32 StopBitLenth);
void UART_PolarityControl(UART_TypeDef *UARTx, u32 Polarity, FunctionalState NewState);
void UART_SingleLineModeCmd(UART_TypeDef *UARTx, FunctionalState NewState);
void UART_SinglelineDirectionConfig(UART_TypeDef *UARTx, u32 Direction);
void UART_MuteModeWakeupMethodConfig(UART_TypeDef *UARTx, u32 MuteModeWakeUpMethod);
void UART_MuteModeCmd(UART_TypeDef *UARTx, FunctionalState NewState);
void UART_LowPowerWakeupConfig(UART_TypeDef *UARTx, u32 Method);
void UART_LoopbackModeCmd(UART_TypeDef *UARTx, FunctionalState NewState);
void UART_BaudRateConfig(UART_TypeDef *UARTx,u32 BaudRate,u32 SampleRate);
void UART_SlaveDeviceAddressConfig(UART_TypeDef *UARTx, u32 Address);
void UART_ITConfig(UART_TypeDef *UARTx, u32 UART_IT, FunctionalState NewState);
FlagStatus UART_GetFlagStatus(UART_TypeDef *UARTx, u32 UART_FLAG);
void UART_ClearFlag(UART_TypeDef *UARTx, u32 UART_CLEAR_FLAG);
ITStatus UART_GetITStatus(UART_TypeDef *UARTx, u32 UART_IT);
void UART_SendData(UART_TypeDef *UARTx, u16 Data);
u16 UART_ReceiveData(UART_TypeDef *UARTx);
void UART_SetFrameIntervalTime(UART_TypeDef *UARTx, u32 FrameIntervalTime);
void UART_SetTimeout(UART_TypeDef *UARTx, u32 Timeout);
void UART_FifoReset(UART_TypeDef *UARTx, u8 FIFO);
void UART_SmartCardCmd(UART_TypeDef *UARTx, FunctionalState NewState);
void UART_SmartCardDirectionConfig(UART_TypeDef *UARTx, u32 Direction);
#ifdef __cplusplus
}
#endif

#endif /*__PT32x0xx_UART_H */
