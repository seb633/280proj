/******************************************************************************
  * @file    CMix_hardware.c
  * @author  CMix Development Team
  * @version V1.0.0
  * @date    2025/09/17
  * @brief   CMix双向DCDC控制器硬件抽象层实现文件
  *          实现UART、ADC、TIM、GPIO等外设的初始化和控制
  ******************************************************************************
  * @attention
  *
  * CMix硬件抽象层(HAL)模块实现
  * 提供硬件外设的初始化、配置和操作接口
  *
  * Copyright (C) 2025, CMix Team, all rights reserved
  *
  *****************************************************************************/

#include "CMix_hardware.h"
#include "CMix_protocol.h"
#include "CMix_config.h"
#include "system_PT32x0xx.h"

/* ========================= 私有变量 ========================= */

// PT32x标准库不使用Handle结构体，直接操作寄存器

/* 私有变量：保存系统状态用于后续调试输出 */
static uint32_t g_system_clock_freq = 0;
static bool g_clock_config_ok = false;

/* ========================= 私有函数声明 ========================= */

static void CMix_Hardware_GPIO_Config(void);
static void CMix_Hardware_Clock_Config(void);

/* ========================= 公共函数实现 ========================= */

/**
 * @brief CMix硬件初始化
 * @param None
 * @retval None
 */
void CMix_Hardware_Init(void)
{
    /* 系统时钟配置 */
    CMix_Hardware_Clock_Config();

    /* GPIO配置 */
    CMix_Hardware_GPIO_Config();

    /* UART初始化 */
    CMix_Hardware_UART_Init();

    /* UART初始化完成后，可以进行调试输出 */
    #if CMIX_DEBUG_ENABLE
    CMix_Hardware_Debug_PrintSystemInfo();
    #endif

    /* ADC初始化 */
    CMix_Hardware_ADC_Init();

    /* 🔧 OPA初始化 - 电压跟随器 */
    CMix_Hardware_OPA_Init();

    /* 🔧 CMP初始化 - 过压保护 */
    CMix_Hardware_CMP_Init();

    /* TIM初始化 */
    CMix_Hardware_TIM_Init();

    /* 启用全局中断 */
    __enable_irq();
}

/**
 * @brief CMix UART初始化
 * @param None
 * @retval None
 */
void CMix_Hardware_UART_Init(void)
{
    /* 使能UART时钟 */
    RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_UART0, ENABLE);
    
    /* 使能GPIO时钟 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);

    /* UART GPIO配置 */
    GPIO_InitTypeDef GPIO_InitStruct;
    
    /* TX引脚配置 - PA15 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OutPP;
    GPIO_InitStruct.GPIO_Pull = GPIO_Pull_Up;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* RX引脚配置 - PB2 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_In;
    GPIO_InitStruct.GPIO_Pull = GPIO_Pull_Up;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* UART配置 */
    UART_InitTypeDef UART_InitStruct;
    UART_InitStruct.UART_BaudRate = 115200;
    UART_InitStruct.UART_SampleRate = UART_SampleRate_16X;
    UART_InitStruct.UART_DataLength = UART_DataLength_8;
    UART_InitStruct.UART_StopBitLength = UART_StopBitLength_1;
    UART_InitStruct.UART_ParityMode = UART_Parity_None;
    UART_InitStruct.UART_Receiver = UART_Receiver_Enable;
    UART_Init(UART0, &UART_InitStruct);

    /* 使能UART接收中断 */
    UART_ITConfig(UART0, UART_IT_RXNE, ENABLE);
    
    /* 配置UART中断优先级 */
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = UART0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    /* 使能UART */
    UART_Cmd(UART0, ENABLE);
}

/**
 * @brief CMix ADC初始化
 * @param None
 * @retval None
 */
