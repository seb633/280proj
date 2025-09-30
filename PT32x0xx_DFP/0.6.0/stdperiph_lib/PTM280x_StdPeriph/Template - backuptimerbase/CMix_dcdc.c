/******************************************************************************
  * @file    CMix_dcdc.c
  * @author  CMix Development Team
  * @version V1.0.0
  * @date    2025/09/17
  * @brief   CMix双向DCDC控制器DCDC控制算法实现文件
  *          实现BUCK/BOOST转换器控制、PI调节、安全保护等功能
  ******************************************************************************
  * @attention
  *
  * CMix DCDC控制模块实现
  * 负责双向DCDC变换器的控制算法和安全保护
  *
  * Copyright (C) 2025, CMix Team, all rights reserved
  *
  *****************************************************************************/

#include "CMix_dcdc.h"
#include "CMix_hardware.h"
#include "CMix_protocol.h"
#include <math.h>
#include <stdio.h>  // 支持sprintf函数

/* ========================= 私有变量 ========================= */

static CMix_PI_Controller_t g_voltage_pi;
static CMix_PI_Controller_t g_current_pi;
static CMix_DCDC_Status_t g_dcdc_status = {0};
static CMix_DCDC_Control_t g_dcdc_control = {0};
static CMix_Safety_Monitor_t g_safety_monitor = {0};

/* ========================= 私有函数声明 ========================= */

static void CMix_DCDC_PI_Controller_Init(CMix_PI_Controller_t *pi, float kp, float ki, float min, float max);
static float CMix_DCDC_PI_Controller_Update(CMix_PI_Controller_t *pi, float setpoint, float feedback);
static void CMix_DCDC_Update_Measurements(void);
static void CMix_DCDC_Mode_Selection(void);
static void CMix_DCDC_PWM_Update(void);
static uint16_t CMix_DCDC_Convert_Voltage(uint16_t adc_value);
static uint16_t CMix_DCDC_Convert_Current(uint16_t adc_value);

/* ========================= 公共函数实现 ========================= */

/**
 * @brief CMix DCDC控制初始化
 * @param None
 * @retval None
 */
void CMix_DCDC_Init(void)
{
    /* 初始化PI控制器 */
    CMix_DCDC_PI_Controller_Init(&g_voltage_pi, CMIX_VOLTAGE_PI_KP, CMIX_VOLTAGE_PI_KI, 
                                 0.0f, 10000.0f);
    CMix_DCDC_PI_Controller_Init(&g_current_pi, CMIX_CURRENT_PI_KP, CMIX_CURRENT_PI_KI, 
                                 0.0f, 10000.0f);

    /* 初始化DCDC状态 */
    g_dcdc_status.mode = CMIX_MODE_AUTO;
    g_dcdc_status.state = CMIX_STATE_INIT;
    g_dcdc_status.pwm_duty_buck = 0;
    g_dcdc_status.pwm_duty_boost = 0;
    g_dcdc_status.efficiency = 0;

    /* 初始化控制参数 */
    g_dcdc_control.voltage_setpoint = 24000;  /* 24V */
    g_dcdc_control.current_limit = 10000;     /* 10A */
    g_dcdc_control.enable = 0;

    /* 初始化安全监控 */
    g_safety_monitor.overvoltage_count = 0;
    g_safety_monitor.overcurrent_count = 0;
    g_safety_monitor.overtemperature_count = 0;
    g_safety_monitor.fault_flags = 0;

    /* 设置初始PWM为0 */
    CMix_Hardware_Set_PWM_Duty(0, 0);  /* BUCK上管 */
    CMix_Hardware_Set_PWM_Duty(1, 0);  /* BUCK下管 */
    CMix_Hardware_Set_PWM_Duty(2, 0);  /* BOOST上管 */
    CMix_Hardware_Set_PWM_Duty(3, 0);  /* BOOST下管 */
}

/**
 * @brief CMix DCDC主控制任务
 * @param None
 * @retval None
 */
void CMix_DCDC_Control_Task(void)
{
    /* 更新测量值 */
    CMix_DCDC_Update_Measurements();

    /* 安全检查 */
    CMix_DCDC_Safety_Check();

    /* 模式选择 */
    CMix_DCDC_Mode_Selection();

    /* PWM更新 */
    CMix_DCDC_PWM_Update();

    /* 更新效率计算 */
    CMix_DCDC_Calculate_Efficiency();

    /* 更新协议状态 */
    CMix_System_Status_t *system_status = CMix_Protocol_Get_System_Status();
    system_status->input_voltage = g_dcdc_status.input_voltage;
    system_status->input_current = g_dcdc_status.input_current;
    system_status->output_voltage = g_dcdc_status.output_voltage;
    system_status->output_current = g_dcdc_status.output_current;
    system_status->output_power = g_dcdc_status.output_power;
    system_status->working_mode = g_dcdc_status.mode;
    system_status->system_state = g_dcdc_status.state;
}

