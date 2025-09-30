/******************************************************************************
  * @file    CMix_main.c
  * @author  CMix Development Team
  * @version V1.0.0
  * @date    2025/09/17
  * @brief   CMix双向DCDC控制器主程序实现文件
  *          实现任务调度、系统管理、状态监控等功能
  ******************************************************************************
  * @attention
  *
  * CMix主应用控制模块实现
  * 负责系统整体控制、任务调度和状态管理
  *
  * Copyright (C) 2025, CMix Team, all rights reserved
  *
  *****************************************************************************/

#include "CMix_main.h"
#include "CMix_hardware.h"
#include "CMix_protocol.h"
#include "CMix_dcdc.h"
#include "CMix_config.h"
#include <stdio.h>  // 支持sprintf函数

/* ========================= 私有变量 ========================= */

static CMix_Application_State_t g_app_state = CMIX_APP_STATE_INIT;
static CMix_Task_Scheduler_t g_task_scheduler = {0};
static CMix_System_Monitor_t g_system_monitor = {0};

/* ========================= 私有函数声明 ========================= */

static void CMix_Main_System_Init(void);
static void CMix_Main_Task_Scheduler_Init(void);
static void CMix_Main_Task_1ms(void);
static void CMix_Main_Task_10ms(void);
static void CMix_Main_Task_100ms(void);
static void CMix_Main_Task_1000ms(void);
static void CMix_Main_System_Monitor(void);
static void CMix_Main_LED_Control(void);
static void CMix_Main_Watchdog_Handler(void);

/* ========================= 主函数 ========================= */

/**
 * @brief CMix主函数
 * @param None
 * @retval None
 */
int main(void)
{
    /* 系统初始化 */
    CMix_Main_System_Init();
    
    /* 应用状态设置为运行 */
    g_app_state = CMIX_APP_STATE_RUNNING;
    
    /* 主循环 */
    while (1) {
        /* 任务调度器 */
        CMix_Main_Task_Scheduler();
        
        /* 看门狗处理 */
        CMix_Main_Watchdog_Handler();
        
        /* CPU空闲时可以进入低功耗模式 */
        if (g_task_scheduler.idle_time > CMIX_IDLE_THRESHOLD) {
            /* 进入低功耗模式 */
            __WFI();
        }
    }
}

/* ========================= 公共函数实现 ========================= */

/**
 * @brief CMix任务调度器
 * @param None
 * @retval None
 */
void CMix_Main_Task_Scheduler(void)
{
    static uint32_t last_tick = 0;
    uint32_t current_tick = CMix_Main_Get_System_Tick();
    uint32_t elapsed_time = current_tick - last_tick;
    
    /* 更新任务计数器 */
    g_task_scheduler.tick_1ms += elapsed_time;
    g_task_scheduler.tick_10ms += elapsed_time;
    g_task_scheduler.tick_100ms += elapsed_time;
    g_task_scheduler.tick_1000ms += elapsed_time;
    
    last_tick = current_tick;
    
    /* 1ms任务 */
    if (g_task_scheduler.tick_1ms >= 1) {
        g_task_scheduler.tick_1ms = 0;
        CMix_Main_Task_1ms();
        g_task_scheduler.task_counter_1ms++;
    }
    
    /* 10ms任务 */
    if (g_task_scheduler.tick_10ms >= 10) {
        g_task_scheduler.tick_10ms = 0;
        CMix_Main_Task_10ms();
        g_task_scheduler.task_counter_10ms++;
    }
    
    /* 100ms任务 */
    if (g_task_scheduler.tick_100ms >= 100) {
        g_task_scheduler.tick_100ms = 0;
        CMix_Main_Task_100ms();
        g_task_scheduler.task_counter_100ms++;
    }
    
    /* 1000ms任务 */
    if (g_task_scheduler.tick_1000ms >= 1000) {
        g_task_scheduler.tick_1000ms = 0;
        CMix_Main_Task_1000ms();
        g_task_scheduler.task_counter_1000ms++;
    }
    
    /* 计算空闲时间 */
    g_task_scheduler.idle_time = 100 - g_task_scheduler.cpu_usage;
}

