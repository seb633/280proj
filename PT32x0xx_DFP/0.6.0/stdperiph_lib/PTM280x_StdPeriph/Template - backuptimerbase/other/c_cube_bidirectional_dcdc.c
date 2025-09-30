/******************************************************************************
  * @file    c_cube_bidirectional_dcdc.c
  * @author  C-Cube Development Team
  * @version V1.0.0
  * @date    2025/09/17
  * @brief   C-Cube双向DCDC控制器完整程序
  *          支持UART协议配置、实时状态监控、BUCK/BOOST模式切换
  ******************************************************************************
  * @attention
  *
  * C-Cube双向DCDC控制器特性：
  * - 支持BUCK/BOOST双向转换
  * - UART协议配置 (115200bps, 8N1)
  * - 实时电压电流功率监控
  * - 自动模式切换
  * - CRC16校验通信
  * - 100ms状态自动上报
  *
  * 硬件连接：
  * - UART: PA15(TX), PB2(RX)
  * - PWM输出: PA8, PA9, PA10, PA11 (TIM1_CH1-4)
  * - ADC输入: PA0(Vin), PA1(Iin), PA2(Vout), PA3(Iout)
  * - 状态LED: PA4
  *
  * Copyright (C) 2025, C-Cube Team, all rights reserved
  *
  *****************************************************************************/

#include "PT32x0xx.h"
#include "PT32x0xx_config.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* ========================= 常量定义 ========================= */

/* C-Cube协议常量 */
#define CCUBE_FRAME_HEADER          0xAA    // 帧头
#define CCUBE_MAX_DATA_LEN          255     // 最大数据长度
#define CCUBE_MAX_FRAME_LEN         260     // 最大帧长度
#define CCUBE_MIN_FRAME_LEN         5       // 最小帧长度

/* C-Cube命令字定义 */
#define CCUBE_CMD_SET_INPUT_VOLTAGE     0x01    // 设置输入电压阈值
#define CCUBE_CMD_SET_OUTPUT_VOLTAGE    0x02    // 设置输出电压阈值
#define CCUBE_CMD_SET_MAX_INPUT_CURRENT 0x03    // 设置最大输入电流
#define CCUBE_CMD_SET_MAX_OUTPUT_CURRENT 0x04   // 设置最大输出电流
#define CCUBE_CMD_SET_MAX_OUTPUT_POWER  0x05    // 设置最大输出功率
#define CCUBE_CMD_QUERY_STATUS          0x06    // 查询实时状态
#define CCUBE_CMD_STATUS_REPORT         0x07    // 实时状态上报
#define CCUBE_CMD_MODE_SWITCH           0x08    // 模式切换
#define CCUBE_CMD_ACK_ERROR             0x09    // ACK/错误码

/* C-Cube工作模式 */
#define CCUBE_MODE_BUCK                 0       // BUCK模式
#define CCUBE_MODE_BOOST                1       // BOOST模式
#define CCUBE_MODE_AUTO                 2       // 自动模式

/* C-Cube错误码 */
#define CCUBE_ERROR_OK                  0x00    // 成功
#define CCUBE_ERROR_INVALID_CMD         0x01    // 无效命令
#define CCUBE_ERROR_INVALID_DATA_LEN    0x02    // 数据长度错误
#define CCUBE_ERROR_CRC_FAILED          0x03    // CRC校验失败
#define CCUBE_ERROR_PARAMETER_OUT_RANGE 0x04    // 参数超出范围
#define CCUBE_ERROR_SYSTEM_BUSY         0x05    // 系统忙

/* C-Cube硬件参数 */
#define CCUBE_PWM_FREQUENCY             100000  // PWM频率 100kHz
#define CCUBE_ADC_SAMPLES               16      // ADC平均采样次数
#define CCUBE_STATUS_REPORT_INTERVAL    100     // 状态上报间隔(ms)

/* ========================= 数据结构定义 ========================= */

/* C-Cube系统状态结构体 */
typedef struct {
    uint16_t input_voltage;         // 输入电压 (mV)
    uint16_t input_current;         // 输入电流 (mA)
    uint16_t output_voltage;        // 输出电压 (mV)
    uint16_t output_current;        // 输出电流 (mA)
    uint16_t output_power;          // 输出功率 (mW)
    uint8_t  working_mode;          // 当前工作模式
    uint8_t  system_state;          // 系统状态
} CCube_Status_t;

/* C-Cube系统参数结构体 */
typedef struct {
    uint16_t input_voltage_threshold;   // 输入电压阈值 (mV)
    uint16_t output_voltage_threshold;  // 输出电压阈值 (mV)
    uint16_t max_input_current;         // 最大输入电流 (mA)
    uint16_t max_output_current;        // 最大输出电流 (mA)
    uint16_t max_output_power;          // 最大输出功率 (mW)
    uint8_t  working_mode;              // 工作模式
    uint16_t buck_duty_cycle;           // BUCK模式占空比 (0-1000)
    uint16_t boost_duty_cycle;          // BOOST模式占空比 (0-1000)
} CCube_Parameters_t;