/**
 * @brief CMix设置DCDC输出电压
 * @param voltage: 目标电压 (mV)
 * @retval None
 */
void CMix_DCDC_Set_Output_Voltage(uint32_t voltage)
{
    if (voltage >= CMIX_MIN_OUTPUT_VOLTAGE && voltage <= CMIX_MAX_OUTPUT_VOLTAGE) {
        g_dcdc_control.voltage_setpoint = voltage;
    }
}

/**
 * @brief CMix设置DCDC电流限制
 * @param current: 电流限制 (mA)
 * @retval None
 */
void CMix_DCDC_Set_Current_Limit(uint32_t current)
{
    if (current >= CMIX_MIN_CURRENT_LIMIT && current <= CMIX_MAX_CURRENT_LIMIT) {
        g_dcdc_control.current_limit = current;
    }
}

/**
 * @brief CMix使能/禁用DCDC
 * @param enable: 1-使能, 0-禁用
 * @retval None
 */
void CMix_DCDC_Enable(uint8_t enable)
{
    g_dcdc_control.enable = enable;
    
    if (!enable) {
        /* 禁用时关闭所有PWM */
        CMix_Hardware_Set_PWM_Duty(1, 0);
        CMix_Hardware_Set_PWM_Duty(2, 0);
        CMix_Hardware_Set_PWM_Duty(3, 0);
        CMix_Hardware_Set_PWM_Duty(4, 0);
        
        g_dcdc_status.state = CMIX_STATE_IDLE;
    }
}

/**
 * @brief CMix设置DCDC工作模式
 * @param mode: 工作模式
 * @retval None
 */
void CMix_DCDC_Set_Mode(CMix_Working_Mode_t mode)
{
    if (mode <= CMIX_MODE_AUTO) {
        g_dcdc_status.mode = mode;
    }
}

/**
 * @brief CMix获取DCDC状态
 * @param None
 * @retval DCDC状态指针
 */
CMix_DCDC_Status_t* CMix_DCDC_Get_Status(void)
{
    return &g_dcdc_status;
}

/**
 * @brief CMix获取安全监控状态
 * @param None
 * @retval 安全监控状态指针
 */
CMix_Safety_Monitor_t* CMix_DCDC_Get_Safety_Status(void)
{
    return &g_safety_monitor;
}

/**
 * @brief CMix计算DCDC效率
 * @param None
 * @retval None
 */
void CMix_DCDC_Calculate_Efficiency(void)
{
    uint32_t input_power = (uint32_t)g_dcdc_status.input_voltage * g_dcdc_status.input_current / 1000;
    uint32_t output_power = (uint32_t)g_dcdc_status.output_voltage * g_dcdc_status.output_current / 1000;
    
    if (input_power > 100) {  /* 避免除零 */
        g_dcdc_status.efficiency = (uint8_t)((output_power * 100) / input_power);
        if (g_dcdc_status.efficiency > 100) {
            g_dcdc_status.efficiency = 100;  /* 限制最大效率 */
        }
    } else {
        g_dcdc_status.efficiency = 0;
    }
    
    g_dcdc_status.output_power = (uint16_t)output_power;
}

/**
 * @brief CMix紧急停机
 * @param fault_code: 故障代码
 * @retval None
 */
void CMix_DCDC_Emergency_Stop(uint8_t fault_code)
{
    /* 立即关闭所有PWM */
    CMix_Hardware_Set_PWM_Duty(1, 0);
    CMix_Hardware_Set_PWM_Duty(2, 0);
    CMix_Hardware_Set_PWM_Duty(3, 0);
    CMix_Hardware_Set_PWM_Duty(4, 0);
    
    /* 设置故障状态 */
    g_dcdc_status.state = CMIX_STATE_FAULT;
    g_safety_monitor.fault_flags |= fault_code;
    
    /* 点亮故障LED */
    CMix_Hardware_GPIO_Write(CMIX_GPIO_FAULT_LED_PORT, CMIX_GPIO_FAULT_LED_PIN, 1);
    
    /* 禁用DCDC */
    g_dcdc_control.enable = 0;
}

/**
 * @brief CMix软启动
 * @param None
 * @retval None
 */
