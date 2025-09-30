#include "CMix_board.h"

#include <stddef.h>

#include "PT32x0xx.h"
#include "PT32x0xx_rcc.h"
#include "PT32x0xx_gpio.h"
#include "PT32x0xx_tim.h"
#include "PT32x0xx_adc.h"
#include "PT32x0xx_uart.h"
#include "PT32x0xx_nvic.h"
#include "system_PT32x0xx.h"
#include "PT32x0xx_i2c.h"
#include "PT32x0xx_config.h"
#include "PT32x0xx_conf.h"

#define CMIX_PWM_FREQUENCY_HZ        100000U
#define CMIX_DEADTIME_TICKS          80U
#define CMIX_ADC_SAMPLE_TIME_CYCLES  31U
#define CMIX_ADC_SETUP_TIME_CYCLES   30U
#define CMIX_ADC_MAX_COUNTS          4095.0f
#define CMIX_ADC_TIMEOUT_ITERATIONS  1000U

typedef struct
{
    const CMix_PinConfig *pin;
    u32 channel;
} CMix_AdcSequenceEntry;

static const CMix_AdcSequenceEntry s_adc_sequence[] =
{
    { &CMix_Pin_IBat_Sense,   ADC_Channel_0 },
    { &CMix_Pin_IOut_Sense,   ADC_Channel_1 },
    { &CMix_Pin_Cell1_Tap,    ADC_Channel_2 },
    { &CMix_Pin_Cell2_Tap,    ADC_Channel_3 },
    { &CMix_Pin_Cell3_Tap,    ADC_Channel_4 },
    { &CMix_Pin_VOut_Bus,     ADC_Channel_5 },
    { &CMix_Pin_VPack_Total,  ADC_Channel_6 },
    { &CMix_Pin_NTC1,         ADC_Channel_9 },
    { &CMix_Pin_NTC2,         ADC_Channel_8 },
    { &CMix_Pin_NTC_Mux_Out,  ADC_Channel_10 }
};

static uint16_t s_pwm_period_ticks = 0;
static bool s_pwm_outputs_requested = false;
static bool s_ntc_mux_selects_ntc4 = false;
static float s_ntc_mux_cached[2] = {0.0f, 0.0f};
static CMix_AnalogMeasurements s_measurement_cache = {0};

static GPIO_TypeDef *CMix_GetGpio(const CMix_PinConfig *pin);
static AFIO_TypeDef *CMix_GetAfio(const CMix_PinConfig *pin);
static uint16_t CMix_GetGpioPinMask(const CMix_PinConfig *pin);
static void CMix_EnableAlternateFunction(const CMix_PinConfig *pin, u8 af_index);
static void CMix_EnableAnalogFunction(const CMix_PinConfig *pin);
static void CMix_ConfigPwmPins(void);
static void CMix_ConfigAdcPins(void);
static void CMix_ConfigUartPins(void);
static void CMix_ConfigDebugPins(void);
static void CMix_ConfigMuxPins(void);
static void CMix_WaitForAdcReady(void);
static uint16_t CMix_ClampDutyTicks(uint16_t duty_ticks);

/**
* @brief 配置I2C的复用引脚
* @param 无
* @retval 无
*/
void I2C_GPIO_Config(void)
{
	/* ????????? + ??(I2C????),??????? */
	GPIO_InitTypeDef GPIO_InitStruct;

	/* SDA: PA12,SCL: PA11(? PT32x0xx_config.h ???)*/
	GPIO_InitStruct.GPIO_Pin  = I2C_SDA_PIN | I2C_SCL_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OutOD;   /* ???? */
	GPIO_InitStruct.GPIO_Pull = GPIO_Pull_Up;      /* ??(?????????) */
	GPIO_Init(GPIOA, &GPIO_InitStruct);
//	GPIO_SetBits(GPIOA, I2C_SDA_PIN);
//	GPIO_SetBits(GPIOA, I2C_SCL_PIN);
	/* ??I2C??????? */
	GPIO_DigitalRemapConfig(I2C_SDA_AFIO, I2C_SDA_PIN, I2C_SDA_AFx, ENABLE); /* I2C0 SDA */
	GPIO_DigitalRemapConfig(I2C_SCL_AFIO, I2C_SCL_PIN, I2C_SCL_AFx, ENABLE); /* I2C0 SCL */
}