/* C-Cube UART接收状态机 */
typedef enum {
    CCUBE_RX_WAIT_HEADER = 0,       // 等待帧头
    CCUBE_RX_WAIT_CMD,              // 等待命令字
    CCUBE_RX_WAIT_LEN,              // 等待数据长度
    CCUBE_RX_WAIT_DATA,             // 等待数据
    CCUBE_RX_WAIT_CRC_LOW,          // 等待CRC低字节
    CCUBE_RX_WAIT_CRC_HIGH          // 等待CRC高字节
} CCube_RX_State_t;

/* C-Cube接收缓冲区 */
typedef struct {
    uint8_t buffer[CCUBE_MAX_FRAME_LEN];
    uint16_t index;
    uint8_t expected_len;
    CCube_RX_State_t state;
} CCube_RX_Buffer_t;

/* ========================= 全局变量 ========================= */

static CCube_Status_t g_ccube_status = {0};
static CCube_Parameters_t g_ccube_parameters = {0};
static CCube_RX_Buffer_t g_ccube_rx_buffer = {0};
static volatile uint32_t g_ccube_timer_counter = 0;
static volatile bool g_ccube_status_report_flag = false;

/* Modbus-RTU CRC16查表 */
static const uint16_t ccube_crc16_table[256] = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

/* ========================= 函数声明 ========================= */

/* 系统初始化 */
static void CCube_System_Init(void);
static void CCube_RCC_Init(void);
static void CCube_GPIO_Init(void);
static void CCube_UART_Init(void);
static void CCube_TIM_Init(void);
static void CCube_ADC_Init(void);
static void CCube_PWM_Init(void);

/* 协议处理 */
static uint16_t CCube_Calculate_CRC16(const uint8_t *data, uint16_t length);
static void CCube_Send_Frame(uint8_t cmd, const uint8_t *data, uint8_t len);
static void CCube_Receive_Handler(uint8_t byte);
static void CCube_Process_Command(uint8_t cmd, const uint8_t *data, uint8_t len);
static void CCube_Send_Status_Report(void);
static void CCube_Send_ACK_Error(uint8_t error_code);

/* DCDC控制 */
static void CCube_Update_ADC_Values(void);
static void CCube_Update_PWM_Output(void);
static void CCube_Mode_Control(void);
static void CCube_Safety_Check(void);

/* 工具函数 */
static void CCube_Delay(uint32_t count);
static void CCube_Print_System_Info(void);

/* ========================= CRC16计算 ========================= */

/**
 * @brief C-Cube CRC16计算
 * @param data: 数据指针
 * @param length: 数据长度
 * @retval CRC16值
 */
static uint16_t CCube_Calculate_CRC16(const uint8_t *data, uint16_t length)
{
    uint16_t crc = 0xFFFF;
    uint16_t i;
    
    for (i = 0; i < length; i++) {
        crc = (crc >> 8) ^ ccube_crc16_table[(crc ^ data[i]) & 0xFF];
    }
    
    return crc;
}

/* ========================= UART协议处理 ========================= */

/**
 * @brief C-Cube发送协议帧
 * @param cmd: 命令字
 * @param data: 数据指针
 * @param len: 数据长度
 * @retval None
 */
static void CCube_Send_Frame(uint8_t cmd, const uint8_t *data, uint8_t len)
{
    uint8_t frame_buffer[CCUBE_MAX_FRAME_LEN];
    uint16_t crc;
    uint16_t i, frame_len;

    /* 构建帧 */
    frame_buffer[0] = CCUBE_FRAME_HEADER;
    frame_buffer[1] = cmd;
    frame_buffer[2] = len;

    /* 复制数据 */
    if (len > 0 && data != NULL) {
        memcpy(&frame_buffer[3], data, len);
    }

    /* 计算CRC16 */
    frame_len = 3 + len;
    crc = CCube_Calculate_CRC16(frame_buffer, frame_len);

    /* 添加CRC16 (低字节在前) */
    frame_buffer[frame_len] = (uint8_t)(crc & 0xFF);
    frame_buffer[frame_len + 1] = (uint8_t)(crc >> 8);

    /* 发送帧 */
    frame_len += 2;
    for (i = 0; i < frame_len; i++) {
        while (UART_GetFlagStatus(UART0, UART_FLAG_TXE) == RESET);
        UART_SendData(UART0, frame_buffer[i]);
    }
}