/**
 * @brief CMix获取应用状态
 * @param None
 * @retval 应用状态
 */
CMix_Application_State_t CMix_Main_Get_Application_State(void)
{
    return g_app_state;
}

/**
 * @brief CMix设置应用状态
 * @param state: 应用状态
 * @retval None
 */
void CMix_Main_Set_Application_State(CMix_Application_State_t state)
{
    g_app_state = state;
}

/**
 * @brief CMix获取系统监控信息
 * @param None
 * @retval 系统监控信息指针
 */
CMix_System_Monitor_t* CMix_Main_Get_System_Monitor(void)
{
    return &g_system_monitor;
}

/**
 * @brief CMix获取任务调度器信息
 * @param None
 * @retval 任务调度器信息指针
 */
CMix_Task_Scheduler_t* CMix_Main_Get_Task_Scheduler(void)
{
    return &g_task_scheduler;
}

/**
 * @brief CMix获取系统时间戳
 * @param None
 * @retval 系统时间戳(ms)
 */
uint32_t CMix_Main_Get_System_Tick(void)
{
    /* 返回系统滴答计数，需要SysTick中断支持 */
    return g_system_monitor.system_tick;
}

/**
 * @brief CMix获取运行时间
 * @param None
 * @retval 运行时间(秒)
 */
uint32_t CMix_Main_Get_Runtime(void)
{
    return g_system_monitor.runtime_seconds;
}

/**
 * @brief CMix系统复位
 * @param reset_type: 复位类型
 * @retval None
 */
void CMix_Main_System_Reset(CMix_Reset_Type_t reset_type)
{
    /* 保存复位原因 */
    g_system_monitor.reset_reason = reset_type;
    
    /* 关闭DCDC */
    CMix_DCDC_Enable(0);
    
    /* 延时确保数据保存 */
    CMix_Hardware_Delay_ms(100);
    
    /* 执行系统复位 */
    CMix_Hardware_System_Reset();
}

/**
 * @brief CMix紧急处理
 * @param emergency_code: 紧急代码
 * @retval None
 */
void CMix_Main_Emergency_Handler(uint8_t emergency_code)
{
    /* 设置应用状态为紧急 */
    g_app_state = CMIX_APP_STATE_EMERGENCY;
    
    /* 记录紧急代码 */
    g_system_monitor.emergency_count++;
    
    /* 紧急停机DCDC */
    CMix_DCDC_Emergency_Stop(emergency_code);
    
    /* 点亮故障LED */
    CMix_Hardware_GPIO_Write(CMIX_GPIO_FAULT_LED_PORT, CMIX_GPIO_FAULT_LED_PIN, 1);
    
    /* 发送紧急状态报告 */
    CMix_Protocol_Send_ACK_Error(CMIX_PROTOCOL_ERROR_SYSTEM_FAULT);
}

/**
 * @brief CMix性能监控
 * @param None
 * @retval None
 */
void CMix_Main_Performance_Monitor(void)
{
    static uint32_t last_performance_check = 0;
    uint32_t current_time = CMix_Main_Get_System_Tick();
    
    if (current_time - last_performance_check >= 1000) {
        /* 每秒更新一次性能指标 */
        
        /* 计算CPU使用率 */
        uint32_t total_tasks = g_task_scheduler.task_counter_1ms + 
                              g_task_scheduler.task_counter_10ms * 10 +
                              g_task_scheduler.task_counter_100ms * 100 +
                              g_task_scheduler.task_counter_1000ms * 1000;
        
        g_task_scheduler.cpu_usage = (total_tasks * 100) / 1000;
        if (g_task_scheduler.cpu_usage > 100) {
            g_task_scheduler.cpu_usage = 100;
        }
        
        /* 重置计数器 */
        g_task_scheduler.task_counter_1ms = 0;
        g_task_scheduler.task_counter_10ms = 0;
        g_task_scheduler.task_counter_100ms = 0;
        g_task_scheduler.task_counter_1000ms = 0;
        
        last_performance_check = current_time;
    }
}

