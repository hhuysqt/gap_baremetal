/************************************************************************************
 * UART driver on uDMA subsystem for GAP8
 *  This UART IP has no flow control. So ioctl is limited.
 * 
 * Author: hhuysqt <1020988872@qq.com>
 * 
 ************************************************************************************/
#ifndef _ARCH_RISCV_SRC_GAP8_UART_H
#define _ARCH_RISCV_SRC_GAP8_UART_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include "GAP8.h"
#include "gap8_udma.h"
#include "gap8_gpio.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

#define GAP8_NR_UART 1

/************************************************************************************
 * Public Types
 ************************************************************************************/

/* Software abstraction
 * inherit class _udma_peripheral
 **/

struct gap8_uart_t {
  struct gap8_udma_peripheral udma;

  /* private */

  uint32_t tx_gpio;
  uint32_t rx_gpio;
  uint32_t coreclock;
  uint32_t baud;
  uint8_t  nr_bits;
  uint8_t  parity_enable;
  uint8_t  stop_bits;
};

/************************************************************************************
 * Public Function Prototypes
 ************************************************************************************/

/* Tests */
struct gap8_uart_t * gap8_uart_initialize(int n);
void gap8_uart_setbaud(struct gap8_uart_t *uart, uint32_t baud, uint32_t clock);
void gap8_uart_sendbytes(struct gap8_uart_t *uart, uint8_t *buff, uint32_t nbytes);
void gap8_uart_recvbytes(struct gap8_uart_t *uart, uint8_t *buff, uint32_t nbytes);

#endif