void CMix_DCDC_Soft_Start(void)
{
    /* 实现软启动逻辑 */
    if (g_dcdc_status.state == CMIX_STATE_IDLE && g_dcdc_control.enable) {
        g_dcdc_status.state = CMIX_STATE_SOFT_START;
        
        /* 重置PI控制器 */
        g_voltage_pi.integral = 0.0f;
        g_current_pi.integral = 0.0f;
    }
}

/**
 * @brief CMix DCDC复位故障
 * @param None
 * @retval None
 */
void CMix_DCDC_Reset_Fault(void)
{
    /* 清除故障标志 */
    g_safety_monitor.fault_flags = 0;
    g_safety_monitor.overvoltage_count = 0;
    g_safety_monitor.overcurrent_count = 0;
    g_safety_monitor.overtemperature_count = 0;
    
    /* 关闭故障LED */
    CMix_Hardware_GPIO_Write(CMIX_GPIO_FAULT_LED_PORT, CMIX_GPIO_FAULT_LED_PIN, 0);
    
    /* 如果DCDC使能，进入软启动 */
    if (g_dcdc_control.enable) {
        g_dcdc_status.state = CMIX_STATE_IDLE;
        CMix_DCDC_Soft_Start();
    }
}

/* ========================= 私有函数实现 ========================= */

/**
 * @brief PI控制器初始化
 * @param pi: PI控制器指针
 * @param kp: 比例系数
 * @param ki: 积分系数
 * @param min: 输出最小值
 * @param max: 输出最大值
 * @retval None
 */
static void CMix_DCDC_PI_Controller_Init(CMix_PI_Controller_t *pi, float kp, float ki, float min, float max)
{
    pi->kp = kp;
    pi->ki = ki;
    pi->integral = 0.0f;
    pi->output_min = min;
    pi->output_max = max;
    pi->last_error = 0.0f;
}

/**
 * @brief PI控制器更新
 * @param pi: PI控制器指针
 * @param setpoint: 设定值
 * @param feedback: 反馈值
 * @retval 控制输出
 */
static float CMix_DCDC_PI_Controller_Update(CMix_PI_Controller_t *pi, float setpoint, float feedback)
{
    float error = setpoint - feedback;
    float output;
    
    /* 比例项 */
    float proportional = pi->kp * error;
    
    /* 积分项 */
    pi->integral += pi->ki * error * CMIX_CONTROL_PERIOD;
    
    /* 积分限幅 */
    if (pi->integral > pi->output_max) {
        pi->integral = pi->output_max;
    } else if (pi->integral < pi->output_min) {
        pi->integral = pi->output_min;
    }
    
    /* 计算输出 */
    output = proportional + pi->integral;
    
    /* 输出限幅 */
    if (output > pi->output_max) {
        output = pi->output_max;
    } else if (output < pi->output_min) {
        output = pi->output_min;
    }
    
    pi->last_error = error;
    
    return output;
}

/**
 * @brief 更新测量值
 * @param None
 * @retval None
 */
static void CMix_DCDC_Update_Measurements(void)
{
    CMix_Voltage_Sensors_t voltage_sensors = CMix_Hardware_Get_Voltage_Sensors();
    CMix_Current_Sensors_t current_sensors = CMix_Hardware_Get_Current_Sensors();
    
    /* 转换ADC值为实际物理量 */
    g_dcdc_status.input_voltage = CMix_DCDC_Convert_Voltage(voltage_sensors.input_voltage);
    g_dcdc_status.output_voltage = CMix_DCDC_Convert_Voltage(voltage_sensors.output_voltage);
    g_dcdc_status.input_current = CMix_DCDC_Convert_Current(current_sensors.input_current);
    g_dcdc_status.output_current = CMix_DCDC_Convert_Current(current_sensors.output_current);
}

/**
 * @brief 安全检查
 * @param None
 * @retval None
 */