/**
* @brief 配置I2C的工作模式
* @param 无
* @retval 无
*/
void I2C_Mode_Config(I2C_TypeDef *I2Cx)
{  
	I2C_InitTypeDef I2C_InitStruct;
	
	I2C_InitStruct.I2C_Acknowledge = I2C_Acknowledge_Disable;
	I2C_InitStruct.I2C_Broadcast = I2C_Broadcast_Enable;
	I2C_InitStruct.I2C_OwnAddress = 0x00;
	I2C_InitStruct.I2C_Prescaler = 3072;
	I2C_Init(I2Cx,&I2C_InitStruct);   
}
void CMix_InitIIC(void)
{
	I2C_GPIO_Config();
	I2C_Mode_Config(I2Cn);
}
void CMix_SystemInit(void)
{
    CMix_InitClocks();
//    CMix_InitGPIO();
    CMix_InitIIC();
//    CMix_ConfigPwmPins();
//    CMix_ConfigAdcPins();
    CMix_ConfigUartPins();
//    CMix_ConfigDebugPins();
//    CMix_ConfigMuxPins();
//    CMix_InitPWMTimers();
    CMix_InitADCSequence();
//    CMix_InitNTCMux();
    CMix_InitUART(115200U);
//    CMix_EnablePWMOutputs(false);
//    CMix_ScheduleADCConversion();
}

void CMix_InitClocks(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_APBPeriph1ClockCmd(RCC_APBPeriph1_TIM1, ENABLE);
    RCC_APBPeriph3ClockCmd(RCC_APBPeriph3_ADC0, ENABLE);
    RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_UART0, ENABLE);
    RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_I2C0, ENABLE);
	
    RCC_APBPeriph1ResetCmd(RCC_APBPeriph1_TIM1, ENABLE);
    RCC_APBPeriph1ResetCmd(RCC_APBPeriph1_TIM1, DISABLE);
    RCC_APBPeriph3ResetCmd(RCC_APBPeriph3_ADC0, ENABLE);
    RCC_APBPeriph3ResetCmd(RCC_APBPeriph3_ADC0, DISABLE);
    RCC_APBPeriph2ResetCmd(RCC_APBPeriph2_UART0, ENABLE);
    RCC_APBPeriph2ResetCmd(RCC_APBPeriph2_UART0, DISABLE);
}

void CMix_InitGPIO(void)
{
    GPIO_InitTypeDef init;
    GPIO_StructInit(&init);

    init.GPIO_Pin = GPIO_Pin_3;
    init.GPIO_Mode = GPIO_Mode_OutPP;
    init.GPIO_Pull = GPIO_Pull_NoPull;
    GPIO_Init(GPIOA, &init);
 //   GPIO_Init(GPIOB, &init);
}

void CMix_InitPWMTimers(void)
{
    TIM_TimeBaseInitTypeDef time_base;
    TIM_OCInitTypeDef oc;
    TIM_BKICRInitTypeDef bk;

    u32 timer_clk = GetClockFreq(CLKSRC_PCLK);
    u32 prescaler = 0U;
    u32 auto_reload = 0U;

    for (prescaler = 0U; prescaler <= 0xFFFFU; ++prescaler)
    {
        u32 divided_clk = timer_clk / (prescaler + 1U);
        if (divided_clk == 0U)
        {
            continue;
        }

        auto_reload = divided_clk / CMIX_PWM_FREQUENCY_HZ;
        if (auto_reload > 1U && auto_reload <= 0x10000U)
        {
            break;
        }
    }

    if (auto_reload <= 1U || auto_reload > 0x10000U)
    {
        prescaler = 63U;
        auto_reload = 1000U;
    }

    s_pwm_period_ticks = (uint16_t)(auto_reload - 1U);

    time_base.TIM_Prescaler = prescaler;
    time_base.TIM_AutoReloadValue = s_pwm_period_ticks;
    time_base.TIM_Direction = TIM_Direction_Up;
    time_base.TIM_CenterAlignedMode = TIM_CenterAlignedMode_Disable;
    TIM_TimeBaseInit(TIM1, &time_base);
    TIM_SetClockDivision(TIM1, TIM_ClockDiv_None);

    TIM_OCStructInit(&oc);
    oc.TIM_OCMode = TIM_OCMode_PWM1;
    oc.TIM_OCIdleState = TIM_OCIdleState_Low;
    oc.TIM_OCNIdleState = TIM_OCNIdleState_Low;
    oc.TIM_OCOutput = TIM_OCOutput_Enable;
    oc.TIM_OCNOutput = TIM_OCNOutput_Enable;
    oc.TIM_UpOCValue = s_pwm_period_ticks / 2U;
    oc.TIM_DownOCValue = s_pwm_period_ticks / 2U;

    oc.TIM_Channel = TIM_Channel_1;
    TIM_OCInit(TIM1, &oc);

    oc.TIM_Channel = TIM_Channel_2;
    TIM_OCInit(TIM1, &oc);

    TIM_BKICRStructInit(&bk);
    bk.TIM_Break = TIM_Break_Enable;
    bk.TIM_BreakPolarity = TIM_BreakPolarity_Low;
    bk.TIM_BreakSource = TIM_BreakSource_TIMxBKIN;
    bk.TIM_BreakInputControl = TIM_BreakInput_OutputIdle;
    bk.TIM_Breakfilter = TIM_Breakfilter_8;
    TIM_BKICRInit(TIM1, &bk);

    TIM_SetDeadTime(TIM1, CMIX_DEADTIME_TICKS);

    TIM_UpdateEvent(TIM1);
    TIM_Cmd(TIM1, ENABLE);
    TIM_SoftwareBreakCMD(TIM1, ENABLE);
    s_pwm_outputs_requested = false;
}

