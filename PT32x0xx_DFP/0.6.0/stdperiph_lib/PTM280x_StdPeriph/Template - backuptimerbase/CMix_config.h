/******************************************************************************
  * @file    CMix_config.h
  * @author  CMix Development Team
  * @version V1.0.0
  * @date    2025/09/17
  * @brief   CMix Bidirectional DCDC Controller Configuration Header
  *          Defines system configuration parameters, hardware pin assignments, and function switches
  ******************************************************************************
  * @attention
  *
  * CMix Bidirectional DCDC Controller System Configuration File
  * Contains all system-level configuration parameters and hardware definitions
  *
  * Copyright (C) 2025, CMix Team, all rights reserved
  *
  *****************************************************************************/

#ifndef __CMIX_CONFIG_H
#define __CMIX_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* ========================= 包含头文件 ========================= */
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

/* 包含PT32x0xx库头文件 */
#include "PT32x0xx.h"
#include "PT32x0xx_rcc.h"
#include "PT32x0xx_gpio.h"
#include "PT32x0xx_adc.h"
#include "PT32x0xx_tim.h"
#include "PT32x0xx_uart.h"
#include "PT32x0xx_cmp.h"
#include "PT32x0xx_dma.h"
#include "PT32x0xx_nvic.h"
#include "PT32x0xx_iwdg.h"
#include "PT32x0xx_opa.h"

/* ========================= 系统配置参数 ========================= */

/* 错误码定义 */
typedef enum {
    CMIX_ERROR_NONE = 0,                // 无错误
    CMIX_ERROR_OVERVOLTAGE = 1,         // 过压错误
    CMIX_ERROR_UNDERVOLTAGE = 2,        // 欠压错误
    CMIX_ERROR_OVERCURRENT = 3,         // 过流错误
    CMIX_ERROR_OVERTEMPERATURE = 4,     // 过温错误
    CMIX_ERROR_COMMUNICATION = 5,       // 通信错误
    CMIX_ERROR_HARDWARE = 6             // 硬件错误
} CMix_Error_t;

/* DCDC 工作模式定义 */
typedef enum {
    CMIX_MODE_AUTO = 0,                 // Auto mode
    CMIX_MODE_BUCK = 1,                 // BUCK mode
    CMIX_MODE_BOOST = 2,                // BOOST mode
    CMIX_MODE_IDLE = 3,                 // IDLE mode
    CMIX_MODE_FAULT = 4                 // FAULT mode
} CMix_Working_Mode_t;

typedef enum {
    CMIX_STATE_INIT = 0,                // 初始化状态
    CMIX_STATE_IDLE = 1,                // 空闲状态
    CMIX_STATE_BUCK = 2,                // BUCK运行状态
    CMIX_STATE_BOOST = 3,               // BOOST运行状态
    CMIX_STATE_FAULT = 4,               // 故障状态
    CMIX_STATE_PROTECT = 5,             // 保护状态
    CMIX_STATE_SOFT_START = 6,          // 软启动状态
    CMIX_STATE_RUNNING = 7              // 运行状态
} CMix_State_t;

typedef enum {
    CMIX_APP_STATE_INIT = 0,            // 应用初始化
    CMIX_APP_STATE_IDLE = 1,            // 应用空闲
    CMIX_APP_STATE_RUNNING = 2,         // 应用运行
    CMIX_APP_STATE_FAULT = 3,           // 应用故障
    CMIX_APP_STATE_EMERGENCY = 4,       // 应用紧急状态
    CMIX_APP_STATE_MAINTENANCE = 5      // 应用维护
} CMix_App_State_t;

/* Mode compatibility aliases */
typedef CMix_Working_Mode_t CMix_Mode_t;
typedef CMix_State_t CMix_System_State_t;

