#include "fault_detect.h"
#include "PT32x0xx.h"
#include "adc_monitor.h"

static volatile uint16_t s_fault = FAULT_NONE;

void fun_Fault_Check(void)
{
    float v = fun_ADC_GetVoltage();
    float i = fun_ADC_GetCurrent();

    uint16_t f = s_fault;
    // 电压故障
    if (v > FAULT_OVERVOLT_V) f |= FAULT_OVERVOLT; else f &= (uint16_t)~FAULT_OVERVOLT;
    if (v < FAULT_UNDERVOLT_V) f |= FAULT_UNDERVOLT; else f &= (uint16_t)~FAULT_UNDERVOLT;
    // 电流故障
    if (i > FAULT_OVERCURR_A) f |= FAULT_OVERCURR; else f &= (uint16_t)~FAULT_OVERCURR;
    // 温度故障（需要温度采样时实现，这里保留）
    // if (temp > TH_OVERHEAT) f |= FAULT_OVERHEAT; else f &= ~FAULT_OVERHEAT;

    s_fault = f;
}

uint16_t fun_Fault_GetCode(void)
{
    return s_fault;
}

void fun_Fault_Clear(void)
{
    s_fault = FAULT_NONE;
}