void CMix_Hardware_ADC_Init(void)
{
    /* 使能ADC时钟 */
    RCC_APBPeriph3ClockCmd(RCC_APBPeriph3_ADC0, ENABLE);
    
    /* 使能GPIO时钟 - GPIOA和GPIOB */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);

    /* 🔧 ADC GPIO配置 - 修正引脚配置 */
    GPIO_InitTypeDef GPIO_InitStruct;
    
    /* 配置Vin采样引脚 - PB12 (高压侧) */
    GPIO_InitStruct.GPIO_Pin = CMIX_ADC_VIN_PIN;  // GPIO_Pin_12
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_In;     // PT32x ADC使用输入模式
    GPIO_InitStruct.GPIO_Pull = GPIO_Pull_NoPull;
    GPIO_Init(CMIX_ADC_VIN_PORT, &GPIO_InitStruct);  // GPIOB
    
    /* 配置其他ADC引脚 - PA1(相A电流), PA2(Vout), PA3(相B电流) */
    GPIO_InitStruct.GPIO_Pin = CMIX_ADC_CURRENT_A_PIN | CMIX_ADC_VOUT_PIN | CMIX_ADC_CURRENT_B_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_In;     // PT32x ADC使用输入模式
    GPIO_InitStruct.GPIO_Pull = GPIO_Pull_NoPull;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ADC基础配置 */
    ADC_InitTypeDef ADC_InitStruct;
    ADC_InitStruct.ADC_Mode = ADC_Mode_Single;
    ADC_InitStruct.ADC_RVSPS = ADC_RVSPS_VDDA;    // 使用VDDA作为参考电压 (5.0V)
    ADC_InitStruct.ADC_Channel = CMIX_ADC_VIN_CHANNEL;  // 默认配置Vin通道
    ADC_InitStruct.ADC_Prescaler = 1;
    ADC_InitStruct.ADC_ChannelSetupTime = 1;
    ADC_InitStruct.ADC_SampleTime = 33;  // 🔧 统一采样时间：239.5周期，适配TP181A1输出阻抗
    ADC_Init(ADC0, &ADC_InitStruct);

    /* 🔧 修正：配置ADC使用TIM1 TRGO触发 */
    ADC_RegularTimerTriggerSource(ADC0, ADC_RegularTimerTriggerSource_TIM1);
    /* 注意：PT32x不需要单独的TriggerCmd，定时器触发源配置后自动生效 */

    /* 使能ADC */
    ADC_Cmd(ADC0, ENABLE);
    
    /* 等待ADC准备就绪 */
    while(ADC_GetFlagStatus(ADC0, ADC_FLAG_RDY) == RESET);
}

/**
 * @brief CMix OPA初始化 - 电压跟随器配置
 * @param None
 * @retval None
 */
void CMix_Hardware_OPA_Init(void)
{
    /* 🔧 OPA电压跟随器配置 - 用于高阻分压驱动ADC */
    
    /* 使能OPA时钟 */
    RCC_APBPeriph3ClockCmd(RCC_APBPeriph3_OPA, ENABLE);
    
    /* 使能GPIO时钟 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    /* 配置OPA GPIO - PT32x OPA引脚配置 */
    GPIO_InitTypeDef GPIO_InitStruct;
    
    /* PA3 - OPA正向输入 */
    GPIO_InitStruct.GPIO_Pin = CMIX_OPA_INPUT_PIN;  // GPIO_Pin_3
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_In;       // PT32x OPA使用输入模式
    GPIO_InitStruct.GPIO_Pull = GPIO_Pull_NoPull;
    GPIO_Init(CMIX_OPA_INPUT_PORT, &GPIO_InitStruct);  // GPIOA
    
    /* OPA输出内部连接到ADC，无需外部GPIO配置 */

    /* 配置OPA为电压跟随器模式 */
    // PTM280x的OPA配置相对简单，主要是使能功能
    OPA_Cmd(CMIX_OPA_UNIT, ENABLE);  // 使能OPA0
    
    /* 注意：PTM280x的OPA不需要复杂的PGA配置，
     * 默认就是电压跟随器功能：输出电压 = 输入电压
     * 这样高阻分压可以通过OPA驱动低阻ADC输入 */
}

/**
 * @brief CMix CMP初始化 - 过压保护配置
 * @param None
 * @retval None
 */
