/******************************************************************************
  * @file    ccube_uart_protocol.c
  * @author  C-Cube Development Team
  * @version V1.0.0
  * @date    2025/09/17
  * @brief   C-Cube UART通信协议实现文件
  *          实现基于Modbus-RTU CRC16校验的自定义通信协议
  ******************************************************************************
  * @attention
  *
  * Copyright (C) 2025, C-Cube Team, all rights reserved
  *
  *****************************************************************************/

#include "ccube_uart_protocol.h"
#include "PT32x0xx_config.h"
#include <string.h>

/* 全局变量 */
static System_Status_t g_system_status = {0};
static System_Parameters_t g_system_parameters = {0};
static RX_Buffer_t g_rx_buffer = {0};

/* Modbus-RTU CRC16查表法 */
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

/**
 * @brief 计算Modbus-RTU CRC16校验值
 * @param data: 数据指针
 * @param length: 数据长度
 * @retval CRC16校验值
 */
uint16_t Calculate_CRC16(const uint8_t *data, uint16_t length)
{
    uint16_t crc = 0xFFFF;
    uint16_t i;
    
    for (i = 0; i < length; i++) {
        crc = (crc >> 8) ^ crc16_table[(crc ^ data[i]) & 0xFF];
    }
    
    return crc;
}

/**
 * @brief UART协议初始化
 * @param None
 * @retval None
 */
void UART_Protocol_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    UART_InitTypeDef UART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    /* 使能时钟 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_UART0, ENABLE);

    /* 配置GPIO */
    // PA15 - UART0_TX
    GPIO_DigitalRemapConfig(AFIOA, GPIO_Pin_15, AFIO_AF_5, ENABLE);
    
    // PB2 - UART0_RX
    GPIO_DigitalRemapConfig(AFIOB, GPIO_Pin_2, AFIO_AF_5, ENABLE);

    /* 配置UART0 */
    UART_InitStruct.UART_BaudRate = 115200;
    UART_InitStruct.UART_DataLength = UART_DataLength_8;
    UART_InitStruct.UART_StopBitLength = UART_StopBitLength_1;
    UART_InitStruct.UART_ParityMode = UART_Parity_None;
    UART_InitStruct.UART_Receiver = UART_Receiver_Enable;
    UART_InitStruct.UART_SampleRate = UART_SampleRate_8X;
    UART_Init(UART0, &UART_InitStruct);

    /* 配置UART0中断 */
    UART_ITConfig(UART0, UART_IT_RXNE, ENABLE);
    NVIC_InitStruct.NVIC_IRQChannel = UART0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0x01;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    /* 使能UART0 */
    UART_Cmd(UART0, ENABLE);
}

/**
 * @brief 发送单个字节
 * @param byte: 要发送的字节
 * @retval None
 */
void UART_SendByte(uint8_t byte)
{
    while (UART_GetFlagStatus(UART0, UART_FLAG_TXE) == RESET);
    UART_SendData(UART0, byte);
}

/**
 * @brief 协议初始化
 * @param None
 * @retval None
 */
void Protocol_Init(void)
{
    /* 初始化系统参数 */
    g_system_parameters.input_voltage_threshold = 60000;    // 60V
    g_system_parameters.output_voltage_threshold = 60000;   // 60V
    g_system_parameters.max_input_current = 150000;         // 150A
    g_system_parameters.max_output_current = 150000;        // 150A
    g_system_parameters.max_output_power = 450000;          // 450W
    g_system_parameters.working_mode = MODE_AUTO;

    /* 初始化系统状态 */
    g_system_status.input_voltage = 0;
    g_system_status.input_current = 0;
    g_system_status.output_voltage = 0;
    g_system_status.output_current = 0;
    g_system_status.output_power = 0;
    g_system_status.working_mode = MODE_AUTO;

    /* 初始化接收缓冲区 */
    memset(&g_rx_buffer, 0, sizeof(g_rx_buffer));
    g_rx_buffer.state = RX_STATE_WAIT_HEADER;

    /* 初始化UART */
    UART_Protocol_Init();
}

/**
 * @brief 发送协议帧
 * @param cmd: 命令字
 * @param data: 数据指针
 * @param len: 数据长度
 * @retval None
 */
