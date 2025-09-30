/*******************************************************************************
 * Device      : AD16H02
 * Author      : AD Application Team
 * Version     : V0.0.1
 * Date        : 20250928
 * Description : Minimal I2C Slave Implementation
 ********************************************************************************/

#include "I2C.h"

/******************************** GLOBAL VARIABLES ********************************/
unsigned char i2c_rx_buffer[I2C_BUFFER_SIZE];  // Receive buffer
unsigned char i2c_tx_buffer[I2C_BUFFER_SIZE];  // Transmit buffer
unsigned char i2c_rx_count;                    // Receive byte count
unsigned char i2c_tx_count;                    // Transmit byte count
unsigned char i2c_slave_addr;                  // Current slave address
unsigned char i2c_data_ready;                  // Data ready flag

/******************************** PRIVATE FUNCTIONS ********************************/
static void I2C_Delay(void)
{
    __delay_us(2);  // Simple delay for I2C timing
}

static void I2C_SDA_Input(void)
{
    TRISB4 = 1;  // Set SDA as input
}

static void I2C_SDA_Output(void)
{
    TRISB4 = 0;  // Set SDA as output
}

static void I2C_Set_SDA_Low(void)
{
    PB4 = 0;
    I2C_SDA_Output();
}

static void I2C_Set_SDA_High(void)
{
    I2C_SDA_Input();  // Release line (pull-up will make it high)
}

static unsigned char I2C_Read_SDA(void)
{
    return PB4;
}

static unsigned char I2C_Read_SCL(void)
{
    return PB5;
}

static unsigned char I2C_WaitForSCL_High(void)
{
    unsigned char timeout = 200;
    while (!I2C_Read_SCL() && timeout--) {
        I2C_Delay();
    }
    return timeout > 0;
}

static unsigned char I2C_WaitForSCL_Low(void)
{
    unsigned char timeout = 200;
    while (I2C_Read_SCL() && timeout--) {
        I2C_Delay();
    }
    return timeout > 0;
}

static unsigned char I2C_ReceiveByte(void)
{
    unsigned char data = 0;
    unsigned char i;
    
    I2C_SDA_Input(); // Ensure SDA is input
    
    for (i = 0; i < 8; i++) {
        if (!I2C_WaitForSCL_High()) return 0xFF; // Timeout
        
        data = (data << 1);
        if (I2C_Read_SDA()) {
            data |= 0x01;
        }
        
        if (!I2C_WaitForSCL_Low()) return 0xFF; // Timeout
    }
    
    return data;
}

static void I2C_SendACK(void)
{
    I2C_Set_SDA_Low();  // Pull SDA low for ACK
    I2C_WaitForSCL_High();
    I2C_WaitForSCL_Low();
    I2C_Set_SDA_High(); // Release SDA
}

static void I2C_SendNACK(void)
{
    I2C_Set_SDA_High(); // Keep SDA high for NACK
    I2C_WaitForSCL_High();
    I2C_WaitForSCL_Low();
}

static void I2C_SendByte(unsigned char data)
{
    unsigned char i;
    
    for (i = 0; i < 8; i++) {
        if (data & 0x80) {
            I2C_Set_SDA_High();
        } else {
            I2C_Set_SDA_Low();
        }
        data <<= 1;
        
        I2C_WaitForSCL_High();
        I2C_WaitForSCL_Low();
    }
    
    I2C_Set_SDA_High(); // Release for ACK/NACK
}

static unsigned char I2C_WaitForStart(void)
{
    unsigned char timeout = 200;
    
    // Wait for both lines high
    while ((!I2C_Read_SDA() || !I2C_Read_SCL()) && timeout--) {
        I2C_Delay();
    }
    if (timeout == 0) return 0;
    
    timeout = 200;
    // Wait for SDA to go low while SCL is high (start condition)
    while (I2C_Read_SDA() && timeout--) {
        if (!I2C_Read_SCL()) {
            // SCL went low, wait for it to go high again
            if (!I2C_WaitForSCL_High()) return 0;
        }
        I2C_Delay();
    }
    
    return (timeout > 0 && I2C_Read_SCL());
}