uint16_t CMix_GetPwmPeriodTicks(void)
{
    return s_pwm_period_ticks;
}

void CMix_UpdateBridgeDuty(uint16_t phase_a_ticks, uint16_t phase_b_ticks)
{
    uint16_t safe_a = CMix_ClampDutyTicks(phase_a_ticks);
    uint16_t safe_b = CMix_ClampDutyTicks(phase_b_ticks);

    TIM1->OCR[0] = ((u32)safe_a << 16) | safe_a;
    TIM1->OCR[1] = ((u32)safe_b << 16) | safe_b;
}

void CMix_EnablePWMOutputs(bool enable)
{
    if (enable)
    {
        if (!s_pwm_outputs_requested && (TIM_GetFlagStatus(TIM1, TIM_FLAG_BIF) == RESET))
        {
            TIM_SoftwareBreakCMD(TIM1, DISABLE);
            s_pwm_outputs_requested = true;
        }
    }
    else
    {
        if (s_pwm_outputs_requested)
        {
            TIM_SoftwareBreakCMD(TIM1, ENABLE);
            s_pwm_outputs_requested = false;
        }
    }
}

void CMix_InitADCSequence(void)
{
    ADC_InitTypeDef adc_init;
    size_t index;

    adc_init.ADC_Channel = s_adc_sequence[0].channel;
    adc_init.ADC_Mode = ADC_Mode_Single;
    adc_init.ADC_Prescaler = 30U;
    adc_init.ADC_RVSPS = ADC_RVSPS_VDDA;
    adc_init.ADC_SampleTime = CMIX_ADC_SAMPLE_TIME_CYCLES;
    adc_init.ADC_ChannelSetupTime = CMIX_ADC_SETUP_TIME_CYCLES;
    ADC_Init(ADC0, &adc_init);

    ADC_SampleTimeConfig(ADC0, CMIX_ADC_SAMPLE_TIME_CYCLES);
    ADC_ChannelSetupTimeConfig(ADC0, CMIX_ADC_SETUP_TIME_CYCLES);
    ADC_RegularTriggerSource(ADC0, ADC_RegularTriggerSource_Software);
    ADC_RegularScanCmd(ADC0, ENABLE);
    ADC_RSCNTConfig(ADC0, (u32)(sizeof(s_adc_sequence) / sizeof(s_adc_sequence[0])));

    for (index = 0U; index < (sizeof(s_adc_sequence) / sizeof(s_adc_sequence[0])); ++index)
    {
        ADC_ScanChannelConfig(ADC0, index, s_adc_sequence[index].channel);
    }

    ADC_Cmd(ADC0, ENABLE);
    CMix_WaitForAdcReady();
}

void CMix_ScheduleADCConversion(void)
{
    ADC_StartOfRegularConversion(ADC0);
}

