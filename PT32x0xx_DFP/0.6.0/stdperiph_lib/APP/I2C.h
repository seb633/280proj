/*******************************************************************************
 * Device      : AD16H02
 * Author      : AD Application Team  
 * Version     : V0.0.1
 * Date        : 20250928
 * Description : Minimal I2C Slave Implementation Header File
 ********************************************************************************/

#ifndef __I2C_H__
#define __I2C_H__

#include "AD16H02.h"

/******************************** I2C PIN DEFINITIONS *******************************/
#define I2C_SCL_PIN         PB5     // I2C Clock Line (SCL) - PB5
#define I2C_SDA_PIN         PB4     // I2C Data Line (SDA) - PB4

// Address pins for I2C slave address configuration
#define I2C_ADDR_PIN0       PB6     // Address bit 0
#define I2C_ADDR_PIN1       PB7     // Address bit 1

/******************************** I2C CONSTANTS *******************************/
#define I2C_BASE_ADDRESS    0x50    // Base I2C slave address (7-bit)
#define I2C_BUFFER_SIZE     8       // I2C data buffer size (minimal)

/******************************** GLOBAL VARIABLES *******************************/
extern unsigned char i2c_rx_buffer[I2C_BUFFER_SIZE];   // Receive buffer
extern unsigned char i2c_tx_buffer[I2C_BUFFER_SIZE];   // Transmit buffer
extern unsigned char i2c_rx_count;                     // Receive byte count
extern unsigned char i2c_tx_count;                     // Transmit byte count  
extern unsigned char i2c_slave_addr;                   // Current slave address
extern unsigned char i2c_data_ready;                   // Data ready flag

/******************************** FUNCTION PROTOTYPES *******************************/
void I2C_Init(void);                                   // Initialize I2C slave
void I2C_ProcessData(void);                            // Process I2C communication
unsigned char I2C_DataAvailable(void);                 // Check if data is available
unsigned char I2C_ReadData(unsigned char *data);       // Read received data
void I2C_WriteData(unsigned char *data, unsigned char length); // Write data for transmission
void I2C_SetSlaveAddress(void);                        // Set slave address based on PB6/PB7

#endif /* __I2C_H__ */