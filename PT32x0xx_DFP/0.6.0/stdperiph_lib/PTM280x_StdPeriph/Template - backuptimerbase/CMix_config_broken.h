/******************************************************************************
  * @file    CMix_config.h
  * @author  CMix Development Team
  * @version V1.0.0
  * @date    2025/09/17
  * @brief   CMix Bidirectional DCDC Controller Configuration Header
  *          Defines system configuration parameters, hardware pin assignments, and function switches
  ******************************************************************************
  */

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
    CMIX_STATE_PROTECT = 5              // 保护状态
} CMix_State_t;

typedef enum {
    CMIX_APP_STATE_INIT = 0,            // 应用初始化
    CMIX_APP_STATE_IDLE = 1,            // 应用空闲
    CMIX_APP_STATE_RUNNING = 2,         // 应用运行
    CMIX_APP_STATE_FAULT = 3,           // 应用故障
    CMIX_APP_STATE_MAINTENANCE = 4      // 应用维护
} CMix_App_State_t;

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

/* Vin ADC Configuration - 高压侧输入电压采样 (正确) */
#define CMIX_ADC_VIN_PORT           GPIOB  // PB12 = ADC0_IN0
#define CMIX_ADC_VIN_PIN            GPIO_Pin_12  // PB12引脚
#define CMIX_ADC_VIN_CHANNEL        0  // ADC0_IN0 = Channel_0

/* 相A电流采样配置 - TP181A1输出A */
#define CMIX_ADC_CURRENT_A_PORT     GPIOA          // 相A电流采样引脚
#define CMIX_ADC_CURRENT_A_PIN      GPIO_Pin_1     // PA1 = ADC0_IN1
#define CMIX_ADC_CURRENT_A_CHANNEL  1  // ADC0_IN1

/* Vout ADC Configuration - 低压侧输出电压采样 (正确) */
#define CMIX_ADC_VOUT_PORT          GPIOA
#define CMIX_ADC_VOUT_PIN           GPIO_Pin_2
#define CMIX_ADC_VOUT_CHANNEL       2

/* 相B电流采样配置 - TP181A1输出B */  
#define CMIX_ADC_CURRENT_B_PORT     GPIOA          // 相B电流采样引脚
#define CMIX_ADC_CURRENT_B_PIN      GPIO_Pin_3     // PA3 = ADC0_IN3
#define CMIX_ADC_CURRENT_B_CHANNEL  3  // ADC0_IN3
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

/* Microcontroller Selection */
#ifndef PTM280x6x7
#define PTM280x6x7
#endif

#ifndef PTM280x
#define PTM280x
#endif

#include "PT32x0xx.h"
#include <stdint.h>
#include <stdbool.h>

/* ========================= System Configuration ========================= */

/* Version Information */
#define CMIX_VERSION_MAJOR          1
#define CMIX_VERSION_MINOR          0
#define CMIX_VERSION_PATCH          0
#define CMIX_VERSION_STRING         "CMix v1.0.0"

/* System Clock Configuration */
#define CMIX_SYSTEM_CLOCK_HZ        48000000    // 48MHz system clock
#define CMIX_APB_CLOCK_HZ           48000000    // APB clock frequency

/* ========================= Type Definitions ========================= */

/* CMix Status Machine */
typedef enum {
    CMIX_STATE_INIT = 0,                // Initialization state
    CMIX_STATE_IDLE = 1,                // Idle state
    CMIX_STATE_RUNNING = 2,             // Running state
    CMIX_STATE_FAULT = 3,               // Fault state
    CMIX_STATE_SOFT_START = 4,          // Soft start state
    CMIX_STATE_EMERGENCY_STOP = 5,      // Emergency stop state
    CMIX_STATE_SHUTDOWN = 6             // Shutdown state
} CMix_State_t;

/* Working Mode Enumeration */
typedef enum {
    CMIX_MODE_IDLE = 0,                 // Idle mode
    CMIX_MODE_BUCK = 1,                 // BUCK mode
    CMIX_MODE_BOOST = 2,                // BOOST mode
    CMIX_MODE_BIDIRECTIONAL = 3,        // Bidirectional mode
    CMIX_MODE_STANDBY = 4,              // Standby mode
    CMIX_MODE_AUTO = 5                  // Auto mode
} CMix_Working_Mode_t;

