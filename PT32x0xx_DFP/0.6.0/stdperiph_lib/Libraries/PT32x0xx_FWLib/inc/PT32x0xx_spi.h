/**
  *******************************************************************************
  * @file    PT32x0xx_spi.h
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   This file contains all the functions prototypes for the SPI firmware library.
	*
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of
  * the content or the codes within the content.
  *
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved
  *******************************************************************************
**/

#ifndef PT32x0xx_SPI_H
#define PT32x0xx_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PT32x0xx.h"

/* Private define ------------------------------------------------------------*/
/** @defgroup SPI_InitTypeDef
* @{
*/
typedef struct
{
	u32 SPI_MasterSlaveMode;            /*!< Specifies the SPI mode,This parameter can be a value of @ref SPI_MasterSlaveMode */

	u32 SPI_DataFrameFormat;            /*!< Specifies the SPI data size,This parameter can be a value of @ref SPI_DataFrameFormat */

	u32 SPI_ClockPolarity;              /*!< Specifies the serial clock Polarity,This parameter can be a value of @ref SPI_ClockPolarity */

	u32 SPI_ClockPhase;                 /*!< Specifies the clock active edge,This parameter can be a value of @ref SPI_ClockPhase */

	u32 SPI_CSS;                        /*!< Specifies whether the CSS signal is managed by hardware,This parameter can be a value of @ref SPI_CSS */

	u32 SPI_SoftwareControlCSS;         /*!< Specifies Software Control CSS,This parameter can be a value of @ref SPI_SoftwareControlCSS */

	u32 SPI_Prescaler;                  /*!< Specifies the Baud Rate prescaler value,This parameter can be a number between 1 and 256 */

	u32 SPI_BaudRate;                   /*!< Specifies the Baud Rate value,This parameter can be a even number between 2 and 254 */

} SPI_InitTypeDef;

/** @defgroup SPI_ALL_PERIPH
* @{
*/
#if defined (PT32G031x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM280x)
#define IS_SPI_ALL_PERIPH(SEL) 						((SEL) == SPI0)
#elif defined (PT32x002x) || defined (PT32x012x) || defined (PTM160x)
#define IS_SPI_ALL_PERIPH(SEL) 						(0)
#else
#define IS_SPI_ALL_PERIPH(SEL) 						((SEL) == SPI0 || (SEL) == SPI1)
#endif

/** @defgroup SPI_CSS
* @{
*/
#define SPI_CSS_HardwareControl						(SPI_CR1_CSS & 0x00000000)
#define SPI_CSS_SoftwareControl						(SPI_CR1_CSS & 0x00000002)
#define IS_SPI_CSS(SEL)								((SEL) == SPI_CSS_HardwareControl || (SEL) == SPI_CSS_SoftwareControl)

/** @defgroup SPI_DataFrameFormat
* @{
*/
#define SPI_DataFrameFormat_4b						(SPI_CR1_DFF & 0x00000030)
#define SPI_DataFrameFormat_5b						(SPI_CR1_DFF & 0x00000040)
#define SPI_DataFrameFormat_6b						(SPI_CR1_DFF & 0x00000050)
#define SPI_DataFrameFormat_7b						(SPI_CR1_DFF & 0x00000060)
#define SPI_DataFrameFormat_8b						(SPI_CR1_DFF & 0x00000070)
#define SPI_DataFrameFormat_9b						(SPI_CR1_DFF & 0x00000080)
#define SPI_DataFrameFormat_10b						(SPI_CR1_DFF & 0x00000090)
#define SPI_DataFrameFormat_11b						(SPI_CR1_DFF & 0x000000A0)
#define SPI_DataFrameFormat_12b						(SPI_CR1_DFF & 0x000000B0)
#define SPI_DataFrameFormat_13b						(SPI_CR1_DFF & 0x000000C0)
#define SPI_DataFrameFormat_14b						(SPI_CR1_DFF & 0x000000D0)
#define SPI_DataFrameFormat_15b						(SPI_CR1_DFF & 0x000000E0)
#define SPI_DataFrameFormat_16b						(SPI_CR1_DFF & 0x000000F0)
#if defined (PT32G031x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM280x)
#define IS_SPI_DataFrameFormat(SEL)	 				((SEL) == SPI_DataFrameFormat_8b)
#else
#define IS_SPI_DataFrameFormat(SEL)	 				(((SEL) & ~SPI_CR1_DFF) == 0 && (SEL) >= SPI_DataFrameFormat_4b && \
														(SEL) <= SPI_DataFrameFormat_16b)