/* ========================= 调试与功能开关 ========================= */
#define CMIX_DEBUG_ENABLE           1   // 启用调试输出
#define CMIX_UART_ENABLE            1   // 启用UART通信
#define CMIX_MODBUS_ENABLE          1   // 启用Modbus协议
#define CMIX_SAFETY_ENABLE          1   // 启用安全保护功能
#define CMIX_PWM_INTERLOCK_ENABLE   1   // 启用PWM互锁保护
#define CMIX_CMP_PROTECTION_ENABLE  1   // 启用硬件比较器保护
#define CMIX_OVER_CURRENT_ENABLE    1   // 启用过流保护
#define CMIX_WATCHDOG_ENABLE        1   // 启用看门狗

/* ========================= 硬件引脚配置 ========================= */

/* 系统时钟配置 */
#define CMIX_SYSTEM_CLOCK_HZ        48000000    // 48MHz系统时钟

/* PWM配置 */
#define CMIX_PWM_FREQUENCY_HZ       100000      // 100kHz PWM频率
#define CMIX_PWM_PERIOD             480         // PWM周期计数值 (48MHz/100kHz)
#define CMIX_PWM_DEADTIME_NS        200         // 死区时间200ns
#define CMIX_PWM_MAX_DUTY           95          // 最大占空比95%
#define CMIX_PWM_MIN_DUTY           5           // 最小占空比5%

/* TIM1 PWM引脚配置 */
#define CMIX_PWM_PHASE_A_PORT       GPIOA       // 相A PWM引脚组
#define CMIX_PWM_PHASE_A_HIGH_PIN   GPIO_Pin_8  // TIM1_CH1 - 相A上管
#define CMIX_PWM_PHASE_A_LOW_PIN    GPIO_Pin_7  // TIM1_CH1N - 相A下管
#define CMIX_PWM_PHASE_B_PORT       GPIOA       // 相B PWM引脚组
#define CMIX_PWM_PHASE_B_HIGH_PIN   GPIO_Pin_9  // TIM1_CH2 - 相B上管
#define CMIX_PWM_PHASE_B_LOW_PIN    GPIO_Pin_10 // TIM1_CH2N - 相B下管

/* TIM1 BKIN外部保护引脚 */
#define CMIX_PWM_BKIN_PORT          GPIOB       // BKIN引脚
#define CMIX_PWM_BKIN_PIN           GPIO_Pin_5  // PB5 = TIM1_BKIN (外部比较器输出)

/* ========================= ADC配置 ========================= */

/* Vin ADC Configuration - 高压侧输入电压采样 */
#define CMIX_ADC_VIN_PORT           GPIOB       // PB12 = ADC0_IN0
#define CMIX_ADC_VIN_PIN            GPIO_Pin_12 // PB12引脚
#define CMIX_ADC_VIN_CHANNEL        0           // ADC0_IN0 = Channel_0

/* 相A电流采样配置 - TP181A1输出A */
#define CMIX_ADC_CURRENT_A_PORT     GPIOA       // 相A电流采样引脚
#define CMIX_ADC_CURRENT_A_PIN      GPIO_Pin_1  // PA1 = ADC0_IN1
#define CMIX_ADC_CURRENT_A_CHANNEL  1           // ADC0_IN1

/* Vout ADC Configuration - 低压侧输出电压采样 */
#define CMIX_ADC_VOUT_PORT          GPIOA
#define CMIX_ADC_VOUT_PIN           GPIO_Pin_2
#define CMIX_ADC_VOUT_CHANNEL       2

/* 相B电流采样配置 - TP181A1输出B */  
#define CMIX_ADC_CURRENT_B_PORT     GPIOA       // 相B电流采样引脚
#define CMIX_ADC_CURRENT_B_PIN      GPIO_Pin_3  // PA3 = ADC0_IN3
#define CMIX_ADC_CURRENT_B_CHANNEL  3           // ADC0_IN3

/* TP181A1电流放大器参数 */
#define CMIX_CURRENT_SENSE_RS       0.00025f    // 分流电阻 0.25mΩ
#define CMIX_CURRENT_SENSE_GAIN     50.0f       // TP181A1增益
#define CMIX_CURRENT_SENSE_VREF     2.5f        // 参考电压 VDDA/2 = 2.5V