void CMix_Hardware_CMP_Init(void)
{
    /* 🔧 CMP过压保护配置 - 硬件快速响应 */
    
    /* 使能CMP时钟 */
    RCC_APBPeriph3ClockCmd(RCC_APBPeriph3_CMP0, ENABLE);
    
    /* 使能GPIO时钟 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);

    /* 配置CMP GPIO引脚 */
    GPIO_InitTypeDef GPIO_InitStruct;
    
    /* 配置CMP1 - PA8 (Vin过压检测) */
    GPIO_InitStruct.GPIO_Pin = CMIX_CMP1_INPUT_PIN;  // GPIO_Pin_8
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_In;        // 输入模式
    GPIO_InitStruct.GPIO_Pull = GPIO_Pull_NoPull;
    GPIO_Init(CMIX_CMP1_INPUT_PORT, &GPIO_InitStruct);  // GPIOA
    
    /* 配置CMP0 - PB4 (Vout过压检测) */  
    GPIO_InitStruct.GPIO_Pin = CMIX_CMP0_INPUT_PIN;  // GPIO_Pin_4
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_In;        // 输入模式
    GPIO_InitStruct.GPIO_Pull = GPIO_Pull_NoPull;
    GPIO_Init(CMIX_CMP0_INPUT_PORT, &GPIO_InitStruct);  // GPIOB

    /* 配置CMP1 - Vin过压保护 (60V阈值) */
    CMP_InitTypeDef CMP_InitStruct;
    CMP_InitStruct.CMP_DigitalFilter = CMP_DigitalFilter_16;          // 数字滤波
    CMP_InitStruct.CMP_InitializationDelayTime = 20;                 // 初始化延时
    CMP_InitStruct.CMP_NegativeInput = CMP_NegativeInput_LDAC;       // 负端接LDAC
    CMP_InitStruct.CMP_OutputPolarity = CMP_OutputPolarity_Normal;   // 正常极性
    CMP_InitStruct.CMP_PositiveInput = CMP_PositiveInput_CMPxP0;          // 正端接PA8
    CMP_Init(CMIX_CMP1_UNIT, &CMP_InitStruct);

    /* 配置CMP0 - Vout过压保护 (55V阈值) */
    CMP_InitStruct.CMP_PositiveInput = CMP_PositiveInput_CMPxP1;          // 正端接PB4
    CMP_Init(CMIX_CMP0_UNIT, &CMP_InitStruct);

    /* 配置CMP中断 */
    NVIC_InitTypeDef NVIC_InitStruct;
    
    /* CMP1中断配置 */
    NVIC_InitStruct.NVIC_IRQChannel = CMP1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0;  // 最高优先级
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    
    /* CMP0中断配置 */
    NVIC_InitStruct.NVIC_IRQChannel = CMP0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0;  // 最高优先级
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    /* 使能CMP中断 */
    CMP_ITConfig(CMIX_CMP1_UNIT, CMP_IT_COF | CMP_IT_COR, ENABLE);
    CMP_ITConfig(CMIX_CMP0_UNIT, CMP_IT_COF | CMP_IT_COR, ENABLE);

    /* 使能CMP */
    CMP_Cmd(CMIX_CMP1_UNIT, ENABLE);  // 使能CMP1
    CMP_Cmd(CMIX_CMP0_UNIT, ENABLE);  // 使能CMP0
    
    /* 等待CMP稳定 */
    for(volatile int i = 0; i < 1000; i++);
}

/**
 * @brief CMP1中断处理函数 - Vin过压保护
 * @param None
 * @retval None
 */
void CMP1_Handler(void)
{
    if (CMP_GetITStatus(CMIX_CMP1_UNIT, CMP_IT_COF) != RESET) {
        /* Vin超过60V阈值 - 立即保护动作 */
        
        /* 🚨 立即关闭所有PWM输出 */
        CMix_Hardware_Set_PWM_Duty(1, 0);
        CMix_Hardware_Set_PWM_Duty(2, 0);
        CMix_Hardware_Set_PWM_Duty(3, 0);
        CMix_Hardware_Set_PWM_Duty(4, 0);
        
        /* 点亮故障LED */
        CMix_Hardware_GPIO_Write(CMIX_GPIO_FAULT_LED_PORT, CMIX_GPIO_FAULT_LED_PIN, 1);
        
        /* 设置故障状态 - 需要在DCDC模块中实现 */
        // CMix_DCDC_Set_Fault_State(CMIX_FAULT_VIN_OVERVOLTAGE);
        
        /* 清除中断标志 */
        CMP_ClearFlag(CMIX_CMP1_UNIT, CMP_IT_COF);
    }
    
    if (CMP_GetITStatus(CMIX_CMP1_UNIT, CMP_IT_COR) != RESET) {
        /* Vin恢复正常 - 但需要ADC二次确认 */
        
        /* 清除中断标志 */
        CMP_ClearFlag(CMIX_CMP1_UNIT, CMP_IT_COR);
    }
}

/**
 * @brief CMP0中断处理函数 - Vout过压保护
 * @param None  
 * @retval None
 */
