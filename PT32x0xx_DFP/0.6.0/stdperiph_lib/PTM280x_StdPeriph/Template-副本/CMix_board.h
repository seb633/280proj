#ifndef CMIX_BOARD_H
#define CMIX_BOARD_H

#include <stdbool.h>
#include <stdint.h>

#include "CMix_pinmap.h"

typedef struct
{
    float v_pack_total;
    float v_out_bus;
    float cell_voltage[3];
    float i_bat;
    float i_out;
    float ntc_temp[4];
} CMix_AnalogMeasurements;

typedef struct
{
    bool fault_bkin_triggered;
    bool fault_over_temperature;
    bool fault_comm_lost;
} CMix_BoardStatus;

void CMix_SystemInit(void);
void CMix_InitClocks(void);
void CMix_InitGPIO(void);
void CMix_InitPWMTimers(void);
uint16_t CMix_GetPwmPeriodTicks(void);
void CMix_UpdateBridgeDuty(uint16_t phase_a_ticks, uint16_t phase_b_ticks);
void CMix_EnablePWMOutputs(bool enable);
void CMix_InitADCSequence(void);
void CMix_ScheduleADCConversion(void);
void CMix_InitUART(uint32_t baudrate);
void CMix_InitNTCMux(void);
void CMix_SelectNTCChannel(bool select_ntc4);
void CMix_ReadAnalogMeasurements(CMix_AnalogMeasurements *meas);
void CMix_UpdateBoardStatus(CMix_BoardStatus *status);
void CMix_ProcessFaults(void);

#endif /* CMIX_BOARD_H */