/**
 * @brief C-Cube接收处理函数
 * @param byte: 接收到的字节
 * @retval None
 */
static void CCube_Receive_Handler(uint8_t byte)
{
    switch (g_ccube_rx_buffer.state) {
        case CCUBE_RX_WAIT_HEADER:
            if (byte == CCUBE_FRAME_HEADER) {
                g_ccube_rx_buffer.buffer[0] = byte;
                g_ccube_rx_buffer.index = 1;
                g_ccube_rx_buffer.state = CCUBE_RX_WAIT_CMD;
            }
            break;

        case CCUBE_RX_WAIT_CMD:
            g_ccube_rx_buffer.buffer[g_ccube_rx_buffer.index++] = byte;
            g_ccube_rx_buffer.state = CCUBE_RX_WAIT_LEN;
            break;

        case CCUBE_RX_WAIT_LEN:
            g_ccube_rx_buffer.buffer[g_ccube_rx_buffer.index++] = byte;
            g_ccube_rx_buffer.expected_len = byte;
            if (g_ccube_rx_buffer.expected_len == 0) {
                g_ccube_rx_buffer.state = CCUBE_RX_WAIT_CRC_LOW;
            } else {
                g_ccube_rx_buffer.state = CCUBE_RX_WAIT_DATA;
            }
            break;

        case CCUBE_RX_WAIT_DATA:
            g_ccube_rx_buffer.buffer[g_ccube_rx_buffer.index++] = byte;
            if (g_ccube_rx_buffer.index >= (3 + g_ccube_rx_buffer.expected_len)) {
                g_ccube_rx_buffer.state = CCUBE_RX_WAIT_CRC_LOW;
            }
            break;

        case CCUBE_RX_WAIT_CRC_LOW:
            g_ccube_rx_buffer.buffer[g_ccube_rx_buffer.index++] = byte;
            g_ccube_rx_buffer.state = CCUBE_RX_WAIT_CRC_HIGH;
            break;

        case CCUBE_RX_WAIT_CRC_HIGH:
            g_ccube_rx_buffer.buffer[g_ccube_rx_buffer.index++] = byte;
            
            /* 完整帧接收完成 */
            {
                uint16_t received_crc, calculated_crc;
                uint16_t crc_index = g_ccube_rx_buffer.index - 2;
                uint8_t cmd = g_ccube_rx_buffer.buffer[1];
                uint8_t len = g_ccube_rx_buffer.buffer[2];
                const uint8_t *data = (len > 0) ? &g_ccube_rx_buffer.buffer[3] : NULL;

                /* 校验CRC */
                received_crc = (uint16_t)g_ccube_rx_buffer.buffer[crc_index] | 
                              ((uint16_t)g_ccube_rx_buffer.buffer[crc_index + 1] << 8);
                calculated_crc = CCube_Calculate_CRC16(g_ccube_rx_buffer.buffer, crc_index);

                if (received_crc == calculated_crc) {
                    CCube_Process_Command(cmd, data, len);
                } else {
                    CCube_Send_ACK_Error(CCUBE_ERROR_CRC_FAILED);
                }
            }

            /* 重置状态机 */
            g_ccube_rx_buffer.state = CCUBE_RX_WAIT_HEADER;
            g_ccube_rx_buffer.index = 0;
            break;

        default:
            g_ccube_rx_buffer.state = CCUBE_RX_WAIT_HEADER;
            g_ccube_rx_buffer.index = 0;
            break;
    }
}

/**
 * @brief C-Cube处理接收到的命令
 * @param cmd: 命令字
 * @param data: 数据指针
 * @param len: 数据长度
 * @retval None
 */
