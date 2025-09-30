/**
  ******************************************************************************
  * @file    PT32x0xx_dma.h
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the DMA firmware
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

#ifndef PT32x0xx_DMA_H
#define PT32x0xx_DMA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PT32x0xx.h"


/* Private define ------------------------------------------------------------*/
/** @defgroup DMA_InitTypeDef
* @{
*/
typedef struct
{
	u32 DMA_SourceBaseAddress;          			/*!< Specifies the Source Address,This parameter can be a value of @ref DMA_SourceBaseAddress */

	u32 DMA_DestinationBaseAddress;     			/*!< Specifies the Destination Address,This parameter can be a value of @ref DMA_DestinationBaseAddress */

	u32 DMA_NumberOfData;          					/*!< Specifies the Number of Blocks for a DMA transfer,This parameter can be a value of @ref DMA_NumberOfData */

	u32 DMA_SourceDataSize;        					/*!< Specifies the DMA Source Data Size,This parameter can be a value of @ref DMA_SourceDataSize */

	u32 DMA_DestinationDataSize;     				/*!< Specifies the DMA Destination Data Size,This parameter can be a value of @ref DMA_DestinationDataSize */

	u32 DMA_SourceAddressIncrement;       			/*!< Specifies the DMA Source Address Increment,This parameter can be a value of @ref DMA_SourceAddressIncrement */

	u32 DMA_DestinationAddressIncrement;  			/*!< Specifies the DMA Destination Address Increment,This parameter can be a value of @ref DMA_DestinationAddressIncrement */

	u32 DMA_Direction;								/*!< Specifies the DMA Direction,This parameter can be a value of @ref DMA_Direction */

	u32 DMA_CircularMode;							/*!< Specifies the DMA Circular mode,This parameter can be a value of @ref DMA_CircularMode */

	u32 DMA_ChannelPriority;						/*!< Specifies the DMA channel config,This parameter can be a value of @ref DMA_ChannelPriority */

	u32 DMA_Burst;                        			/*!<Transmission data burst length,This parameter can be a value of @ref DMA_BURST */
} DMA_InitTypeDef;

/** @defgroup DMA_ALL_PERIPH
* @{
*/
#if defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_DMA_ALL_PERIPH(SEL)      					(0)
#else
#define IS_DMA_ALL_PERIPH(SEL)      					((SEL) == DMA0)
#endif

/** @defgroup DMA_Channel
* @{
*/
#if defined (PT32G031x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM280x)
#define IS_DMA_Channel(SEL)         					((SEL) ==DMA0_CH0 || (SEL) == DMA0_CH1)
#else
#define IS_DMA_Channel(SEL)         					((SEL) == DMA0_CH0 || (SEL) == DMA0_CH1 || (SEL) == DMA0_CH2 || \
															(SEL) == DMA0_CH3 || (SEL) == DMA0_CH4 || (SEL) == DMA0_CH5)
#endif

typedef enum
{
	DMA_CHNUM_0 = 0,
	DMA_CHNUM_1,
	DMA_CHNUM_2,
	DMA_CHNUM_3,
	DMA_CHNUM_4,
	DMA_CHNUM_5,
} DMA_ChannelNum;

#if defined(PT32G031x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM280x)
#define IS_DMA_ChannelNum(SEL)        					((SEL) == DMA_CHNUM_0 || (SEL) == DMA_CHNUM_1)
#else
#define IS_DMA_ChannelNum(SEL)        					((SEL) <= DMA_CHNUM_5)
#endif

/** @defgroup DMA_NumberOfData
* @{
*/
#define IS_DMA_NumberOfData(SEL)      					(SEL > 0 && SEL < 65536)  	//!<Number of data to transfer

/** @defgroup DMA_SourceAddressIncrement
* @{
*/
#define DMA_SourceAddressIncrement_Disable    			(DMA_CCR_SINC & 0x00000000)
#define DMA_SourceAddressIncrement_Enable     			(DMA_CCR_SINC & 0x00000002)
#define IS_DMA_SourceAddressIncrement(SEL)       		((SEL) == DMA_SourceAddressIncrement_Disable || \
															(SEL) == DMA_SourceAddressIncrement_Enable)

/** @defgroup DMA_DestinationAddressIncrement
* @{
*/
#define DMA_DestinationAddressIncrement_Disable    		(DMA_CCR_DINC & 0x00000000)
#define DMA_DestinationAddressIncrement_Enable     		(DMA_CCR_DINC & 0x00000004)
#define IS_DMA_DestinationAddressIncrement(SEL)       	((SEL) == DMA_DestinationAddressIncrement_Disable || \
															(SEL) == DMA_DestinationAddressIncrement_Enable)
