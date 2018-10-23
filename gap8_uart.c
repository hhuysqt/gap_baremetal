/************************************************************************************
 * UART driver on uDMA subsystem for GAP8
 *  This UART IP has no flow control. So ioctl is limited.
 * 
 * Author: hhuysqt <1020988872@qq.com>
 * 
 ************************************************************************************/

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include "gap8_uart.h"
#include <stddef.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/


/****************************************************************************
 * Private Function prototype
 ****************************************************************************/

/* uart ISR routine */
static void uart_tx_isr(struct gap8_udma_peripheral *arg);
static void uart_rx_isr(struct gap8_udma_peripheral *arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* instantiate the UART */

static struct gap8_uart_t uarts[GAP8_NR_UART] = {
  {
    .udma = {
      .regs  = (UDMA_reg_t*)UART,
      .id    = GAP8_UDMA_ID_UART,
      .on_tx = uart_tx_isr,
      .on_rx = uart_rx_isr,
    },

    .tx_gpio = GAP8_PIN_A7_UART_TX | GAP8_PIN_PULL_UP | GAP8_PIN_SPEED_HIGH,
    .rx_gpio = GAP8_PIN_B6_UART_RX | GAP8_PIN_PULL_UP | GAP8_PIN_SPEED_HIGH,
    .coreclock = 0, // to be modified
    .baud = 115200,
    .nr_bits = 8,
    .parity_enable = 0,
    .stop_bits = 1,
  }
};

/****************************************************************************
 * Private Function prototype
 ****************************************************************************/
static void uart_tx_isr(struct gap8_udma_peripheral *arg)
{

}

static void uart_rx_isr(struct gap8_udma_peripheral *arg)
{

}


/****************************************************************************
 * Public Functions
 ****************************************************************************/

/* Tests */
struct gap8_uart_t * gap8_uart_initialize(int n)
{
  struct gap8_uart_t *the_uart;
  UART_reg_t *uartreg;
  uint32_t cfgreg = 0;

  if (n > GAP8_NR_UART)
    return NULL;

  the_uart = &uarts[n];
  uartreg = (UART_reg_t*)&the_uart->udma;

  gap8_udma_init(&the_uart->udma);

  /* Setup baudrate etc. */
  cfgreg = UART_SETUP_BIT_LENGTH(the_uart->nr_bits - 5) | 
           UART_SETUP_PARITY_ENA(the_uart->parity_enable & 0x1) |
           UART_SETUP_STOP_BITS(the_uart->stop_bits - 1) |
           UART_SETUP_TX_ENA(1) | 
           UART_SETUP_RX_ENA(1);
  uartreg->SETUP = cfgreg;

  gap8_configpin(the_uart->tx_gpio);
  gap8_configpin(the_uart->rx_gpio);

  gap8_udma_tx_setirq(&the_uart->udma, 1);
  gap8_udma_rx_setirq(&the_uart->udma, 1);
}

void gap8_uart_setbaud(struct gap8_uart_t *uart, uint32_t baud, uint32_t baseclock)
{
  uint16_t div = baseclock / baud;
  UART_reg_t *uartreg = (UART_reg_t*)&uart->udma;

  uartreg->SETUP = (uartreg->SETUP & ~(UART_SETUP_CLKDIV_MASK)) | UART_SETUP_CLKDIV(div);

  uart->coreclock = baseclock;
  uart->baud = baud;
}

void gap8_uart_sendbytes(struct gap8_uart_t *uart, uint8_t *buff, uint32_t nbytes)
{
  struct gap8_udma_peripheral *theudma = (struct gap8_udma_peripheral*)&uart->udma;

  gap8_udma_tx_start(theudma, buff, nbytes, 1);
  while (gap8_udma_tx_poll(theudma, buff) != OK)
    {
      // sleep
    }
}

void gap8_uart_recvbytes(struct gap8_uart_t *uart, uint8_t *buff, uint32_t nbytes);

