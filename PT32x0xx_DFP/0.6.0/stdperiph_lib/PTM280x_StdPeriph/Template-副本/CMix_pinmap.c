#include "CMix_pinmap.h"

const CMix_PinConfig CMix_Pin_PhaseA_PWM = {
    CMIX_PORT_PA,
    0U,
    CMIX_PERIPH_TIM1,
    CMIX_AF_TIM1,
    1U,
    "TIM1_CH1 Phase-A PWM"
};

const CMix_PinConfig CMix_Pin_PhaseA_PWMN = {
    CMIX_PORT_PA,
    7U,
    CMIX_PERIPH_TIM1,
    CMIX_AF_TIM1,
    1U,
    "TIM1_CH1N Phase-A complementary PWM"
};

const CMix_PinConfig CMix_Pin_PhaseB_PWM = {
    CMIX_PORT_PA,
    1U,
    CMIX_PERIPH_TIM1,
    CMIX_AF_TIM1,
    2U,
    "TIM1_CH2 Phase-B PWM"
};

const CMix_PinConfig CMix_Pin_PhaseB_PWMN = {
    CMIX_PORT_PB,
    1U,
    CMIX_PERIPH_TIM1,
    CMIX_AF_TIM1,
    2U,
    "TIM1_CH2N Phase-B complementary PWM"
};

const CMix_PinConfig CMix_Pin_BKIN = {
    CMIX_PORT_PA,
    11U,
    CMIX_PERIPH_TIM1,
    CMIX_AF_TIM1,
    0U,
    "TIM1_BKIN hardware brake"
};

const CMix_PinConfig CMix_Pin_IBat_Sense = {
    CMIX_PORT_PB,
    9U,
    CMIX_PERIPH_ADC0,
    CMIX_AF_ANALOG,
    58U,
    "ADC0_IN0 battery current sense"
};

const CMix_PinConfig CMix_Pin_IOut_Sense = {
    CMIX_PORT_PA,
    5U,
    CMIX_PERIPH_ADC0,
    CMIX_AF_ANALOG,
    5U,
    "ADC0_IN1 output current sense"
};

// const CMix_PinConfig CMix_Pin_Cell1_Tap = {
//     CMIX_PORT_PA,
//     2U,
//     CMIX_PERIPH_ADC0,
//     CMIX_AF_ANALOG,
//     2U,
//     "ADC0_IN2 cell1 voltage"
// };

// const CMix_PinConfig CMix_Pin_Cell2_Tap = {
//     CMIX_PORT_PA,
//     3U,
//     CMIX_PERIPH_ADC0,
//     CMIX_AF_ANALOG,
//     3U,
//     "ADC0_IN3 cell2 voltage"
// };

// const CMix_PinConfig CMix_Pin_Cell3_Tap = {
//     CMIX_PORT_PA,
//     4U,
//     CMIX_PERIPH_ADC0,
//     CMIX_AF_ANALOG,
//     4U,
//     "ADC0_IN4 cell3 voltage"
// };

const CMix_PinConfig CMix_Pin_VOut_Bus = {
    CMIX_PORT_PB,
    13U,
    CMIX_PERIPH_ADC0,
    CMIX_AF_ANALOG,
    1U,
    "ADC0_IN5 boost bus voltage"
};
//already checked
const CMix_PinConfig CMix_Pin_VPack_Total = {
    CMIX_PORT_PA,
    6U,
    CMIX_PERIPH_ADC0,
    CMIX_AF_ANALOG,
    6U,
    "ADC0_IN6 pack total voltage"
};

const CMix_PinConfig CMix_Pin_NTC1 = {
    CMIX_PORT_PB,
    5U,
    CMIX_PERIPH_ADC0,
    CMIX_AF_ANALOG,
    9U,
    "ADC0_IN9 thermistor 1"
};

const CMix_PinConfig CMix_Pin_NTC2 = {
    CMIX_PORT_PB,
    4U,
    CMIX_PERIPH_ADC0,
    CMIX_AF_ANALOG,
    8U,
    "ADC0_IN8 thermistor 2"
};

const CMix_PinConfig CMix_Pin_NTC3_Gpio = {
    CMIX_PORT_PB,
    3U,
    CMIX_PERIPH_GPIO,
    CMIX_AF_NONE,
    0U,
    "GPIO selector for NTC3/NTC4 mux"
};

const CMix_PinConfig CMix_Pin_NTC3 = {
    CMIX_PORT_PB,
    6U,
    CMIX_PERIPH_ADC0,
    CMIX_AF_ANALOG,
    10U,
    "ADC0_IN10 NTC3/4 mux output"
};

const CMix_PinConfig CMix_Pin_UART_Tx = {
    CMIX_PORT_PA,
    8U,
    CMIX_PERIPH_UART0,
    CMIX_AF_UART0,
    0U,
    "UART0_TX host interface"
};

const CMix_PinConfig CMix_Pin_UART_Rx = {
    CMIX_PORT_PB,
    2U,
    CMIX_PERIPH_UART0,
    CMIX_AF_UART0,
    0U,
    "UART0_RX host interface"
};

const CMix_PinConfig CMix_Pin_SWDIO = {
    CMIX_PORT_PA,
    13U,
    CMIX_PERIPH_SWD,
    CMIX_AF_SWD,
    0U,
    "SWDIO debug"
};

const CMix_PinConfig CMix_Pin_SWCLK = {
    CMIX_PORT_PA,
    14U,
    CMIX_PERIPH_SWD,
    CMIX_AF_SWD,
    0U,
    "SWCLK debug"
};

const CMix_PinConfig CMix_Pin_NTC4_External = {
    CMIX_PORT_INVALID,
    0U,
    CMIX_PERIPH_EXTERNAL,
    CMIX_AF_ANALOG,
    0U,
    "External mux input for NTC4"
};
