/******************************************************************************
  * @file    ccube_timer.h
  * @author  C-Cube Development Team
  * @version V1.0.0
  * @date    2025/09/17
  * @brief   C-Cube定时器头文件
  *          实现100ms定时状态上报功能
  ******************************************************************************
  * @attention
  *
  * Copyright (C) 2025, C-Cube Team, all rights reserved
  *
  *****************************************************************************/

#ifndef __CCUBE_TIMER_H
#define __CCUBE_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PT32x0xx.h"
#include <stdint.h>
#include <stdbool.h>

/* 函数声明 */

/**
 * @brief 定时器初始化 (100ms中断)
 * @param None
 * @retval None
 */
void Timer_StatusReport_Init(void);

/**
 * @brief 定时器状态上报处理 (在主循环中调用)
 * @param None
 * @retval None
 */
void Timer_StatusReport_Handler(void);

/**
 * @brief 更新系统状态 (模拟ADC采集)
 * @param None
 * @retval None
 */
void Update_SystemStatus(void);

#ifdef __cplusplus
}
#endif

#endif /* __CCUBE_TIMER_H */