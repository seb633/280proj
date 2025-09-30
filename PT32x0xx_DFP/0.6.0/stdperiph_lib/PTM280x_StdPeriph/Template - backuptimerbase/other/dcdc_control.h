#ifndef __DCDC_CONTROL_H__
#define __DCDC_CONTROL_H__

/** Initialize DCDC control module (timers, states, etc.). */
void fun_DCDC_Init(void);
/** Update mode state (BUCK/BOOST/FAULT/IDLE) based on commands/sensors. */
void fun_DCDC_ModeUpdate(void);
/** Execute one control step (PI loop, protections, apply PWM). */
void fun_DCDC_ControlLoop(void);
/** Force BUCK mode (56V -> 6.4V charge). */
void fun_DCDC_SetBuckMode(void);
/** Force BOOST mode (6.4V -> 52V discharge). */
void fun_DCDC_SetBoostMode(void);
/** Set target voltage/current. Units: V and A. */
void fun_DCDC_SetTargetVoltageCurrent(float voltage, float current);

// 状态查询接口（供通信/调试使用）
/** DCDC operating modes. */
typedef enum {
	DCDC_MODE_IDLE = 0,
	DCDC_MODE_BUCK,
	DCDC_MODE_BOOST,
	DCDC_MODE_FAULT
} dcdc_mode_t;

dcdc_mode_t fun_DCDC_GetMode(void);
/** Read configured targets. Pointers may be NULL. */
void fun_DCDC_GetTargets(float *voltage_V, float *current_A);

// Optional: expose internal states for diagnostics/telemetry
float fun_DCDC_GetDutyBuck(void);
float fun_DCDC_GetDutyBoost(void);
float fun_DCDC_GetPIIntegr(void);

// 由板级驱动实现的PWM占空应用接口（0.0~1.0）
/** Apply PWM duty (0..1) to BUCK side. Implement in BSP. */
void fun_DCDC_ApplyBuckDuty(float duty);
/** Apply PWM duty (0..1) to BOOST side. Implement in BSP. */
void fun_DCDC_ApplyBoostDuty(float duty);

// Boost模式需要：比较器与刹车PWM控制
/** Set BOOST comparator trip threshold in Volts (pre-divider). */
void fun_DCDC_SetBoostCmpThreshold(float vthr_V);
/** Initialize comparator with given threshold (BSP). */
void fun_DCDC_BoostCmpInit(float vthr_V); // BSP: configure CMP and reference
/** Returns 1 if comparator is currently tripped. */
int  fun_DCDC_BoostCmpTrip(void);
/** Control timer brake (BKIN). 1=enable brake, 0=disable. */
void fun_DCDC_ApplyBrake(int enable);

#endif