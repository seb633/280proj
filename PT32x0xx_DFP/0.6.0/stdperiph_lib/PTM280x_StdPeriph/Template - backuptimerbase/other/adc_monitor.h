#ifndef __ADC_MONITOR_H__
#define __ADC_MONITOR_H__

/**
 * Initialize ADC monitor module. Configure ADC clocks/channels in BSP if needed.
 */
void fun_ADC_Init(void);

/**
 * Sample all required channels and update cached measurements.
 * Call this at the beginning of each control loop iteration.
 */
void fun_ADC_Update(void);

/** Get measured bus/battery voltage (Volts). */
float fun_ADC_GetVoltage(void);

/** Get measured current (Amperes). */
float fun_ADC_GetCurrent(void);

/** Additional multi-channel getters (preferred): */
float fun_ADC_GetVin(void);   // Input bus voltage (V)
float fun_ADC_GetVout(void);  // Output/battery voltage (V)
float fun_ADC_GetIin(void);   // Input current (A)
float fun_ADC_GetIout(void);  // Output current (A)

#endif