static void CCube_Process_Command(uint8_t cmd, const uint8_t *data, uint8_t len)
{
    uint16_t value16;
    uint8_t value8;

    switch (cmd) {
        case CCUBE_CMD_SET_INPUT_VOLTAGE:
            if (len == 2) {
                value16 = (uint16_t)data[0] | ((uint16_t)data[1] << 8);
                if (value16 >= 10000 && value16 <= 100000) { // 10V~100V
                    g_ccube_parameters.input_voltage_threshold = value16;
                    CCube_Send_ACK_Error(CCUBE_ERROR_OK);
                } else {
                    CCube_Send_ACK_Error(CCUBE_ERROR_PARAMETER_OUT_RANGE);
                }
            } else {
                CCube_Send_ACK_Error(CCUBE_ERROR_INVALID_DATA_LEN);
            }
            break;

        case CCUBE_CMD_SET_OUTPUT_VOLTAGE:
            if (len == 2) {
                value16 = (uint16_t)data[0] | ((uint16_t)data[1] << 8);
                if (value16 >= 5000 && value16 <= 100000) { // 5V~100V
                    g_ccube_parameters.output_voltage_threshold = value16;
                    CCube_Send_ACK_Error(CCUBE_ERROR_OK);
                } else {
                    CCube_Send_ACK_Error(CCUBE_ERROR_PARAMETER_OUT_RANGE);
                }
            } else {
                CCube_Send_ACK_Error(CCUBE_ERROR_INVALID_DATA_LEN);
            }
            break;

        case CCUBE_CMD_SET_MAX_INPUT_CURRENT:
            if (len == 2) {
                value16 = (uint16_t)data[0] | ((uint16_t)data[1] << 8);
                if (value16 >= 1000 && value16 <= 200000) { // 1A~200A
                    g_ccube_parameters.max_input_current = value16;
                    CCube_Send_ACK_Error(CCUBE_ERROR_OK);
                } else {
                    CCube_Send_ACK_Error(CCUBE_ERROR_PARAMETER_OUT_RANGE);
                }
            } else {
                CCube_Send_ACK_Error(CCUBE_ERROR_INVALID_DATA_LEN);
            }
            break;

        case CCUBE_CMD_SET_MAX_OUTPUT_CURRENT:
            if (len == 2) {
                value16 = (uint16_t)data[0] | ((uint16_t)data[1] << 8);
                if (value16 >= 1000 && value16 <= 200000) { // 1A~200A
                    g_ccube_parameters.max_output_current = value16;
                    CCube_Send_ACK_Error(CCUBE_ERROR_OK);
                } else {
                    CCube_Send_ACK_Error(CCUBE_ERROR_PARAMETER_OUT_RANGE);
                }
            } else {
                CCube_Send_ACK_Error(CCUBE_ERROR_INVALID_DATA_LEN);
            }
            break;

        case CCUBE_CMD_SET_MAX_OUTPUT_POWER:
            if (len == 2) {
                value16 = (uint16_t)data[0] | ((uint16_t)data[1] << 8);
                if (value16 >= 10000 && value16 <= 500000) { // 10W~500W
                    g_ccube_parameters.max_output_power = value16;
                    CCube_Send_ACK_Error(CCUBE_ERROR_OK);
                } else {
                    CCube_Send_ACK_Error(CCUBE_ERROR_PARAMETER_OUT_RANGE);
                }
            } else {
                CCube_Send_ACK_Error(CCUBE_ERROR_INVALID_DATA_LEN);
            }
            break;

        case CCUBE_CMD_QUERY_STATUS:
            if (len == 0) {
                CCube_Send_Status_Report();
            } else {
                CCube_Send_ACK_Error(CCUBE_ERROR_INVALID_DATA_LEN);
            }
            break;

        case CCUBE_CMD_MODE_SWITCH:
            if (len == 1) {
                value8 = data[0];
                if (value8 <= CCUBE_MODE_AUTO) {
                    g_ccube_parameters.working_mode = value8;
                    g_ccube_status.working_mode = value8;
                    CCube_Send_ACK_Error(CCUBE_ERROR_OK);
                } else {
                    CCube_Send_ACK_Error(CCUBE_ERROR_PARAMETER_OUT_RANGE);
                }
            } else {
                CCube_Send_ACK_Error(CCUBE_ERROR_INVALID_DATA_LEN);
            }
            break;

        default:
            CCube_Send_ACK_Error(CCUBE_ERROR_INVALID_CMD);
            break;
    }
}

/**
 * @brief C-Cube发送状态上报
 * @param None
 * @retval None
 */
static void CCube_Send_Status_Report(void)
{
    uint8_t status_data[11];

    /* 按协议格式打包状态数据 */
    status_data[0] = (uint8_t)(g_ccube_status.input_voltage & 0xFF);
    status_data[1] = (uint8_t)(g_ccube_status.input_voltage >> 8);
    status_data[2] = (uint8_t)(g_ccube_status.input_current & 0xFF);
    status_data[3] = (uint8_t)(g_ccube_status.input_current >> 8);
    status_data[4] = (uint8_t)(g_ccube_status.output_voltage & 0xFF);
    status_data[5] = (uint8_t)(g_ccube_status.output_voltage >> 8);
    status_data[6] = (uint8_t)(g_ccube_status.output_current & 0xFF);
    status_data[7] = (uint8_t)(g_ccube_status.output_current >> 8);
    status_data[8] = (uint8_t)(g_ccube_status.output_power & 0xFF);
    status_data[9] = (uint8_t)(g_ccube_status.output_power >> 8);
    status_data[10] = g_ccube_status.working_mode;

    CCube_Send_Frame(CCUBE_CMD_STATUS_REPORT, status_data, 11);
}

/**
 * @brief C-Cube发送ACK/错误响应
 * @param error_code: 错误码
 * @retval None
 */