void Protocol_SendFrame(uint8_t cmd, const uint8_t *data, uint8_t len)
{
    uint8_t frame_buffer[PROTOCOL_MAX_FRAME_LEN];
    uint16_t crc;
    uint16_t i;
    uint16_t frame_len;

    /* 构建帧 */
    frame_buffer[FRAME_OFFSET_HEADER] = PROTOCOL_FRAME_HEADER;
    frame_buffer[FRAME_OFFSET_CMD] = cmd;
    frame_buffer[FRAME_OFFSET_LEN] = len;

    /* 复制数据 */
    if (len > 0 && data != NULL) {
        memcpy(&frame_buffer[FRAME_OFFSET_DATA], data, len);
    }

    /* 计算CRC16 (不包含CRC字段本身) */
    frame_len = FRAME_OFFSET_DATA + len;
    crc = Calculate_CRC16(frame_buffer, frame_len);

    /* 添加CRC16 (低字节在前) */
    frame_buffer[frame_len] = (uint8_t)(crc & 0xFF);
    frame_buffer[frame_len + 1] = (uint8_t)(crc >> 8);

    /* 发送帧 */
    frame_len += 2; // 加上CRC长度
    for (i = 0; i < frame_len; i++) {
        UART_SendByte(frame_buffer[i]);
    }
}

/**
 * @brief 接收处理函数 (在UART中断中调用)
 * @param byte: 接收到的字节
 * @retval None
 */
void Protocol_ReceiveHandler(uint8_t byte)
{
    switch (g_rx_buffer.state) {
        case RX_STATE_WAIT_HEADER:
            if (byte == PROTOCOL_FRAME_HEADER) {
                g_rx_buffer.buffer[0] = byte;
                g_rx_buffer.index = 1;
                g_rx_buffer.state = RX_STATE_WAIT_CMD;
            }
            break;

        case RX_STATE_WAIT_CMD:
            g_rx_buffer.buffer[g_rx_buffer.index++] = byte;
            g_rx_buffer.state = RX_STATE_WAIT_LEN;
            break;

        case RX_STATE_WAIT_LEN:
            g_rx_buffer.buffer[g_rx_buffer.index++] = byte;
            g_rx_buffer.expected_len = byte;
            if (g_rx_buffer.expected_len == 0) {
                g_rx_buffer.state = RX_STATE_WAIT_CRC_LOW;
            } else {
                g_rx_buffer.state = RX_STATE_WAIT_DATA;
            }
            break;

        case RX_STATE_WAIT_DATA:
            g_rx_buffer.buffer[g_rx_buffer.index++] = byte;
            if (g_rx_buffer.index >= (FRAME_OFFSET_DATA + g_rx_buffer.expected_len)) {
                g_rx_buffer.state = RX_STATE_WAIT_CRC_LOW;
            }
            break;

        case RX_STATE_WAIT_CRC_LOW:
            g_rx_buffer.buffer[g_rx_buffer.index++] = byte;
            g_rx_buffer.state = RX_STATE_WAIT_CRC_HIGH;
            break;

        case RX_STATE_WAIT_CRC_HIGH:
            g_rx_buffer.buffer[g_rx_buffer.index++] = byte;
            
            /* 完整帧接收完成，进行处理 */
            {
                Protocol_Frame_t frame;
                uint16_t received_crc, calculated_crc;
                uint16_t crc_index = g_rx_buffer.index - 2;

                /* 提取帧内容 */
                frame.header = g_rx_buffer.buffer[FRAME_OFFSET_HEADER];
                frame.cmd = g_rx_buffer.buffer[FRAME_OFFSET_CMD];
                frame.len = g_rx_buffer.buffer[FRAME_OFFSET_LEN];
                
                if (frame.len > 0) {
                    memcpy(frame.data, &g_rx_buffer.buffer[FRAME_OFFSET_DATA], frame.len);
                }

                /* 提取和校验CRC */
                received_crc = (uint16_t)g_rx_buffer.buffer[crc_index] | 
                              ((uint16_t)g_rx_buffer.buffer[crc_index + 1] << 8);
                calculated_crc = Calculate_CRC16(g_rx_buffer.buffer, crc_index);

                if (received_crc == calculated_crc) {
                    /* CRC校验成功，处理命令 */
                    Protocol_ProcessCommand(&frame);
                } else {
                    /* CRC校验失败 */
                    Protocol_SendAckError(ERROR_CRC_FAILED);
                }
            }

            /* 重置接收状态机 */
            g_rx_buffer.state = RX_STATE_WAIT_HEADER;
            g_rx_buffer.index = 0;
            break;

        default:
            /* 异常状态，重置状态机 */
            g_rx_buffer.state = RX_STATE_WAIT_HEADER;
            g_rx_buffer.index = 0;
            break;
    }
}

