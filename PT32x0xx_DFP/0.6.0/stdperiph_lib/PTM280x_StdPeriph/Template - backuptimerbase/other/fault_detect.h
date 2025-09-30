#ifndef __FAULT_DETECT_H__
#define __FAULT_DETECT_H__

#include <stdint.h>

// Basic fault codes (bitmask). Extend as needed.
typedef enum {
	FAULT_NONE      = 0x0000,
	FAULT_OVERVOLT  = 0x0001,
	FAULT_UNDERVOLT = 0x0002,
	FAULT_OVERCURR  = 0x0004,
	FAULT_OVERHEAT  = 0x0008,
} fault_code_t;

// Thresholds (example values). Adjust per hardware.
#ifndef FAULT_OVERVOLT_V
#define FAULT_OVERVOLT_V 60.0f
#endif
#ifndef FAULT_UNDERVOLT_V
#define FAULT_UNDERVOLT_V 5.0f
#endif
#ifndef FAULT_OVERCURR_A
#define FAULT_OVERCURR_A 10.0f
#endif

void fun_Fault_Check(void);
uint16_t fun_Fault_GetCode(void);
void fun_Fault_Clear(void);

#endif