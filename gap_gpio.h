/************************************************************************************
 * PIN driver for GAP8
 *  GAP8 has only 1 port. Each pin could be configured to GPIO or alternative
 *  functions.
 * 
 * Author: hhuysqt <1020988872@qq.com>
 * 
 ************************************************************************************/
#ifndef _ARCH_RISCV_SRC_GAP8_GPIO_H
#define _ARCH_RISCV_SRC_GAP8_GPIO_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include "GAP8.h"
#include <stdint.h>
#include <stdbool.h>


/************************************************************************************
 * Pre-Processor Declarations
 ************************************************************************************/

#define MAX_PIN_NUM   47

/*
 * GPIO software abstraction: bitmap configuration of pins
 * 
 * |31 18|   17  |     16     |15              8|7      0|
 * | --- | drive | pull-up/OD | alt-func-number | pinnum |
 * | --- | 1-bit |   1-bit    |      2-bit      |  8-bit |
 **/
#define GAP8_PIN_SPEED_HIGH   (1L << 17)
#define GAP8_PIN_SPEED_LOW    (0L << 17)

#define GAP8_PIN_PULL_UP      (1L << 16)
#define GAP8_PIN_PULL_NONE    (0L << 16)

#define GAP8_PIN_A4_SPIM1_MISO    ((0L << 8) | 0)
#define GAP8_PIN_A4_GPIOA0        ((1L << 8) | 0)

#define GAP8_PIN_B3_SPIM1_MOSI    ((0L << 8) | 1)
#define GAP8_PIN_B3_GPIOA1        ((1L << 8) | 1)

#define GAP8_PIN_A5_SPIM1_CS0     ((0L << 8) | 2)
#define GAP8_PIN_A5_GPIOA2        ((1L << 8) | 2)
#define GAP8_PIN_A5_I2C1_SDA      ((2L << 8) | 2)

#define GAP8_PIN_B4_SPIM1_SCK     ((0L << 8) | 3)
#define GAP8_PIN_B4_GPIOA3        ((1L << 8) | 3)
#define GAP8_PIN_B4_I2C1_SCL      ((2L << 8) | 3)

#define GAP8_PIN_A3_ORCA_TXSYNC   ((0L << 8) | 4)
#define GAP8_PIN_A3_GPIOA0        ((1L << 8) | 4)
#define GAP8_PIN_A3_SPIM1_CS0     ((2L << 8) | 4)

#define GAP8_PIN_B2_ORCA_RXSYNC   ((0L << 8) | 5)
#define GAP8_PIN_B2_GPIOA1        ((1L << 8) | 5)
#define GAP8_PIN_B2_SPIM1_CS1     ((2L << 8) | 5)

#define GAP8_PIN_A2_ORCA_TX1      ((0L << 8) | 6)
#define GAP8_PIN_A2_GPIOA2        ((1L << 8) | 6)

#define GAP8_PIN_B1_ORCA_TXQ      ((0L << 8) | 7)
#define GAP8_PIN_B1_GPIOA3        ((1L << 8) | 7)

#define GAP8_PIN_A44_ORCA_RXI     ((0L << 8) | 8)
#define GAP8_PIN_A44_GPIOA4       ((1L << 8) | 8)
#define GAP8_PIN_A44_SPIS0_D0     ((2L << 8) | 8)
#define GAP8_PIN_A44_SPIS0_D2     ((3L << 8) | 8)

#define GAP8_PIN_B40_ORCA_RXQ     ((0L << 8) | 9)
#define GAP8_PIN_B40_GPIOA5       ((1L << 8) | 9)
#define GAP8_PIN_B40_SPIS0_D1     ((2L << 8) | 9)
#define GAP8_PIN_B40_SPIS0_D3     ((3L << 8) | 9)

#define GAP8_PIN_A43_CAM_PCLK     ((0L << 8) | 10)
#define GAP8_PIN_A43_GPIOA4       ((1L << 8) | 10)
#define GAP8_PIN_A43_TIM1_CH0     ((2L << 8) | 10)

#define GAP8_PIN_A37_CAM_HSYNC    ((0L << 8) | 11)
#define GAP8_PIN_A37_GPIOA5       ((1L << 8) | 11)
#define GAP8_PIN_A37_TIM1_CH1     ((2L << 8) | 11)

#define GAP8_PIN_B39_CAM_D0       ((0L << 8) | 12)
#define GAP8_PIN_B39_GPIOA6       ((1L << 8) | 12)
#define GAP8_PIN_B39_TIM1_CH2     ((2L << 8) | 12)

#define GAP8_PIN_A42_CAM_D1       ((0L << 8) | 13)
#define GAP8_PIN_A42_GPIOA7       ((1L << 8) | 13)
#define GAP8_PIN_A42_TIM1_CH3     ((2L << 8) | 13)

