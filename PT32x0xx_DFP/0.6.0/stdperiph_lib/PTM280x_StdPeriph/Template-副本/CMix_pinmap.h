#ifndef CMIX_PINMAP_H
#define CMIX_PINMAP_H

#include <stdint.h>

/**
 * Logical pin representation derived from PTM280x_C-Cube_Pinmap.csv.
 */
typedef enum
{
    CMIX_PORT_PA = 0,
    CMIX_PORT_PB,
    CMIX_PORT_PC,
    CMIX_PORT_PD,
    CMIX_PORT_INVALID
} CMix_Port;

typedef enum
{
    CMIX_PERIPH_GPIO = 0,
    CMIX_PERIPH_TIM1,
    CMIX_PERIPH_ADC0,
    CMIX_PERIPH_UART0,
    CMIX_PERIPH_SWD,
    CMIX_PERIPH_EXTERNAL
} CMix_Peripheral;

typedef enum
{
    CMIX_AF_NONE = 0,
    CMIX_AF_TIM1,
    CMIX_AF_UART0,
    CMIX_AF_SWD,
    CMIX_AF_ANALOG
} CMix_AlternateFunction;

typedef struct
{
    CMix_Port port;           /* MCU GPIO port */
    uint8_t pin;              /* MCU GPIO pin index */
    CMix_Peripheral periph;   /* Logical peripheral mapping */
    CMix_AlternateFunction af;/* Alternate function selection */
    uint8_t channel;          /* Peripheral channel (timer/adc) */
    const char *description;  /* Human readable note */
} CMix_PinConfig;

extern const CMix_PinConfig CMix_Pin_PhaseA_PWM;
extern const CMix_PinConfig CMix_Pin_PhaseA_PWMN;
extern const CMix_PinConfig CMix_Pin_PhaseB_PWM;
extern const CMix_PinConfig CMix_Pin_PhaseB_PWMN;
extern const CMix_PinConfig CMix_Pin_BKIN;
extern const CMix_PinConfig CMix_Pin_IBat_Sense;
extern const CMix_PinConfig CMix_Pin_IOut_Sense;
extern const CMix_PinConfig CMix_Pin_Cell1_Tap;
extern const CMix_PinConfig CMix_Pin_Cell2_Tap;
extern const CMix_PinConfig CMix_Pin_Cell3_Tap;
extern const CMix_PinConfig CMix_Pin_VOut_Bus;
extern const CMix_PinConfig CMix_Pin_VPack_Total;
extern const CMix_PinConfig CMix_Pin_NTC1;
extern const CMix_PinConfig CMix_Pin_NTC2;
extern const CMix_PinConfig CMix_Pin_NTC3_Gpio;
extern const CMix_PinConfig CMix_Pin_NTC_Mux_Out;
extern const CMix_PinConfig CMix_Pin_UART_Tx;
extern const CMix_PinConfig CMix_Pin_UART_Rx;
extern const CMix_PinConfig CMix_Pin_SWDIO;
extern const CMix_PinConfig CMix_Pin_SWCLK;
extern const CMix_PinConfig CMix_Pin_NTC4_External;

#endif /* CMIX_PINMAP_H */