/* System State Definition (for compatibility) */
typedef CMix_State_t CMix_System_State_t;

/* Mode compatibility aliases */
typedef CMix_Working_Mode_t CMix_Mode_t;

/* GPIO Pin Definitions */
#define CMIX_GPIO_LED_PORT              GPIOA
#define CMIX_GPIO_LED_PIN               GPIO_Pin_4
#define CMIX_GPIO_FAULT_LED_PORT        GPIOA  
#define CMIX_GPIO_FAULT_LED_PIN         GPIO_Pin_5
#define CMIX_GPIO_RELAY_PORT            GPIOA
#define CMIX_GPIO_RELAY_PIN             GPIO_Pin_6

/* ========================= Hardware Pin Configuration ========================= */

/* UART Communication Pins */
#define CMIX_UART_PORT              UART0
#define CMIX_UART_TX_PORT           AFIOA
#define CMIX_UART_TX_PIN            GPIO_Pin_15
#define CMIX_UART_TX_AFIO           AFIO_AF_5
#define CMIX_UART_RX_PORT           AFIOB
#define CMIX_UART_RX_PIN            GPIO_Pin_2
#define CMIX_UART_RX_AFIO           AFIO_AF_5
#define CMIX_UART_IRQn              UART0_IRQn
#define CMIX_UART_BAUDRATE          115200

/* PWM Output Pins */
#define CMIX_PWM_TIMER              TIM1
#define CMIX_PWM_TIMER_IRQn         TIM1_UP_IRQn
#define CMIX_PWM_FREQUENCY_HZ       100000      // 100kHz PWM频率
#define CMIX_PWM_PERIOD             480         // PWM period for 100kHz at 48MHz
#define CMIX_PWM_DEADTIME           10          // 死区时间 200ns at 48MHz (200ns * 48MHz = 9.6 ≈ 10)

/* 🔧 TIM1_BKIN硬件保护配置 - CMP输出直接绑定 */
#define CMIX_TIM1_BKIN_PORT         GPIOA         // 改为PA8，使用CMP1输出
#define CMIX_TIM1_BKIN_PIN          GPIO_Pin_8    // PA8作为BKIN输入
#define CMIX_TIM1_BKIN_AF           AFIO_AF_2     // 复用功能2
#define CMIX_TIM1_BKIN_TRIGGER      TIM_BreakPolarity_Low  // 低电平触发保护

/* 🔧 修正PWM引脚配置 - 符合checklist要求 */
#define CMIX_PWM_CH1_PORT           GPIOA
#define CMIX_PWM_CH1_PIN            GPIO_Pin_5        // TIM1_CH1 = PA5
#define CMIX_PWM_CH1N_PIN           GPIO_Pin_7        // TIM1_CH1N = PA7 (互补输出)
#define CMIX_PWM_CH1_AF             AFIO_AF_2

/* 保留原有BUCK/BOOST定义以保持兼容性 */
#define CMIX_PWM_BUCK_HIGH_PORT     AFIOA
#define CMIX_PWM_BUCK_HIGH_PIN      GPIO_Pin_5        // 修正为PA5
#define CMIX_PWM_BUCK_HIGH_AFIO     AFIO_AF_2
#define CMIX_PWM_BUCK_HIGH_CHANNEL  TIM_Channel_1

#define CMIX_PWM_BUCK_LOW_PORT      AFIOA  
#define CMIX_PWM_BUCK_LOW_PIN       GPIO_Pin_7        // 修正为PA7 (互补输出)
#define CMIX_PWM_BUCK_LOW_AFIO      AFIO_AF_2
#define CMIX_PWM_BUCK_LOW_CHANNEL   TIM_Channel_1     // 同一个通道的互补输出
#define CMIX_PWM_BOOST_HIGH_CHANNEL TIM_Channel_3

#define CMIX_PWM_BOOST_LOW_PORT     AFIOA
#define CMIX_PWM_BOOST_LOW_PIN      GPIO_Pin_11
#define CMIX_PWM_BOOST_LOW_AFIO     AFIO_AF_2
#define CMIX_PWM_BOOST_LOW_CHANNEL  TIM_Channel_4

