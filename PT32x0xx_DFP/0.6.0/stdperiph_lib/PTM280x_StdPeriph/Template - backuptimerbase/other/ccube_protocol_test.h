/******************************************************************************
  * @file    ccube_protocol_test.h
  * @author  C-Cube Development Team
  * @version V1.0.0
  * @date    2025/09/17
  * @brief   C-Cube UART通信协议测试工具头文件
  ******************************************************************************
  * @attention
  *
  * Copyright (C) 2025, C-Cube Team, all rights reserved
  *
  *****************************************************************************/

#ifndef __CCUBE_PROTOCOL_TEST_H
#define __CCUBE_PROTOCOL_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* 测试函数声明 */

/**
 * @brief 运行所有协议测试用例
 * @param None
 * @retval None
 */
void Protocol_RunAllTests(void);

/**
 * @brief 测试CRC16计算功能
 * @param None
 * @retval None
 */
void Protocol_TestCRC16(void);

/**
 * @brief 测试协议帧解析
 * @param None
 * @retval None
 */
void Protocol_TestFrameParsing(void);

/**
 * @brief 测试系统状态上报
 * @param None
 * @retval None
 */
void Protocol_TestStatusReport(void);

/**
 * @brief 协议性能测试
 * @param None
 * @retval None
 */
void Protocol_PerformanceTest(void);

/**
 * @brief 运行完整的协议测试套件
 * @param None
 * @retval None
 */
void Protocol_RunCompleteTestSuite(void);

/**
 * @brief 测试延时函数
 * @param count: 延时计数
 * @retval None
 */
void Protocol_Test_Delay(uint32_t count);

/**
 * @brief 打印协议帧的十六进制格式
 * @param data: 数据指针
 * @param len: 数据长度
 * @param description: 描述字符串
 * @retval None
 */
void Protocol_PrintFrame(const uint8_t *data, uint16_t len, const char *description);

#ifdef __cplusplus
}
#endif

#endif /* __CCUBE_PROTOCOL_TEST_H */