static void CCube_Send_ACK_Error(uint8_t error_code)
{
    CCube_Send_Frame(CCUBE_CMD_ACK_ERROR, &error_code, 1);
}

/* ========================= 硬件初始化 ========================= */

/**
 * @brief C-Cube时钟初始化
 * @param None
 * @retval None
 */
static void CCube_RCC_Init(void)
{
    /* 使能相关外设时钟 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_UART0, ENABLE);
    RCC_APBPeriph1ClockCmd(RCC_APBPeriph1_TIM1, ENABLE);
    RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_ADC, ENABLE);
}

/**
 * @brief C-Cube GPIO初始化
 * @param None
 * @retval None
 */
static void CCube_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    /* 状态LED - PA4 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OutputType = GPIO_OutputType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_High;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_ResetBits(GPIOA, GPIO_Pin_4);

    /* ADC输入引脚 - PA0,PA1,PA2,PA3 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_ANA;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* PWM输出引脚 - PA8,PA9,PA10,PA11 */
    GPIO_DigitalRemapConfig(AFIOA, GPIO_Pin_8, AFIO_AF_2, ENABLE);   // TIM1_CH1
    GPIO_DigitalRemapConfig(AFIOA, GPIO_Pin_9, AFIO_AF_2, ENABLE);   // TIM1_CH2
    GPIO_DigitalRemapConfig(AFIOA, GPIO_Pin_10, AFIO_AF_2, ENABLE);  // TIM1_CH3
    GPIO_DigitalRemapConfig(AFIOA, GPIO_Pin_11, AFIO_AF_2, ENABLE);  // TIM1_CH4
}

/**
 * @brief C-Cube UART初始化
 * @param None
 * @retval None
 */
static void CCube_UART_Init(void)
{
    UART_InitTypeDef UART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    /* 配置UART引脚 */
    GPIO_DigitalRemapConfig(AFIOA, GPIO_Pin_15, AFIO_AF_5, ENABLE);  // TX
    GPIO_DigitalRemapConfig(AFIOB, GPIO_Pin_2, AFIO_AF_5, ENABLE);   // RX

    /* 配置UART0 */
    UART_InitStruct.UART_BaudRate = 115200;
    UART_InitStruct.UART_DataLength = UART_DataLength_8;
    UART_InitStruct.UART_StopBitLength = UART_StopBitLength_1;
    UART_InitStruct.UART_ParityMode = UART_Parity_None;
    UART_InitStruct.UART_Receiver = UART_Receiver_Enable;
    UART_InitStruct.UART_SampleRate = UART_SampleRate_8X;
    UART_Init(UART0, &UART_InitStruct);

    /* 配置UART中断 */
    UART_ITConfig(UART0, UART_IT_RXNE, ENABLE);
    NVIC_InitStruct.NVIC_IRQChannel = UART0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0x01;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    /* 使能UART0 */
    UART_Cmd(UART0, ENABLE);

    /* 初始化接收缓冲区 */
    memset(&g_ccube_rx_buffer, 0, sizeof(g_ccube_rx_buffer));
    g_ccube_rx_buffer.state = CCUBE_RX_WAIT_HEADER;
}

/**
 * @brief C-Cube定时器初始化
 * @param None
 * @retval None
 */
static void CCube_TIM_Init(void)
{
    TIM_InitTypeDef TIM_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    
    /* 配置TIM1基本定时器功能 (用于100ms中断) */
    TIM_InitStruct.TIM_Period = 99;                     // 自动重载值
    TIM_InitStruct.TIM_Prescaler = 47999;               // 预分频值 (48MHz/48000 = 1kHz)
    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_Init(TIM1, &TIM_InitStruct);
    
    /* 使能TIM1更新中断 */
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    
    /* 配置NVIC */
    NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0x02;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    
    /* 启动TIM1 */
    TIM_Cmd(TIM1, ENABLE);
}

/**
 * @brief C-Cube ADC初始化
 * @param None
 * @retval None
 */
static void CCube_ADC_Init(void)
{
    ADC_InitTypeDef ADC_InitStruct;
    
    /* 配置ADC */
    ADC_InitStruct.ADC_ScanConvMode = ENABLE;
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_NbrOfChannel = 4;
    ADC_Init(ADC1, &ADC_InitStruct);
    
    /* 配置ADC通道 */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239_5Cycles);  // Vin
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239_5Cycles);  // Iin
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239_5Cycles);  // Vout
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_239_5Cycles);  // Iout
    
    /* 使能ADC */
    ADC_Cmd(ADC1, ENABLE);
    
    /* 启动ADC校准 */
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
    
    /* 启动ADC转换 */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/**
 * @brief C-Cube PWM初始化
 * @param None
 * @retval None
 */