#define GAP8_PIN_B38_CAM_D2       ((0L << 8) | 14)
#define GAP8_PIN_B38_GPIOA8       ((1L << 8) | 14)
#define GAP8_PIN_B38_TIM2_CH0     ((2L << 8) | 14)

#define GAP8_PIN_A41_CAM_D3       ((0L << 8) | 15)
#define GAP8_PIN_A41_GPIOA9       ((1L << 8) | 15)
#define GAP8_PIN_A41_TIM2_CH1     ((2L << 8) | 15)

#define GAP8_PIN_B37_CAM_D4       ((0L << 8) | 16)
#define GAP8_PIN_B37_GPIOA10      ((1L << 8) | 16)
#define GAP8_PIN_B37_TIM2_CH2     ((2L << 8) | 16)

#define GAP8_PIN_A40_CAM_D5       ((0L << 8) | 17)
#define GAP8_PIN_A40_GPIOA11      ((1L << 8) | 17)
#define GAP8_PIN_A40_TIM2_CH3     ((2L << 8) | 17)

#define GAP8_PIN_B36_CAM_D6       ((0L << 8) | 18)
#define GAP8_PIN_B36_GPIOA12      ((1L << 8) | 18)
#define GAP8_PIN_B36_TIM3_CH0     ((2L << 8) | 18)

#define GAP8_PIN_A38_CAM_D7       ((0L << 8) | 19)
#define GAP8_PIN_A38_GPIOA13      ((1L << 8) | 19)
#define GAP8_PIN_A38_TIM3_CH1     ((2L << 8) | 19)

#define GAP8_PIN_A36_CAM_VSYNC    ((0L << 8) | 20)
#define GAP8_PIN_A36_GPIOA14      ((1L << 8) | 20)
#define GAP8_PIN_A36_TIM3_CH2     ((2L << 8) | 20)

#define GAP8_PIN_B34_I2C1_SDA     ((0L << 8) | 21)
#define GAP8_PIN_B34_GPIOA15      ((1L << 8) | 21)
#define GAP8_PIN_B34_TIM3_CH3     ((2L << 8) | 21)

#define GAP8_PIN_D1_I2C1_SCL      ((0L << 8) | 22)
#define GAP8_PIN_D1_GPIOA16       ((1L << 8) | 22)
#define GAP8_PIN_D1_ORCA_CLK      ((2L << 8) | 22)

#define GAP8_PIN_B11_TIM0_CH0     ((0L << 8) | 23)
#define GAP8_PIN_B11_GPIOA17      ((1L << 8) | 23)

#define GAP8_PIN_A13_TIM0_CH1     ((0L << 8) | 24)
#define GAP8_PIN_A13_GPIOA18      ((1L << 8) | 24)
#define GAP8_PIN_A13_TIM1_CH0     ((2L << 8) | 24)

#define GAP8_PIN_B12_TIM0_CH2     ((0L << 8) | 25)
#define GAP8_PIN_B12_GPIOA19      ((1L << 8) | 25)
#define GAP8_PIN_B12_TIM2_CH0     ((2L << 8) | 25)

#define GAP8_PIN_A14_TIM0_CH3     ((0L << 8) | 26)
#define GAP8_PIN_A14_GPIOA20      ((1L << 8) | 26)
#define GAP8_PIN_A14_TIM3_CH0     ((2L << 8) | 26)

#define GAP8_PIN_B13_I2S1_SCK     ((0L << 8) | 27)
#define GAP8_PIN_B13_GPIOA21      ((1L << 8) | 27)
#define GAP8_PIN_B13_SPIS0_SCK    ((2L << 8) | 27)
#define GAP8_PIN_B13_I2S1_SDI     ((3L << 8) | 27)

#define GAP8_PIN_A15_I2S1_WS      ((0L << 8) | 28)
#define GAP8_PIN_A15_GPIOA22      ((1L << 8) | 28)
#define GAP8_PIN_A15_SPIS0_CS     ((2L << 8) | 28)
#define GAP8_PIN_A15_HYPER_CKN    ((3L << 8) | 28)

#define GAP8_PIN_B14_I2S1_SDI     ((0L << 8) | 29)
#define GAP8_PIN_B14_GPIOA23      ((1L << 8) | 29)
#define GAP8_PIN_B14_SPIS0_D2     ((2L << 8) | 29)
#define GAP8_PIN_B14_HYPER_CK     ((3L << 8) | 29)

#define GAP8_PIN_B6_UART_RX       ((0L << 8) | 30)
#define GAP8_PIN_B6_GPIOA24       ((1L << 8) | 30)