/* ADC Collection Pins */
#define CMIX_ADC_UNIT               ADC0  // 修正为ADC0

/* Vin ADC Configuration - 高压侧输入电压采样 */
#define CMIX_ADC_VIN_PORT           GPIOB  // PB12 = ADC0_IN0
#define CMIX_ADC_VIN_PIN            GPIO_Pin_12  // PB12引脚
#define CMIX_ADC_VIN_CHANNEL        ADC_Channel_0  // ADC0_IN0 = Channel_0

/* 相A电流采样配置 - TP181A1输出A */
#define CMIX_ADC_CURRENT_A_PORT     GPIOA          // 相A电流采样引脚
#define CMIX_ADC_CURRENT_A_PIN      GPIO_Pin_1     // PA1 = ADC0_IN1
#define CMIX_ADC_CURRENT_A_CHANNEL  ADC_Channel_1  // ADC0_IN1

/* Vout ADC Configuration - 低压侧输出电压采样 (正确) */
#define CMIX_ADC_VOUT_PORT          GPIOA
#define CMIX_ADC_VOUT_PIN           GPIO_Pin_2
#define CMIX_ADC_VOUT_CHANNEL       ADC_Channel_2

/* 相B电流采样配置 - TP181A1输出B */  
#define CMIX_ADC_CURRENT_B_PORT     GPIOA          // 相B电流采样引脚
#define CMIX_ADC_CURRENT_B_PIN      GPIO_Pin_3     // PA3 = ADC0_IN3
#define CMIX_ADC_CURRENT_B_CHANNEL  ADC_Channel_3  // ADC0_IN3

/* TP181A1电流放大器参数 */
#define CMIX_CURRENT_SENSE_RS       0.00025f       // 分流电阻 0.25mΩ
#define CMIX_CURRENT_SENSE_GAIN     50.0f          // TP181A1增益
#define CMIX_CURRENT_VREF           2.5f           // VDDA/2 = 2.5V基准

/* 过流保护阈值ADC计数值 */
#define CMIX_CURRENT_TRIP_40A_COUNTS    2457    // 40A对应的ADC计数
#define CMIX_CURRENT_TRIP_20A_COUNTS    2252    // 20A对应的ADC计数  
#define CMIX_CURRENT_TRIP_10A_COUNTS    1638    // 10A对应的ADC计数

/* 外部比较器配置 - TP181A1过流硬切断 */
#define CMIX_EXT_COMP_BKIN_PORT         GPIOB          // 外部比较器BKIN端口
#define CMIX_EXT_COMP_BKIN_PIN          GPIO_Pin_5     // PB5外部比较器输出
#define CMIX_EXT_COMP_TRIP_VOLTAGE      3.0f           // 40A对应的比较器阈值电压(V)

/* 硬件保护说明：
 * TP181A1_OUT_A -> 外部比较器A正输入 
 * TP181A1_OUT_B -> 外部比较器B正输入
 * 比较器负输入 -> 固定阈值3.0V (对应40A)
 * 比较器输出(或门) -> PB5 -> TIM1_BKIN (低电平触发)
 */

/* ========================= OPA Configuration ========================= */

/* OPA0 电压跟随器配置 - 缓冲Vin信号 */
#define CMIX_OPA_UNIT               OPA0           // 使用OPA0
#define CMIX_OPA_INPUT_PORT         GPIOA          // 正向输入从分压网络
#define CMIX_OPA_INPUT_PIN          GPIO_Pin_3     // PA3输入(Vin分压后)
// OPA输出内部连接到ADC，无需外部GPIO
#define CMIX_OPA_MODE               OPA_MODE_FOLLOWER  // 跟随器模式

/* ========================= CMP Configuration ========================= */