void CMix_DCDC_Safety_Check(void)
{
    uint8_t fault_detected = 0;
    
    /* 过压保护 */
    if (g_dcdc_status.input_voltage > CMIX_MAX_INPUT_VOLTAGE || 
        g_dcdc_status.output_voltage > CMIX_MAX_OUTPUT_VOLTAGE) {
        g_safety_monitor.overvoltage_count++;
        if (g_safety_monitor.overvoltage_count > CMIX_FAULT_THRESHOLD) {
            fault_detected = 1;
            g_safety_monitor.fault_flags |= CMIX_ERROR_OVERVOLTAGE;
        }
    } else {
        g_safety_monitor.overvoltage_count = 0;
    }
    
    /* 过流保护 */
    if (g_dcdc_status.input_current > CMIX_MAX_INPUT_CURRENT || 
        g_dcdc_status.output_current > CMIX_MAX_OUTPUT_CURRENT) {
        g_safety_monitor.overcurrent_count++;
        if (g_safety_monitor.overcurrent_count > CMIX_FAULT_THRESHOLD) {
            fault_detected = 1;
            g_safety_monitor.fault_flags |= CMIX_ERROR_OVERCURRENT;
        }
    } else {
        g_safety_monitor.overcurrent_count = 0;
    }
    
    /* 欠压保护 */
    if (g_dcdc_status.input_voltage < CMIX_MIN_INPUT_VOLTAGE) {
        fault_detected = 1;
        g_safety_monitor.fault_flags |= CMIX_ERROR_HARDWARE;
    }
    
    /* 如果检测到故障，触发紧急停机 */
    if (fault_detected) {
        CMix_DCDC_Emergency_Stop(g_safety_monitor.fault_flags);
    }
}

/**
 * @brief 模式选择
 * @param None
 * @retval None
 */
static void CMix_DCDC_Mode_Selection(void)
{
    if (g_dcdc_status.mode == CMIX_MODE_AUTO) {
        /* 自动模式：根据输入输出电压自动选择BUCK或BOOST */
        if (g_dcdc_status.input_voltage > g_dcdc_control.voltage_setpoint + CMIX_VOLTAGE_HYSTERESIS) {
            g_dcdc_status.active_mode = CMIX_MODE_BUCK;
        } else if (g_dcdc_status.input_voltage < g_dcdc_control.voltage_setpoint - CMIX_VOLTAGE_HYSTERESIS) {
            g_dcdc_status.active_mode = CMIX_MODE_BOOST;
        }
        /* 在滞回区间内保持当前模式 */
    } else {
        /* 手动模式 */
        g_dcdc_status.active_mode = g_dcdc_status.mode;
    }
}

/**
 * @brief PWM更新
 * @param None
 * @retval None
 */
static void CMix_DCDC_PWM_Update(void)
{
    float voltage_error, current_error;
    float voltage_output, current_output;
    uint16_t pwm_duty = 0;
    
    if (!g_dcdc_control.enable || g_dcdc_status.state == CMIX_STATE_FAULT) {
        /* 禁用状态，关闭所有PWM */
        CMix_Hardware_Set_PWM_Duty(1, 0);
        CMix_Hardware_Set_PWM_Duty(2, 0);
        CMix_Hardware_Set_PWM_Duty(3, 0);
        CMix_Hardware_Set_PWM_Duty(4, 0);
        return;
    }
    
    /* 电压环控制 */
    voltage_output = CMix_DCDC_PI_Controller_Update(&g_voltage_pi, 
                                                    (float)g_dcdc_control.voltage_setpoint,
                                                    (float)g_dcdc_status.output_voltage);
    
    /* 电流环控制 */
    current_output = CMix_DCDC_PI_Controller_Update(&g_current_pi, 
                                                    (float)g_dcdc_control.current_limit,
                                                    (float)g_dcdc_status.output_current);
    
    /* 取电压环和电流环输出的最小值 */
    pwm_duty = (uint16_t)(voltage_output < current_output ? voltage_output : current_output);
    
    /* 软启动处理 */
    if (g_dcdc_status.state == CMIX_STATE_SOFT_START) {
        static uint16_t soft_start_counter = 0;
        uint16_t max_duty = (soft_start_counter * 10000) / CMIX_SOFT_START_TIME;
        
        if (pwm_duty > max_duty) {
            pwm_duty = max_duty;
        }
        
        soft_start_counter++;
        if (soft_start_counter >= CMIX_SOFT_START_TIME) {
            g_dcdc_status.state = CMIX_STATE_RUNNING;
            soft_start_counter = 0;
        }
    }
    
    /* 根据模式设置PWM */
    if (g_dcdc_status.active_mode == CMIX_MODE_BUCK) {
        /* BUCK模式 */
        g_dcdc_status.pwm_duty_buck = pwm_duty;
        g_dcdc_status.pwm_duty_boost = 0;
        
        CMix_Hardware_Set_PWM_Duty(1, pwm_duty);           /* BUCK上管 */
        CMix_Hardware_Set_PWM_Duty(2, 10000 - pwm_duty);   /* BUCK下管 */
        CMix_Hardware_Set_PWM_Duty(3, 0);                  /* BOOST上管关闭 */
        CMix_Hardware_Set_PWM_Duty(4, 10000);              /* BOOST下管常开 */
        
    } else if (g_dcdc_status.active_mode == CMIX_MODE_BOOST) {
        /* BOOST模式 */
        g_dcdc_status.pwm_duty_buck = 0;
        g_dcdc_status.pwm_duty_boost = pwm_duty;
        
        CMix_Hardware_Set_PWM_Duty(1, 10000);              /* BUCK上管常开 */
        CMix_Hardware_Set_PWM_Duty(2, 0);                  /* BUCK下管关闭 */
        CMix_Hardware_Set_PWM_Duty(3, pwm_duty);           /* BOOST上管 */
        CMix_Hardware_Set_PWM_Duty(4, 10000 - pwm_duty);   /* BOOST下管 */
    }
}