/**
 * @brief 处理接收到的命令
 * @param frame: 协议帧指针
 * @retval None
 */
void Protocol_ProcessCommand(const Protocol_Frame_t *frame)
{
    uint16_t value16;
    uint8_t value8;

    switch (frame->cmd) {
        case CMD_SET_INPUT_VOLTAGE:
            if (frame->len == 2) {
                value16 = (uint16_t)frame->data[0] | ((uint16_t)frame->data[1] << 8);
                Handle_SetInputVoltage(value16);
                Protocol_SendAckError(ERROR_OK);
            } else {
                Protocol_SendAckError(ERROR_INVALID_DATA_LEN);
            }
            break;

        case CMD_SET_OUTPUT_VOLTAGE:
            if (frame->len == 2) {
                value16 = (uint16_t)frame->data[0] | ((uint16_t)frame->data[1] << 8);
                Handle_SetOutputVoltage(value16);
                Protocol_SendAckError(ERROR_OK);
            } else {
                Protocol_SendAckError(ERROR_INVALID_DATA_LEN);
            }
            break;

        case CMD_SET_MAX_INPUT_CURRENT:
            if (frame->len == 2) {
                value16 = (uint16_t)frame->data[0] | ((uint16_t)frame->data[1] << 8);
                Handle_SetMaxInputCurrent(value16);
                Protocol_SendAckError(ERROR_OK);
            } else {
                Protocol_SendAckError(ERROR_INVALID_DATA_LEN);
            }
            break;

        case CMD_SET_MAX_OUTPUT_CURRENT:
            if (frame->len == 2) {
                value16 = (uint16_t)frame->data[0] | ((uint16_t)frame->data[1] << 8);
                Handle_SetMaxOutputCurrent(value16);
                Protocol_SendAckError(ERROR_OK);
            } else {
                Protocol_SendAckError(ERROR_INVALID_DATA_LEN);
            }
            break;

        case CMD_SET_MAX_OUTPUT_POWER:
            if (frame->len == 2) {
                value16 = (uint16_t)frame->data[0] | ((uint16_t)frame->data[1] << 8);
                Handle_SetMaxOutputPower(value16);
                Protocol_SendAckError(ERROR_OK);
            } else {
                Protocol_SendAckError(ERROR_INVALID_DATA_LEN);
            }
            break;

        case CMD_QUERY_STATUS:
            if (frame->len == 0) {
                Handle_QueryStatus();
            } else {
                Protocol_SendAckError(ERROR_INVALID_DATA_LEN);
            }
            break;

        case CMD_MODE_SWITCH:
            if (frame->len == 1) {
                value8 = frame->data[0];
                Handle_ModeSwitch(value8);
                Protocol_SendAckError(ERROR_OK);
            } else {
                Protocol_SendAckError(ERROR_INVALID_DATA_LEN);
            }
            break;

        default:
            Protocol_SendAckError(ERROR_INVALID_CMD);
            break;
    }
}

/**
 * @brief 处理设置输入电压阈值命令
 * @param voltage_mv: 电压值(mV)
 * @retval None
 */
void Handle_SetInputVoltage(uint16_t voltage_mv)
{
    if (voltage_mv >= 10000 && voltage_mv <= 100000) { // 10V~100V
        g_system_parameters.input_voltage_threshold = voltage_mv;
    }
}

/**
 * @brief 处理设置输出电压阈值命令
 * @param voltage_mv: 电压值(mV)
 * @retval None
 */
void Handle_SetOutputVoltage(uint16_t voltage_mv)
{
    if (voltage_mv >= 5000 && voltage_mv <= 100000) { // 5V~100V
        g_system_parameters.output_voltage_threshold = voltage_mv;
    }
}