/** @defgroup DMA_SourceDataSize
* @{
*/
#define DMA_SourceDataSize_Byte      					(DMA_CCR_SSIZE & 0x00000000)
#define DMA_SourceDataSize_Half      					(DMA_CCR_SSIZE & 0x00000008)
#define DMA_SourceDataSize_Word      					(DMA_CCR_SSIZE & 0x00000010)
#define IS_DMA_SourceDataSize(SEL)     					(((SEL) & ~DMA_CCR_SSIZE) == 0 && (SEL) <= DMA_SourceDataSize_Word) //!<Source data size

/** @defgroup DMA_BURST
  * @{
  */
#define DMA_Burst_1Unit               					(DMA_CCR_BURST & 0x00000000)
#define DMA_Burst_4Unit               					(DMA_CCR_BURST & 0x00000020)
#define DMA_Burst_8Unit               					(DMA_CCR_BURST & 0x00000040)
#define DMA_Burst_16Unit              					(DMA_CCR_BURST & 0x00000060)
#if defined(PT32G031x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM280x)
#define IS_DMA_BURST(SEL)             					((SEL) == DMA_Burst_1Unit)
#else
#define IS_DMA_BURST(SEL)             					(((SEL) & ~DMA_CCR_BURST) == 0 && (SEL) <= DMA_Burst_16Unit)
#endif

/** @defgroup DMA_DestinationDataSize
* @{
*/
#define DMA_DestinationDataSize_Byte      				(DMA_CCR_DSIZE & 0x00000000)
#define DMA_DestinationDataSize_Half      				(DMA_CCR_DSIZE & 0x00000100)
#define DMA_DestinationDataSize_Word      				(DMA_CCR_DSIZE & 0x00000200)
#define IS_DMA_DestinationDataSize(SEL)    				(((SEL) & ~DMA_CCR_DSIZE) == 0 && (SEL) <= DMA_DestinationDataSize_Word) //!<Destination data size

/** @defgroup DMA_ChannelPriority
* @{
*/
#define DMA_ChannelPriority_0    						(DMA_CCR_CPC & 0x00000000)
#define DMA_ChannelPriority_1    						(DMA_CCR_CPC & 0x00000400)
#define DMA_ChannelPriority_2    						(DMA_CCR_CPC & 0x00000800)
#define DMA_ChannelPriority_3    						(DMA_CCR_CPC & 0x00000C00)
#if defined(PT32G031x) || defined (PTM280x)
#define IS_DMA_ChannelPriority(SEL)       				((SEL) == DMA_ChannelPriority_0 || (SEL) == DMA_ChannelPriority_1)
#elif defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x)
#define IS_DMA_ChannelPriority(SEL)       				((SEL) == DMA_ChannelPriority_0)
#else
#define IS_DMA_ChannelPriority(SEL)       				(((SEL) & ~DMA_CCR_CPC) == 0 && (SEL) <= DMA_ChannelPriority_3)
#endif

/** @defgroup DMA_Direction
* @{
*/
#define DMA_Direction_PeripheralToMemory      			(DMA_CCR_DIR & 0x00000000)
#define DMA_Direction_MemoryToMemory          			(DMA_CCR_DIR & 0x00001000)
#define DMA_Direction_MemoryToPeripheral      			(DMA_CCR_DIR & 0x00002000)
#define IS_DMA_Direction(SEL)       					(((SEL) & ~DMA_CCR_DIR) == 0 && (SEL) <= DMA_Direction_MemoryToPeripheral)

/** @defgroup DMA_CircularMode
* @{
*/
#define DMA_CircularMode_Disable    					(DMA_CCR_CIRC & 0x00000000)
#define DMA_CircularMode_Enable     					(DMA_CCR_CIRC & 0x00004000)
#define IS_DMA_CircularMode(SEL)       					((SEL) == DMA_CircularMode_Disable || (SEL) == DMA_CircularMode_Enable)

