/******************************************************************************
  * @file    ccube_main.c (旧版本，已合并到 c_cube_bidirectional_dcdc.c)
  * @author  C-Cube Development Team
  * @version V1.0.0
  * @date    2025/09/17
  * @brief   C-Cube双向DCDC控制器 - 重定向到完整版本
  ******************************************************************************
  * @attention
  *
  * 注意：本文件已被合并到 c_cube_bidirectional_dcdc.c 中
  * 新的完整C-Cube双向DCDC控制器程序包含：
  * - UART协议通信
  * - DCDC控制算法
  * - PWM输出控制
  * - ADC电压电流采集
  * - 安全保护功能
  * - 自动模式切换
  *
  * 请使用: c_cube_bidirectional_dcdc.c
  *
  * Copyright (C) 2025, C-Cube Team, all rights reserved
  *
  *****************************************************************************/

#include "PT32x0xx.h"
#include "PT32x0xx_config.h"
#include <stdio.h>

/* 重定向到完整版本的说明 */

/* 私有函数声明 */
static void RCC_Configuration(void);
static void GPIO_Configuration(void);
static void NVIC_Configuration(void);
static void System_Init(void);
static void Print_SystemInfo(void);

/**
 * @brief 系统时钟配置
 * @param None
 * @retval None
 */
static void RCC_Configuration(void)
{
    /* 使能相关外设时钟 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_UART0, ENABLE);
    RCC_APBPeriph1ClockCmd(RCC_APBPeriph1_TIM1, ENABLE);
}

/**
 * @brief GPIO配置
 * @param None
 * @retval None
 */
static void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    /* 配置LED指示灯 (假设使用PA0作为状态指示) */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OutputType = GPIO_OutputType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_High;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /* LED初始状态为关闭 */
    GPIO_ResetBits(GPIOA, GPIO_Pin_0);
}

/**
 * @brief NVIC配置
 * @param None
 * @retval None
 */