#endif

/** @defgroup SPI_ClockPolarity
* @{
*/
#define SPI_ClockPolarity_Low						(SPI_CR1_CPOL & 0x00000000)
#define SPI_ClockPolarity_High						(SPI_CR1_CPOL & 0x00000100)
#define IS_SPI_ClockPolarity(SEL)					((SEL) == SPI_ClockPolarity_Low || (SEL) == SPI_ClockPolarity_High)

/** @defgroup SPI_ClockPhase
* @{
*/
#define SPI_ClockPhase_1Edge						(SPI_CR1_CPHA & 0x00000000)
#define SPI_ClockPhase_2Edge						(SPI_CR1_CPHA & 0x00000200)
#define IS_SPI_ClockPhase(SEL)						((SEL) == SPI_ClockPhase_1Edge || (SEL) == SPI_ClockPhase_2Edge)

/** @defgroup SPI_MasterSlaveMode
* @{
*/
#define SPI_MasterSlaveMode_Master   				(SPI_CR1_MSM & 0x00000000)
#define SPI_MasterSlaveMode_Slave					(SPI_CR1_MSM & 0x00001000)
#define IS_SPI_MasterSlaveMode(SEL)					((SEL) == SPI_MasterSlaveMode_Master || (SEL) == SPI_MasterSlaveMode_Slave)

/** @defgroup SPI_FrameFormat
* @{
*/
#define SPI_FrameFormat_MSB							(SPI_CR1_FF & 0x00000000)
#define SPI_FrameFormat_LSB							(SPI_CR1_FF & 0x00004000)
#define IS_SPI_FrameFormat(SEL)						((SEL) == SPI_FrameFormat_MSB || (SEL) == SPI_FrameFormat_LSB)

/** @defgroup SPI_RXDMAE
* @{
*/
#define SPI_RXDMAE_Disable							(SPI_CR1_RXDMAE & 0x00000000)
#define SPI_RXDMAE_Enable							(SPI_CR1_RXDMAE & 0x00010000)
#define IS_SPI_RXDMAE(SEL)							((SEL) == SPI_RXDMAE_Disable || (SEL) == SPI_RXDMAE_Enable)

/** @defgroup SPI_TXDMAE
* @{
*/
#define SPI_TXDMAE_Disable							(SPI_CR1_TXDMAE & 0x00000000)
#define SPI_TXDMAE_Enable							(SPI_CR1_TXDMAE & 0x00020000)
#define IS_SPI_TXDMAE(SEL)							((SEL) == SPI_TXDMAE_Disable || (SEL) == SPI_TXDMAE_Enable)

/** @defgroup SPI_DMATrigger
* @{
*/
#define SPI_DMA_TriggerControl_1					(SPI_CR1_DMATC & 0x00000000)
#define SPI_DMA_TriggerControl_4					(SPI_CR1_DMATC & 0x00100000)
#define IS_SPI_DMATrigger(SEL)						((SEL) == SPI_DMA_TriggerControl_1 || (SEL) == SPI_DMA_TriggerControl_4)

/** @defgroup SPI loopback mode
* @{
*/
#define SPI_LoopBackMode_Disable					(SPI_CR1_LBM & 0x00000000)
#define SPI_LoopBackMode_Enable						(SPI_CR1_LBM & 0x80000000)
#define IS_SPI_LoopBackMode(SEL)					((SEL) == SPI_LoopBackMode_Disable || (SEL) == SPI_LoopBackMode_Enable)

/** @defgroup SPI_SoftwareControlCSS
* @{
*/
#define SPI_SoftwareControlCSS_Low          		(SPI_CR2_SWCS & 0x00000000)
#define SPI_SoftwareControlCSS_High         		(SPI_CR2_SWCS & 0x00000001)
#define IS_SPI_SoftwareControlCSS(SEL)				((SEL) == SPI_SoftwareControlCSS_Low || (SEL) == SPI_SoftwareControlCSS_High)

/** @defgroup SPI_BaudRate
* @{
*/
#define IS_SPI_BaudRate(SEL) 						(((SEL) & 0x01) == 0  && (SEL) < 255)

/** @defgroup SPI_Prescaler
* @{
*/
#define IS_SPI_Prescaler(SEL) 			 			((SEL) >= 1 && (SEL) <= 256)