#define GAP8_PIN_A7_UART_TX       ((0L << 8) | 31)
#define GAP8_PIN_A7_GPIOA25       ((1L << 8) | 31)

#define GAP8_PIN_D2_SPIM0_D0      ((0L << 8) | 32)
#define GAP8_PIN_D2_HYPER_D0      ((3L << 8) | 32)

#define GAP8_PIN_A11_SPIM0_D1     ((0L << 8) | 33)
#define GAP8_PIN_A11_HYPER_D1     ((3L << 8) | 33)

#define GAP8_PIN_B10_SPIM0_D2     ((0L << 8) | 34)
#define GAP8_PIN_B10_GPIOA26      ((1L << 8) | 34)
#define GAP8_PIN_B10_I2C1_SDA     ((2L << 8) | 34)
#define GAP8_PIN_B10_HYPER_D2     ((3L << 8) | 34)

#define GAP8_PIN_A10_SPIM0_D3     ((0L << 8) | 35)
#define GAP8_PIN_A10_GPIOA27      ((1L << 8) | 35)
#define GAP8_PIN_A10_I2C1_SCL     ((2L << 8) | 35)
#define GAP8_PIN_A10_HYPER_D3     ((3L << 8) | 35)

#define GAP8_PIN_B8_SPIM0_CS0     ((0L << 8) | 36)
#define GAP8_PIN_B8_HYPER_D4      ((3L << 8) | 36)

#define GAP8_PIN_A8_SPIM0_CS1     ((0L << 8) | 37)
#define GAP8_PIN_A8_GPIOA28       ((1L << 8) | 37)
#define GAP8_PIN_A8_SPIS0_D3      ((2L << 8) | 37)
#define GAP8_PIN_A8_HYPER_D5      ((3L << 8) | 37)

#define GAP8_PIN_B7_SPIM0_SCK     ((0L << 8) | 38)
#define GAP8_PIN_B7_HYPER_D6      ((3L << 8) | 38)

#define GAP8_PIN_A9_SPIS0_CS      ((0L << 8) | 39)
#define GAP8_PIN_A9_GPIOA29       ((1L << 8) | 39)
#define GAP8_PIN_A9_SPIM1_CS0     ((2L << 8) | 39)
#define GAP8_PIN_A9_HYPER_D7      ((3L << 8) | 39)

#define GAP8_PIN_B15_SPIS0_D0     ((0L << 8) | 40)
#define GAP8_PIN_B15_GPIOA30      ((1L << 8) | 40)
#define GAP8_PIN_B15_SPIM1_CS1    ((2L << 8) | 40)
#define GAP8_PIN_B15_HYPER_CS0    ((3L << 8) | 40)

#define GAP8_PIN_A16_SPIS0_D1     ((0L << 8) | 41)
#define GAP8_PIN_A16_GPIOA31      ((1L << 8) | 41)
#define GAP8_PIN_A16_HYPER_CS1    ((3L << 8) | 41)

#define GAP8_PIN_B9_SPIS0_SCK     ((0L << 8) | 42)
#define GAP8_PIN_B9_HYPER_RWDS    ((3L << 8) | 42)

#define GAP8_PIN_B22_I2C0_SDA     ((0L << 8) | 43)
#define GAP8_PIN_A25_I2C0_SCL     ((0L << 8) | 44)
#define GAP8_PIN_A24_I2S0_SCK     ((0L << 8) | 45)
#define GAP8_PIN_A26_I2S0_WS      ((0L << 8) | 46)
#define GAP8_PIN_B23_I2S0_SDI     ((0L << 8) | 47)


/************************************************************************************
 * Public Function Prototypes
 ************************************************************************************/

/************************************************************************************
 * Name: gap8_configpin
 *
 * Description:
 *   Configure a pin based on bit-encoded description of the pin.
 *
 * Returned Value:
 *   OK on success
 *   ERROR on invalid port, or when pin is locked as ALT function.
 *
 ************************************************************************************/

int gap8_configpin(uint32_t cfgset);

/************************************************************************************
 * Name: gap8_gpiowrite
 *
 * Description:
 *   Write one or zero to the selected GPIO pin
 *
 ************************************************************************************/

void gap8_gpiowrite(uint32_t pinset, bool value);

/************************************************************************************
 * Name: gap8_gpioread
 *
 * Description:
 *   Read one or zero from the selected GPIO pin
 *
 ************************************************************************************/

bool gap8_gpioread(uint32_t pinset);

/************************************************************************************
 * Name: gap8_gpioirqset
 *
 * Description:
 *   Enable or disable interrupt on GPIO
 *
 ************************************************************************************/

bool gap8_gpioirqset(uint32_t pinset, bool enable);


#endif
