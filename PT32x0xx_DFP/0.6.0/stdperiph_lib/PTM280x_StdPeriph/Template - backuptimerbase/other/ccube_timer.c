/******************************************************************************
  * @file    ccube_timer.c
  * @author  C-Cube Development Team
  * @version V1.0.0
  * @date    2025/09/17
  * @brief   C-Cube定时器实现文件
  *          实现100ms定时状态上报功能
  ******************************************************************************
  * @attention
  *
  * Copyright (C) 2025, C-Cube Team, all rights reserved
  *
  *****************************************************************************/

#include "ccube_timer.h"
#include "ccube_uart_protocol.h"
#include "PT32x0xx_config.h"

static volatile uint32_t g_timer_counter = 0;
static volatile bool g_status_report_flag = false;

/**
 * @brief 定时器初始化 (使用TIM1, 100ms中断)
 * @param None
 * @retval None
 */
void Timer_StatusReport_Init(void)
{
    TIM_InitTypeDef TIM_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    
    /* 使能TIM1时钟 */
    RCC_APBPeriph1ClockCmd(RCC_APBPeriph1_TIM1, ENABLE);
    
    /* 配置TIM1 */
    // 假设系统时钟48MHz，预分频器设为47999，计数器设为99
    // 定时器频率 = 48MHz / (47999+1) / (99+1) = 10Hz = 100ms
    TIM_InitStruct.TIM_Period = 99;                     // 自动重载值
    TIM_InitStruct.TIM_Prescaler = 47999;               // 预分频值
    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;    // 时钟分频
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数
    TIM_Init(TIM1, &TIM_InitStruct);
    
    /* 使能TIM1更新中断 */
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    
    /* 配置NVIC */
    NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0x02;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    
    /* 启动TIM1 */
    TIM_Cmd(TIM1, ENABLE);
}

/**
 * @brief 定时器状态上报处理
 * @param None
 * @retval None
 */
void Timer_StatusReport_Handler(void)
{
    if (g_status_report_flag) {
        g_status_report_flag = false;
        
        /* 这里可以添加模拟的ADC采集代码来更新系统状态 */
        Update_SystemStatus();
        
        /* 发送状态上报 */
        Protocol_SendStatusReport();
    }
}

/**
 * @brief 更新系统状态 (模拟实际的ADC采集)
 * @param None
 * @retval None
 */
void Update_SystemStatus(void)
{
    System_Status_t *status = Get_SystemStatus();
    
    /* 这里应该是实际的ADC采集代码，现在用模拟数据 */
    // 模拟输入电压 50V-70V之间变化
    status->input_voltage = 50000 + (g_timer_counter % 20001);
    
    // 模拟输入电流 5A-15A之间变化
    status->input_current = 5000 + (g_timer_counter % 10001);
    
    // 模拟输出电压 58V-62V之间变化
    status->output_voltage = 58000 + (g_timer_counter % 4001);
    
    // 模拟输出电流 10A-20A之间变化
    status->output_current = 10000 + (g_timer_counter % 10001);
    
    // 计算输出功率
    status->output_power = (uint16_t)((uint32_t)status->output_voltage * status->output_current / 1000);
    
    // 工作模式从系统参数获取
    System_Parameters_t *params = Get_SystemParameters();
    status->working_mode = params->working_mode;
}

/**
 * @brief TIM1更新中断处理函数
 * @param None
 * @retval None
 */
void TIM1_UP_Handler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        
        g_timer_counter++;
        g_status_report_flag = true;
    }
}