void CMP0_Handler(void)
{
    if (CMP_GetITStatus(CMIX_CMP0_UNIT, CMP_IT_COF) != RESET) {
        /* Vout超过55V阈值 - 立即保护动作 */
        
        /* 🚨 立即关闭所有PWM输出 */
        CMix_Hardware_Set_PWM_Duty(1, 0);
        CMix_Hardware_Set_PWM_Duty(2, 0);
        CMix_Hardware_Set_PWM_Duty(3, 0);
        CMix_Hardware_Set_PWM_Duty(4, 0);
        
        /* 点亮故障LED */
        CMix_Hardware_GPIO_Write(CMIX_GPIO_FAULT_LED_PORT, CMIX_GPIO_FAULT_LED_PIN, 1);
        
        /* 设置故障状态 */
        // CMix_DCDC_Set_Fault_State(CMIX_FAULT_VOUT_OVERVOLTAGE);
        
        /* 清除中断标志 */
        CMP_ClearFlag(CMIX_CMP0_UNIT, CMP_IT_COF);
    }
    
    if (CMP_GetITStatus(CMIX_CMP0_UNIT, CMP_IT_COR) != RESET) {
        /* Vout恢复正常 - 但需要ADC二次确认 */
        
        /* 清除中断标志 */
        CMP_ClearFlag(CMIX_CMP0_UNIT, CMP_IT_COR);
    }
}

/**
 * @brief CMix TIM初始化
 * @param None
 * @retval None
 */
void CMix_Hardware_TIM_Init(void)
{
    /* 使能TIM1时钟 */
    RCC_APBPeriph1ClockCmd(RCC_APBPeriph1_TIM1, ENABLE);
    
    /* 使能GPIO时钟 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);

    /* 🔧 PWM引脚配置 - 修正为checklist要求的PA5/PA7 */
    GPIO_InitTypeDef GPIO_InitStruct;
    
    /* 配置TIM1_CH1主输出引脚 - PA5 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OutPP;     // 推挽输出模式
    GPIO_InitStruct.GPIO_Pull = GPIO_Pull_NoPull;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /* 配置TIM1_CH1N互补输出引脚 - PA7 */  
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OutPP;     // 推挽输出模式
    GPIO_InitStruct.GPIO_Pull = GPIO_Pull_NoPull;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /* 配置PWM引脚的复用功能 */
    GPIO_DigitalRemapConfig(AFIOA, GPIO_Pin_5, AFIO_AF_2, ENABLE);  // TIM1_CH1
    GPIO_DigitalRemapConfig(AFIOA, GPIO_Pin_7, AFIO_AF_2, ENABLE);  // TIM1_CH1N
    
    /* 🔧 配置BKIN保护引脚 - PA8 (CMP1输出直接连接) */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;        // PA8 作为BKIN输入(CMP1输出)
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_In;     // 输入模式
    GPIO_InitStruct.GPIO_Pull = GPIO_Pull_Up;     // 上拉，低电平触发保护
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /* 🔧 配置外部比较器BKIN输入 - PB5 (外部TP181A1过流比较器输出) */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;        // PB5 作为外部比较器BKIN输入
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_In;     // 输入模式
    GPIO_InitStruct.GPIO_Pull = GPIO_Pull_Up;     // 上拉，低电平触发保护
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* TIM1基础配置 - 🔧 修正PWM频率为100kHz */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    // 48MHz / (0+1) / (479+1) = 100kHz PWM频率
    TIM_TimeBaseStruct.TIM_AutoReloadValue = 479;      // ARR = 479 (480个计数)
    TIM_TimeBaseStruct.TIM_Prescaler = 0;              // PSC = 0 (不分频)
    TIM_TimeBaseStruct.TIM_Direction = TIM_Direction_Up;
    TIM_TimeBaseStruct.TIM_CenterAlignedMode = TIM_CenterAlignedMode_Disable;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStruct);

    /* 🔧 配置TIM1 TRGO用于触发ADC - 每个PWM周期触发一次ADC */
    TIM_MasterModeInitTypeDef TIM_MasterModeInitStruct;
    TIM_MasterModeInitStruct.TIM_Synchronization = 0;
    TIM_MasterModeInitStruct.TIM_MasterMode = TIM_MasterMode_Update;  // 更新事件产生TRGO
    TIM_MasterModeInit(TIM1, &TIM_MasterModeInitStruct);

    /* PWM模式配置 - 只使用TIM1_CH1及其互补输出 */
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCInitStruct.TIM_Channel = TIM_Channel_1;
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Low;     // 空闲时低电平
    TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Low;  // 互补空闲时低电平
    TIM_OCInitStruct.TIM_OCOutput = TIM_OCOutput_Enable;       // 主输出使能
    TIM_OCInitStruct.TIM_OCNOutput = TIM_OCNOutput_Enable;     // 互补输出使能
    TIM_OCInitStruct.TIM_UpOCValue = 0;    // 初始占空比为0
    TIM_OCInitStruct.TIM_DownOCValue = 0;

    /* 只初始化TIM1_CH1通道 (PA5主输出 + PA7互补输出) */
    TIM_OCInit(TIM1, &TIM_OCInitStruct);

    /* 🔧 配置BKIN和刹车功能 - 关键安全功能！ */
    TIM_BKICRInitTypeDef TIM_BKICRInitStruct;
    TIM_BKICRInitStruct.TIM_Break = TIM_Break_Enable;                                    // 使能刹车功能
    TIM_BKICRInitStruct.TIM_BreakPolarity = TIM_BreakPolarity_Low;                      // 低电平触发刹车
    TIM_BKICRInitStruct.TIM_BreakSource = TIM_BreakSource_CMP0 | TIM_BreakSource_CMP1;  // CMP0和CMP1触发刹车
    TIM_BKICRInitStruct.TIM_BreakInputControl = TIM_BreakInput_TIMOFF;                  // 刹车时关闭TIM输出
    TIM_BKICRInitStruct.TIM_Breakfilter = TIM_Breakfilter_None;                         // 无滤波
    TIM_BKICRInit(TIM1, &TIM_BKICRInitStruct);
    
    /* 🔧 配置死区时间 - 防止上下桥臂直通 */
    TIM_SetDeadTime(TIM1, 10);  // 死区时间约200ns (取决于时钟配置)

    /* 🔧 配置BKIN引脚复用功能 */
    // PA8 配置为TIM1_BKIN功能 (CMP1输出直接路由)
    GPIO_DigitalRemapConfig(AFIOA, GPIO_Pin_8, AFIO_AF_2, ENABLE);

    /* 使能TIM1 */
    TIM_Cmd(TIM1, ENABLE);
}