/* 电流感应兼容别名 */
#define CMIX_CURRENT_VREF           CMIX_CURRENT_SENSE_VREF

/* ADC配置参数 */
#define CMIX_ADC_RESOLUTION         4095        // 12位ADC分辨率
#define CMIX_ADC_VREF               5.0f        // ADC参考电压5V
#define CMIX_ADC_SAMPLE_TIME        239         // ADC采样时间 239.5周期

/* ========================= UART配置 ========================= */
#define CMIX_UART_PORT              UART0       // UART0
#define CMIX_UART_BAUDRATE          115200      // 波特率115200
#define CMIX_UART_TX_PORT           GPIOA       // TX引脚端口
#define CMIX_UART_TX_PIN            GPIO_Pin_15 // PA15 = UART0_TX
#define CMIX_UART_RX_PORT           GPIOB       // RX引脚端口
#define CMIX_UART_RX_PIN            GPIO_Pin_2  // PB2 = UART0_RX

/* ========================= Modbus协议配置 ========================= */
#define CMIX_PROTOCOL_MAX_DATA_LEN  64          // 最大数据长度
#define CMIX_PROTOCOL_MAX_FRAME_LEN 128         // 最大帧长度
#define CMIX_PROTOCOL_FRAME_HEADER  0x7E        // 帧头标识
#define CMIX_MODBUS_SLAVE_ADDRESS   1           // Modbus从站地址
#define CMIX_MODBUS_TIMEOUT_MS      1000        // Modbus超时时间

/* ========================= 比较器配置 ========================= */
#define CMIX_CMP_VIN_OVERVOLTAGE    CMP1        // Vin过压保护比较器
#define CMIX_CMP_VOUT_UNDERVOLTAGE  CMP0        // Vout欠压保护比较器
#define CMIX_CMP_OVERTEMP           CMP2        // 过温保护比较器
#define CMIX_CMP_OVERCURRENT        CMP3        // 过流保护比较器

/* CMP单元定义 */
#define CMIX_CMP1_UNIT              CMP1        // CMP1单元
#define CMIX_CMP0_UNIT              CMP0        // CMP0单元
#define CMIX_CMP2_UNIT              CMP2        // CMP2单元
#define CMIX_CMP3_UNIT              CMP3        // CMP3单元

/* CMP引脚配置 */
#define CMIX_CMP1_INPUT_PORT        GPIOA       // CMP1输入引脚端口
#define CMIX_CMP1_INPUT_PIN         GPIO_Pin_8  // PA8 = CMP1_IN
#define CMIX_CMP0_INPUT_PORT        GPIOB       // CMP0输入引脚端口  
#define CMIX_CMP0_INPUT_PIN         GPIO_Pin_4  // PB4 = CMP0_IN

/* ========================= OPA配置 ========================= */
#define CMIX_OPA_UNIT               OPA0        // OPA单元
#define CMIX_OPA_INPUT_PORT         GPIOA       // OPA输入引脚端口
#define CMIX_OPA_INPUT_PIN          GPIO_Pin_3  // PA3 = OPA0_IN

/* ========================= 保护阈值 ========================= */
#define CMIX_VIN_OVERVOLTAGE_V      60.0f       // Vin过压保护阈值60V
#define CMIX_VOUT_UNDERVOLTAGE_V    55.0f       // Vout欠压保护阈值55V
#define CMIX_CURRENT_LIMIT_A        40.0f       // 电流限制40A
#define CMIX_TEMP_LIMIT_C           85.0f       // 温度限制85°C

/* 电压电流限制 */
#define CMIX_MIN_OUTPUT_VOLTAGE     12.0f       // 最小输出电压12V
#define CMIX_MAX_OUTPUT_VOLTAGE     60.0f       // 最大输出电压60V
#define CMIX_MIN_INPUT_VOLTAGE      10.0f       // 最小输入电压10V
#define CMIX_MAX_INPUT_VOLTAGE      70.0f       // 最大输入电压70V
#define CMIX_MIN_CURRENT_LIMIT      1.0f        // 最小电流限制1A
#define CMIX_MAX_CURRENT_LIMIT      50.0f       // 最大电流限制50A
#define CMIX_MAX_INPUT_CURRENT      50.0f       // 最大输入电流50A
#define CMIX_MAX_OUTPUT_CURRENT     50.0f       // 最大输出电流50A