/******************************** PUBLIC FUNCTIONS ********************************/
void I2C_Init(void)
{
    unsigned char i;
    
    // Clear buffers
    for (i = 0; i < I2C_BUFFER_SIZE; i++) {
        i2c_rx_buffer[i] = 0;
        i2c_tx_buffer[i] = 0;
    }
    
    i2c_rx_count = 0;
    i2c_tx_count = 0;
    i2c_data_ready = 0;
    
    // Set slave address
    I2C_SetSlaveAddress();
    
    // Configure pins as inputs (idle state)
    TRISB4 = 1; // SDA as input
    TRISB5 = 1; // SCL as input
}

void I2C_SetSlaveAddress(void)
{
    unsigned char addr_bits = 0;
    
    // Configure address pins as inputs
    TRISB6 = 1; // PB6 as input
    TRISB7 = 1; // PB7 as input
    
    __delay_ms(1); // Small delay
    
    // Read address configuration
    if (I2C_ADDR_PIN0) addr_bits |= 0x01;
    if (I2C_ADDR_PIN1) addr_bits |= 0x02;
    
    i2c_slave_addr = I2C_BASE_ADDRESS | addr_bits;
}

unsigned char I2C_DataAvailable(void)
{
    return i2c_data_ready;
}

unsigned char I2C_ReadData(unsigned char *data)
{
    unsigned char i;
    unsigned char count = i2c_rx_count;
    
    if (!i2c_data_ready) return 0;
    
    for (i = 0; i < count && i < I2C_BUFFER_SIZE; i++) {
        data[i] = i2c_rx_buffer[i];
    }
    
    // Clear after reading
    i2c_data_ready = 0;
    i2c_rx_count = 0;
    
    return count;
}

void I2C_WriteData(unsigned char *data, unsigned char length)
{
    unsigned char i;
    unsigned char actual_len = (length < I2C_BUFFER_SIZE) ? length : I2C_BUFFER_SIZE;
    
    for (i = 0; i < actual_len; i++) {
        i2c_tx_buffer[i] = data[i];
    }
    
    i2c_tx_count = actual_len;
}

void I2C_ProcessData(void)
{
    unsigned char addr_byte;
    unsigned char rw_bit;
    unsigned char received_addr;
    unsigned char data_byte;
    unsigned char i;
    
    // Wait for start condition
    if (!I2C_WaitForStart()) {
        return;
    }
    
    // Receive address byte
    addr_byte = I2C_ReceiveByte();
    if (addr_byte == 0xFF) return; // Timeout
    
    rw_bit = addr_byte & 0x01;
    received_addr = addr_byte >> 1;
    
    // Check address match
    if (received_addr != i2c_slave_addr) {
        I2C_SendNACK();
        return;
    }
    
    // Address matched
    I2C_SendACK();
    
    if (rw_bit == 0) {
        // Master write (slave receive)
        i2c_rx_count = 0;
        
        for (i = 0; i < I2C_BUFFER_SIZE; i++) {
            data_byte = I2C_ReceiveByte();
            if (data_byte == 0xFF) break; // Timeout or stop
            
            i2c_rx_buffer[i] = data_byte;
            i2c_rx_count++;
            I2C_SendACK();
            
            // Simple check for stop condition
            I2C_Delay();
            if (I2C_Read_SCL() && I2C_Read_SDA()) {
                break; // Likely stop condition
            }
        }
        
        if (i2c_rx_count > 0) {
            i2c_data_ready = 1;
        }
    } else {
        // Master read (slave transmit)
        for (i = 0; i < i2c_tx_count && i < I2C_BUFFER_SIZE; i++) {
            I2C_SendByte(i2c_tx_buffer[i]);
            
            // Wait for ACK/NACK
            if (!I2C_WaitForSCL_High()) break;
            if (I2C_Read_SDA()) break; // NACK received
            if (!I2C_WaitForSCL_Low()) break;
        }
    }
    
    // Wait for stop condition (simple implementation)
    I2C_Delay();
}