/**
 * @brief CMix UART发送字节
 * @param byte: 发送的字节
 * @retval None
 */
void CMix_Hardware_UART_Send_Byte(uint8_t byte)
{
    /* 等待发送完成 */
    while (UART_GetFlagStatus(UART0, UART_FLAG_TXE) == RESET);
    
    /* 发送数据 */
    UART_SendData(UART0, byte);
}

/**
 * @brief CMix UART发送字符串
 * @param str: 字符串指针
 * @retval None
 */
void CMix_Hardware_UART_Send_String(const char *str)
{
    while (*str) {
        CMix_Hardware_UART_Send_Byte(*str++);
    }
}

/**
 * @brief CMix ADC读取值
 * @param channel: ADC通道 (0-3)
 * @retval ADC值 (0-4095)
 */
uint16_t CMix_Hardware_ADC_Read(uint8_t channel)
{
    if (channel > 3) return 0;

    /* 配置ADC通道 */
    ADC_InitTypeDef ADC_InitStruct;
    ADC_InitStruct.ADC_Mode = ADC_Mode_Single;
    ADC_InitStruct.ADC_RVSPS = ADC_RVSPS_VDDA;
    ADC_InitStruct.ADC_Channel = channel;
    ADC_InitStruct.ADC_Prescaler = 1;
    ADC_InitStruct.ADC_ChannelSetupTime = 1;
    ADC_InitStruct.ADC_SampleTime = 33;  // Longer sample time for accuracy
    ADC_Init(ADC0, &ADC_InitStruct);

    /* 启动转换 */
    ADC_StartOfRegularConversion(ADC0);

    /* 等待转换完成 */
    while (ADC_GetFlagStatus(ADC0, ADC_FLAG_EOC) == RESET);

    /* 返回转换结果 */
    return ADC_GetConversionValue(ADC0);
}

/**
 * @brief CMix获取ADC值 (兼容接口)
 * @param channel: ADC通道号
 * @retval ADC转换结果
 */
uint16_t CMix_Hardware_ADC_Get_Value(uint8_t channel)
{
    return CMix_Hardware_ADC_Read(channel);
}

/**
 * @brief CMix设置PWM占空比
 * @param channel: PWM通道 (1-4)
 * @param duty_cycle: 占空比 (0-10000, 对应0-100.00%)
 * @retval None
 */
