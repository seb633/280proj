#include "CMix_control.h"

#include <string.h>

static void CMix_ControlHandleIdle(CMix_ControlContext *ctx);
static void CMix_ControlHandlePrecharge(CMix_ControlContext *ctx);
static void CMix_ControlHandleActive(CMix_ControlContext *ctx);
static void CMix_ControlHandleFault(CMix_ControlContext *ctx);
static void CMix_ControlApplyDuty(CMix_ControlContext *ctx);
static float CMix_ClampFloat(float value, float min_value, float max_value);
static float CMix_ComputeDutyFromMeasurements(const CMix_ControlContext *ctx);

static const float CMIX_PRECHARGE_ENTRY_LEVEL = 0.05f;
static const float CMIX_PRECHARGE_TARGET_DUTY = 0.10f;
static const float CMIX_PRECHARGE_STEP = 0.01f;
static const float CMIX_MIN_ACTIVE_DUTY = 0.05f;
static const float CMIX_MAX_ACTIVE_DUTY = 0.95f;

void CMix_ControlInit(CMix_ControlContext *ctx)
{
    if (ctx == NULL)
    {
        return;
    }

    memset(ctx, 0, sizeof(*ctx));
    ctx->state = CMIX_STATE_IDLE;
    ctx->direction = CMIX_DIRECTION_BUCK;
}

void CMix_ControlSetDirection(CMix_ControlContext *ctx, CMix_PowerDirection direction)
{
    if (ctx == NULL)
    {
        return;
    }

    if (ctx->direction != direction)
    {
        ctx->direction = direction;
        if (ctx->state == CMIX_STATE_ACTIVE)
        {
            ctx->state = CMIX_STATE_PRECHARGE;
            ctx->duty_cmd_phase_a = 0.0f;
            ctx->duty_cmd_phase_b = 0.0f;
        }
    }
}

void CMix_ControlUpdate(CMix_ControlContext *ctx)
{
    if (ctx == NULL)
    {
        return;
    }

    CMix_ReadAnalogMeasurements(&ctx->measurements);
    CMix_UpdateBoardStatus(&ctx->board_status);

    if (ctx->board_status.fault_bkin_triggered || ctx->board_status.fault_over_temperature)
    {
        ctx->state = CMIX_STATE_FAULT;
    }

    switch (ctx->state)
    {
        case CMIX_STATE_IDLE:
            CMix_ControlHandleIdle(ctx);
            break;
        case CMIX_STATE_PRECHARGE:
            CMix_ControlHandlePrecharge(ctx);
            break;
        case CMIX_STATE_ACTIVE:
            CMix_ControlHandleActive(ctx);
            break;
        case CMIX_STATE_FAULT:
        default:
            CMix_ControlHandleFault(ctx);
            break;
    }

    CMix_ProcessFaults();
    CMix_ControlApplyDuty(ctx);
    CMix_ScheduleADCConversion();
}

void CMix_ControlNotifyFault(CMix_ControlContext *ctx)
{
    if (ctx == NULL)
    {
        return;
    }

    ctx->state = CMIX_STATE_FAULT;
}

static void CMix_ControlHandleIdle(CMix_ControlContext *ctx)
{
    CMix_EnablePWMOutputs(false);
    ctx->duty_cmd_phase_a = 0.0f;
    ctx->duty_cmd_phase_b = 0.0f;

    if (!ctx->board_status.fault_bkin_triggered)
    {
        if ((ctx->measurements.v_pack_total > CMIX_PRECHARGE_ENTRY_LEVEL) ||
            (ctx->measurements.v_out_bus > CMIX_PRECHARGE_ENTRY_LEVEL))
        {
            ctx->state = CMIX_STATE_PRECHARGE;
        }
    }
}

static void CMix_ControlHandlePrecharge(CMix_ControlContext *ctx)
{
    CMix_EnablePWMOutputs(true);

    float target = CMIX_PRECHARGE_TARGET_DUTY;
    float next_duty = ctx->duty_cmd_phase_a + CMIX_PRECHARGE_STEP;
    ctx->duty_cmd_phase_a = CMix_ClampFloat(next_duty, 0.0f, target);
    ctx->duty_cmd_phase_b = ctx->duty_cmd_phase_a;

    if (ctx->duty_cmd_phase_a >= target && !ctx->board_status.fault_bkin_triggered)
    {
        ctx->state = CMIX_STATE_ACTIVE;
    }
}

static void CMix_ControlHandleActive(CMix_ControlContext *ctx)
{
    CMix_EnablePWMOutputs(true);

    float duty = CMix_ComputeDutyFromMeasurements(ctx);
    ctx->duty_cmd_phase_a = duty;
    ctx->duty_cmd_phase_b = duty;

    if ((ctx->measurements.v_pack_total <= CMIX_PRECHARGE_ENTRY_LEVEL) &&
        (ctx->measurements.v_out_bus <= CMIX_PRECHARGE_ENTRY_LEVEL))
    {
        ctx->state = CMIX_STATE_IDLE;
    }
}

static void CMix_ControlHandleFault(CMix_ControlContext *ctx)
{
    CMix_EnablePWMOutputs(false);
    ctx->duty_cmd_phase_a = 0.0f;
    ctx->duty_cmd_phase_b = 0.0f;
}

static void CMix_ControlApplyDuty(CMix_ControlContext *ctx)
{
    uint16_t period = CMix_GetPwmPeriodTicks();
    float phase_a = CMix_ClampFloat(ctx->duty_cmd_phase_a, 0.0f, 1.0f);
    float phase_b = CMix_ClampFloat(ctx->duty_cmd_phase_b, 0.0f, 1.0f);

    if (period == 0U)
    {
        CMix_UpdateBridgeDuty(0U, 0U);
        return;
    }

    uint16_t ticks_a = (uint16_t)(phase_a * (float)(period + 1U));
    uint16_t ticks_b = (uint16_t)(phase_b * (float)(period + 1U));

    if (ticks_a > period)
    {
        ticks_a = period;
    }
    if (ticks_b > period)
    {
        ticks_b = period;
    }

    CMix_UpdateBridgeDuty(ticks_a, ticks_b);
}

static float CMix_ClampFloat(float value, float min_value, float max_value)
{
    if (value < min_value)
    {
        return min_value;
    }
    if (value > max_value)
    {
        return max_value;
    }
    return value;
}

static float CMix_ComputeDutyFromMeasurements(const CMix_ControlContext *ctx)
{
    float duty;

    if (ctx->direction == CMIX_DIRECTION_BUCK)
    {
        if (ctx->measurements.v_out_bus <= 0.01f)
        {
            duty = CMIX_MIN_ACTIVE_DUTY;
        }
        else
        {
            duty = ctx->measurements.v_pack_total / ctx->measurements.v_out_bus;
        }
    }
    else
    {
        if (ctx->measurements.v_pack_total <= 0.01f)
        {
            duty = CMIX_MAX_ACTIVE_DUTY;
        }
        else
        {
            duty = ctx->measurements.v_out_bus / ctx->measurements.v_pack_total;
        }
    }

    return CMix_ClampFloat(duty, CMIX_MIN_ACTIVE_DUTY, CMIX_MAX_ACTIVE_DUTY);
}