static void CCube_PWM_Init(void)
{
    TIM_OCInitTypeDef TIM_OCInitStruct;
    
    /* 配置PWM模式 */
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 0;                                 // 初始占空比为0
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    
    /* 配置4个PWM通道 */
    TIM_OC1Init(TIM1, &TIM_OCInitStruct);                          // CH1 - BUCK上管
    TIM_OC2Init(TIM1, &TIM_OCInitStruct);                          // CH2 - BUCK下管
    TIM_OC3Init(TIM1, &TIM_OCInitStruct);                          // CH3 - BOOST上管
    TIM_OC4Init(TIM1, &TIM_OCInitStruct);                          // CH4 - BOOST下管
    
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
    
    /* 使能PWM输出 */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

/* ========================= DCDC控制算法 ========================= */

/**
 * @brief C-Cube更新ADC采集值
 * @param None
 * @retval None
 */
static void CCube_Update_ADC_Values(void)
{
    static uint32_t adc_sum[4] = {0};
    static uint16_t adc_count = 0;
    uint16_t adc_values[4];
    
    /* 读取ADC值 */
    if (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)) {
        adc_values[0] = ADC_GetConversionValue(ADC1);  // 会自动轮换通道
        
        /* 累加采样值 */
        adc_sum[adc_count % 4] += adc_values[0];
        adc_count++;
        
        /* 每采集完一轮4个通道，计算平均值 */
        if (adc_count % (4 * CCUBE_ADC_SAMPLES) == 0) {
            /* 计算平均值并转换为实际物理量 */
            // 假设ADC参考电压3.3V，12位ADC，需要根据实际硬件调整
            uint16_t vin_adc = adc_sum[0] / CCUBE_ADC_SAMPLES;
            uint16_t iin_adc = adc_sum[1] / CCUBE_ADC_SAMPLES;
            uint16_t vout_adc = adc_sum[2] / CCUBE_ADC_SAMPLES;
            uint16_t iout_adc = adc_sum[3] / CCUBE_ADC_SAMPLES;
            
            /* 转换为实际值 (需要根据硬件分压电阻和电流传感器调整) */
            g_ccube_status.input_voltage = (uint32_t)vin_adc * 3300 * 20 / 4095;    // 假设20倍分压
            g_ccube_status.input_current = (uint32_t)iin_adc * 3300 * 100 / 4095;   // 假设电流传感器100A/V
            g_ccube_status.output_voltage = (uint32_t)vout_adc * 3300 * 20 / 4095;  // 假设20倍分压
            g_ccube_status.output_current = (uint32_t)iout_adc * 3300 * 100 / 4095; // 假设电流传感器100A/V
            
            /* 计算输出功率 */
            g_ccube_status.output_power = (uint32_t)g_ccube_status.output_voltage * 
                                         g_ccube_status.output_current / 1000;
            
            /* 清零累加器 */
            memset(adc_sum, 0, sizeof(adc_sum));
        }
    }
}

/**
 * @brief C-Cube模式控制
 * @param None
 * @retval None
 */
static void CCube_Mode_Control(void)
{
    /* 根据当前模式和电压情况自动切换 */
    if (g_ccube_parameters.working_mode == CCUBE_MODE_AUTO) {
        if (g_ccube_status.input_voltage < g_ccube_status.output_voltage) {
            /* 输入电压低于输出电压，使用BOOST模式 */
            g_ccube_status.working_mode = CCUBE_MODE_BOOST;
        } else {
            /* 输入电压高于输出电压，使用BUCK模式 */
            g_ccube_status.working_mode = CCUBE_MODE_BUCK;
        }
    } else {
        /* 手动模式 */
        g_ccube_status.working_mode = g_ccube_parameters.working_mode;
    }
}

/**
 * @brief C-Cube更新PWM输出
 * @param None
 * @retval None
 */