void CMix_Hardware_Set_PWM_Duty(uint8_t channel, uint16_t duty_cycle)
{
    uint16_t pulse = (duty_cycle * CMIX_PWM_PERIOD) / 10000;

    if (channel >= 1 && channel <= 4) {
        TIM_SetOCxValue(TIM1, channel, pulse);
    }
}

/**
 * @brief CMix GPIO写引脚
 * @param port: GPIO端口
 * @param pin: GPIO引脚
 * @param state: 引脚状态 (0或1)
 * @retval None
 */
void CMix_Hardware_GPIO_Write(GPIO_TypeDef *port, uint16_t pin, uint8_t state)
{
    if (state) {
        GPIO_SetBits(port, pin);
    } else {
        GPIO_ResetBits(port, pin);
    }
}

/**
 * @brief CMix GPIO读引脚
 * @param port: GPIO端口
 * @param pin: GPIO引脚
 * @retval 引脚状态 (0或1)
 */
uint8_t CMix_Hardware_GPIO_Read(GPIO_TypeDef *port, uint16_t pin)
{
    return (GPIO_ReadDataBit(port, pin) == 1) ? 1 : 0;
}

/**
 * @brief CMix延时函数(毫秒)
 * @param ms: 延时毫秒数
 * @retval None
 */
void CMix_Hardware_Delay_ms(uint32_t ms)
{
    uint32_t i, j;
    /* 简单延时实现，实际项目中建议使用SysTick */
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 8000; j++) {
            __NOP();
        }
    }
}

/**
 * @brief CMix获取系统时钟
 * @param None
 * @retval 系统时钟频率(Hz)
 */
uint32_t CMix_Hardware_Get_System_Clock(void)
{
    return GetSystemClock();
}

/**
 * @brief CMix系统复位
 * @param None
 * @retval None
 */
void CMix_Hardware_System_Reset(void)
{
    NVIC_SystemReset();
}

/**
 * @brief CMix获取电压传感器值
 * @param None
 * @retval 电压传感器结构体
 */
CMix_Voltage_Sensors_t CMix_Hardware_Get_Voltage_Sensors(void)
{
    CMix_Voltage_Sensors_t sensors;
    
    sensors.input_voltage = CMix_Hardware_ADC_Read(0);
    sensors.output_voltage = CMix_Hardware_ADC_Read(2);
    
    return sensors;
}

/**
 * @brief CMix获取电流传感器值
 * @param None
 * @retval 电流传感器结构体
 */
CMix_Current_Sensors_t CMix_Hardware_Get_Current_Sensors(void)
{
    CMix_Current_Sensors_t sensors;
    
    /* 获取ADC原始值 */
    uint16_t adc_current_a = CMix_Hardware_ADC_Read(CMIX_ADC_CURRENT_A_CHANNEL);
    uint16_t adc_current_b = CMix_Hardware_ADC_Read(CMIX_ADC_CURRENT_B_CHANNEL);
    
    /* 使用TP181A1换算公式转换为实际电流 */
    sensors.input_current = CMix_Hardware_Convert_Current(adc_current_a);   // 相A电流
    sensors.output_current = CMix_Hardware_Convert_Current(adc_current_b);  // 相B电流
    
    return sensors;
}

/**
 * @brief 将ADC原始值转换为实际电流值 (TP181A1)
 * @param adc_raw: ADC原始值 (0-4095 for 12-bit ADC)
 * @retval 电流值 (A)，负值表示反向电流
 */
float CMix_Hardware_Convert_Current(uint16_t adc_raw)
{
    /* ADC转电压：ADC_RAW * VDDA / 4095 */
    float vadc = (float)adc_raw * 5.0f / 4095.0f;
    
    /* 减去基准电压（VDDA/2） */
    float vdiff = vadc - CMIX_CURRENT_VREF;
    
    /* 电流计算：I = (Vadc - Vref) / (Rs * Gain) */
    float current = vdiff / (CMIX_CURRENT_SENSE_RS * CMIX_CURRENT_SENSE_GAIN);
    
    /* 溢出保护 */
    if (current > 100.0f) current = 100.0f;      // 最大100A保护
    if (current < -100.0f) current = -100.0f;    // 最小-100A保护
    
    return current;
}

/**
 * @brief 电流校准函数
 * @param phase: 相别 ('A' or 'B')
 * @param raw_current: 原始电流值
 * @param offset_correction: 零点校正值
 * @retval 校准后的电流值
 */