/**
 * @brief CMix调试信息输出
 * @param None
 * @retval None
 */
void CMix_Main_Debug_Print(void)
{
    #if CMIX_DEBUG_ENABLE
    char msg_buffer[128];
    
    CMix_Protocol_Send_Debug_Message("=== CMix System Status ===");
    
    sprintf(msg_buffer, "App State: %d", g_app_state);
    CMix_Protocol_Send_Debug_Message(msg_buffer);
    
    sprintf(msg_buffer, "Runtime: %d seconds", g_system_monitor.runtime_seconds);
    CMix_Protocol_Send_Debug_Message(msg_buffer);
    
    sprintf(msg_buffer, "CPU Usage: %d%%", g_task_scheduler.cpu_usage);
    CMix_Protocol_Send_Debug_Message(msg_buffer);
    
    sprintf(msg_buffer, "Memory Usage: %d%%", g_system_monitor.memory_usage);
    CMix_Protocol_Send_Debug_Message(msg_buffer);
    
    sprintf(msg_buffer, "Temperature: %d°C", g_system_monitor.temperature);
    CMix_Protocol_Send_Debug_Message(msg_buffer);
    
    sprintf(msg_buffer, "Emergency Count: %d", g_system_monitor.emergency_count);
    CMix_Protocol_Send_Debug_Message(msg_buffer);
    
    sprintf(msg_buffer, "Error Count: %d", g_system_monitor.error_count);
    CMix_Protocol_Send_Debug_Message(msg_buffer);
    
    CMix_Protocol_Send_Debug_Message("========================");
    #endif
}

/* ========================= 私有函数实现 ========================= */

/**
 * @brief CMix系统初始化
 * @param None
 * @retval None
 */
static void CMix_Main_System_Init(void)
{
    /* 硬件初始化 */
    CMix_Hardware_Init();
    
    /* 协议初始化 */
    CMix_Protocol_Init();
    
    /* DCDC初始化 */
    CMix_DCDC_Init();
    
    /* 任务调度器初始化 */
    CMix_Main_Task_Scheduler_Init();
    
    /* 系统监控初始化 */
    g_system_monitor.system_tick = 0;
    g_system_monitor.runtime_seconds = 0;
    g_system_monitor.temperature = 25;  /* 默认温度 */
    g_system_monitor.memory_usage = 50; /* 默认内存使用率 */
    g_system_monitor.reset_reason = CMIX_RESET_POWER_ON;
    g_system_monitor.emergency_count = 0;
    g_system_monitor.error_count = 0;
    
    /* 硬件自检 */
    uint8_t self_test_result = CMix_Hardware_Self_Test();
    if (self_test_result != 0) {
        CMix_Main_Emergency_Handler(self_test_result);
    }
    
    /* 开机LED指示 */
    CMix_Hardware_GPIO_Write(CMIX_GPIO_LED_PORT, CMIX_GPIO_LED_PIN, 1);
    CMix_Hardware_Delay_ms(500);
    CMix_Hardware_GPIO_Write(CMIX_GPIO_LED_PORT, CMIX_GPIO_LED_PIN, 0);
    
    /* 发送启动信息 */
    #if CMIX_DEBUG_ENABLE
    char msg_buffer[64];
    
    CMix_Protocol_Send_Debug_Message("CMix DCDC Controller V1.0.0 Started");
    
    sprintf(msg_buffer, "System Clock: %d MHz", CMix_Hardware_Get_System_Clock() / 1000000);
    CMix_Protocol_Send_Debug_Message(msg_buffer);
    #endif
}

/**
 * @brief CMix任务调度器初始化
 * @param None
 * @retval None
 */
