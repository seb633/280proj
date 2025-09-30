/******************************************************************************
  * @file    ccube_uart_protocol.h
  * @author  C-Cube Development Team
  * @version V1.0.0
  * @date    2025/09/17
  * @brief   C-Cube UART通信协议头文件
  *          实现基于Modbus-RTU CRC16校验的自定义通信协议
  ******************************************************************************
  * @attention
  *
  * Copyright (C) 2025, C-Cube Team, all rights reserved
  *
  *****************************************************************************/

#ifndef __CCUBE_UART_PROTOCOL_H
#define __CCUBE_UART_PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PT32x0xx.h"
#include <stdint.h>
#include <stdbool.h>

/* 协议常量定义 */
#define PROTOCOL_FRAME_HEADER       0xAA    // 帧头
#define PROTOCOL_MAX_DATA_LEN       255     // 最大数据长度
#define PROTOCOL_MAX_FRAME_LEN      260     // 最大帧长度 (头+命令+长度+数据+CRC)
#define PROTOCOL_MIN_FRAME_LEN      5       // 最小帧长度 (头+命令+长度+CRC)

/* 帧结构偏移量定义 */
#define FRAME_OFFSET_HEADER         0       // 帧头偏移
#define FRAME_OFFSET_CMD            1       // 命令字偏移
#define FRAME_OFFSET_LEN            2       // 数据长度偏移
#define FRAME_OFFSET_DATA           3       // 数据偏移

/* 命令字定义 */
typedef enum {
    CMD_SET_INPUT_VOLTAGE       = 0x01,     // 设置输入电压阈值
    CMD_SET_OUTPUT_VOLTAGE      = 0x02,     // 设置输出电压阈值
    CMD_SET_MAX_INPUT_CURRENT   = 0x03,     // 设置最大输入电流
    CMD_SET_MAX_OUTPUT_CURRENT  = 0x04,     // 设置最大输出电流
    CMD_SET_MAX_OUTPUT_POWER    = 0x05,     // 设置最大输出功率
    CMD_QUERY_STATUS            = 0x06,     // 查询实时状态
    CMD_STATUS_REPORT           = 0x07,     // 实时状态上报
    CMD_MODE_SWITCH             = 0x08,     // 模式切换
    CMD_ACK_ERROR               = 0x09      // ACK/错误码
} Protocol_Command_t;

/* 工作模式定义 */
typedef enum {
    MODE_BUCK                   = 0,        // BUCK模式
    MODE_BOOST                  = 1,        // BOOST模式
    MODE_AUTO                   = 2         // 自动模式
} Working_Mode_t;

/* 错误码定义 */
typedef enum {
    ERROR_OK                    = 0x00,     // 成功
    ERROR_INVALID_CMD           = 0x01,     // 无效命令
    ERROR_INVALID_DATA_LEN      = 0x02,     // 数据长度错误
    ERROR_CRC_FAILED            = 0x03,     // CRC校验失败
    ERROR_PARAMETER_OUT_RANGE   = 0x04,     // 参数超出范围
    ERROR_SYSTEM_BUSY           = 0x05      // 系统忙
} Error_Code_t;

/* 系统状态结构体 */
typedef struct {
    uint16_t input_voltage;         // 输入电压 (mV)
    uint16_t input_current;         // 输入电流 (mA)
    uint16_t output_voltage;        // 输出电压 (mV)
    uint16_t output_current;        // 输出电流 (mA)
    uint16_t output_power;          // 输出功率 (mW)
    uint8_t  working_mode;          // 当前工作模式
} System_Status_t;

/* 系统参数结构体 */
typedef struct {
    uint16_t input_voltage_threshold;   // 输入电压阈值 (mV)
    uint16_t output_voltage_threshold;  // 输出电压阈值 (mV)
    uint16_t max_input_current;         // 最大输入电流 (mA)
    uint16_t max_output_current;        // 最大输出电流 (mA)
    uint16_t max_output_power;          // 最大输出功率 (mW)
    uint8_t  working_mode;              // 工作模式
} System_Parameters_t;

/* 协议帧结构体 */
typedef struct {
    uint8_t header;                     // 帧头 0xAA
    uint8_t cmd;                        // 命令字
    uint8_t len;                        // 数据长度
    uint8_t data[PROTOCOL_MAX_DATA_LEN]; // 数据
    uint16_t crc;                       // CRC16校验
} Protocol_Frame_t;

/* 接收状态机状态 */
typedef enum {
    RX_STATE_WAIT_HEADER = 0,           // 等待帧头
    RX_STATE_WAIT_CMD,                  // 等待命令字
    RX_STATE_WAIT_LEN,                  // 等待数据长度
    RX_STATE_WAIT_DATA,                 // 等待数据
    RX_STATE_WAIT_CRC_LOW,              // 等待CRC低字节
    RX_STATE_WAIT_CRC_HIGH              // 等待CRC高字节
} RX_State_t;

/* 接收缓冲区结构体 */
typedef struct {
    uint8_t buffer[PROTOCOL_MAX_FRAME_LEN]; // 接收缓冲区
    uint16_t index;                         // 当前接收索引
    uint8_t expected_len;                   // 期望的数据长度
    RX_State_t state;                       // 接收状态机状态
} RX_Buffer_t;

/* 函数声明 */

/* CRC16校验函数 */
uint16_t Calculate_CRC16(const uint8_t *data, uint16_t length);

/* 协议帧发送函数 */
void Protocol_SendFrame(uint8_t cmd, const uint8_t *data, uint8_t len);

/* 协议帧接收处理函数 */
void Protocol_ReceiveHandler(uint8_t byte);

/* 命令处理函数 */
void Protocol_ProcessCommand(const Protocol_Frame_t *frame);

/* 具体命令处理函数 */
void Handle_SetInputVoltage(uint16_t voltage_mv);
void Handle_SetOutputVoltage(uint16_t voltage_mv);
void Handle_SetMaxInputCurrent(uint16_t current_ma);
void Handle_SetMaxOutputCurrent(uint16_t current_ma);
void Handle_SetMaxOutputPower(uint16_t power_mw);
void Handle_QueryStatus(void);
void Handle_ModeSwitch(uint8_t mode);

/* 状态上报函数 */
void Protocol_SendStatusReport(void);

/* ACK/错误响应函数 */
void Protocol_SendAckError(Error_Code_t error_code);

/* 协议初始化函数 */
void Protocol_Init(void);

/* 获取系统状态函数 */
System_Status_t* Get_SystemStatus(void);
System_Parameters_t* Get_SystemParameters(void);

/* UART硬件相关函数 */
void UART_Protocol_Init(void);
void UART_SendByte(uint8_t byte);

#ifdef __cplusplus
}
#endif

#endif /* __CCUBE_UART_PROTOCOL_H */