float CMix_Hardware_Calibrate_Current(char phase, float raw_current, float offset_correction)
{
    /* 零点校正 */
    float calibrated = raw_current - offset_correction;
    
    /* 增益校正（如果需要） */
    // calibrated *= gain_correction_factor;
    
    return calibrated;
}

/**
 * @brief 获取相A电流值
 * @param None
 * @retval 相A电流 (A)
 */
float CMix_Hardware_Get_Current_A(void)
{
    uint16_t adc_raw = CMix_Hardware_ADC_Read(CMIX_ADC_CURRENT_A_CHANNEL);
    return CMix_Hardware_Convert_Current(adc_raw);
}

/**
 * @brief 获取相B电流值
 * @param None
 * @retval 相B电流 (A)
 */
float CMix_Hardware_Get_Current_B(void)
{
    uint16_t adc_raw = CMix_Hardware_ADC_Read(CMIX_ADC_CURRENT_B_CHANNEL);
    return CMix_Hardware_Convert_Current(adc_raw);
}

/**
 * @brief 检查是否发生过流
 * @param current_a: 相A电流
 * @param current_b: 相B电流
 * @param trip_threshold: 过流阈值 (A)
 * @retval 1: 过流, 0: 正常
 */
uint8_t CMix_Hardware_Check_Overcurrent(float current_a, float current_b, float trip_threshold)
{
    if ((fabs(current_a) > trip_threshold) || (fabs(current_b) > trip_threshold)) {
        return 1;  // 过流
    }
    return 0;  // 正常
}

/* ========================= 私有函数实现 ========================= */

/**
 * @brief CMix时钟配置
 * @param None
 * @retval None
 */
static void CMix_Hardware_Clock_Config(void)
{
    /* 🔧 明确配置48MHz系统时钟 */
    /* 使用内部HSI时钟，简化配置 */
    /* PT32x默认使用HSI，无需额外配置 */
    
    /* 验证系统时钟频率 */
    uint32_t system_clock = GetSystemClock();
    g_system_clock_freq = system_clock;
    g_clock_config_ok = (system_clock == 48000000);
    
    /* 注意：不能在UART初始化前使用printf */
    /* 时钟状态已保存，将在UART初始化后通过调试函数输出 */
}

/**
 * @brief CMix GPIO基础配置
 * @param None
 * @retval None
 */
static void CMix_Hardware_GPIO_Config(void)
{
    /* 验证芯片类型配置 */
    #ifdef PTM280x
        /* PTM280x芯片：只使能GPIOA和GPIOB */
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
    #else
        #error "未定义PTM280x芯片类型，请检查编译器宏定义"
    #endif

    /* LED指示灯配置 */
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = CMIX_GPIO_LED_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OutPP;
    GPIO_InitStruct.GPIO_Pull = GPIO_Pull_NoPull;
    GPIO_Init(CMIX_GPIO_LED_PORT, &GPIO_InitStruct);

    /* 故障指示LED配置 */
    GPIO_InitStruct.GPIO_Pin = CMIX_GPIO_FAULT_LED_PIN;
    GPIO_Init(CMIX_GPIO_FAULT_LED_PORT, &GPIO_InitStruct);

    /* 继电器控制引脚配置 */
    GPIO_InitStruct.GPIO_Pin = CMIX_GPIO_RELAY_PIN;
    GPIO_Init(CMIX_GPIO_RELAY_PORT, &GPIO_InitStruct);

    /* 初始状态设置 */
    CMix_Hardware_GPIO_Write(CMIX_GPIO_LED_PORT, CMIX_GPIO_LED_PIN, 0);
    CMix_Hardware_GPIO_Write(CMIX_GPIO_FAULT_LED_PORT, CMIX_GPIO_FAULT_LED_PIN, 0);
    CMix_Hardware_GPIO_Write(CMIX_GPIO_RELAY_PORT, CMIX_GPIO_RELAY_PIN, 0);
}

/* ========================= 中断处理函数 ========================= */

/**
 * @brief UART中断处理函数
 * @param None
 * @retval None
 */
void UART0_Handler(void)
{
    if (UART_GetITStatus(UART0, UART_IT_RXNE) != RESET) {
        uint8_t received_byte = UART_ReceiveData(UART0);
        CMix_Protocol_Receive_Handler(received_byte);
        UART_ClearFlag(UART0, UART_FLAG_RXNE);
    }
}

/**
 * @brief 硬故障中断处理函数
 * @param None
 * @retval None
 */
