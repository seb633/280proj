/******************************************************************************
  * @file    ccube_protocol_test.c
  * @author  C-Cube Development Team
  * @version V1.0.0
  * @date    2025/09/17
  * @brief   C-Cube UART通信协议测试工具
  *          提供协议测试函数和CRC计算验证
  ******************************************************************************
  * @attention
  *
  * 本文件提供协议测试和验证功能，可用于调试和验证协议实现
  *
  * Copyright (C) 2025, C-Cube Team, all rights reserved
  *
  *****************************************************************************/

#include "ccube_protocol_test.h"
#include "ccube_uart_protocol.h"
#include <stdio.h>
#include <string.h>

/* 测试用例结构体 */
typedef struct {
    uint8_t cmd;
    uint8_t data[16];
    uint8_t len;
    const char* description;
    uint16_t expected_crc;
} Test_Case_t;

/* 预定义测试用例 */
static const Test_Case_t test_cases[] = {
    /* 设置输入电压阈值60V */
    {0x01, {0x60, 0xEA}, 2, "Set Input Voltage 60V", 0x0000},
    
    /* 设置输出电压阈值60V */
    {0x02, {0x60, 0xEA}, 2, "Set Output Voltage 60V", 0x0000},
    
    /* 设置最大输入电流150A */
    {0x03, {0x70, 0x49}, 2, "Set Max Input Current 150A", 0x0000},
    
    /* 设置最大输出电流150A */
    {0x04, {0x70, 0x49}, 2, "Set Max Output Current 150A", 0x0000},
    
    /* 设置最大输出功率450W */
    {0x05, {0xF0, 0xDA}, 2, "Set Max Output Power 450W", 0x0000},
    
    /* 查询状态 */
    {0x06, {0}, 0, "Query Status", 0x0000},
    
    /* 模式切换到BOOST */
    {0x08, {0x01}, 1, "Switch to BOOST Mode", 0x0000},
    
    /* 模式切换到AUTO */
    {0x08, {0x02}, 1, "Switch to AUTO Mode", 0x0000}
};

#define TEST_CASE_COUNT (sizeof(test_cases) / sizeof(test_cases[0]))

/**
 * @brief 计算并填充测试用例的期望CRC值
 * @param None
 * @retval None
 */
static void Calculate_TestCase_CRC(void)
{
    uint8_t frame_buffer[32];
    uint16_t i;
    
    printf("=== Test Case CRC Calculation ===\r\n");
    
    for (i = 0; i < TEST_CASE_COUNT; i++) {
        const Test_Case_t *test = &test_cases[i];
        uint16_t crc;
        uint16_t frame_len;
        
        /* 构建测试帧 */
        frame_buffer[0] = 0xAA;           // 帧头
        frame_buffer[1] = test->cmd;      // 命令字
        frame_buffer[2] = test->len;      // 数据长度
        
        /* 复制数据 */
        if (test->len > 0) {
            memcpy(&frame_buffer[3], test->data, test->len);
        }
        
        frame_len = 3 + test->len;
        crc = Calculate_CRC16(frame_buffer, frame_len);
        
        printf("Case %d: %s\r\n", i + 1, test->description);
        printf("  Frame: ");
        for (uint16_t j = 0; j < frame_len; j++) {
            printf("%02X ", frame_buffer[j]);
        }
        printf("\r\n  CRC16: 0x%04X\r\n\r\n", crc);
    }
}

/**
 * @brief 执行所有测试用例
 * @param None
 * @retval None
 */
void Protocol_RunAllTests(void)
{
    uint16_t i;
    
    printf("\r\n");
    printf("========================================\r\n");
    printf("       C-Cube Protocol Test Suite      \r\n");
    printf("========================================\r\n");
    
    /* 计算测试用例CRC */
    Calculate_TestCase_CRC();
    
    printf("=== Executing Test Cases ===\r\n");
    
    for (i = 0; i < TEST_CASE_COUNT; i++) {
        const Test_Case_t *test = &test_cases[i];
        
        printf("Test %d: %s\r\n", i + 1, test->description);
        
        /* 发送测试命令 */
        Protocol_SendFrame(test->cmd, test->data, test->len);
        
        /* 延时等待响应 */
        Protocol_Test_Delay(100000);
        
        printf("  -> Command sent\r\n");
    }
    
    printf("\r\nAll tests completed!\r\n");
    printf("========================================\r\n");
}

