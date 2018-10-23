/************************************************************************************
 * FC domain event unit
 *  Peripherals share the same IRQ entry
 * 
 * Author: hhuysqt <1020988872@qq.com>
 * 
 ************************************************************************************/

/*
 * uDMA event ID for IRQ
 **/
#define GAP8_UDMA_EVENT_LVDS_RX      0
#define GAP8_UDMA_EVENT_LVDS_TX      1
#define GAP8_UDMA_EVENT_SPIM0_RX     2
#define GAP8_UDMA_EVENT_SPIM0_TX     3
#define GAP8_UDMA_EVENT_SPIM1_RX     4
#define GAP8_UDMA_EVENT_SPIM1_TX     5
#define GAP8_UDMA_EVENT_HYPERBUS_RX  6
#define GAP8_UDMA_EVENT_HYPERBUS_TX  7
#define GAP8_UDMA_EVENT_UART_RX      8
#define GAP8_UDMA_EVENT_UART_TX      9
#define GAP8_UDMA_EVENT_I2C0_RX      10
#define GAP8_UDMA_EVENT_I2C0_TX      11
#define GAP8_UDMA_EVENT_I2C1_RX      12
#define GAP8_UDMA_EVENT_I2C1_TX      13
#define GAP8_UDMA_EVENT_TCDM_RX      14
#define GAP8_UDMA_EVENT_TCDM_TX      15
#define GAP8_UDMA_EVENT_SAI_CH0      16
#define GAP8_UDMA_EVENT_SAI_CH1      17
#define GAP8_UDMA_EVENT_CPI_RX       18

#define GAP8_UDMA_MIN_EVENT          0
#define GAP8_UDMA_MAX_EVENT          18
