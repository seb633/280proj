/******************************************************************************
  * @file    CMix_main.h
  * @author  CMix Development Team
  * @version V1.0.0
  * @date    2025/09/17
  * @brief   CMix双向DCDC控制器主程序头文件
  *          定义主程序接口和应用层功能
  ******************************************************************************
  * @attention
  *
  * CMix主程序模块
  * 负责系统调度、任务管理和应用逻辑
  *
  * Copyright (C) 2025, CMix Team, all rights reserved
  *
  *****************************************************************************/

#ifndef __CMIX_MAIN_H
#define __CMIX_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "CMix_config.h"
#include "CMix_protocol.h"
#include "CMix_dcdc.h"
#include "CMix_hardware.h"

/* ========================= 应用配置 ========================= */

/* 任务调度配置 */
#define CMIX_TASK_CONTROL_INTERVAL_MS       1      // 控制任务间隔 1ms
#define CMIX_TASK_COMMUNICATION_INTERVAL_MS 10     // 通信任务间隔 10ms
#define CMIX_TASK_MONITOR_INTERVAL_MS       100    // 监控任务间隔 100ms
#define CMIX_TASK_LED_INTERVAL_MS           500    // LED任务间隔 500ms

/* 应用状态类型别名，用于兼容性 */
typedef CMix_App_State_t CMix_Application_State_t;

/* 任务调度器结构体 */
typedef struct {
    uint32_t tick_1ms;                  // 1ms任务计数器
    uint32_t tick_10ms;                 // 10ms任务计数器
    uint32_t tick_100ms;                // 100ms任务计数器
    uint32_t tick_1000ms;               // 1000ms任务计数器
    uint32_t task_counter_1ms;          // 1ms任务执行计数
    uint32_t task_counter_10ms;         // 10ms任务执行计数
    uint32_t task_counter_100ms;        // 100ms任务执行计数
    uint32_t task_counter_1000ms;       // 1000ms任务执行计数
    uint8_t cpu_usage;                  // CPU使用率
    uint8_t idle_time;                  // 空闲时间
} CMix_Task_Scheduler_t;

/* 系统监控结构体 */
typedef struct {
    uint32_t system_tick;               // 系统滴答计数
    uint32_t runtime_seconds;           // 运行时间(秒)
    uint8_t temperature;                // 系统温度
    uint8_t memory_usage;               // 内存使用率
    uint8_t reset_reason;               // 复位原因
    uint16_t emergency_count;           // 紧急事件计数
    uint16_t error_count;               // 错误计数
    uint32_t total_energy_kwh;          // 总能量(千瓦时)
} CMix_System_Monitor_t;

/* 复位类型枚举 */
typedef enum {
    CMIX_RESET_POWER_ON = 0,            // 上电复位
    CMIX_RESET_SOFTWARE,                // 软件复位
    CMIX_RESET_WATCHDOG,                // 看门狗复位
    CMIX_RESET_EXTERNAL,                // 外部复位
    CMIX_RESET_BROWN_OUT                // 欠压复位
} CMix_Reset_Type_t;

/* 任务状态结构体 */
typedef struct {
    uint32_t control_task_counter;      // 控制任务计数
    uint32_t communication_task_counter; // 通信任务计数
    uint32_t monitor_task_counter;      // 监控任务计数
    uint32_t led_task_counter;          // LED任务计数
    uint32_t system_tick;               // 系统滴答计数
    CMix_App_State_t app_state;         // 应用状态
    bool system_ready;                  // 系统就绪标志
} CMix_App_Status_t;

/* ========================= 函数声明 ========================= */

/* 主程序入口 */
int main(void);

/* 系统初始化 */
void CMix_System_Init(void);
void CMix_System_Start(void);
void CMix_System_Stop(void);
void CMix_System_Reset(void);

/* 任务调度 */
void CMix_Main_Task_Scheduler(void);
void CMix_Task_Scheduler(void);
void CMix_Task_Control(void);
void CMix_Task_Communication(void);
void CMix_Task_Monitor(void);
void CMix_Task_LED_Indication(void);

/* 系统时间 */
uint32_t CMix_Main_Get_System_Tick(void);

/* 应用状态管理 */
void CMix_App_Set_State(CMix_App_State_t state);
CMix_App_State_t CMix_App_Get_State(void);
CMix_App_Status_t* CMix_App_Get_Status(void);

/* 系统监控 */
void CMix_System_Monitor(void);
void CMix_System_Print_Info(void);
void CMix_System_Print_Status(void);

/* 错误处理 */
void CMix_Error_Handler(CMix_Error_t error);
void CMix_System_Fault_Handler(void);

/* 电源管理 */
void CMix_Power_Save_Mode(void);
void CMix_Power_Normal_Mode(void);

/* 系统测试 */
void CMix_System_Self_Test(void);
bool CMix_System_Health_Check(void);

#ifdef __cplusplus
}
#endif

#endif /* __CMIX_MAIN_H */