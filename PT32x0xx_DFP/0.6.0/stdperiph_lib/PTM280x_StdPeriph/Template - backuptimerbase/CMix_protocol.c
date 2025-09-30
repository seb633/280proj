/******************************************************************************
  * @file    CMix_protocol.c
  * @author  CMix Development Team
  * @version V1.0.0
  * @date    2025/09/17
  * @brief   CMix双向DCDC控制器UART通信协议实现文件
  *          实现协议解析、CRC校验、命令处理等功能
  ******************************************************************************
  * @attention
  *
  * CMix UART通信协议模块实现
  * 负责与上位机的通信协议解析和处理
  *
  * Copyright (C) 2025, CMix Team, all rights reserved
  *
  *****************************************************************************/

#include "CMix_protocol.h"
#include "CMix_hardware.h"
#include <string.h>

/* ========================= 私有变量 ========================= */

static CMix_System_Status_t g_system_status = {0};
static CMix_System_Parameters_t g_system_parameters = {0};
static CMix_RX_Buffer_t g_rx_buffer = {0};

/* Modbus-RTU CRC16查表 */
static const uint16_t crc16_table[256] = {
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

/* ========================= 私有函数声明 ========================= */

static void CMix_Protocol_Handle_Set_Input_Voltage(const uint8_t *data, uint8_t len);
static void CMix_Protocol_Handle_Set_Output_Voltage(const uint8_t *data, uint8_t len);
static void CMix_Protocol_Handle_Set_Max_Input_Current(const uint8_t *data, uint8_t len);
static void CMix_Protocol_Handle_Set_Max_Output_Current(const uint8_t *data, uint8_t len);
static void CMix_Protocol_Handle_Set_Max_Output_Power(const uint8_t *data, uint8_t len);
static void CMix_Protocol_Handle_Query_Status(void);
static void CMix_Protocol_Handle_Mode_Switch(const uint8_t *data, uint8_t len);

/* ========================= 公共函数实现 ========================= */

/**
 * @brief CMix协议初始化
 * @param None
 * @retval None
 */
void CMix_Protocol_Init(void)
{
    /* 初始化系统参数 */
    g_system_parameters.input_voltage_threshold = 60000;    // 60V
    g_system_parameters.output_voltage_threshold = 60000;   // 60V
    g_system_parameters.max_input_current = 150000;         // 150A
    g_system_parameters.max_output_current = 150000;        // 150A
    g_system_parameters.max_output_power = 450000;          // 450W
    g_system_parameters.working_mode = CMIX_MODE_AUTO;

    /* 初始化系统状态 */
    memset(&g_system_status, 0, sizeof(g_system_status));
    g_system_status.working_mode = CMIX_MODE_AUTO;
    g_system_status.system_state = CMIX_STATE_INIT;

    /* 初始化接收缓冲区 */
    memset(&g_rx_buffer, 0, sizeof(g_rx_buffer));
    g_rx_buffer.state = CMIX_RX_STATE_WAIT_HEADER;
}

/**
 * @brief CMix CRC16计算
 * @param data: 数据指针
 * @param length: 数据长度
 * @retval CRC16值
 */
uint16_t CMix_Protocol_Calculate_CRC16(const uint8_t *data, uint16_t length)
{
    uint16_t crc = 0xFFFF;
    uint16_t i;
    
    for (i = 0; i < length; i++) {
        crc = (crc >> 8) ^ crc16_table[(crc ^ data[i]) & 0xFF];
    }
    
    return crc;
}

/**
 * @brief CMix发送协议帧
 * @param cmd: 命令字
 * @param data: 数据指针
 * @param len: 数据长度
 * @retval None
 */
void CMix_Protocol_Send_Frame(uint8_t cmd, const uint8_t *data, uint8_t len)
{
    uint8_t frame_buffer[CMIX_PROTOCOL_MAX_FRAME_LEN];
    uint16_t crc;
    uint16_t i, frame_len;

    /* 构建帧 */
    frame_buffer[0] = CMIX_PROTOCOL_FRAME_HEADER;
    frame_buffer[1] = cmd;
    frame_buffer[2] = len;

    /* 复制数据 */
    if (len > 0 && data != NULL) {
        memcpy(&frame_buffer[3], data, len);
    }

    /* 计算CRC16 */
    frame_len = 3 + len;
    crc = CMix_Protocol_Calculate_CRC16(frame_buffer, frame_len);

    /* 添加CRC16 (低字节在前) */
    frame_buffer[frame_len] = (uint8_t)(crc & 0xFF);
    frame_buffer[frame_len + 1] = (uint8_t)(crc >> 8);

    /* 发送帧 */
    frame_len += 2;
    for (i = 0; i < frame_len; i++) {
        CMix_Hardware_UART_Send_Byte(frame_buffer[i]);
    }
}

/**
 * @brief CMix接收处理函数
 * @param byte: 接收到的字节
 * @retval None
 */
void CMix_Protocol_Receive_Handler(uint8_t byte)
{
    switch (g_rx_buffer.state) {
        case CMIX_RX_STATE_WAIT_HEADER:
            if (byte == CMIX_PROTOCOL_FRAME_HEADER) {
                g_rx_buffer.buffer[0] = byte;
                g_rx_buffer.index = 1;
                g_rx_buffer.state = CMIX_RX_STATE_WAIT_CMD;
            }
            break;

        case CMIX_RX_STATE_WAIT_CMD:
            g_rx_buffer.buffer[g_rx_buffer.index++] = byte;
            g_rx_buffer.state = CMIX_RX_STATE_WAIT_LEN;
            break;

        case CMIX_RX_STATE_WAIT_LEN:
            g_rx_buffer.buffer[g_rx_buffer.index++] = byte;
            g_rx_buffer.expected_len = byte;
            if (g_rx_buffer.expected_len == 0) {
                g_rx_buffer.state = CMIX_RX_STATE_WAIT_CRC_LOW;
            } else {
                g_rx_buffer.state = CMIX_RX_STATE_WAIT_DATA;
            }
            break;

        case CMIX_RX_STATE_WAIT_DATA:
            g_rx_buffer.buffer[g_rx_buffer.index++] = byte;
            if (g_rx_buffer.index >= (3 + g_rx_buffer.expected_len)) {
                g_rx_buffer.state = CMIX_RX_STATE_WAIT_CRC_LOW;
            }
            break;

        case CMIX_RX_STATE_WAIT_CRC_LOW:
            g_rx_buffer.buffer[g_rx_buffer.index++] = byte;
            g_rx_buffer.state = CMIX_RX_STATE_WAIT_CRC_HIGH;
            break;

        case CMIX_RX_STATE_WAIT_CRC_HIGH:
            g_rx_buffer.buffer[g_rx_buffer.index++] = byte;
            
            /* 完整帧接收完成 */
            {
                uint16_t received_crc, calculated_crc;
                uint16_t crc_index = g_rx_buffer.index - 2;
                uint8_t cmd = g_rx_buffer.buffer[1];
                uint8_t len = g_rx_buffer.buffer[2];
                const uint8_t *data = (len > 0) ? &g_rx_buffer.buffer[3] : NULL;

                /* 校验CRC */
                received_crc = (uint16_t)g_rx_buffer.buffer[crc_index] | 
                              ((uint16_t)g_rx_buffer.buffer[crc_index + 1] << 8);
                calculated_crc = CMix_Protocol_Calculate_CRC16(g_rx_buffer.buffer, crc_index);

                if (received_crc == calculated_crc) {
                    CMix_Protocol_Process_Command(cmd, data, len);
                } else {
                    CMix_Protocol_Send_ACK_Error(CMIX_PROTOCOL_ERROR_CRC_FAILED);
                }
            }

            /* 重置状态机 */
            g_rx_buffer.state = CMIX_RX_STATE_WAIT_HEADER;
            g_rx_buffer.index = 0;
            break;

        default:
            g_rx_buffer.state = CMIX_RX_STATE_WAIT_HEADER;
            g_rx_buffer.index = 0;
            break;
    }
}

/**
 * @brief CMix处理接收到的命令
 * @param cmd: 命令字
 * @param data: 数据指针
 * @param len: 数据长度
 * @retval None
 */
void CMix_Protocol_Process_Command(uint8_t cmd, const uint8_t *data, uint8_t len)
{
    switch (cmd) {
        case CMIX_CMD_SET_INPUT_VOLTAGE:
            CMix_Protocol_Handle_Set_Input_Voltage(data, len);
            break;

        case CMIX_CMD_SET_OUTPUT_VOLTAGE:
            CMix_Protocol_Handle_Set_Output_Voltage(data, len);
            break;

        case CMIX_CMD_SET_MAX_INPUT_CURRENT:
            CMix_Protocol_Handle_Set_Max_Input_Current(data, len);
            break;

        case CMIX_CMD_SET_MAX_OUTPUT_CURRENT:
            CMix_Protocol_Handle_Set_Max_Output_Current(data, len);
            break;

        case CMIX_CMD_SET_MAX_OUTPUT_POWER:
            CMix_Protocol_Handle_Set_Max_Output_Power(data, len);
            break;

        case CMIX_CMD_QUERY_STATUS:
            CMix_Protocol_Handle_Query_Status();
            break;

        case CMIX_CMD_MODE_SWITCH:
            CMix_Protocol_Handle_Mode_Switch(data, len);
            break;

        default:
            CMix_Protocol_Send_ACK_Error(CMIX_PROTOCOL_ERROR_INVALID_CMD);
            break;
    }
}

/**
 * @brief CMix发送状态上报
 * @param None
 * @retval None
 */
void CMix_Protocol_Send_Status_Report(void)
{
    uint8_t status_data[11];

    /* 按协议格式打包状态数据 */
    status_data[0] = (uint8_t)(g_system_status.input_voltage & 0xFF);
    status_data[1] = (uint8_t)(g_system_status.input_voltage >> 8);
    status_data[2] = (uint8_t)(g_system_status.input_current & 0xFF);
    status_data[3] = (uint8_t)(g_system_status.input_current >> 8);
    status_data[4] = (uint8_t)(g_system_status.output_voltage & 0xFF);
    status_data[5] = (uint8_t)(g_system_status.output_voltage >> 8);
    status_data[6] = (uint8_t)(g_system_status.output_current & 0xFF);
    status_data[7] = (uint8_t)(g_system_status.output_current >> 8);
    status_data[8] = (uint8_t)(g_system_status.output_power & 0xFF);
    status_data[9] = (uint8_t)(g_system_status.output_power >> 8);
    status_data[10] = g_system_status.working_mode;

    CMix_Protocol_Send_Frame(CMIX_CMD_STATUS_REPORT, status_data, 11);
}

/**
 * @brief CMix发送ACK/错误响应
 * @param error_code: 错误码
 * @retval None
 */
void CMix_Protocol_Send_ACK_Error(CMix_Protocol_Error_t error_code)
{
    uint8_t error_data = (uint8_t)error_code;
    CMix_Protocol_Send_Frame(CMIX_CMD_ACK_ERROR, &error_data, 1);
}

/**
 * @brief CMix获取系统状态指针
 * @param None
 * @retval 系统状态指针
 */
CMix_System_Status_t* CMix_Protocol_Get_System_Status(void)
{
    return &g_system_status;
}

/**
 * @brief CMix获取系统参数指针
 * @param None
 * @retval 系统参数指针
 */
CMix_System_Parameters_t* CMix_Protocol_Get_System_Parameters(void)
{
    return &g_system_parameters;
}

/* ========================= 私有函数实现 ========================= */

/**
 * @brief 处理设置输入电压阈值命令
 * @param data: 数据指针
 * @param len: 数据长度
 * @retval None
 */
static void CMix_Protocol_Handle_Set_Input_Voltage(const uint8_t *data, uint8_t len)
{
    if (len == 2) {
        uint32_t voltage = (uint32_t)data[0] | ((uint32_t)data[1] << 8);
        if (voltage >= 10000 && voltage <= 100000) { // 10V~100V
            g_system_parameters.input_voltage_threshold = voltage;
            CMix_Protocol_Send_ACK_Error(CMIX_PROTOCOL_ERROR_OK);
        } else {
            CMix_Protocol_Send_ACK_Error(CMIX_PROTOCOL_ERROR_PARAMETER_OUT_RANGE);
        }
    } else {
        CMix_Protocol_Send_ACK_Error(CMIX_PROTOCOL_ERROR_INVALID_DATA_LEN);
    }
}

/**
 * @brief 处理设置输出电压阈值命令
 * @param data: 数据指针
 * @param len: 数据长度
 * @retval None
 */
static void CMix_Protocol_Handle_Set_Output_Voltage(const uint8_t *data, uint8_t len)
{
    if (len == 2) {
        uint32_t voltage = (uint32_t)data[0] | ((uint32_t)data[1] << 8);
        if (voltage >= 5000 && voltage <= 100000) { // 5V~100V
            g_system_parameters.output_voltage_threshold = voltage;
            CMix_Protocol_Send_ACK_Error(CMIX_PROTOCOL_ERROR_OK);
        } else {
            CMix_Protocol_Send_ACK_Error(CMIX_PROTOCOL_ERROR_PARAMETER_OUT_RANGE);
        }
    } else {
        CMix_Protocol_Send_ACK_Error(CMIX_PROTOCOL_ERROR_INVALID_DATA_LEN);
    }
}

/**
 * @brief 处理设置最大输入电流命令
 * @param data: 数据指针
 * @param len: 数据长度
 * @retval None
 */
static void CMix_Protocol_Handle_Set_Max_Input_Current(const uint8_t *data, uint8_t len)
{
    if (len == 2) {
        uint32_t current = (uint32_t)data[0] | ((uint32_t)data[1] << 8);
        if (current >= 1000 && current <= 65535) { // 1A~65.535A (受16位限制)
            g_system_parameters.max_input_current = current;
            CMix_Protocol_Send_ACK_Error(CMIX_PROTOCOL_ERROR_OK);
        } else {
            CMix_Protocol_Send_ACK_Error(CMIX_PROTOCOL_ERROR_PARAMETER_OUT_RANGE);
        }
    } else {
        CMix_Protocol_Send_ACK_Error(CMIX_PROTOCOL_ERROR_INVALID_DATA_LEN);
    }
}

/**
 * @brief 处理设置最大输出电流命令
 * @param data: 数据指针
 * @param len: 数据长度
 * @retval None
 */
static void CMix_Protocol_Handle_Set_Max_Output_Current(const uint8_t *data, uint8_t len)
{
    if (len == 2) {
        uint32_t current = (uint32_t)data[0] | ((uint32_t)data[1] << 8);
        if (current >= 1000 && current <= 65535) { // 1A~65.535A (受16位限制)
            g_system_parameters.max_output_current = current;
            CMix_Protocol_Send_ACK_Error(CMIX_PROTOCOL_ERROR_OK);
        } else {
            CMix_Protocol_Send_ACK_Error(CMIX_PROTOCOL_ERROR_PARAMETER_OUT_RANGE);
        }
    } else {
        CMix_Protocol_Send_ACK_Error(CMIX_PROTOCOL_ERROR_INVALID_DATA_LEN);
    }
}

/**
 * @brief 处理设置最大输出功率命令
 * @param data: 数据指针
 * @param len: 数据长度
 * @retval None
 */
static void CMix_Protocol_Handle_Set_Max_Output_Power(const uint8_t *data, uint8_t len)
{
    if (len == 2) {
        uint32_t power = (uint32_t)data[0] | ((uint32_t)data[1] << 8);
        if (power >= 10000 && power <= 65535) { // 10W~65.535W (受16位限制)
            g_system_parameters.max_output_power = power;
            CMix_Protocol_Send_ACK_Error(CMIX_PROTOCOL_ERROR_OK);
        } else {
            CMix_Protocol_Send_ACK_Error(CMIX_PROTOCOL_ERROR_PARAMETER_OUT_RANGE);
        }
    } else {
        CMix_Protocol_Send_ACK_Error(CMIX_PROTOCOL_ERROR_INVALID_DATA_LEN);
    }
}

/**
 * @brief 处理查询状态命令
 * @param None
 * @retval None
 */
static void CMix_Protocol_Handle_Query_Status(void)
{
    CMix_Protocol_Send_Status_Report();
}

/**
 * @brief 处理模式切换命令
 * @param data: 数据指针
 * @param len: 数据长度
 * @retval None
 */
static void CMix_Protocol_Handle_Mode_Switch(const uint8_t *data, uint8_t len)
{
    if (len == 1) {
        uint8_t mode = data[0];
        if (mode <= CMIX_MODE_AUTO) {
            g_system_parameters.working_mode = mode;
            g_system_status.working_mode = mode;
            CMix_Protocol_Send_ACK_Error(CMIX_PROTOCOL_ERROR_OK);
        } else {
            CMix_Protocol_Send_ACK_Error(CMIX_PROTOCOL_ERROR_PARAMETER_OUT_RANGE);
        }
    } else {
        CMix_Protocol_Send_ACK_Error(CMIX_PROTOCOL_ERROR_INVALID_DATA_LEN);
    }
}

/**
 * @brief CMix协议测试发送命令
 * @param None
 * @retval None
 */
void CMix_Protocol_Test_Send_Commands(void)
{
    uint8_t test_data[2];
    
    /* 测试设置输入电压阈值为60V */
    test_data[0] = 0x60;    // 60000 & 0xFF
    test_data[1] = 0xEA;    // 60000 >> 8
    CMix_Protocol_Send_Frame(CMIX_CMD_SET_INPUT_VOLTAGE, test_data, 2);
    
    /* 延时 */
    CMix_Hardware_Delay_ms(100);
    
    /* 测试查询状态 */
    CMix_Protocol_Send_Frame(CMIX_CMD_QUERY_STATUS, NULL, 0);
    
    /* 延时 */
    CMix_Hardware_Delay_ms(100);
    
    /* 测试模式切换到BOOST */
    test_data[0] = CMIX_MODE_BOOST;
    CMix_Protocol_Send_Frame(CMIX_CMD_MODE_SWITCH, test_data, 1);
}

/**
 * @brief CMix协议打印帧数据
 * @param data: 数据指针
 * @param len: 数据长度
 * @param description: 描述字符串
 * @retval None
 */
void CMix_Protocol_Print_Frame(const uint8_t *data, uint16_t len, const char *description)
{
    /* 调试信息已改为通过协议发送，不再使用printf */
    (void)data;        // 避免未使用变量警告
    (void)len;
    (void)description;
}

/* ========================= 调试信息发送函数 ========================= */

/**
 * @brief 发送调试信息
 * @param message: 调试信息字符串
 * @retval None
 */
void CMix_Protocol_Send_Debug_Message(const char *message)
{
    uint8_t len;
    
    if (message == NULL) {
        return;
    }
    
    len = strlen(message);
    if (len > CMIX_PROTOCOL_MAX_DATA_LEN) {
        len = CMIX_PROTOCOL_MAX_DATA_LEN;
    }
    
    CMix_Protocol_Send_Frame(CMIX_CMD_DEBUG_INFO, (const uint8_t*)message, len);
}

/**
 * @brief 发送系统信息
 * @param system_clock: 系统时钟频率
 * @param clock_ok: 时钟配置是否正常
 * @param build_date: 编译日期
 * @param build_time: 编译时间
 * @retval None
 */
void CMix_Protocol_Send_System_Info(uint32_t system_clock, bool clock_ok, const char *build_date, const char *build_time)
{
    uint8_t info_data[64];
    uint8_t len = 0;
    
    /* 系统时钟频率 (4字节) */
    info_data[len++] = (uint8_t)(system_clock & 0xFF);
    info_data[len++] = (uint8_t)((system_clock >> 8) & 0xFF);
    info_data[len++] = (uint8_t)((system_clock >> 16) & 0xFF);
    info_data[len++] = (uint8_t)((system_clock >> 24) & 0xFF);
    
    /* 时钟状态 (1字节) */
    info_data[len++] = clock_ok ? 1 : 0;
    
    /* 编译日期和时间 (最多30字节) */
    if (build_date && build_time) {
        uint8_t date_len = strlen(build_date);
        uint8_t time_len = strlen(build_time);
        
        if (date_len > 15) date_len = 15;
        if (time_len > 15) time_len = 15;
        
        /* 日期长度 */
        info_data[len++] = date_len;
        memcpy(&info_data[len], build_date, date_len);
        len += date_len;
        
        /* 时间长度 */
        info_data[len++] = time_len;
        memcpy(&info_data[len], build_time, time_len);
        len += time_len;
    } else {
        /* 无编译信息 */
        info_data[len++] = 0;  // 日期长度为0
        info_data[len++] = 0;  // 时间长度为0
    }
    
    CMix_Protocol_Send_Frame(CMIX_CMD_SYSTEM_INFO, info_data, len);
}