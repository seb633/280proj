/******************************************************************************
  * @file    CMix_hardware.h
  * @author  CMix Development Team
  * @version V1.0.0
  * @date    2025/09/17
  * @brief   CMix双向DCDC控制器硬件抽象层头文件
  *          定义硬件初始化和底层驱动接口
  ******************************************************************************
  * @attention
  *
  * CMix硬件抽象层(HAL)模块
  * 负责硬件初始化、GPIO、UART、TIM、ADC等底层驱动
  *
  * Copyright (C) 2025, CMix Team, all rights reserved
  *
  *****************************************************************************/

#ifndef __CMIX_HARDWARE_H
#define __CMIX_HARDWARE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "CMix_config.h"
#include <math.h>  // 支持fabs函数

/* ========================= 硬件初始化 ========================= */

/* 系统初始化 */
void CMix_Hardware_Init(void);
void CMix_Hardware_RCC_Init(void);
void CMix_Hardware_GPIO_Init(void);

/* UART硬件初始化 */
void CMix_Hardware_UART_Init(void);
void CMix_Hardware_UART_Send_Byte(uint8_t byte);
bool CMix_Hardware_UART_Is_TX_Ready(void);

/* TIM硬件初始化 */
void CMix_Hardware_TIM_Init(void);
void CMix_Hardware_Set_PWM_Duty(uint8_t channel, uint16_t duty_cycle);
void CMix_Hardware_TIM_Enable_PWM(bool enable);

/* ADC硬件初始化 */
void CMix_Hardware_ADC_Init(void);
uint16_t CMix_Hardware_ADC_Read(uint8_t channel);
void CMix_Hardware_ADC_Start_Conversion(void);
bool CMix_Hardware_ADC_Is_Conversion_Done(void);
uint16_t CMix_Hardware_ADC_Get_Value(uint8_t channel);

/* OPA硬件初始化 */
void CMix_Hardware_OPA_Init(void);

/* CMP硬件初始化 */
void CMix_Hardware_CMP_Init(void);

/* GPIO控制 */
void CMix_Hardware_GPIO_Write(GPIO_TypeDef *port, uint16_t pin, uint8_t state);
uint8_t CMix_Hardware_GPIO_Read(GPIO_TypeDef *port, uint16_t pin);
void CMix_Hardware_LED_On(void);
void CMix_Hardware_LED_Off(void);
void CMix_Hardware_LED_Toggle(void);

/* 中断控制 */
void CMix_Hardware_Enable_Interrupts(void);
void CMix_Hardware_Disable_Interrupts(void);

/* 延时和时间 */
void CMix_Hardware_Delay_us(uint32_t us);
void CMix_Hardware_Delay_ms(uint32_t ms);
uint32_t CMix_Hardware_Get_Tick(void);

/* 看门狗 */
void CMix_Hardware_Watchdog_Init(void);
void CMix_Hardware_Watchdog_Feed(void);

/* 电源管理 */
void CMix_Hardware_Enter_Sleep_Mode(void);
void CMix_Hardware_Enter_Stop_Mode(void);

/* ========================= 数据结构定义 ========================= */

/* 电压传感器数据结构 */
typedef struct {
    uint16_t input_voltage;             // 输入电压ADC值
    uint16_t output_voltage;            // 输出电压ADC值
} CMix_Voltage_Sensors_t;

/* 电流传感器数据结构 */
typedef struct {
    float input_current;                // 相A电流值 (A)
    float output_current;               // 相B电流值 (A)
} CMix_Current_Sensors_t;

/* ========================= 硬件状态查询 ========================= */

/* 传感器读取 */
CMix_Voltage_Sensors_t CMix_Hardware_Get_Voltage_Sensors(void);
CMix_Current_Sensors_t CMix_Hardware_Get_Current_Sensors(void);

/* TP181A1电流换算 */
float CMix_Hardware_Convert_Current(uint16_t adc_raw);
float CMix_Hardware_Calibrate_Current(char phase, float raw_current, float offset_correction);
float CMix_Hardware_Get_Current_A(void);
float CMix_Hardware_Get_Current_B(void);
uint8_t CMix_Hardware_Check_Overcurrent(float current_a, float current_b, float trip_threshold);

/* 系统状态 */
bool CMix_Hardware_Is_System_Ready(void);
uint32_t CMix_Hardware_Get_System_Clock(void);
float CMix_Hardware_Get_MCU_Temperature(void);
void CMix_Hardware_System_Reset(void);
void CMix_Hardware_Delay_ms(uint32_t ms);
void CMix_Hardware_Watchdog_Feed(void);
uint8_t CMix_Hardware_Self_Test(void);

/* ADC状态 */
bool CMix_Hardware_ADC_Is_Ready(void);
uint16_t CMix_Hardware_ADC_Get_Reference_Voltage(void);

/* PWM状态 */
bool CMix_Hardware_PWM_Is_Enabled(void);
uint16_t CMix_Hardware_PWM_Get_Frequency(void);

/* ========================= 中断回调函数 ========================= */

/* UART中断回调 */
void CMix_Hardware_UART_RX_Callback(uint8_t data);
void CMix_Hardware_UART_TX_Callback(void);

/* TIM中断回调 */
void CMix_Hardware_TIM_Update_Callback(void);
void CMix_Hardware_TIM_PWM_Callback(void);

/* ADC中断回调 */
void CMix_Hardware_ADC_Conversion_Complete_Callback(void);

/* GPIO中断回调 */
void CMix_Hardware_GPIO_EXTI_Callback(uint16_t pin);

/* ========================= 调试函数 ========================= */

#if CMIX_DEBUG_ENABLE
/* 系统信息打印 (仅在UART初始化后调用) */
void CMix_Hardware_Debug_PrintSystemInfo(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __CMIX_HARDWARE_H */