/* CMP1 过压保护配置 - Vin vs 60V阈值 */
#define CMIX_CMP1_UNIT              CMP1           // 使用CMP1
#define CMIX_CMP1_INPUT_PORT        GPIOA          // 正向输入端口
#define CMIX_CMP1_INPUT_PIN         GPIO_Pin_8     // PA8正向输入
#define CMIX_CMP1_OUTPUT_PORT       GPIOA          // 输出端口  
#define CMIX_CMP1_OUTPUT_PIN        GPIO_Pin_8     // PA8输出
#define CMIX_CMP1_LDAC_THRESHOLD    3000           // LDAC阈值3.0V对应60V

/* CMP0 过压保护配置 - Vout vs 55V阈值 */
#define CMIX_CMP0_UNIT              CMP0           // 使用CMP0
#define CMIX_CMP0_INPUT_PORT        GPIOB          // 正向输入端口
#define CMIX_CMP0_INPUT_PIN         GPIO_Pin_4     // PB4正向输入
#define CMIX_CMP0_OUTPUT_PORT       GPIOB          // 输出端口
#define CMIX_CMP0_OUTPUT_PIN        GPIO_Pin_4     // PB4输出
#define CMIX_CMP0_LDAC_THRESHOLD    2750           // LDAC阈值2.75V对应55V

/* Status Indicator LED */
#define CMIX_LED_PORT               GPIOA
#define CMIX_LED_PIN                GPIO_Pin_4

/* ========================= Function Configuration ========================= */

/* Protocol Configuration */
#define CMIX_PROTOCOL_FRAME_HEADER      0xAA
#define CMIX_PROTOCOL_MAX_DATA_LEN      255
#define CMIX_PROTOCOL_MAX_FRAME_LEN     260
#define CMIX_PROTOCOL_MIN_FRAME_LEN     5

/* DCDC Control Configuration */
#define CMIX_DCDC_MAX_DUTY_CYCLE        950     // Max duty cycle 95%
#define CMIX_DCDC_MIN_DUTY_CYCLE        50      // Min duty cycle 5%
#define CMIX_DCDC_INITIAL_DUTY_CYCLE    500     // Initial duty cycle 50%
#define CMIX_DCDC_PI_KP                 10      // PI controller proportional coefficient
#define CMIX_DCDC_PI_KI                 1       // PI controller integral coefficient

/* ADC Configuration */
#define CMIX_ADC_SAMPLES_COUNT          16      // ADC average sampling count
#define CMIX_ADC_REFERENCE_VOLTAGE      3300    // ADC reference voltage (mV)
#define CMIX_ADC_RESOLUTION             4095    // 12-bit ADC resolution

/* Voltage Current Conversion Coefficients */
#define CMIX_VOLTAGE_DIVIDER_RATIO      20      // Voltage divider ratio
#define CMIX_CURRENT_SENSOR_RATIO       100     // Current sensor ratio (A/V)

/* Timer Configuration */
#define CMIX_STATUS_REPORT_INTERVAL_MS  100     // Status report interval
#define CMIX_CONTROL_LOOP_INTERVAL_US   1000    // Control loop interval 1ms

/* 安全保护参数 */
#define CMIX_DEFAULT_INPUT_VOLTAGE_MAX      100000  // 100V
#define CMIX_DEFAULT_OUTPUT_VOLTAGE_MAX     100000  // 100V
#define CMIX_DEFAULT_INPUT_CURRENT_MAX      200000  // 200A
#define CMIX_DEFAULT_OUTPUT_CURRENT_MAX     200000  // 200A
#define CMIX_DEFAULT_OUTPUT_POWER_MAX       500000  // 500W

/* 电压电流范围限制 */
#define CMIX_MIN_INPUT_VOLTAGE          10000   // 最小输入电压 10V
#define CMIX_MAX_INPUT_VOLTAGE          100000  // 最大输入电压 100V
#define CMIX_MIN_OUTPUT_VOLTAGE         5000    // 最小输出电压 5V
#define CMIX_MAX_OUTPUT_VOLTAGE         100000  // 最大输出电压 100V
#define CMIX_VOLTAGE_HYSTERESIS         1000    // 电压滞回 1V

