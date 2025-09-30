#include "dcdc_control.h"
// 可选：根据需要包含ADC/通信接口
#include "adc_monitor.h"
#include "fault_detect.h"
#include "PT32x0xx.h"  // 需要包含寄存器定义
#include <math.h>


static volatile dcdc_mode_t s_mode = DCDC_MODE_IDLE;
static float s_targetVoltage_V = 0.0f; // 目标输出电压
static float s_targetCurrent_A = 0.0f; // 目标限流/充放电电流
static float s_boostCmpVthr = 0.0f;    // Boost比较器目标阈值（V）

// 简易PI参数（示例），请按需求调整
#ifndef DCDC_KP
#define DCDC_KP 0.01f
#endif
#ifndef DCDC_KI
#define DCDC_KI 0.001f
#endif
#ifndef DCDC_DT
#define DCDC_DT 0.001f // 1ms 控制周期（按主循环频率调整）
#endif

// Voltage-loop PI
static float s_pi_v_int = 0.0f;
// Current-loop PI
static float s_pi_i_int = 0.0f;

// Duty trackers (for telemetry)
static float s_dutyBuck = 0.0f;
static float s_dutyBoost = 0.0f;

// Comparator debounce/hold (in control loop ticks)
#ifndef BOOST_CMP_DEBOUNCE_TICKS
#define BOOST_CMP_DEBOUNCE_TICKS 3
#endif
#ifndef BOOST_CMP_HOLD_TICKS
#define BOOST_CMP_HOLD_TICKS 10
#endif
static int s_cmpDebounce = 0;
static int s_cmpHold = 0;

