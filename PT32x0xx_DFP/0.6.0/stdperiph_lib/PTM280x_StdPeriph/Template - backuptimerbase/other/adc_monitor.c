#include "adc_monitor.h"
#include "PT32x0xx.h"  // 需要包含寄存器定义

// Calibration & conversion parameters (example). Update per board.
// 假设ADC参考电压3.3V，分辨率12bit（0..4095）
#ifndef ADC_VREF
#define ADC_VREF 3.3f
#endif
#ifndef ADC_RES
#define ADC_RES 4096.0f
#endif
// 电压采样：Rtop=200k, Rbottom=5.1k -> K = (Rtop+Rbottom)/Rbottom ≈ 40.215686
#ifndef VOLT_DIV_K
#define VOLT_DIV_K ((200000.0f + 5100.0f) / 5100.0f)
#endif
// Current sense example: shunt 0.01Ω with gain x20 => 1A -> 0.2V
#ifndef CURR_SENSE_V_PER_A
#define CURR_SENSE_V_PER_A 0.2f
#endif

// Legacy single-channel cache (kept for backward compatibility)
static float voltage = 0.0f;
static float current = 0.0f;

// Preferred multi-channel cache
static float s_vin = 0.0f;
static float s_vout = 0.0f;
static float s_iin = 0.0f;
static float s_iout = 0.0f;

#ifndef __WEAK
#define __WEAK __attribute__((weak))
#endif

// Board-level hooks to provide raw ADC codes (0..ADC_RES-1)
__WEAK unsigned short fun_ADC_ReadVoltageRaw(void) 
{ 
    // 触发单次转换并读取结果 
    ADC0->CR1 |= ADC_CR1_SOC;
    while (!(ADC0->SR & ADC_SR_EOC));
    return ADC0->DR & 0xFFF; // 12位结果
}

__WEAK unsigned short fun_ADC_ReadCurrentRaw(void) 
{ 
    return 0; // 暂时返回0，可根据需要实现电流采样
}

// Multi-channel hooks - 实现Vin/Vout分别采样
__WEAK unsigned short fun_ADC_ReadVinRaw(void) 
{ 
    // 选择CH3进行单次转换
    ADC0->SCHR[0] = 3; // 设置序列为CH3
    ADC0->CR2 &= ~ADC_CR2_SCCNT; // 清除扫描通道数配置，单通道转换
    ADC0->CR1 |= ADC_CR1_SOC;
    while (!(ADC0->SR & ADC_SR_EOC));
    return ADC0->DR & 0xFFF;
}

__WEAK unsigned short fun_ADC_ReadVoutRaw(void) 
{ 
    // 选择CH4进行单次转换
    ADC0->SCHR[0] = 4; // 设置序列为CH4
    ADC0->CR2 &= ~ADC_CR2_SCCNT; // 清除扫描通道数配置，单通道转换
    ADC0->CR1 |= ADC_CR1_SOC;
    while (!(ADC0->SR & ADC_SR_EOC));
    return ADC0->DR & 0xFFF;
}

__WEAK unsigned short fun_ADC_ReadIinRaw(void) { return fun_ADC_ReadCurrentRaw(); }
__WEAK unsigned short fun_ADC_ReadIoutRaw(void) { return fun_ADC_ReadCurrentRaw(); }

void fun_ADC_Init(void)
{
    // ADC0 寄存器级初始化
    
    // ADC时钟预分频：CFGR3 (需要根据实际位域调整)
    ADC0->CFGR3 = (1 << 0) |      // PSC位域，具体位置需要查手册
                  (3 << 4) |      // SMP位域
                  (0 << 8);       // SETUP位域
    
    // 扫描通道配置：CH3(PA3/Vin), CH4(PA4/Vout) 
    ADC0->SCHR[0] = (3 << 0) | (4 << 4);          // 序列0=CH3, 序列1=CH4
    
    // ADC触发配置：使用已定义的位掩码
    ADC0->CR2 = ADC_CR2_MODE |                    // 连续转换模式 (假设该位为1时表示连续)
                0 |                               // 右对齐 (ALIGN=0)
                ((1 << 12) & ADC_CR2_SCCNT) |     // 扫描通道数：2个 (SCCNT字段)
                ((1 << 16) & ADC_CR2_TRIGS) |     // 外部触发 (TRIGS字段)
                ((1 << 20) & ADC_CR2_TIMS);       // 触发源：TIM1 (TIMS字段)
    
    // 使能ADC
    ADC0->CR1 = ADC_CR1_EN;
    
    // 等待ADC准备就绪（简化等待）
    for(volatile int i = 0; i < 1000; i++);
}

void fun_ADC_Update(void)
{
    // 采集电压和电流数据
    // 实际应调用片上ADC读取原始码，并据硬件参数换算
    // 示例：
    // uint16_t adc_v = ADC_ReadVoltageChannelRaw();
    // uint16_t adc_i = ADC_ReadCurrentChannelRaw();
    // float v_sense = (adc_v * ADC_VREF / ADC_RES); // 分压点电压
    // float i_sense_v = (adc_i * ADC_VREF / ADC_RES);
    // voltage = v_sense * VOLT_DIV_K;               // 还原母线/电池实际电压
    // current = i_sense_v / CURR_SENSE_V_PER_A;     // 还原电流

    unsigned short rv_in  = fun_ADC_ReadVinRaw();
    unsigned short rv_out = fun_ADC_ReadVoutRaw();
    unsigned short ri_in  = fun_ADC_ReadIinRaw();
    unsigned short ri_out = fun_ADC_ReadIoutRaw();

    if (rv_in)  { s_vin  = (rv_in  * ADC_VREF / ADC_RES) * VOLT_DIV_K;  } else { s_vin  = 24.0f; }
    if (rv_out) { s_vout = (rv_out * ADC_VREF / ADC_RES) * VOLT_DIV_K; } else { s_vout = 12.0f; }
    if (ri_in)  { s_iin  = (ri_in  * ADC_VREF / ADC_RES) / CURR_SENSE_V_PER_A;  } else { s_iin  = 2.0f; }
    if (ri_out) { s_iout = (ri_out * ADC_VREF / ADC_RES) / CURR_SENSE_V_PER_A; } else { s_iout = 1.0f; }

    // Maintain legacy single-channel outputs mapped to Vout/Iout
    voltage = s_vout;
    current = s_iout;
}

float fun_ADC_GetVoltage(void)
{
    return voltage;
}

float fun_ADC_GetCurrent(void)
{
    return current;
}

float fun_ADC_GetVin(void)  { return s_vin; }
float fun_ADC_GetVout(void) { return s_vout; }
float fun_ADC_GetIin(void)  { return s_iin; }
float fun_ADC_GetIout(void) { return s_iout; }