/**
 * @brief 电压ADC值转换
 * @param adc_value: ADC原始值
 * @retval 电压值 (mV)
 */
static uint16_t CMix_DCDC_Convert_Voltage(uint16_t adc_value)
{
    /* 假设ADC参考电压3.3V，12位ADC，电压分压比1:20 */
    uint32_t voltage_mv = (adc_value * 3300 * 20) / 4095;
    return (uint16_t)voltage_mv;
}

/**
 * @brief 电流ADC值转换
 * @param adc_value: ADC原始值
 * @retval 电流值 (mA)
 */
static uint16_t CMix_DCDC_Convert_Current(uint16_t adc_value)
{
    /* 假设电流传感器灵敏度10mV/A，偏置电压1.65V */
    int32_t current_ma = ((int32_t)adc_value * 3300 / 4095 - 1650) * 100;
    if (current_ma < 0) current_ma = 0;
    return (uint16_t)current_ma;
}

/**
 * @brief CMix DCDC状态机处理
 * @param None
 * @retval None
 */
void CMix_DCDC_State_Machine(void)
{
    static uint32_t state_timer = 0;
    
    switch (g_dcdc_status.state) {
        case CMIX_STATE_INIT:
            /* 初始化状态 */
            if (g_dcdc_control.enable) {
                g_dcdc_status.state = CMIX_STATE_IDLE;
            }
            break;
            
        case CMIX_STATE_IDLE:
            /* 空闲状态 */
            if (g_dcdc_control.enable) {
                CMix_DCDC_Soft_Start();
            }
            break;
            
        case CMIX_STATE_SOFT_START:
            /* 软启动状态 */
            state_timer++;
            if (state_timer >= CMIX_SOFT_START_TIME) {
                g_dcdc_status.state = CMIX_STATE_RUNNING;
                state_timer = 0;
            }
            break;
            
        case CMIX_STATE_RUNNING:
            /* 正常运行状态 */
            if (!g_dcdc_control.enable) {
                g_dcdc_status.state = CMIX_STATE_IDLE;
            }
            break;
            
        case CMIX_STATE_FAULT:
            /* 故障状态 */
            /* 需要手动复位故障才能退出 */
            break;
            
        default:
            g_dcdc_status.state = CMIX_STATE_INIT;
            break;
    }
}

/**
 * @brief CMix DCDC参数调试输出
 * @param None
 * @retval None
 */
void CMix_DCDC_Debug_Print(void)
{
    #if CMIX_DEBUG_ENABLE
    char msg_buffer[128];
    
    CMix_Protocol_Send_Debug_Message("=== CMix DCDC Status ===");
    
    sprintf(msg_buffer, "Mode: %d, State: %d", g_dcdc_status.mode, g_dcdc_status.state);
    CMix_Protocol_Send_Debug_Message(msg_buffer);
    
    sprintf(msg_buffer, "Vin: %dmV, Vout: %dmV", g_dcdc_status.input_voltage, g_dcdc_status.output_voltage);
    CMix_Protocol_Send_Debug_Message(msg_buffer);
    
    sprintf(msg_buffer, "Iin: %dmA, Iout: %dmA", g_dcdc_status.input_current, g_dcdc_status.output_current);
    CMix_Protocol_Send_Debug_Message(msg_buffer);
    
    sprintf(msg_buffer, "Power: %dW, Efficiency: %d%%", g_dcdc_status.output_power, g_dcdc_status.efficiency);
    CMix_Protocol_Send_Debug_Message(msg_buffer);
    
    sprintf(msg_buffer, "PWM Buck: %d, Boost: %d", g_dcdc_status.pwm_duty_buck, g_dcdc_status.pwm_duty_boost);
    CMix_Protocol_Send_Debug_Message(msg_buffer);
    
    sprintf(msg_buffer, "Faults: 0x%02X", g_safety_monitor.fault_flags);
    CMix_Protocol_Send_Debug_Message(msg_buffer);
    
    CMix_Protocol_Send_Debug_Message("=======================");
    #endif
}