void HardFault_Handler(void)
{
    /* 点亮故障LED */
    CMix_Hardware_GPIO_Write(CMIX_GPIO_FAULT_LED_PORT, CMIX_GPIO_FAULT_LED_PIN, 1);
    
    /* 关闭所有PWM输出 */
    CMix_Hardware_Set_PWM_Duty(1, 0);
    CMix_Hardware_Set_PWM_Duty(2, 0);
    CMix_Hardware_Set_PWM_Duty(3, 0);
    CMix_Hardware_Set_PWM_Duty(4, 0);
    
    /* 无限循环等待复位 */
    while (1) {
        __NOP();
    }
}

/**
 * @brief 硬件看门狗超时处理
 * @param None
 * @retval None
 */
void CMix_Hardware_Watchdog_Feed(void)
{
    /* 实际项目中添加看门狗喂狗代码 */
    /* IWDG_ReloadCounter(); */
}

/**
 * @brief 电源管理初始化
 * @param None
 * @retval None
 */
void CMix_Hardware_Power_Management_Init(void)
{
    /* 配置低功耗模式 */
    PWR_PVDLevelConfig(PWR_PVDLevel_2); // Use available PWR_PVDLevel_2 instead of PWR_PVDLevel_2V9
    PWR_PVDCmd(ENABLE);
    
    /* Note: PT32x EXTI configuration simplified due to different API
     * EXTI_Init function structure differs from STM32 HAL
     * For now, keeping basic PVD configuration only
     * Full EXTI setup would require PT32x-specific implementation:
     * EXTI_ITConfig(EXTIx, EXTI_IT_x, ENABLE);
     * EXTI_TriggerTypeConfig(EXTIx, EXTI_Pin_x, EXTI_Trigger_Rising);
     */
}

/**
 * @brief CMix硬件自检
 * @param None
 * @retval 自检结果 (0:成功, 非0:失败)
 */
uint8_t CMix_Hardware_Self_Test(void)
{
    uint8_t result = 0;
    
    /* ADC自检 */
    uint16_t adc_test = CMix_Hardware_ADC_Read(0);
    if (adc_test == 0 || adc_test == 0xFFF) {
        result |= 0x01;  /* ADC故障 */
    }
    
    /* PWM自检 */
    CMix_Hardware_Set_PWM_Duty(1, 5000);  /* 50%占空比 */
    CMix_Hardware_Delay_ms(10);
    CMix_Hardware_Set_PWM_Duty(1, 0);     /* 关闭PWM */
    
    /* UART自检 */
    CMix_Hardware_UART_Send_String("CMix Hardware Self-Test\r\n");
    
    return result;
}

/**
 * @brief Get system clock frequency (PT32x implementation)
 * @param None
 * @retval System clock frequency in Hz
 */
u32 GetSystemClock(void)
{
    // Use PT32x GetClockFreq function to get system clock
    return GetClockFreq(CLKSRC_SYSCLK);
}

/**
 * @brief Assert failed handler for debugging
 * @param file: Source file name
 * @param line: Line number where assert failed
 * @retval None
 */
void assert_failed(uint8_t* file, u32 line)
{
    /* User can add their own implementation to report the file name and line number,
       e.g. printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    
    /* Infinite loop for debugging - can be replaced with reset or error handling */
    while (1)
    {
        /* Add breakpoint here for debugging */
    }
}

/**
 * @brief 打印系统调试信息 (仅在UART初始化后调用)
 * @param None
 * @retval None
 */
#if CMIX_DEBUG_ENABLE
void CMix_Hardware_Debug_PrintSystemInfo(void)
{
    /* 发送系统信息通过协议 */
    CMix_Protocol_Send_System_Info(g_system_clock_freq, g_clock_config_ok, __DATE__, __TIME__);
    
    /* 发送各个调试信息 */
    CMix_Protocol_Send_Debug_Message("=== CMix硬件初始化完成 ===");
    
    #ifdef PTM280x
    CMix_Protocol_Send_Debug_Message("目标芯片: PTM280x LQFP32 (GPIOA/B可用)");
    #else
    CMix_Protocol_Send_Debug_Message("目标芯片: 未知类型");
    #endif
    
    CMix_Protocol_Send_Debug_Message("硬件版本: CMix V1.0");
    CMix_Protocol_Send_Debug_Message("初始化状态: UART/ADC/TIM/GPIO/OPA/CMP 已配置");
    CMix_Protocol_Send_Debug_Message("========================");
}
#endif