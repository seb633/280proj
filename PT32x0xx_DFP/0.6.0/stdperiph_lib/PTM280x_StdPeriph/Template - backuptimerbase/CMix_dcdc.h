/******************************************************************************
  * @file    CMix_dcdc.h
  * @author  CMix Development Team
  * @version V1.0.0
  * @date    2025/09/17
  * @brief   CMix双向DCDC控制器控制算法头文件
  *          定义DCDC控制相关的数据结构和函数接口
  ******************************************************************************
  * @attention
  *
  * CMix DCDC控制模块
  * 负责双向DC-DC转换的控制算法和PWM输出
  *
  * Copyright (C) 2025, CMix Team, all rights reserved
  *
  *****************************************************************************/

#ifndef __CMIX_DCDC_H
#define __CMIX_DCDC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "CMix_config.h"

/* ========================= DCDC控制数据结构 ========================= */

/* PI控制器结构体 */
typedef struct {
    float kp;                               // 比例系数
    float ki;                               // 积分系数
    float integral;                         // 积分累计值
    float output;                           // 控制输出
    float output_max;                       // 输出上限
    float output_min;                       // 输出下限
    float error_prev;                       // 上次误差
    float last_error;                       // 最后一次误差 (兼容性)
} CMix_PI_Controller_t;

/* DCDC控制参数结构体 */
typedef struct {
    uint16_t buck_duty_cycle;               // BUCK模式占空比 (0-1000)
    uint16_t boost_duty_cycle;              // BOOST模式占空比 (0-1000)
    uint32_t voltage_setpoint;              // 电压设定值 (mV)
    uint16_t current_limit;                 // 电流限制 (mA)
    uint8_t enable;                         // 控制使能标志
    CMix_Mode_t current_mode;               // 当前工作模式
    CMix_State_t system_state;              // 系统状态
    CMix_Error_t error_code;                // 错误码
    bool safety_shutdown;                   // 安全关机标志
} CMix_DCDC_Control_t;

/* DCDC状态结构体 */
typedef struct {
    uint32_t input_voltage;                 // 输入电压 (mV)
    uint32_t input_current;                 // 输入电流 (mA)
    uint32_t output_voltage;                // 输出电压 (mV)
    uint32_t output_current;                // 输出电流 (mA)
    uint32_t output_power;                  // 输出功率 (mW)
    uint16_t pwm_duty_buck;                 // BUCK PWM占空比
    uint16_t pwm_duty_boost;                // BOOST PWM占空比
    uint8_t efficiency;                     // 效率百分比
    CMix_Working_Mode_t mode;               // 工作模式
    CMix_Working_Mode_t active_mode;        // 当前激活模式
    CMix_System_State_t state;              // 系统状态
} CMix_DCDC_Status_t;

/* 安全监控结构体 */
typedef struct {
    uint16_t overvoltage_count;             // 过压计数
    uint16_t overcurrent_count;             // 过流计数
    uint16_t overtemperature_count;         // 过温计数
    uint8_t fault_flags;                    // 故障标志位
} CMix_Safety_Monitor_t;

/* DCDC测量值结构体 */
typedef struct {
    uint16_t vin_raw;                       // 输入电压原始ADC值
    uint16_t iin_raw;                       // 输入电流原始ADC值
    uint16_t vout_raw;                      // 输出电压原始ADC值
    uint16_t iout_raw;                      // 输出电流原始ADC值
    
    uint16_t vin_filtered;                  // 输入电压滤波值 (mV)
    uint16_t iin_filtered;                  // 输入电流滤波值 (mA)
    uint16_t vout_filtered;                 // 输出电压滤波值 (mV)
    uint16_t iout_filtered;                 // 输出电流滤波值 (mA)
    uint16_t power_calculated;              // 计算的输出功率 (mW)
    
    uint32_t sample_count;                  // 采样计数
    bool adc_ready;                         // ADC数据就绪标志
} CMix_DCDC_Measurements_t;

/* ========================= 函数声明 ========================= */

/* DCDC初始化和配置 */
void CMix_DCDC_Init(void);
void CMix_DCDC_Set_Mode(CMix_Mode_t mode);
void CMix_DCDC_Enable_Control(bool enable);
void CMix_DCDC_Enable(uint8_t enable);
void CMix_DCDC_Set_Output_Voltage(uint32_t voltage);
void CMix_DCDC_Set_Current_Limit(uint32_t current);

/* DCDC控制算法 */
void CMix_DCDC_Control_Loop(void);
void CMix_DCDC_Control_Task(void);
void CMix_DCDC_State_Machine(void);
void CMix_DCDC_Mode_Auto_Switch(void);
void CMix_DCDC_Update_PWM_Output(void);
void CMix_DCDC_Calculate_Efficiency(void);

/* DCDC模式切换 */
void CMix_DCDC_Set_Mode(CMix_Working_Mode_t mode);

/* 安全保护功能 */
void CMix_DCDC_Safety_Check(void);
void CMix_DCDC_Emergency_Shutdown(void);
void CMix_DCDC_Emergency_Stop(uint8_t emergency_code);
bool CMix_DCDC_Is_Safe_To_Operate(void);

/* PI控制器 */
void CMix_DCDC_PI_Init(CMix_PI_Controller_t *pi, float kp, float ki, float output_min, float output_max);
float CMix_DCDC_PI_Update(CMix_PI_Controller_t *pi, float setpoint, float feedback);
void CMix_DCDC_PI_Reset(CMix_PI_Controller_t *pi);

/* DCDC状态和参数 */
CMix_DCDC_Control_t* CMix_DCDC_Get_Control_Status(void);
CMix_DCDC_Measurements_t* CMix_DCDC_Get_Measurements(void);
CMix_DCDC_Status_t* CMix_DCDC_Get_Status(void);
CMix_Safety_Monitor_t* CMix_DCDC_Get_Safety_Status(void);
void CMix_DCDC_Set_Target_Voltage(uint32_t voltage_mv);
void CMix_DCDC_Set_Target_Current(uint16_t current_ma);

/* 安全保护功能 */
void CMix_DCDC_Safety_Check(void);
void CMix_DCDC_Emergency_Shutdown(void);
bool CMix_DCDC_Is_Safe_To_Operate(void);

/* 调试和监控 */
void CMix_DCDC_Print_Status(void);
void CMix_DCDC_Print_Measurements(void);
void CMix_DCDC_Debug_Print(void);

#ifdef __cplusplus
}
#endif

#endif /* __CMIX_DCDC_H */