/** @defgroup SPI_IT
* @{
*/
#define SPI_IT_RXNE 								SPI_IER_RXNE
#define SPI_IT_RXHF 								SPI_IER_RXHF
#define SPI_IT_OVR 									SPI_IER_OVR
#define SPI_IT_RXTO 								SPI_IER_RXTO
#define SPI_IT_TXE									SPI_IER_TXE
#define SPI_IT_TXHE									SPI_IER_TXHE
#if defined (PT32G031x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM280x)
#define SPI_IT_ALL									(SPI_IT_RXNE | SPI_IT_OVR | SPI_IT_RXTO | SPI_IT_TXE)
#else
#define SPI_IT_ALL									((u32)0x0000031B)
#endif
#define IS_SPI_IT(SEL)								(((SEL) & ~SPI_IT_ALL) == 0 && (SEL) != 0)

/** @defgroup SPI_FLAG
* @{
*/
#define	SPI_FLAG_RXNE   							SPI_SR_RXNE
#define	SPI_FLAG_RXHF   							SPI_SR_RXHF
#define	SPI_FLAG_RXF    							SPI_SR_RXF
#define	SPI_FLAG_OVR    							SPI_SR_OVR
#define	SPI_FLAG_RXTO   							SPI_SR_RXTO
#define	SPI_FLAG_TXE    							SPI_SR_TXE
#define	SPI_FLAG_TXHE 								SPI_SR_TXHE
#define	SPI_FLAG_TNF								SPI_SR_TNF
#define	SPI_FLAG_BSY								SPI_SR_BSY
#if defined (PT32G031x) || defined(PT32x060x) || defined(PT32x063x) || defined(PT32x066x) || defined (PTM280x)
#define SPI_FLAG_ALL								(SPI_FLAG_RXNE | SPI_FLAG_OVR | SPI_FLAG_RXTO | SPI_FLAG_TXE | SPI_FLAG_BSY)
#else
#define SPI_FLAG_ALL								 ((u32)0x0000871F)
#endif
#define IS_SPI_FLAG(SEL) 							(((SEL) & ~SPI_FLAG_ALL) == 0 && (SEL) != 0)

/** @defgroup SPI_RESET_KEY
* @{
*/
#define SPI_Reset_RXFR    							((u32)0x0000C5D6)
#define SPI_Reset_TXFR   							((u32)0x0000A1B2)

/** @defgroup SPI_FIFO
* @{
*/
#define SPI_FIFO_RX    								0
#define SPI_FIFO_TX    								1
#define IS_SPI_FIFO(SEL) 				  			((SEL) == SPI_FIFO_RX || (SEL) == SPI_FIFO_TX)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void SPI_Init(SPI_TypeDef *SPIx, SPI_InitTypeDef *SPI_InitStruct);
void SPI_StructInit(SPI_InitTypeDef *SPI_InitStruct);
void SPI_Cmd(SPI_TypeDef *SPIx, FunctionalState NewState);
void SPI_SlaveOutputConfig(SPI_TypeDef *SPIx, FunctionalState NewState);
void SPI_FrameFormatConfig(SPI_TypeDef *SPIx, u16 Format);
void SPI_ReceiveDMACmd(SPI_TypeDef *SPIx, FunctionalState NewState);
void SPI_TransferDMACmd(SPI_TypeDef *SPIx, FunctionalState NewState);
void SPI_DMATriggerControlConfig(SPI_TypeDef *SPIx, u32 DMA_TriggerControl);
void SPI_SoftwareControlCSSConfig(SPI_TypeDef *SPIx, u32 SWCS_Signal);
void SPI_BaudRateConfig(SPI_TypeDef *SPIx, u32 BaudRate, u32 Prescaler);
void SPI_ITConfig(SPI_TypeDef *SPIx, u8 SPI_IT, FunctionalState NewState);
FlagStatus SPI_GetFlagStatus(SPI_TypeDef *SPIx, u16 SPI_FLAG);
void SPI_ClearFlag(SPI_TypeDef *SPIx, u16 SPI_FLAG);
u8 SPI_GetFifoSize(SPI_TypeDef *SPIx, u8 SPI_FIFO);
void SPI_SendData(SPI_TypeDef *SPIx, u16 Data);
u16 SPI_ReceiveData(SPI_TypeDef *SPIx);
void SPI_FifoReset(SPI_TypeDef *SPIx, u8 SPI_FIFO);

#ifdef __cplusplus
}
#endif

#endif /*__PT32x0xx_SPI_H */