static void CCube_Update_PWM_Output(void)
{
    uint16_t buck_duty = 0;
    uint16_t boost_duty = 0;
    
    /* 简单的PI控制算法 (实际应用中需要更复杂的控制算法) */
    if (g_ccube_status.working_mode == CCUBE_MODE_BUCK) {
        /* BUCK模式控制 */
        if (g_ccube_status.output_voltage < g_ccube_parameters.output_voltage_threshold) {
            buck_duty = g_ccube_parameters.buck_duty_cycle;
            if (buck_duty < 950) buck_duty += 1;  // 增加占空比
        } else if (g_ccube_status.output_voltage > g_ccube_parameters.output_voltage_threshold) {
            buck_duty = g_ccube_parameters.buck_duty_cycle;
            if (buck_duty > 50) buck_duty -= 1;   // 减少占空比
        }
        g_ccube_parameters.buck_duty_cycle = buck_duty;
        
        /* 设置BUCK PWM */
        TIM_SetCompare1(TIM1, buck_duty);      // BUCK上管
        TIM_SetCompare2(TIM1, 1000 - buck_duty); // BUCK下管(互补)
        TIM_SetCompare3(TIM1, 0);              // BOOST关闭
        TIM_SetCompare4(TIM1, 0);
        
    } else if (g_ccube_status.working_mode == CCUBE_MODE_BOOST) {
        /* BOOST模式控制 */
        if (g_ccube_status.output_voltage < g_ccube_parameters.output_voltage_threshold) {
            boost_duty = g_ccube_parameters.boost_duty_cycle;
            if (boost_duty < 950) boost_duty += 1;  // 增加占空比
        } else if (g_ccube_status.output_voltage > g_ccube_parameters.output_voltage_threshold) {
            boost_duty = g_ccube_parameters.boost_duty_cycle;
            if (boost_duty > 50) boost_duty -= 1;   // 减少占空比
        }
        g_ccube_parameters.boost_duty_cycle = boost_duty;
        
        /* 设置BOOST PWM */
        TIM_SetCompare1(TIM1, 0);              // BUCK关闭
        TIM_SetCompare2(TIM1, 0);
        TIM_SetCompare3(TIM1, boost_duty);     // BOOST上管
        TIM_SetCompare4(TIM1, 1000 - boost_duty); // BOOST下管(互补)
    } else {
        /* 关闭所有PWM */
        TIM_SetCompare1(TIM1, 0);
        TIM_SetCompare2(TIM1, 0);
        TIM_SetCompare3(TIM1, 0);
        TIM_SetCompare4(TIM1, 0);
    }
}

/**
 * @brief C-Cube安全检查
 * @param None
 * @retval None
 */
static void CCube_Safety_Check(void)
{
    bool safety_fault = false;
    
    /* 检查电流限制 */
    if (g_ccube_status.input_current > g_ccube_parameters.max_input_current) {
        safety_fault = true;
    }
    if (g_ccube_status.output_current > g_ccube_parameters.max_output_current) {
        safety_fault = true;
    }
    
    /* 检查功率限制 */
    if (g_ccube_status.output_power > g_ccube_parameters.max_output_power) {
        safety_fault = true;
    }
    
    /* 检查电压范围 */
    if (g_ccube_status.input_voltage > g_ccube_parameters.input_voltage_threshold * 1.2) {
        safety_fault = true;  // 输入过压
    }
    if (g_ccube_status.output_voltage > g_ccube_parameters.output_voltage_threshold * 1.2) {
        safety_fault = true;  // 输出过压
    }
    
    /* 如果有安全故障，关闭所有PWM */
    if (safety_fault) {
        TIM_SetCompare1(TIM1, 0);
        TIM_SetCompare2(TIM1, 0);
        TIM_SetCompare3(TIM1, 0);
        TIM_SetCompare4(TIM1, 0);
        g_ccube_status.system_state = 1;  // 故障状态
    } else {
        g_ccube_status.system_state = 0;  // 正常状态
    }
}

/* ========================= 系统初始化 ========================= */

/**
 * @brief C-Cube系统初始化
 * @param None
 * @retval None
 */