/* 🔧 电流保护优化 - 根据实际硬件功率等级调整 */
#define CMIX_MIN_CURRENT_LIMIT          1000    // 最小电流限制 1A
#define CMIX_MAX_INPUT_CURRENT          10000   // 最大输入电流 10A (小功率应用)
#define CMIX_MAX_OUTPUT_CURRENT         20000   // 最大输出电流 20A (小功率应用)
#define CMIX_MAX_CURRENT_LIMIT          20000   // Max current limit 20A
#define CMIX_MAX_OUTPUT_POWER           500000  // Max output power 500W

/* 电流传感器配置 */
#define CMIX_CURRENT_SENSE_VIN_V_PER_A  0.1f    // V/A - 输入电流传感器灵敏度
#define CMIX_CURRENT_SENSE_VOUT_V_PER_A 0.05f   // V/A - 输出电流传感器灵敏度
#define CMIX_CURRENT_PROTECTION_DELAY   10      // ms - 电流保护延时，避免浪涌误触发

/* PI Controller Parameters */
#define CMIX_VOLTAGE_PI_KP              0.5f    // Voltage loop proportional coefficient
#define CMIX_VOLTAGE_PI_KI              0.01f   // Voltage loop integral coefficient
#define CMIX_CURRENT_PI_KP              1.0f    // Current loop proportional coefficient
#define CMIX_CURRENT_PI_KI              0.02f   // Current loop integral coefficient
#define CMIX_CONTROL_PERIOD             0.001f  // Control period 1ms

/* System Protection Parameters */
#define CMIX_FAULT_THRESHOLD            5       // Fault detection threshold (consecutive times)
#define CMIX_SOFT_START_TIME            1000    // Soft start time 1000ms
#define CMIX_IDLE_THRESHOLD             80      // Idle threshold 80%
#define CMIX_MAX_TEMPERATURE            85      // Max temperature 85°C
#define CMIX_MAX_MEMORY_USAGE           90      // Max memory usage 90%
#define CMIX_MAX_ERROR_COUNT            100     // Max error count

/* Emergency Code Definitions */
#define CMIX_EMERGENCY_OVERTEMPERATURE  0x01    // Overtemperature emergency stop
#define CMIX_EMERGENCY_TOO_MANY_ERRORS  0x02    // Too many errors emergency stop

#define CMIX_OVERVOLTAGE_PROTECTION_RATIO   120     // Overvoltage protection ratio 120%
#define CMIX_OVERCURRENT_PROTECTION_RATIO   110     // Overcurrent protection ratio 110%

/* Debug Configuration */
#define CMIX_DEBUG_ENABLE               1       // Enable debug output
#define CMIX_DEBUG_UART_ENABLE          1       // Enable UART debug
#define CMIX_DEBUG_PRINT_STATUS         1       // Enable status printing

/* ========================= Error Code Definitions ========================= */

/* CMix Error Codes */
typedef enum {
    CMIX_ERROR_NONE = 0,                // No error
    CMIX_ERROR_OVERVOLTAGE = 1,         // Overvoltage
    CMIX_ERROR_OVERCURRENT = 2,         // Overcurrent
    CMIX_ERROR_OVERPOWER = 3,           // Overpower
    CMIX_ERROR_OVERTEMPERATURE = 4,     // Overtemperature
    CMIX_ERROR_COMMUNICATION = 5,       // Communication error
    CMIX_ERROR_HARDWARE = 6             // Hardware error
} CMix_Error_t;

/* Common Macro Definitions */
#define CMIX_MAX(a, b)              ((a) > (b) ? (a) : (b))
#define CMIX_MIN(a, b)              ((a) < (b) ? (a) : (b))
#define CMIX_CLAMP(val, min, max)   CMIX_MIN(CMIX_MAX(val, min), max)
#define CMIX_ARRAY_SIZE(arr)        (sizeof(arr) / sizeof((arr)[0]))

/* Bit Operation Macros */
#define CMIX_SET_BIT(reg, bit)      ((reg) |= (1U << (bit)))
#define CMIX_CLEAR_BIT(reg, bit)    ((reg) &= ~(1U << (bit)))
#define CMIX_READ_BIT(reg, bit)     (((reg) >> (bit)) & 1U)
#define CMIX_TOGGLE_BIT(reg, bit)   ((reg) ^= (1U << (bit)))

#ifdef __cplusplus
}
#endif

#endif /* __CMIX_CONFIG_H */