/**
 * @brief 处理设置最大输入电流命令
 * @param current_ma: 电流值(mA)
 * @retval None
 */
void Handle_SetMaxInputCurrent(uint16_t current_ma)
{
    if (current_ma >= 1000 && current_ma <= 200000) { // 1A~200A
        g_system_parameters.max_input_current = current_ma;
    }
}

/**
 * @brief 处理设置最大输出电流命令
 * @param current_ma: 电流值(mA)
 * @retval None
 */
void Handle_SetMaxOutputCurrent(uint16_t current_ma)
{
    if (current_ma >= 1000 && current_ma <= 200000) { // 1A~200A
        g_system_parameters.max_output_current = current_ma;
    }
}

/**
 * @brief 处理设置最大输出功率命令
 * @param power_mw: 功率值(mW)
 * @retval None
 */
void Handle_SetMaxOutputPower(uint16_t power_mw)
{
    if (power_mw >= 10000 && power_mw <= 500000) { // 10W~500W
        g_system_parameters.max_output_power = power_mw;
    }
}

/**
 * @brief 处理查询状态命令
 * @param None
 * @retval None
 */
void Handle_QueryStatus(void)
{
    Protocol_SendStatusReport();
}

/**
 * @brief 处理模式切换命令
 * @param mode: 工作模式
 * @retval None
 */
void Handle_ModeSwitch(uint8_t mode)
{
    if (mode <= MODE_AUTO) {
        g_system_parameters.working_mode = mode;
        g_system_status.working_mode = mode;
        // 这里可以添加实际的模式切换代码
    }
}

/**
 * @brief 发送状态上报
 * @param None
 * @retval None
 */
void Protocol_SendStatusReport(void)
{
    uint8_t status_data[11];

    /* 按照协议文档格式打包状态数据 */
    // Vin (2字节, little-endian)
    status_data[0] = (uint8_t)(g_system_status.input_voltage & 0xFF);
    status_data[1] = (uint8_t)(g_system_status.input_voltage >> 8);
    
    // Iin (2字节, little-endian)
    status_data[2] = (uint8_t)(g_system_status.input_current & 0xFF);
    status_data[3] = (uint8_t)(g_system_status.input_current >> 8);
    
    // Vout (2字节, little-endian)
    status_data[4] = (uint8_t)(g_system_status.output_voltage & 0xFF);
    status_data[5] = (uint8_t)(g_system_status.output_voltage >> 8);
    
    // Iout (2字节, little-endian)
    status_data[6] = (uint8_t)(g_system_status.output_current & 0xFF);
    status_data[7] = (uint8_t)(g_system_status.output_current >> 8);
    
    // Pout (2字节, little-endian)
    status_data[8] = (uint8_t)(g_system_status.output_power & 0xFF);
    status_data[9] = (uint8_t)(g_system_status.output_power >> 8);
    
    // Mode (1字节)
    status_data[10] = g_system_status.working_mode;

    Protocol_SendFrame(CMD_STATUS_REPORT, status_data, 11);
}

/**
 * @brief 发送ACK/错误响应
 * @param error_code: 错误码
 * @retval None
 */
void Protocol_SendAckError(Error_Code_t error_code)
{
    uint8_t error_data = (uint8_t)error_code;
    Protocol_SendFrame(CMD_ACK_ERROR, &error_data, 1);
}

/**
 * @brief 获取系统状态指针
 * @param None
 * @retval 系统状态指针
 */
System_Status_t* Get_SystemStatus(void)
{
    return &g_system_status;
}

/**
 * @brief 获取系统参数指针
 * @param None
 * @retval 系统参数指针
 */
System_Parameters_t* Get_SystemParameters(void)
{
    return &g_system_parameters;
}

/**
 * @brief UART0中断处理函数
 * @param None
 * @retval None
 */
void UART0_Handler(void)
{
    if (UART_GetFlagStatus(UART0, UART_FLAG_RXNE) != RESET) {
        uint8_t received_byte = (uint8_t)UART_ReceiveData(UART0);
        Protocol_ReceiveHandler(received_byte);
    }
}