/* 控制算法参数 */
#define CMIX_VOLTAGE_HYSTERESIS     1.0f        // 电压滞环1V

/* 系统监控阈值 */
#define CMIX_IDLE_THRESHOLD         100         // 空闲阈值 (适合uint8_t)
#define CMIX_MAX_ERROR_COUNT        10          // 最大错误计数
#define CMIX_MAX_TEMPERATURE        80.0f       // 最大温度
#define CMIX_MAX_MEMORY_USAGE       80          // 最大内存使用率%
#define CMIX_FAULT_THRESHOLD        5           // 故障阈值计数

/* 紧急状态代码 */
#define CMIX_EMERGENCY_TOO_MANY_ERRORS     1    // 错误过多
#define CMIX_EMERGENCY_OVERTEMPERATURE     2    // 过温

/* ========================= 软启动配置 ========================= */
#define CMIX_SOFT_START_TIME_MS     1000        // 软启动时间1秒
#define CMIX_SOFT_START_TIME        1000        // 软启动时间1秒 (兼容别名)
#define CMIX_SOFT_START_STEP        1           // 软启动步长1%

/* ========================= 控制算法参数 ========================= */
#define CMIX_CONTROL_PERIOD         0.0001f     // 控制周期100μs (10kHz)
#define CMIX_VOLTAGE_PI_KP          0.5f        // 电压环P参数
#define CMIX_VOLTAGE_PI_KI          0.1f        // 电压环I参数
#define CMIX_CURRENT_PI_KP          0.3f        // 电流环P参数
#define CMIX_CURRENT_PI_KI          0.05f       // 电流环I参数

/* ========================= 看门狗配置 ========================= */
#define CMIX_IWDG_TIMEOUT_MS        2000        // 看门狗超时2秒

/* ========================= GPIO指示灯配置 ========================= */
#define CMIX_LED_RUN_PORT           GPIOA       // 运行指示灯
#define CMIX_LED_RUN_PIN            GPIO_Pin_4  // PA4
#define CMIX_LED_FAULT_PORT         GPIOA       // 故障指示灯
#define CMIX_LED_FAULT_PIN          GPIO_Pin_5  // PA5

/* GPIO兼容别名 */
#define CMIX_GPIO_LED_PORT          CMIX_LED_RUN_PORT
#define CMIX_GPIO_LED_PIN           CMIX_LED_RUN_PIN
#define CMIX_GPIO_FAULT_LED_PORT    CMIX_LED_FAULT_PORT
#define CMIX_GPIO_FAULT_LED_PIN     CMIX_LED_FAULT_PIN
#define CMIX_GPIO_RELAY_PORT        GPIOA       // 继电器控制
#define CMIX_GPIO_RELAY_PIN         GPIO_Pin_6  // PA6

/* ========================= 工具宏定义 ========================= */
#define CMIX_SET_BIT(reg, bit)      ((reg) |= (1U << (bit)))
#define CMIX_CLEAR_BIT(reg, bit)    ((reg) &= ~(1U << (bit)))
#define CMIX_READ_BIT(reg, bit)     (((reg) >> (bit)) & 1U)
#define CMIX_TOGGLE_BIT(reg, bit)   ((reg) ^= (1U << (bit)))

/* ========================= 版本信息 ========================= */
#define CMIX_VERSION_MAJOR          1
#define CMIX_VERSION_MINOR          0
#define CMIX_VERSION_PATCH          0
#define CMIX_VERSION_STRING         "CMix v1.0.0"

#ifdef __cplusplus
}
#endif

#endif /* __CMIX_CONFIG_H */