static void NVIC_Configuration(void)
{
    /* NVIC优先级分组 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}

/**
 * @brief 系统初始化
 * @param None
 * @retval None
 */
static void System_Init(void)
{
    /* 基本系统配置 */
    RCC_Configuration();
    NVIC_Configuration();
    GPIO_Configuration();
    
    /* 协议相关初始化 */
    Protocol_Init();            // 初始化UART协议
    Timer_StatusReport_Init();  // 初始化定时器
    
    /* 使能全局中断 */
    __enable_irq();
}

/**
 * @brief 打印系统信息
 * @param None
 * @retval None
 */
static void Print_SystemInfo(void)
{
    System_Parameters_t *params = Get_SystemParameters();
    
    printf("\r\n");
    printf("=====================================\r\n");
    printf("      C-Cube UART Protocol v1.0     \r\n");
    printf("=====================================\r\n");
    printf("UART Config: 115200bps, 8N1\r\n");
    printf("TX Pin: PA15, RX Pin: PB2\r\n");
    printf("Protocol: 0xAA + CMD + LEN + DATA + CRC16\r\n");
    printf("Status Report: Every 100ms\r\n");
    printf("\r\n");
    printf("System Parameters:\r\n");
    printf("- Input Voltage Threshold: %u mV\r\n", params->input_voltage_threshold);
    printf("- Output Voltage Threshold: %u mV\r\n", params->output_voltage_threshold);
    printf("- Max Input Current: %u mA\r\n", params->max_input_current);
    printf("- Max Output Current: %u mA\r\n", params->max_output_current);
    printf("- Max Output Power: %u mW\r\n", params->max_output_power);
    printf("- Working Mode: %u\r\n", params->working_mode);
    printf("\r\n");
    printf("Command List:\r\n");
    printf("0x01: Set Input Voltage (2 bytes)\r\n");
    printf("0x02: Set Output Voltage (2 bytes)\r\n");
    printf("0x03: Set Max Input Current (2 bytes)\r\n");
    printf("0x04: Set Max Output Current (2 bytes)\r\n");
    printf("0x05: Set Max Output Power (2 bytes)\r\n");
    printf("0x06: Query Status (0 bytes)\r\n");
    printf("0x07: Status Report (11 bytes)\r\n");
    printf("0x08: Mode Switch (1 byte)\r\n");
    printf("0x09: ACK/Error (1 byte)\r\n");
    printf("\r\n");
    printf("System Ready!\r\n");
    printf("=====================================\r\n");
}

/**
 * @brief 主函数
 * @param None
 * @retval int
 */
int main(void)
{
    static uint32_t led_counter = 0;
    
    /* 系统初始化 */
    System_Init();
    
    /* 打印系统信息 */
    Print_SystemInfo();
    
    /* 主循环 */
    while (1) {
        /* 处理定时状态上报 */
        Timer_StatusReport_Handler();
        
        /* LED状态指示 (每1000次循环闪烁一次) */
        led_counter++;
        if (led_counter >= 100000) {
            led_counter = 0;
            GPIO_ToggleBits(GPIOA, GPIO_Pin_0);
        }
        
        /* 这里可以添加其他主循环任务 */
        // 例如：ADC采集、PWM控制、故障检测等
    }
}

/**
 * @brief 测试命令发送函数 (用于调试)
 * @param None
 * @retval None
 */
void Test_SendCommands(void)
{
    uint8_t test_data[2];
    
    /* 测试设置输入电压阈值为60V (60000mV) */
    test_data[0] = 0x60;    // 60000 & 0xFF = 0x60
    test_data[1] = 0xEA;    // 60000 >> 8 = 0xEA
    Protocol_SendFrame(CMD_SET_INPUT_VOLTAGE, test_data, 2);
    
    /* 延时 */
    for (volatile uint32_t i = 0; i < 100000; i++);
    
    /* 测试查询状态 */
    Protocol_SendFrame(CMD_QUERY_STATUS, NULL, 0);
    
    /* 延时 */
    for (volatile uint32_t i = 0; i < 100000; i++);
    
    /* 测试模式切换到BOOST */
    test_data[0] = MODE_BOOST;
    Protocol_SendFrame(CMD_MODE_SWITCH, test_data, 1);
}

/**
 * @brief 协议帧示例和计算说明
 * @param None
 * @retval None
 */
void Protocol_Examples(void)
{
    /*
     * 协议帧示例：
     * 
     * 1. 设置输入电压阈值60V (60000mV = 0xEA60)
     *    帧格式: AA 01 02 60 EA CRC16
     *    说明: 
     *    - 0xAA: 帧头
     *    - 0x01: 命令字(设置输入电压)
     *    - 0x02: 数据长度(2字节)
     *    - 0x60: 60000的低字节
     *    - 0xEA: 60000的高字节
     *    - CRC16: 前5字节的Modbus-RTU CRC16校验
     * 
     * 2. 状态上报帧示例 (假设数据)
     *    帧格式: AA 07 0B 70 17 10 27 60 EA 40 42 20 6D 01 CRC16
     *    数据解析:
     *    - Vin = 0x1770 = 6000mV
     *    - Iin = 0x2710 = 10000mA
     *    - Vout = 0xEA60 = 60000mV
     *    - Iout = 0x4240 = 16960mA
     *    - Pout = 0x6D20 = 27936mW
     *    - Mode = 0x01 = BOOST模式
     * 
     * 3. CRC16计算 (Modbus-RTU)
     *    初始值: 0xFFFF
     *    多项式: 0xA001
     *    数据: AA 01 02 60 EA
     *    结果: 使用查表法快速计算
     */
}

/* 中断处理函数在对应的.c文件中已定义 */

#ifdef USE_FULL_ASSERT
/**
 * @brief 断言失败处理函数
 * @param file: 文件名
 * @param line: 行号
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);
    while (1) {
        /* 无限循环 */
    }
}
#endif