/** @defgroup DMA_IT
* @{
*/
#define DMA_IT_TC0E               						DMA_IER_TC0E
#define DMA_IT_TC1E               						DMA_IER_TC1E
#define DMA_IT_TC2E               						DMA_IER_TC2E
#define DMA_IT_TC3E              	 					DMA_IER_TC3E
#define DMA_IT_TC4E               						DMA_IER_TC4E
#define DMA_IT_TC5E               						DMA_IER_TC5E
#define DMA_IT_TE0E               						DMA_IER_TE0E
#define DMA_IT_TE1E               						DMA_IER_TE1E
#define DMA_IT_TE2E               						DMA_IER_TE2E
#define DMA_IT_TE3E               						DMA_IER_TE3E
#define DMA_IT_TE4E               						DMA_IER_TE4E
#define DMA_IT_TE5E               						DMA_IER_TE5E
#define DMA_IT_TH0E               						DMA_IER_TH0E
#define DMA_IT_TH1E               						DMA_IER_TH1E
#define DMA_IT_TH2E               						DMA_IER_TH2E
#define DMA_IT_TH3E               						DMA_IER_TH3E
#define DMA_IT_TH4E               						DMA_IER_TH4E
#define DMA_IT_TH5E               						DMA_IER_TH5E
#if defined(PT32G031x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM280x)
#define DMA_IT_ALL                						(DMA_IT_TC0E | DMA_IT_TC1E | DMA_IT_TE0E | DMA_IT_TE1E | DMA_IT_TH0E | DMA_IT_TH1E)
#else
#define DMA_IT_ALL                						((u32)0x003F3F3F)

#endif
#define IS_DMA_IT(SEL)            						(((SEL) & ~DMA_IT_ALL) == 0 && (SEL) != 0)

/** @defgroup DMA_FLAG
* @{
*/
#define DMA_FLAG_TC0F           						DMA_SR_TC0F
#define DMA_FLAG_TC1F           						DMA_SR_TC1F
#define DMA_FLAG_TC2F           						DMA_SR_TC2F
#define DMA_FLAG_TC3F           						DMA_SR_TC3F
#define DMA_FLAG_TC4F           						DMA_SR_TC4F
#define DMA_FLAG_TC5F          	 						DMA_SR_TC5F
#define DMA_FLAG_TE0F           						DMA_SR_TE0F
#define DMA_FLAG_TE1F           						DMA_SR_TE1F
#define DMA_FLAG_TE2F           						DMA_SR_TE2F
#define DMA_FLAG_TE3F           						DMA_SR_TE3F
#define DMA_FLAG_TE4F           						DMA_SR_TE4F
#define DMA_FLAG_TE5F           						DMA_SR_TE5F
#define DMA_FLAG_C0THF          						DMA_SR_TH0F
#define DMA_FLAG_C1THF          						DMA_SR_TH1F
#define DMA_FLAG_C2THF          						DMA_SR_TH2F
#define DMA_FLAG_C3THF          						DMA_SR_TH3F
#define DMA_FLAG_C4THF          						DMA_SR_TH4F
#define DMA_FLAG_C5THF          						DMA_SR_TH5F
#define DMA_FLAG_CEF            						DMA_SR_CEF
#if defined(PT32G031x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM280x)
#define DMA_FLAG_ALL            						(DMA_FLAG_TC0F | DMA_FLAG_TC1F | DMA_FLAG_TE0F | DMA_FLAG_TE1F | \
															DMA_FLAG_C0THF | DMA_FLAG_C1THF | DMA_FLAG_CEF)
#else
#define DMA_FLAG_ALL            						((u32)0x803F3F3F)
#endif
#define IS_DMA_FLAG(SEL)    							(((SEL) & ~DMA_FLAG_ALL) == 0 && (SEL) != 0)

/** @defgroup DMA_ChannelxPERIPH
* @{
*/
#define DMA_CH_SPI0_TX                 					(0x00)
#define DMA_CH_SPI0_RX                 					(0x01)
#define DMA_CH_SPI1_TX                 					(0x02)
#define DMA_CH_SPI1_RX                 					(0x03)
#define DMA_CH_UART0_TX                					(0x0A)
#define DMA_CH_UART0_RX                					(0x0B)
#define DMA_CH_UART1_TX                					(0x0C)
#define DMA_CH_UART1_RX                					(0x0D)
#define DMA_CH_UART2_TX                					(0x0E)
#define DMA_CH_UART2_RX                					(0x0F)
#define DMA_CH_UART3_TX                					(0x10)
#define DMA_CH_UART3_RX                					(0x11)
#define DMA_CH_LPUART0_TX              					(0x12)
#define DMA_CH_LPUART0_RX              					(0x13)
#define DMA_CH_LPUART1_TX              					(0x14)
#define DMA_CH_LPUART1_RX              					(0x15)
#define DMA_CH_USART0_TX               					(0x16)
#define DMA_CH_USART0_RX               					(0x17)
#define DMA_CH_ADC0                    					(0x1E)
#define DMA_CH_SDADC0									(0x1F)