static float clampf(float x, float lo, float hi)
{
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

void fun_DCDC_Init(void)
{
    // 初始化PWM、GPIO、保护、计时器等（寄存器级配置）
    s_mode = DCDC_MODE_IDLE;
    s_targetVoltage_V = 0.0f;
    s_targetCurrent_A = 0.0f;
    s_pi_v_int = 0.0f;
    s_pi_i_int = 0.0f;
    s_dutyBuck = 0.0f;
    s_dutyBoost = 0.0f;
    s_cmpDebounce = 0;
    s_cmpHold = 0;
    
    // TIM1 基本配置：50kHz PWM (48MHz / 960 = 50kHz)
    TIM1->PSCR = 0;        // 预分频器=1
    TIM1->ARR = 960;       // 自动重载值，决定PWM频率
    
    // TIM1 输出比较模式配置：CH1 PWM模式1
    TIM1->OCMR = (6 << 0) |           // OC1M = PWM模式1
                 TIM_OCMR_OC1E |      // 使能 CH1 输出
                 TIM_OCMR_OC1NE;      // 使能 CH1N 互补输出
    
    // TIM1 极性控制：正常极性
    TIM1->PCR = 0; // CC1P=0, C1NP=0 (正常极性)
    
    // TIM1 死区时间配置：约200ns (48MHz时钟)
    TIM1->DTCR = 10; // 死区时间 = 10 * (1/48MHz) ≈ 208ns
    
    // TIM1 刹车输入配置
    TIM1->BKICR = TIM_BKICR_BKE |        // 使能刹车功能
                  TIM_BKICR_BKINE |      // 使能 BKIN 输入
                  TIM_BKICR_CMP0E;       // 使能 CMP0 刹车输入
    
    // TIM1 ADC触发配置：更新事件触发ADC
    TIM1->TACR = TIM_TACR_TAE;  // 使能TIM1更新事件触发ADC
    
    // 初始占空比设为0
    TIM1->OCR[0] = 0;  // 使用OCR数组，OCR[0]对应CH1
    
    // 启动TIM1
    TIM1->CR |= TIM_CR_EN;
}

void fun_DCDC_ModeUpdate(void)
{
    // 根据命令或检测自动切换Buck/Boost
    // 示例策略（可按需替换）：
    //  - 若存在故障信号，进入FAULT
    //  - 若上位机/命令指定模式，则按命令切换
    //  - 否则可根据电压关系自动判断（例如母线与电池电压）
    // 注意：实际实现中建议由Comm/Fault模块提供状态/命令，这里仅留接口
}

void fun_DCDC_ControlLoop(void)
{
    // 故障处理
    if (fun_Fault_GetCode() != FAULT_NONE)
    {
        s_mode = DCDC_MODE_FAULT;
    s_pi_v_int = 0.0f;
    s_pi_i_int = 0.0f;
        fun_DCDC_ApplyBuckDuty(0.0f);
        fun_DCDC_ApplyBoostDuty(0.0f);
        return;
    }

    // Measurements (updated by fun_ADC_Update)
    float v_meas = fun_ADC_GetVout();  // Prefer Vout for regulation
    float i_meas = fun_ADC_GetIout();  // Prefer Iout for current limiting

    // 目标
    const float v_ref = s_targetVoltage_V;
    const float i_lim = s_targetCurrent_A;

    // Outer voltage loop -> current reference
    // Gains for voltage loop
#ifndef DCDC_V_KP
#define DCDC_V_KP 0.02f
#endif
#ifndef DCDC_V_KI
#define DCDC_V_KI 0.002f
#endif
    float err_v = (v_ref - v_meas);
    s_pi_v_int += DCDC_V_KI * err_v * DCDC_DT;
    s_pi_v_int = clampf(s_pi_v_int, -i_lim, i_lim);
    float i_ref = DCDC_V_KP * err_v + s_pi_v_int;
    // Clamp i_ref by configured current limit
    if (i_lim > 0.0f) i_ref = clampf(i_ref, -i_lim, i_lim);

    // Inner current loop -> duty command
#ifndef DCDC_I_KP
#define DCDC_I_KP 0.01f
#endif
#ifndef DCDC_I_KI
#define DCDC_I_KI 0.001f
#endif
    float err_i = (i_ref - i_meas);
    s_pi_i_int += DCDC_I_KI * err_i * DCDC_DT;
    s_pi_i_int = clampf(s_pi_i_int, -1.0f, 1.0f);
    float u = DCDC_I_KP * err_i + s_pi_i_int;
    float duty = clampf(u, 0.0f, 0.98f);

    // 按模式应用到相应PWM
    if (s_mode == DCDC_MODE_BUCK)
    {
        fun_DCDC_ApplyBrake(0);
        fun_DCDC_ApplyBuckDuty(duty);
        s_dutyBuck = duty;
        fun_DCDC_ApplyBoostDuty(0.0f);
        s_dutyBoost = 0.0f;
    }
    else if (s_mode == DCDC_MODE_BOOST)
    {
        // Boost: comparator + timer brake with debounce/hold
        int tripped = fun_DCDC_BoostCmpTrip();
        if (tripped) {
            if (s_cmpDebounce < BOOST_CMP_DEBOUNCE_TICKS) s_cmpDebounce++;
            if (s_cmpDebounce >= BOOST_CMP_DEBOUNCE_TICKS) s_cmpHold = BOOST_CMP_HOLD_TICKS;
        } else {
            s_cmpDebounce = 0;
        }

        if (s_cmpHold > 0) {
            fun_DCDC_ApplyBrake(1);
            fun_DCDC_ApplyBoostDuty(0.0f);
            s_dutyBoost = 0.0f;
            s_cmpHold--;
        } else {
            fun_DCDC_ApplyBrake(0);
            fun_DCDC_ApplyBoostDuty(duty);
            s_dutyBoost = duty;
        }
        fun_DCDC_ApplyBuckDuty(0.0f);
        s_dutyBuck = 0.0f;
    }
    else
    {
        fun_DCDC_ApplyBrake(0);
        fun_DCDC_ApplyBuckDuty(0.0f);
        fun_DCDC_ApplyBoostDuty(0.0f);
        s_dutyBuck = 0.0f;
        s_dutyBoost = 0.0f;
    }
}

void fun_DCDC_SetBuckMode(void)
{
    // 设置为Buck模式（56V -> 6.4V 充电）
    s_mode = DCDC_MODE_BUCK;
    s_pi_v_int = 0.0f;
    s_pi_i_int = 0.0f;
}

void fun_DCDC_SetBoostMode(void)
{
    // 设置为Boost模式（6.4V -> 52V 放电）
    s_mode = DCDC_MODE_BOOST;
    s_pi_v_int = 0.0f;
    s_pi_i_int = 0.0f;
}

void fun_DCDC_SetTargetVoltageCurrent(float voltage, float current)
{
    // 设置目标电压/电流（单位：V/A）
    s_targetVoltage_V = voltage;
    s_targetCurrent_A = current;
}

// 默认的PWM应用函数（弱实现，板级可重写）
#ifndef __WEAK
#define __WEAK __attribute__((weak))
#endif

__WEAK void fun_DCDC_ApplyBuckDuty(float duty) 
{ 
    // Buck模式：正常PWM控制
    duty = clampf(duty, 0.0f, 0.95f); // 限制最大占空比
    s_dutyBuck = duty;
    u16 ccr_val = (u16)(duty * TIM1->ARR);
    TIM1->OCR[0] = ccr_val;  // 使用OCR数组
}

__WEAK void fun_DCDC_ApplyBoostDuty(float duty) 
{ 
    // Boost模式：互补PWM控制 (1-duty)
    duty = clampf(duty, 0.05f, 1.0f); // 限制最小占空比
    s_dutyBoost = duty;
    u16 ccr_val = (u16)((1.0f - duty) * TIM1->ARR);
    TIM1->OCR[0] = ccr_val;  // 使用OCR数组
}

__WEAK void fun_DCDC_ApplyBrake(int enable) 
{ 
    if (enable) {
        // 使能软件刹车
        TIM1->BKICR |= TIM_BKICR_SWE;
        // 注意：这个芯片可能没有直接的软件刹车触发，需要检查手册
    } else {
        // 禁用软件刹车
        TIM1->BKICR &= ~TIM_BKICR_SWE;
        TIM1->SR2 &= ~TIM_SR2_BIF; // 清除刹车中断标志
    }
}

dcdc_mode_t fun_DCDC_GetMode(void)
{
    return s_mode;
}

void fun_DCDC_GetTargets(float *voltage_V, float *current_A)
{
    if (voltage_V) *voltage_V = s_targetVoltage_V;
    if (current_A) *current_A = s_targetCurrent_A;
}

void fun_DCDC_SetBoostCmpThreshold(float vthr_V)
{
    s_boostCmpVthr = vthr_V;
    // 若已经处于BOOST模式，可同步下发到比较器
    if (s_mode == DCDC_MODE_BOOST) {
        fun_DCDC_BoostCmpInit(vthr_V);
    }
}

// 提供默认的弱实现，防止未实现导致链接错误
__WEAK void fun_DCDC_BoostCmpInit(float vthr_V) 
{ 
    // LDAC配置：5位DAC，输出参考电压
    LDAC0->CR = LDAC_CR_EN | LDAC_CR_OE; // 使能LDAC，输出使能
    
    // 计算LDAC码值：假设LDAC满量程对应3.3V，5位=32级
    // vthr_V对应分压后的电压，需要根据具体分压比计算
    // 先定义一个临时的分压系数，应该从adc_monitor.h中引用
    #ifndef VOLT_DIV_K_TEMP
    #define VOLT_DIV_K_TEMP 40.0f  // 临时定义
    #endif
    float ldac_v = vthr_V / VOLT_DIV_K_TEMP; // 转换为分压点电压
    u8 ldac_code = (u8)(ldac_v * 31.0f / 3.3f); // 5位DAC编码
    if (ldac_code > 31) ldac_code = 31;
    LDAC0->DR = ldac_code & LDAC_DR_DATA;
    
    // CMP0配置：暂时简化配置，需要根据具体手册调整输入选择
    CMP0->CR1 = CMP_CR1_EN;               // 使能比较器
    CMP0->CR2 = 0; // 默认设置，无滤波
}

__WEAK int  fun_DCDC_BoostCmpTrip(void) 
{ 
    // 读取比较器输出状态，需要根据实际CMP寄存器结构调整
    return (CMP0->SR & (1 << 0)) ? 1 : 0; // 假设SR寄存器第0位是输出状态
}
// Telemetry getters
float fun_DCDC_GetDutyBuck(void)  { return s_dutyBuck; }
float fun_DCDC_GetDutyBoost(void) { return s_dutyBoost; }
float fun_DCDC_GetPIIntegr(void)  { return s_pi_v_int; }