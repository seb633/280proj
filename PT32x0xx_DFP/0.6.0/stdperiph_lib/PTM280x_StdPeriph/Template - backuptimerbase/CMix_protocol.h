/******************************************************************************
  * @file    CMix_protocol.h
  * @author  CMix Development Team
  * @version V1.0.0
  * @date    2025/09/17
  * @brief   CMix双向DCDC控制器UART通信协议头文件
  *          定义协议命令、数据结构和通信接口
  ******************************************************************************
  * @attention
  *
  * CMix UART通信协议模块
  * 负责与上位机的通信协议解析和处理
  *
  * Copyright (C) 2025, CMix Team, all rights reserved
  *
  *****************************************************************************/

#ifndef __CMIX_PROTOCOL_H
#define __CMIX_PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "CMix_config.h"

/* ========================= 协议命令定义 ========================= */

typedef enum {
    CMIX_CMD_SET_INPUT_VOLTAGE      = 0x01,     // 设置输入电压阈值
    CMIX_CMD_SET_OUTPUT_VOLTAGE     = 0x02,     // 设置输出电压阈值
    CMIX_CMD_SET_MAX_INPUT_CURRENT  = 0x03,     // 设置最大输入电流
    CMIX_CMD_SET_MAX_OUTPUT_CURRENT = 0x04,     // 设置最大输出电流
    CMIX_CMD_SET_MAX_OUTPUT_POWER   = 0x05,     // 设置最大输出功率
    CMIX_CMD_QUERY_STATUS           = 0x06,     // 查询实时状态
    CMIX_CMD_STATUS_REPORT          = 0x07,     // 实时状态上报
    CMIX_CMD_MODE_SWITCH            = 0x08,     // 模式切换
    CMIX_CMD_ACK_ERROR              = 0x09,     // ACK/错误码
    CMIX_CMD_DEBUG_INFO             = 0x0A,     // 调试信息输出
    CMIX_CMD_SYSTEM_INFO            = 0x0B      // 系统信息上报
} CMix_Protocol_Command_t;

/* 协议错误码 */
typedef enum {
    CMIX_PROTOCOL_ERROR_OK                  = 0x00,     // 成功
    CMIX_PROTOCOL_ERROR_INVALID_CMD         = 0x01,     // 无效命令
    CMIX_PROTOCOL_ERROR_INVALID_DATA_LEN    = 0x02,     // 数据长度错误
    CMIX_PROTOCOL_ERROR_CRC_FAILED          = 0x03,     // CRC校验失败
    CMIX_PROTOCOL_ERROR_PARAMETER_OUT_RANGE = 0x04,     // 参数超出范围
    CMIX_PROTOCOL_ERROR_SYSTEM_BUSY         = 0x05,     // 系统忙
    CMIX_PROTOCOL_ERROR_SYSTEM_FAULT        = 0x06      // 系统故障
} CMix_Protocol_Error_t;

/* 协议接收状态机 */
typedef enum {
    CMIX_RX_STATE_WAIT_HEADER = 0,          // 等待帧头
    CMIX_RX_STATE_WAIT_CMD,                 // 等待命令字
    CMIX_RX_STATE_WAIT_LEN,                 // 等待数据长度
    CMIX_RX_STATE_WAIT_DATA,                // 等待数据
    CMIX_RX_STATE_WAIT_CRC_LOW,             // 等待CRC低字节
    CMIX_RX_STATE_WAIT_CRC_HIGH             // 等待CRC高字节
} CMix_RX_State_t;

/* ========================= 数据结构定义 ========================= */

/* 协议帧结构体 */
typedef struct {
    uint8_t header;                         // 帧头 0xAA
    uint8_t cmd;                            // 命令字
    uint8_t len;                            // 数据长度
    uint8_t data[CMIX_PROTOCOL_MAX_DATA_LEN]; // 数据
    uint16_t crc;                           // CRC16校验
} CMix_Protocol_Frame_t;

/* 接收缓冲区结构体 */
typedef struct {
    uint8_t buffer[CMIX_PROTOCOL_MAX_FRAME_LEN]; // 接收缓冲区
    uint16_t index;                              // 当前接收索引
    uint8_t expected_len;                        // 期望的数据长度
    CMix_RX_State_t state;                       // 接收状态机状态
} CMix_RX_Buffer_t;

/* 系统状态结构体 */
typedef struct {
    uint16_t input_voltage;                 // 输入电压 (mV)
    uint16_t input_current;                 // 输入电流 (mA)
    uint16_t output_voltage;                // 输出电压 (mV)
    uint16_t output_current;                // 输出电流 (mA)
    uint16_t output_power;                  // 输出功率 (mW)
    uint8_t  working_mode;                  // 当前工作模式
    uint8_t  system_state;                  // 系统状态
    uint8_t  error_code;                    // 错误码
} CMix_System_Status_t;

/* 系统参数结构体 */
typedef struct {
    uint32_t input_voltage_threshold;       // 输入电压阈值 (mV)
    uint32_t output_voltage_threshold;      // 输出电压阈值 (mV)
    uint32_t max_input_current;             // 最大输入电流 (mA)
    uint32_t max_output_current;            // 最大输出电流 (mA)
    uint32_t max_output_power;              // 最大输出功率 (mW)
    uint8_t  working_mode;                  // 工作模式
} CMix_System_Parameters_t;

/* ========================= 函数声明 ========================= */

/* 协议初始化和基础功能 */
void CMix_Protocol_Init(void);
uint16_t CMix_Protocol_Calculate_CRC16(const uint8_t *data, uint16_t length);

/* 协议帧发送和接收 */
void CMix_Protocol_Send_Frame(uint8_t cmd, const uint8_t *data, uint8_t len);
void CMix_Protocol_Receive_Handler(uint8_t byte);
void CMix_Protocol_Process_Command(uint8_t cmd, const uint8_t *data, uint8_t len);

/* 状态和参数管理 */
void CMix_Protocol_Send_Status_Report(void);
void CMix_Protocol_Send_ACK_Error(CMix_Protocol_Error_t error_code);

/* 调试信息发送 */
void CMix_Protocol_Send_Debug_Message(const char *message);
void CMix_Protocol_Send_System_Info(uint32_t system_clock, bool clock_ok, const char *build_date, const char *build_time);

/* 系统状态和参数访问 */
CMix_System_Status_t* CMix_Protocol_Get_System_Status(void);
CMix_System_Parameters_t* CMix_Protocol_Get_System_Parameters(void);

/* 协议测试和调试 */
void CMix_Protocol_Test_Send_Commands(void);
void CMix_Protocol_Print_Frame(const uint8_t *data, uint16_t len, const char *description);

#ifdef __cplusplus
}
#endif

#endif /* __CMIX_PROTOCOL_H */