void CMix_InitUART(uint32_t baudrate)
{
    UART_InitTypeDef uart_init;

    uart_init.UART_BaudRate = baudrate;
    uart_init.UART_DataLength = UART_DataLength_8;
    uart_init.UART_StopBitLength = UART_StopBitLength_1;
    uart_init.UART_ParityMode = UART_Parity_None;
    uart_init.UART_Receiver = UART_Receiver_Enable;
    uart_init.UART_SampleRate = UART_SampleRate_8X;
    UART_Init(UART0, &uart_init);
    UART_Cmd(UART0, ENABLE);

    // GPIO_DigitalRemapConfig(UART_TX_PORT, UART_TX_PIN, UART_TX_AFIO,ENABLE);	//UART_TX数字功能复用
	// GPIO_DigitalRemapConfig(UART_RX_PORT, UART_RX_PIN, UART_RX_AFIO,ENABLE);	//UART_RX数字功能复用
}

void CMix_InitNTCMux(void)
{
    CMix_SelectNTCChannel(false);
}

void CMix_SelectNTCChannel(bool select_ntc4)
{
    GPIO_TypeDef *gpio = CMix_GetGpio(&CMix_Pin_NTC3_Gpio);
    uint16_t mask = CMix_GetGpioPinMask(&CMix_Pin_NTC3_Gpio);

    if (gpio == NULL)
    {
        return;
    }

    s_ntc_mux_selects_ntc4 = select_ntc4;

    if (select_ntc4)
    {
        GPIO_SetBits(gpio, mask);
    }
    else
    {
        GPIO_ResetBits(gpio, mask);
    }
}

void CMix_ReadAnalogMeasurements(CMix_AnalogMeasurements *meas)
{
    size_t index;

    if (meas == NULL)
    {
        return;
    }

    if (ADC_GetFlagStatus(ADC0, ADC_FLAG_EOS) == RESET)
    {
        *meas = s_measurement_cache;
        return;
    }

    for (index = 0U; index < (sizeof(s_adc_sequence) / sizeof(s_adc_sequence[0])); ++index)
    {
        u16 raw_counts = ADC_GetRegularScanConversionValue(ADC0, index);
        float normalized = (float)raw_counts / CMIX_ADC_MAX_COUNTS;

        switch (index)
        {
            case 0:
                s_measurement_cache.i_bat = normalized;
                break;
            case 1:
                s_measurement_cache.i_out = normalized;
                break;
            case 2:
            case 3:
            case 4:
                s_measurement_cache.cell_voltage[index - 2U] = normalized;
                break;
            case 5:
                s_measurement_cache.v_out_bus = normalized;
                break;
            case 6:
                s_measurement_cache.v_pack_total = normalized;
                break;
            case 7:
                s_measurement_cache.ntc_temp[0] = normalized;
                break;
            case 8:
                s_measurement_cache.ntc_temp[1] = normalized;
                break;
            case 9:
                s_ntc_mux_cached[s_ntc_mux_selects_ntc4 ? 1U : 0U] = normalized;
                s_measurement_cache.ntc_temp[2] = s_ntc_mux_cached[0];
                s_measurement_cache.ntc_temp[3] = s_ntc_mux_cached[1];
                break;
            default:
                break;
        }
    }

    *meas = s_measurement_cache;
    ADC_ClearFlag(ADC0, ADC_FLAG_EOS | ADC_FLAG_EOC);
}

void CMix_UpdateBoardStatus(CMix_BoardStatus *status)
{
    if (status == NULL)
    {
        return;
    }

    status->fault_bkin_triggered = (TIM_GetFlagStatus(TIM1, TIM_FLAG_BIF) != RESET);

    status->fault_over_temperature = false;
    for (size_t i = 0U; i < (sizeof(s_measurement_cache.ntc_temp) / sizeof(s_measurement_cache.ntc_temp[0])); ++i)
    {
        if (s_measurement_cache.ntc_temp[i] > 0.95f)
        {
            status->fault_over_temperature = true;
            break;
        }
    }

    status->fault_comm_lost = false;
}

void CMix_ProcessFaults(void)
{
    if (TIM_GetFlagStatus(TIM1, TIM_FLAG_BIF) != RESET)
    {
        s_pwm_outputs_requested = false;
        TIM_SoftwareBreakCMD(TIM1, ENABLE);
    }
}

static GPIO_TypeDef *CMix_GetGpio(const CMix_PinConfig *pin)
{
    if (pin == NULL)
    {
        return NULL;
    }

    switch (pin->port)
    {
        case CMIX_PORT_PA:
            return GPIOA;
        case CMIX_PORT_PB:
            return GPIOB;
        default:
            return NULL;
    }
}

