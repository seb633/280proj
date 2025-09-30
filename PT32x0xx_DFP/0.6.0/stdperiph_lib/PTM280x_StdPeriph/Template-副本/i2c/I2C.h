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

/******************************** PWM CONTROL COMMANDS *******************************/
#define PWM_CMD_SET_DUTY    0x01    // 设置PWM占空比命令
#define PWM_CMD_SET_FREQ    0x02    // 设置PWM频率命令  
#define PWM_CMD_START       0x03    // 启动PWM命令
#define PWM_CMD_STOP        0x04    // 停止PWM命令
#define PWM_CMD_STATUS      0x05    // 查询PWM状态命令

// PWM响应状态
#define PWM_STATUS_STOPPED  0x00    // PWM停止状态
#define PWM_STATUS_RUNNING  0x01    // PWM运行状态
#define PWM_RESPONSE_OK     0xAA    // 命令执行成功
#define PWM_RESPONSE_ERROR  0xFF    // 命令执行失败

/******************************** GLOBAL VARIABLES *******************************/
extern unsigned char i2c_rx_buffer[I2C_BUFFER_SIZE];   // Receive buffer
extern unsigned char i2c_tx_buffer[I2C_BUFFER_SIZE];   // Transmit buffer
extern unsigned char i2c_rx_count;                     // Receive byte count
extern unsigned char i2c_tx_count;                     // Transmit byte count  
extern unsigned char i2c_slave_addr;                   // Current slave address
extern unsigned char i2c_data_ready;                   // Data ready flag

// PWM控制变量
extern unsigned int pwm_duty_value;                     // PWM占空比值
extern unsigned int pwm_freq_value;                     // PWM频率值
extern unsigned char pwm_running;                       // PWM运行状态标志

/******************************** FUNCTION PROTOTYPES *******************************/
void I2C_Init(void);                                   // Initialize I2C slave
void I2C_ProcessData(void);                            // Process I2C communication
unsigned char I2C_DataAvailable(void);                 // Check if data is available
unsigned char I2C_ReadData(unsigned char *data);       // Read received data
void I2C_WriteData(unsigned char *data, unsigned char length); // Write data for transmission
void I2C_SetSlaveAddress(void);                        // Set slave address based on PB6/PB7

// PWM控制函数
void I2C_ProcessPWMCommand(void);                       // 处理PWM控制命令
void I2C_SetPWMDuty(unsigned int duty);                // 设置PWM占空比
void I2C_SetPWMFreq(unsigned int freq);                // 设置PWM频率
void I2C_StartPWM(void);                               // 启动PWM
void I2C_StopPWM(void);                                // 停止PWM
unsigned char I2C_GetPWMStatus(void);                  // 获取PWM状态

#endif /* __I2C_H__ */