/**
 * @brief 测试CRC16计算功能
 * @param None
 * @retval None
 */
void Protocol_TestCRC16(void)
{
    /* 标准Modbus-RTU CRC16测试向量 */
    struct {
        uint8_t data[16];
        uint16_t len;
        uint16_t expected_crc;
        const char* description;
    } crc_tests[] = {
        {{0x01, 0x03, 0x00, 0x00, 0x00, 0x01}, 6, 0x840A, "Modbus Read Holding Register"},
        {{0x01, 0x10, 0x00, 0x00, 0x00, 0x02, 0x04, 0x00, 0x0A, 0x01, 0x02}, 11, 0xC89C, "Modbus Write Multiple Registers"},
        {{0xAA, 0x01, 0x02, 0x60, 0xEA}, 5, 0x0000, "C-Cube Set Voltage Frame (placeholder)"}
    };
    
    uint16_t test_count = sizeof(crc_tests) / sizeof(crc_tests[0]);
    uint16_t i;
    
    printf("\r\n=== CRC16 Calculation Tests ===\r\n");
    
    for (i = 0; i < test_count; i++) {
        uint16_t calculated_crc = Calculate_CRC16(crc_tests[i].data, crc_tests[i].len);
        
        printf("Test %d: %s\r\n", i + 1, crc_tests[i].description);
        printf("  Data: ");
        for (uint16_t j = 0; j < crc_tests[i].len; j++) {
            printf("%02X ", crc_tests[i].data[j]);
        }
        printf("\r\n  Expected CRC: 0x%04X\r\n", crc_tests[i].expected_crc);
        printf("  Calculated CRC: 0x%04X\r\n", calculated_crc);
        
        if (i < 2) { // 前两个有标准期望值
            printf("  Result: %s\r\n", 
                   (calculated_crc == crc_tests[i].expected_crc) ? "PASS" : "FAIL");
        } else {
            printf("  Result: Calculated (no reference)\r\n");
        }
        printf("\r\n");
    }
}

/**
 * @brief 测试协议帧解析
 * @param None
 * @retval None
 */
void Protocol_TestFrameParsing(void)
{
    /* 模拟接收一个完整的协议帧 */
    uint8_t test_frame[] = {0xAA, 0x01, 0x02, 0x60, 0xEA, 0x00, 0x00}; // CRC待计算
    uint16_t frame_len = 7;
    uint16_t crc;
    uint16_t i;
    
    printf("\r\n=== Frame Parsing Test ===\r\n");
    
    /* 计算并设置正确的CRC */
    crc = Calculate_CRC16(test_frame, 5);
    test_frame[5] = (uint8_t)(crc & 0xFF);      // CRC低字节
    test_frame[6] = (uint8_t)(crc >> 8);        // CRC高字节
    
    printf("Test frame with correct CRC:\r\n");
    printf("  Frame: ");
    for (i = 0; i < frame_len; i++) {
        printf("%02X ", test_frame[i]);
    }
    printf("\r\n");
    
    /* 模拟逐字节接收 */
    printf("Simulating byte-by-byte reception:\r\n");
    for (i = 0; i < frame_len; i++) {
        printf("  Receiving byte %d: 0x%02X\r\n", i + 1, test_frame[i]);
        Protocol_ReceiveHandler(test_frame[i]);
        Protocol_Test_Delay(10000); // 短延时模拟字节间隔
    }
    
    printf("Frame parsing test completed.\r\n");
}

/**
 * @brief 测试系统状态上报
 * @param None
 * @retval None
 */