static void CMix_Main_Task_Scheduler_Init(void)
{
    g_task_scheduler.tick_1ms = 0;
    g_task_scheduler.tick_10ms = 0;
    g_task_scheduler.tick_100ms = 0;
    g_task_scheduler.tick_1000ms = 0;
    g_task_scheduler.task_counter_1ms = 0;
    g_task_scheduler.task_counter_10ms = 0;
    g_task_scheduler.task_counter_100ms = 0;
    g_task_scheduler.task_counter_1000ms = 0;
    g_task_scheduler.cpu_usage = 0;
    g_task_scheduler.idle_time = 100;
}

/**
 * @brief CMix 1ms任务
 * @param None
 * @retval None
 */
static void CMix_Main_Task_1ms(void)
{
    /* 系统滴答计数 */
    g_system_monitor.system_tick++;
    
    /* DCDC控制任务 */
    CMix_DCDC_Control_Task();
    
    /* DCDC状态机 */
    CMix_DCDC_State_Machine();
}

/**
 * @brief CMix 10ms任务
 * @param None
 * @retval None
 */
static void CMix_Main_Task_10ms(void)
{
    /* LED控制 */
    CMix_Main_LED_Control();
    
    /* 系统监控 */
    CMix_Main_System_Monitor();
}

/**
 * @brief CMix 100ms任务
 * @param None
 * @retval None
 */
static void CMix_Main_Task_100ms(void)
{
    /* 发送状态报告 */
    if (g_app_state == CMIX_APP_STATE_RUNNING) {
        CMix_Protocol_Send_Status_Report();
    }
    
    /* 性能监控 */
    CMix_Main_Performance_Monitor();
    
    /* 调试信息输出 */
    #if CMIX_DEBUG_ENABLE
    static uint8_t debug_counter = 0;
    if (++debug_counter >= 10) {  /* 每秒输出一次 */
        debug_counter = 0;
        
        /* 获取系统状态 */
        float vin = CMix_Hardware_ADC_Get_Value(CMIX_ADC_VIN_CHANNEL) * 5.0f / 4095.0f * 20.0f;  // 假设20:1分压
        float vout = CMix_Hardware_ADC_Get_Value(CMIX_ADC_VOUT_CHANNEL) * 5.0f / 4095.0f * 20.0f;
        float current_a = CMix_Hardware_Get_Current_A();  // 相A电流 (A)
        float current_b = CMix_Hardware_Get_Current_B();  // 相B电流 (A)
        
        /* 获取DCDC状态 */
        CMix_DCDC_Status_t* dcdc_status = CMix_DCDC_Get_Status();
        char msg_buffer[128];
        
        /* UART输出系统状态 */
        sprintf(msg_buffer, "[%s] Vin=%.2fV, Vout=%.2fV, Ia=%.3fA, Ib=%.3fA, Mode=%s",
               (g_app_state == CMIX_APP_STATE_RUNNING) ? "RUN" : "IDLE",
               vin, vout, current_a, current_b,
               (dcdc_status->mode == CMIX_MODE_BUCK) ? "BUCK" : "BOOST");
        CMix_Protocol_Send_Debug_Message(msg_buffer);
               
        /* 检查过流状态 */
        if (CMix_Hardware_Check_Overcurrent(current_a, current_b, 40.0f)) {
            sprintf(msg_buffer, "[FAULT] Overcurrent detected! Ia=%.3fA, Ib=%.3fA", current_a, current_b);
            CMix_Protocol_Send_Debug_Message(msg_buffer);
        }
    }
    #endif
}

/**
 * @brief CMix 1000ms任务
 * @param None
 * @retval None
 */
static void CMix_Main_Task_1000ms(void)
{
    /* 运行时间计数 */
    g_system_monitor.runtime_seconds++;
    
    /* 内存使用率监控 */
    /* 这里可以添加实际的内存使用率计算 */
    
    /* 温度监控 */
    /* 这里可以添加实际的温度传感器读取 */
    
    /* 长期统计信息更新 */
    g_system_monitor.total_energy_kwh += (CMix_DCDC_Get_Status()->output_power / 3600000);
}

/**
 * @brief CMix系统监控
 * @param None
 * @retval None
 */
