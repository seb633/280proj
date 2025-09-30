#ifndef CMIX_CONTROL_H
#define CMIX_CONTROL_H

#include <stdbool.h>
#include <stdint.h>

#include "CMix_board.h"

typedef enum
{
    CMIX_DIRECTION_BUCK = 0,
    CMIX_DIRECTION_BOOST
} CMix_PowerDirection;

typedef enum
{
    CMIX_STATE_IDLE = 0,
    CMIX_STATE_PRECHARGE,
    CMIX_STATE_ACTIVE,
    CMIX_STATE_FAULT
} CMix_ControlState;

typedef struct
{
    CMix_ControlState state;
    CMix_PowerDirection direction;
    CMix_AnalogMeasurements measurements;
    CMix_BoardStatus board_status;
    float duty_cmd_phase_a;
    float duty_cmd_phase_b;
} CMix_ControlContext;

void CMix_ControlInit(CMix_ControlContext *ctx);
void CMix_ControlSetDirection(CMix_ControlContext *ctx, CMix_PowerDirection direction);
void CMix_ControlUpdate(CMix_ControlContext *ctx);
void CMix_ControlNotifyFault(CMix_ControlContext *ctx);

#endif /* CMIX_CONTROL_H */
