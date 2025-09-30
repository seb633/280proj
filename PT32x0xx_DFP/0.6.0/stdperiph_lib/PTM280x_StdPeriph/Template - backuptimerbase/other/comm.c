#include "comm.h"
#include "PT32x0xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dcdc_control.h"
#include "adc_monitor.h"
#include "fault_detect.h"

// Minimal command buffer. We reuse main.c's ring buffer via UartReadBytes().
// Commands (uppercase):
//  MODE BUCK | MODE BOOST
//  SET <V> <I>
//  GET  (reports MODE, targets, measurements, fault)
//  FAULT
//  CLRFAULT
static char s_cmdBuf[64];
static int s_cmdLen = 0;

static void handle_cmd(const char *cmd)
{
    // 命令格式（示例）：
    // MODE BUCK
    // MODE BOOST
    // SET 6.4 2.0   -> 目标电压6.4V、电流2.0A
    // GET
    if (strncmp(cmd, "MODE ", 5) == 0) {
        const char *m = cmd + 5;
        if (strncmp(m, "BUCK", 4) == 0) {
            fun_DCDC_SetBuckMode();
            printf("OK MODE=BUCK\r\n");
        } else if (strncmp(m, "BOOST", 5) == 0) {
            fun_DCDC_SetBoostMode();
            printf("OK MODE=BOOST\r\n");
        } else {
            printf("ERR MODE\r\n");
        }
        return;
    }
    if (strncmp(cmd, "SET ", 4) == 0) {
        float v = 0.0f, i = 0.0f;
        if (sscanf(cmd + 4, "%f %f", &v, &i) == 2) {
            fun_DCDC_SetTargetVoltageCurrent(v, i);
            printf("OK V=%.3f I=%.3f\r\n", v, i);
        } else {
            printf("ERR SET\r\n");
        }
        return;
    }
    if (strncmp(cmd, "GET", 3) == 0) {
        float v_t, i_t, vm, im, db, ds, pi;
        dcdc_mode_t m = fun_DCDC_GetMode();
        fun_DCDC_GetTargets(&v_t, &i_t);
        vm = fun_ADC_GetVout();
        im = fun_ADC_GetIout();
        db = fun_DCDC_GetDutyBuck();
        ds = fun_DCDC_GetDutyBoost();
        pi = fun_DCDC_GetPIIntegr();
        printf("MODE=%d Vt=%.3f It=%.3f Vm=%.3f Im=%.3f Db=%.3f Ds=%.3f PI=%.4f F=0x%04X\r\n", (int)m, v_t, i_t, vm, im, db, ds, pi, (unsigned)fun_Fault_GetCode());
        return;
    }

    if (strncmp(cmd, "FAULT", 5) == 0) {
        printf("F=0x%04X\r\n", (unsigned)fun_Fault_GetCode());
        return;
    }

    if (strncmp(cmd, "CLRFAULT", 8) == 0) {
        fun_Fault_Clear();
        printf("OK CLRFAULT\r\n");
        return;
    }

    if (strncmp(cmd, "SETCMP ", 7) == 0) {
        float vthr = 0.0f;
        if (sscanf(cmd + 7, "%f", &vthr) == 1) {
            fun_DCDC_SetBoostCmpThreshold(vthr);
            printf("OK SETCMP Vthr=%.3f\r\n", vthr);
        } else {
            printf("ERR SETCMP\r\n");
        }
        return;
    }
    printf("ERR CMD\r\n");
}

void fun_Comm_Init(void)
{
    s_cmdLen = 0;
}

void fun_Comm_Process(void)
{
    // 这里假设用户在 main 的 UART 中断中放入了接收环形缓冲
    // 如果你希望这里直接读取，可将 UartReadBytes 声明为 extern 并使用
    extern unsigned short UartReadBytes(unsigned char *pBuf, unsigned short size);
    unsigned char tmp[16];
    unsigned short n = UartReadBytes(tmp, sizeof(tmp));
    for (unsigned short k = 0; k < n; ++k) {
        char c = (char)tmp[k];
        if (c == '\r' || c == '\n') {
            if (s_cmdLen > 0) {
                s_cmdBuf[s_cmdLen] = '\0';
                handle_cmd(s_cmdBuf);
                s_cmdLen = 0;
            }
        } else if (s_cmdLen < (int)sizeof(s_cmdBuf) - 1) {
            s_cmdBuf[s_cmdLen++] = c;
        } else {
            // 超长复位
            s_cmdLen = 0;
        }
    }
}