static AFIO_TypeDef *CMix_GetAfio(const CMix_PinConfig *pin)
{
    if (pin == NULL)
    {
        return NULL;
    }

    switch (pin->port)
    {
        case CMIX_PORT_PA:
            return AFIOA;
        case CMIX_PORT_PB:
            return AFIOB;
        default:
            return NULL;
    }
}

static uint16_t CMix_GetGpioPinMask(const CMix_PinConfig *pin)
{
    if (pin == NULL || pin->pin >= 16U)
    {
        return 0U;
    }

    return (uint16_t)(1U << pin->pin);
}

static void CMix_EnableAlternateFunction(const CMix_PinConfig *pin, u8 af_index)
{
    AFIO_TypeDef *afio = CMix_GetAfio(pin);
    uint16_t mask = CMix_GetGpioPinMask(pin);

    if (afio != NULL && mask != 0U)
    {
        GPIO_DigitalRemapConfig(afio, mask, af_index, ENABLE);
    }
}

static void CMix_EnableAnalogFunction(const CMix_PinConfig *pin)
{
    AFIO_TypeDef *afio = CMix_GetAfio(pin);
    uint16_t mask = CMix_GetGpioPinMask(pin);

    if (afio != NULL && mask != 0U)
    {
        GPIO_AnalogRemapConfig(afio, mask, ENABLE);
    }
}

static void CMix_ConfigPwmPins(void)
{
    CMix_EnableAlternateFunction(&CMix_Pin_PhaseA_PWM, AFIO_AF_3);
    CMix_EnableAlternateFunction(&CMix_Pin_PhaseA_PWMN, AFIO_AF_3);
    CMix_EnableAlternateFunction(&CMix_Pin_PhaseB_PWM, AFIO_AF_3);
    CMix_EnableAlternateFunction(&CMix_Pin_PhaseB_PWMN, AFIO_AF_3);
    CMix_EnableAlternateFunction(&CMix_Pin_BKIN, AFIO_AF_3);

    GPIO_TypeDef *bkin_port = CMix_GetGpio(&CMix_Pin_BKIN);
    uint16_t bkin_mask = CMix_GetGpioPinMask(&CMix_Pin_BKIN);
    if (bkin_port != NULL && bkin_mask != 0U)
    {
        GPIO_PullUpConfig(bkin_port, bkin_mask, ENABLE);
    }
}

static void CMix_ConfigAdcPins(void)
{
    for (size_t index = 0U; index < (sizeof(s_adc_sequence) / sizeof(s_adc_sequence[0])); ++index)
    {
        CMix_EnableAnalogFunction(s_adc_sequence[index].pin);
    }
}

static void CMix_ConfigUartPins(void)
{
    CMix_EnableAlternateFunction(&CMix_Pin_UART_Tx, AFIO_AF_5);
    CMix_EnableAlternateFunction(&CMix_Pin_UART_Rx, AFIO_AF_5);
}

static void CMix_ConfigDebugPins(void)
{
    (void)CMix_Pin_SWDIO;
    (void)CMix_Pin_SWCLK;
}

static void CMix_ConfigMuxPins(void)
{
    GPIO_InitTypeDef init;
    GPIO_TypeDef *gpio = CMix_GetGpio(&CMix_Pin_NTC3_Gpio);

    if (gpio == NULL)
    {
        return;
    }

    GPIO_StructInit(&init);
    init.GPIO_Pin = CMix_GetGpioPinMask(&CMix_Pin_NTC3_Gpio);
    init.GPIO_Mode = GPIO_Mode_OutPP;
    init.GPIO_Pull = GPIO_Pull_NoPull;
    GPIO_Init(gpio, &init);

    CMix_SelectNTCChannel(false);
}

static void CMix_WaitForAdcReady(void)
{
    u32 timeout = CMIX_ADC_TIMEOUT_ITERATIONS;
    while ((ADC_GetFlagStatus(ADC0, ADC_FLAG_RDY) == RESET) && (timeout-- > 0U))
    {
    }
}

static uint16_t CMix_ClampDutyTicks(uint16_t duty_ticks)
{
    if (duty_ticks > s_pwm_period_ticks)
    {
        return s_pwm_period_ticks;
    }
    return duty_ticks;
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, u32 line)
{
    (void)file;
    (void)line;
    while (1)
    {
        __NOP();
    }
}
#endif /* USE_FULL_ASSERT */