static void CMix_Main_System_Monitor(void)
{
    /* 监控系统关键参数 */
    CMix_DCDC_Status_t *dcdc_status = CMix_DCDC_Get_Status();
    CMix_Safety_Monitor_t *safety_status = CMix_DCDC_Get_Safety_Status();
    
    /* 检查系统错误 */
    if (safety_status->fault_flags != 0) {
        g_system_monitor.error_count++;
        
        /* 如果错误过多，触发紧急处理 */
        if (g_system_monitor.error_count > CMIX_MAX_ERROR_COUNT) {
            CMix_Main_Emergency_Handler(CMIX_EMERGENCY_TOO_MANY_ERRORS);
        }
    }
    
    /* 检查温度 */
    if (g_system_monitor.temperature > CMIX_MAX_TEMPERATURE) {
        CMix_Main_Emergency_Handler(CMIX_EMERGENCY_OVERTEMPERATURE);
    }
    
    /* 检查内存使用率 */
    if (g_system_monitor.memory_usage > CMIX_MAX_MEMORY_USAGE) {
        /* 尝试清理内存 */
        /* 这里可以添加内存清理代码 */
    }
}

/**
 * @brief CMix LED控制
 * @param None
 * @retval None
 */
static void CMix_Main_LED_Control(void)
{
    static uint16_t led_counter = 0;
    uint8_t led_state = 0;
    
    led_counter++;
    
    switch (g_app_state) {
        case CMIX_APP_STATE_INIT:
            /* 初始化状态：快速闪烁 */
            led_state = (led_counter % 20 < 10) ? 1 : 0;
            break;
            
        case CMIX_APP_STATE_RUNNING:
            /* 运行状态：慢速闪烁 */
            led_state = (led_counter % 100 < 10) ? 1 : 0;
            break;
            
        case CMIX_APP_STATE_FAULT:
            /* 故障状态：常亮 */
            led_state = 1;
            break;
            
        case CMIX_APP_STATE_EMERGENCY:
            /* 紧急状态：快速闪烁 */
            led_state = (led_counter % 10 < 5) ? 1 : 0;
            break;
            
        default:
            led_state = 0;
            break;
    }
    
    CMix_Hardware_GPIO_Write(CMIX_GPIO_LED_PORT, CMIX_GPIO_LED_PIN, led_state);
}

/**
 * @brief CMix看门狗处理
 * @param None
 * @retval None
 */
static void CMix_Main_Watchdog_Handler(void)
{
    static uint32_t last_watchdog_feed = 0;
    uint32_t current_time = CMix_Main_Get_System_Tick();
    
    /* 每100ms喂一次狗 */
    if (current_time - last_watchdog_feed >= 100) {
        CMix_Hardware_Watchdog_Feed();
        last_watchdog_feed = current_time;
    }
}

/**
 * @brief SysTick中断处理函数
 * @param None
 * @retval None
 */
void SysTick_Handler(void)
{
    /* 系统时钟中断，提供1ms时基 */
    /* 这个函数由硬件自动调用 */
}

/**
 * @brief CMix应用程序版本信息
 * @param None
 * @retval 版本字符串
 */
const char* CMix_Main_Get_Version(void)
{
    return "CMix DCDC Controller V1.0.0 Build " __DATE__ " " __TIME__;
}

/**
 * @brief CMix获取编译信息
 * @param None
 * @retval 编译信息字符串
 */
const char* CMix_Main_Get_Build_Info(void)
{
    return "Compiler: " __VERSION__ " Target: PT32x MCU";
}

/**
 * @brief CMix系统参数保存
 * @param None
 * @retval 保存结果 (0:成功, 非0:失败)
 */
uint8_t CMix_Main_Save_Parameters(void)
{
    /* 这里可以添加参数保存到EEPROM或Flash的代码 */
    /* 目前返回成功 */
    return 0;
}

/**
 * @brief CMix系统参数加载
 * @param None
 * @retval 加载结果 (0:成功, 非0:失败)
 */
uint8_t CMix_Main_Load_Parameters(void)
{
    /* 这里可以添加从EEPROM或Flash加载参数的代码 */
    /* 目前返回成功 */
    return 0;
}