#define DMA_CH_TIM1_AREV_OR_UEV       					(0x20)
#define DMA_CH_TIM1_TEV               					(0x21)
#define DMA_CH_TIM1_OC1_OR_IC1        					(0x22)
#define DMA_CH_TIM1_OC2_OR_IC2        					(0x23)
#define DMA_CH_TIM1_OC3_OR_IC3        					(0x24)
#define DMA_CH_TIM1_OC4_OR_IC4        					(0x25)

#define DMA_CH_TIM5_AREV_OR_UEV       					(0x26)
#define DMA_CH_TIM5_TEV               					(0x27)
#define DMA_CH_TIM5_OC1_OR_IC1        					(0x28)

#define DMA_CH_TIM6_AREV_OR_UEV       					(0x29)
#define DMA_CH_TIM6_TEV               					(0x2A)
#define DMA_CH_TIM6_OC1_OR_IC1        					(0x2B)

#define DMA_CH_TIM7_AREV_OR_UEV       					(0x2C)
#define DMA_CH_TIM7_TEV               					(0x2D)
#define DMA_CH_TIM7_OC1_OR_IC1        					(0x2E)

#define DMA_CH_TIM8_AREV_OR_UEV       					(0x2F)
#define DMA_CH_TIM8_TEV               					(0x30)
#define DMA_CH_TIM8_OC1_OR_IC1        					(0x31)
#define DMA_CH_TIM8_OC2_OR_IC2        					(0x32)
#define DMA_CH_TIM8_OC3_OR_IC3        					(0x33)
#define DMA_CH_TIM8_OC4_OR_IC4        					(0x34)

#define DMA_CH_LPTIM_AREV_OR_UEV      					(0x35)
#define DMA_CH_LPTIM_TEV              					(0x36)
#define DMA_CH_LPTIM_OC1_OR_IC1       					(0x37)
#define DMA_CH_LPTIM_IC2              					(0x38)
#if defined(PT32G031x) || defined (PTM280x)
#define IS_DMA_ChannelPERIPH(SEL)               		((SEL) <= DMA_CH_SPI0_RX || \
															(SEL) == DMA_CH_UART0_TX || (SEL) == DMA_CH_UART0_RX || \
														 	((SEL) >= DMA_CH_ADC0 && (SEL) <= DMA_CH_TIM1_OC4_OR_IC4) || \
														 	((SEL) >= DMA_CH_TIM8_AREV_OR_UEV && (SEL) <= DMA_CH_TIM8_OC4_OR_IC4))
#elif defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x)
#define IS_DMA_ChannelPERIPH(SEL)               		((SEL) <= DMA_CH_SPI0_RX || \
															((SEL) >= DMA_CH_UART0_TX && (SEL) <= DMA_CH_UART1_RX) || \
															((SEL) >= DMA_CH_LPUART0_TX && (SEL) <= DMA_CH_LPUART1_RX) || (SEL) == DMA_CH_SDADC0 || \
														 	(SEL) == DMA_CH_TIM5_AREV_OR_UEV || (SEL) == DMA_CH_TIM5_OC1_OR_IC1 || \
															(SEL) == DMA_CH_TIM6_AREV_OR_UEV || (SEL) == DMA_CH_TIM6_OC1_OR_IC1 || \
															(SEL) == DMA_CH_LPTIM_AREV_OR_UEV || (SEL) == DMA_CH_LPTIM_OC1_OR_IC1)
#else
#define IS_DMA_ChannelPERIPH(SEL) 						((SEL) <= DMA_CH_LPTIM_IC2)
#endif


/*! function declaration and definition*/
void DMA_Init(DMA_Channel_TypeDef *DMA_Channelx, DMA_InitTypeDef *DMA_InitStruct);
void DMA_StructInit(DMA_InitTypeDef *DMA_InitStruct);
void DMA_Cmd(DMA_Channel_TypeDef *DMA_Channelx, FunctionalState NewState);
u32 DMA_GetCurrentSourceAddress(DMA_Channel_TypeDef *DMA_Channelx);
u32 DMA_GetCurrentDestinationAddress(DMA_Channel_TypeDef *DMA_Channelx);
u16 DMA_GetNumberOfData(DMA_Channel_TypeDef *DMA_Channelx);
void DMA_ITConfig(DMA_TypeDef *DMAx, u32 DMA_IT, FunctionalState NewState);
void DMA_ClearITFlag(DMA_TypeDef *DMAx, u32 DMA_FLAG);
FlagStatus DMA_GetFlagStatus(DMA_TypeDef *DMAx, u32 DMA_FLAG);
void DMA_PeripheralConfig(DMA_TypeDef *DMAx, DMA_ChannelNum ch, u32 Peripheral);


#ifdef __cplusplus
}
#endif

#endif /*__PT32x0xx_DMA_H */