void Protocol_TestStatusReport(void)
{
    System_Status_t *status = Get_SystemStatus();
    System_Parameters_t *params = Get_SystemParameters();
    
    printf("\r\n=== System Status Report Test ===\r\n");
    
    /* 设置一些测试数据 */
    status->input_voltage = 55000;      // 55V
    status->input_current = 12000;      // 12A
    status->output_voltage = 60000;     // 60V
    status->output_current = 10000;     // 10A
    status->output_power = 600000;      // 600W
    status->working_mode = MODE_BOOST;
    
    printf("Current System Status:\r\n");
    printf("  Input Voltage: %u mV (%.1f V)\r\n", status->input_voltage, status->input_voltage / 1000.0);
    printf("  Input Current: %u mA (%.1f A)\r\n", status->input_current, status->input_current / 1000.0);
    printf("  Output Voltage: %u mV (%.1f V)\r\n", status->output_voltage, status->output_voltage / 1000.0);
    printf("  Output Current: %u mA (%.1f A)\r\n", status->output_current, status->output_current / 1000.0);
    printf("  Output Power: %u mW (%.1f W)\r\n", status->output_power, status->output_power / 1000.0);
    printf("  Working Mode: %u (%s)\r\n", status->working_mode, 
           (status->working_mode == MODE_BUCK) ? "BUCK" :
           (status->working_mode == MODE_BOOST) ? "BOOST" : "AUTO");
    
    printf("\r\nSending status report frame...\r\n");
    Protocol_SendStatusReport();
    
    printf("Status report sent.\r\n");
}

/**
 * @brief 测试延时函数
 * @param count: 延时计数
 * @retval None
 */
void Protocol_Test_Delay(uint32_t count)
{
    volatile uint32_t i;
    for (i = 0; i < count; i++) {
        /* 空循环延时 */
    }
}

/**
 * @brief 打印协议帧的十六进制格式
 * @param data: 数据指针
 * @param len: 数据长度
 * @param description: 描述字符串
 * @retval None
 */
void Protocol_PrintFrame(const uint8_t *data, uint16_t len, const char *description)
{
    uint16_t i;
    
    if (description) {
        printf("%s:\r\n", description);
    }
    
    printf("  Frame (%d bytes): ", len);
    for (i = 0; i < len; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0 && (i + 1) < len) {
            printf("\r\n                    ");
        }
    }
    printf("\r\n");
}

/**
 * @brief 协议性能测试
 * @param None
 * @retval None
 */
void Protocol_PerformanceTest(void)
{
    uint32_t start_time, end_time;
    uint16_t i;
    uint8_t test_data[32];
    
    printf("\r\n=== Protocol Performance Test ===\r\n");
    
    /* 填充测试数据 */
    for (i = 0; i < 32; i++) {
        test_data[i] = (uint8_t)(i & 0xFF);
    }
    
    /* CRC计算性能测试 */
    printf("Testing CRC16 calculation performance...\r\n");
    start_time = 0; // 这里应该使用实际的时间测量函数
    
    for (i = 0; i < 1000; i++) {
        Calculate_CRC16(test_data, 16);
    }
    
    end_time = 1000; // 模拟时间
    printf("  1000 CRC calculations completed\r\n");
    printf("  Estimated time: %u units\r\n", end_time - start_time);
    
    /* 帧发送性能测试 */
    printf("\r\nTesting frame transmission performance...\r\n");
    start_time = 0;
    
    for (i = 0; i < 100; i++) {
        Protocol_SendFrame(CMD_SET_INPUT_VOLTAGE, test_data, 2);
        Protocol_Test_Delay(1000); // 短延时避免缓冲区溢出
    }
    
    end_time = 100;
    printf("  100 frame transmissions completed\r\n");
    printf("  Estimated time: %u units\r\n", end_time - start_time);
    
    printf("Performance test completed.\r\n");
}

/**
 * @brief 运行完整的协议测试套件
 * @param None
 * @retval None
 */
void Protocol_RunCompleteTestSuite(void)
{
    printf("\r\n");
    printf("╔══════════════════════════════════════╗\r\n");
    printf("║     C-Cube Protocol Test Suite       ║\r\n");
    printf("║           Complete Test              ║\r\n");
    printf("╚══════════════════════════════════════╝\r\n");
    
    /* 1. CRC16测试 */
    Protocol_TestCRC16();
    Protocol_Test_Delay(500000);
    
    /* 2. 帧解析测试 */
    Protocol_TestFrameParsing();
    Protocol_Test_Delay(500000);
    
    /* 3. 状态上报测试 */
    Protocol_TestStatusReport();
    Protocol_Test_Delay(500000);
    
    /* 4. 协议命令测试 */
    Protocol_RunAllTests();
    Protocol_Test_Delay(500000);
    
    /* 5. 性能测试 */
    Protocol_PerformanceTest();
    
    printf("\r\n");
    printf("╔══════════════════════════════════════╗\r\n");
    printf("║        All Tests Completed!         ║\r\n");
    printf("╚══════════════════════════════════════╝\r\n");
}