static void CCube_System_Init(void)
{
    /* 基本系统初始化 */
    CCube_RCC_Init();
    CCube_GPIO_Init();
    CCube_UART_Init();
    CCube_TIM_Init();
    CCube_ADC_Init();
    CCube_PWM_Init();
    
    /* 初始化系统参数 */
    g_ccube_parameters.input_voltage_threshold = 60000;    // 60V
    g_ccube_parameters.output_voltage_threshold = 60000;   // 60V
    g_ccube_parameters.max_input_current = 150000;         // 150A
    g_ccube_parameters.max_output_current = 150000;        // 150A
    g_ccube_parameters.max_output_power = 450000;          // 450W
    g_ccube_parameters.working_mode = CCUBE_MODE_AUTO;
    g_ccube_parameters.buck_duty_cycle = 500;              // 50%初始占空比
    g_ccube_parameters.boost_duty_cycle = 500;             // 50%初始占空比
    
    /* 初始化系统状态 */
    memset(&g_ccube_status, 0, sizeof(g_ccube_status));
    g_ccube_status.working_mode = CCUBE_MODE_AUTO;
    
    /* 使能全局中断 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    __enable_irq();
}

/**
 * @brief C-Cube打印系统信息
 * @param None
 * @retval None
 */
static void CCube_Print_System_Info(void)
{
    printf("\r\n");
    printf("=========================================\r\n");
    printf("     C-Cube双向DCDC控制器 v1.0         \r\n");
    printf("=========================================\r\n");
    printf("硬件配置:\r\n");
    printf("- UART: PA15(TX), PB2(RX), 115200bps\r\n");
    printf("- PWM: PA8-PA11 (TIM1_CH1-4)\r\n");
    printf("- ADC: PA0(Vin), PA1(Iin), PA2(Vout), PA3(Iout)\r\n");
    printf("- LED: PA4\r\n");
    printf("\r\n");
    printf("协议配置:\r\n");
    printf("- 帧格式: 0xAA + CMD + LEN + DATA + CRC16\r\n");
    printf("- 状态上报: 每100ms自动发送\r\n");
    printf("- 支持命令: 0x01-0x09\r\n");
    printf("\r\n");
    printf("DCDC参数:\r\n");
    printf("- 输入电压阈值: %u mV\r\n", g_ccube_parameters.input_voltage_threshold);
    printf("- 输出电压阈值: %u mV\r\n", g_ccube_parameters.output_voltage_threshold);
    printf("- 最大输入电流: %u mA\r\n", g_ccube_parameters.max_input_current);
    printf("- 最大输出电流: %u mA\r\n", g_ccube_parameters.max_output_current);
    printf("- 最大输出功率: %u mW\r\n", g_ccube_parameters.max_output_power);
    printf("- 工作模式: %s\r\n", 
           (g_ccube_parameters.working_mode == CCUBE_MODE_BUCK) ? "BUCK" :
           (g_ccube_parameters.working_mode == CCUBE_MODE_BOOST) ? "BOOST" : "AUTO");
    printf("\r\n");
    printf("C-Cube系统就绪!\r\n");
    printf("=========================================\r\n");
}

/**
 * @brief C-Cube延时函数
 * @param count: 延时计数
 * @retval None
 */
static void CCube_Delay(uint32_t count)
{
    volatile uint32_t i;
    for (i = 0; i < count; i++) {
        /* 空循环 */
    }
}

/* ========================= 中断处理函数 ========================= */

/**
 * @brief C-Cube UART0中断处理
 * @param None
 * @retval None
 */
void UART0_Handler(void)
{
    if (UART_GetFlagStatus(UART0, UART_FLAG_RXNE) != RESET) {
        uint8_t received_byte = (uint8_t)UART_ReceiveData(UART0);
        CCube_Receive_Handler(received_byte);
    }
}

/**
 * @brief C-Cube TIM1更新中断处理
 * @param None
 * @retval None
 */
void TIM1_UP_Handler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        
        g_ccube_timer_counter++;
        g_ccube_status_report_flag = true;
    }
}

/* ========================= 主程序 ========================= */

/**
 * @brief C-Cube主函数
 * @param None
 * @retval int
 */
int main(void)
{
    uint32_t led_counter = 0;
    uint32_t control_counter = 0;
    
    /* C-Cube系统初始化 */
    CCube_System_Init();
    
    /* 打印系统信息 */
    CCube_Print_System_Info();
    
    printf("C-Cube双向DCDC控制器启动...\r\n");
    
    /* 主控制循环 */
    while (1) {
        /* 处理状态上报 (100ms) */
        if (g_ccube_status_report_flag) {
            g_ccube_status_report_flag = false;
            CCube_Send_Status_Report();
        }
        
        /* DCDC控制任务 (1ms) */
        control_counter++;
        if (control_counter >= 1000) {  // 约1ms
            control_counter = 0;
            
            /* 更新ADC采集值 */
            CCube_Update_ADC_Values();
            
            /* 模式控制 */
            CCube_Mode_Control();
            
            /* 更新PWM输出 */
            CCube_Update_PWM_Output();
            
            /* 安全检查 */
            CCube_Safety_Check();
        }
        
        /* LED状态指示 (约500ms闪烁) */
        led_counter++;
        if (led_counter >= 500000) {
            led_counter = 0;
            GPIO_ToggleBits(GPIOA, GPIO_Pin_4);
            
            /* 打印当前状态到调试串口 */
            printf("C-Cube状态: Vin=%umV, Iin=%umA, Vout=%umV, Iout=%umA, Pout=%umW, Mode=%u\r\n",
                   g_ccube_status.input_voltage,
                   g_ccube_status.input_current,
                   g_ccube_status.output_voltage,
                   g_ccube_status.output_current,
                   g_ccube_status.output_power,
                   g_ccube_status.working_mode);
        }
    }
}

/* ========================= 断言处理 ========================= */

#ifdef USE_FULL_ASSERT
/**
 * @brief C-Cube断言失败处理
 * @param file: 文件名
 * @param line: 行号
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    printf("C-Cube Assert Failed: file %s on line %d\r\n", file, line);
    while (1) {
        /* 断言失败，停止运行 */
    }
}
#endif

/************************ (C) COPYRIGHT C-Cube Team *****END OF FILE****/