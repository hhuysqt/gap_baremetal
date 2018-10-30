/************************************************************************************
 * uDMA driver for GAP8
 *  GAP8 features a simple uDMA subsystem. Peripherals including UART, SPI, I2C, I2S,
 *  CPI, LVDS, Hyperbus, have config registers memory-mapped, but not data registers.
 *  The only way to send or receive data is using the uDMA. These peripherals share 
 *  the same uDMA ISR.
 * 
 *  uDMA sybsystem drivers are object oriented to some extend. Peripherals inherit
 *  the udma class, which handels all the data exchange stuff.
 * 
 * Author: hhuysqt <1020988872@qq.com>
 * 
 ************************************************************************************/
#ifndef _ARCH_RISCV_SRC_GAP8_UDMA_H
#define _ARCH_RISCV_SRC_GAP8_UDMA_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include "GAP8.h"
#include "gap8_interrupt.h"
#include <stdbool.h>

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/*
 * uDMA channel ID
 **/
#define GAP8_UDMA_ID_LVDS   0
#define GAP8_UDMA_ID_SPIM0  1
#define GAP8_UDMA_ID_SPIM1  2
#define GAP8_UDMA_ID_HYPER  3
#define GAP8_UDMA_ID_UART   4
#define GAP8_UDMA_ID_I2C0   5
#define GAP8_UDMA_ID_I2C1   6
#define GAP8_UDMA_ID_TCDM   7   /* l2 to fc-l1 */
#define GAP8_UDMA_ID_I2S    8
#define GAP8_UDMA_ID_CPI    9

/* Total udma channels */
#define GAP8_UDMA_NR_CHANNELS  10

/************************************************************************************
 * Public Types
 ************************************************************************************/

/*
 * Software abstraction for uDMA
 **/

/*
 * One round of data exchange on one channel gathered into linked list because
 * threads would request for data exchange simultaneously.
 * Private for udma driver.
 **/
struct __udma_queue {
  uint8_t   *buff;        /* Memory address. either TX or RX  */
  uint32_t  block_size;   /* Size of a data block in bytes    */
  int  block_count;  /* Number of blocks to send or recv */
};

/*
 * This is the base class of uDMA subsystem. Peripherals connected to uDMA
 * should inherited this class.
 **/
struct gap8_udma_peripheral {
  /* public */

  UDMA_reg_t   *regs;           /* Hardware config regs */
  uint32_t      id;             /*    GAP8_UDMA_ID_x    */
  void (*on_tx)(struct gap8_udma_peripheral *arg);     /* tx callback */
  void (*on_rx)(struct gap8_udma_peripheral *arg);     /* rx callback */

  /* private */

  struct __udma_queue tx;        /* TX queue */
  struct __udma_queue rx;        /* RX queue */

  // TODO: semaphores
};


/************************************************************************************
 * Public Function Prototypes
 ************************************************************************************/

/************************************************************************************
 * Name: gap8_udma_init
 *
 * Description:
 *   Initialize (and enable) a udma peripheral.
 * 
 * Input:
 *   instance: pointer to a peripheral instance connected to uDMA
 * 
 ************************************************************************************/

int gap8_udma_init(struct gap8_udma_peripheral *instance);

/************************************************************************************
 * Name: gap8_udma_deinit
 * 
 * Description:
 *   Deinit a udma peripheral
 * 
 ************************************************************************************/

int gap8_udma_deinit(struct gap8_udma_peripheral *instance);

/************************************************************************************
 * Name: gap8_udma_tx_setirq
 *
 * Description:
 *   Enable or disable the tx interrupt.
 * 
 ************************************************************************************/

int gap8_udma_tx_setirq(struct gap8_udma_peripheral *instance, bool enable);

/************************************************************************************
 * Name: gap8_udma_rx_setirq
 *
 * Description:
 *   Enable or disable the rx interrupt.
 * 
 ************************************************************************************/

int gap8_udma_rx_setirq(struct gap8_udma_peripheral *instance, bool enable);

/************************************************************************************
 * Name: gap8_udma_tx_start
 * 
 * Description:
 *   Send size * count bytes non-blocking.
 * 
 * Return ERROR if unable to send. The caller should poll on execution, or register
 * a on_tx to get the signal.
 * 
 ************************************************************************************/

int gap8_udma_tx_start(struct gap8_udma_peripheral *instance, 
                   uint8_t *buff, uint32_t size, int count);

/************************************************************************************
 * Name: gap8_udma_rx_start
 * 
 * Description:
 *   Receive size * count bytes
 * 
 * Return ERROR if unable to send. The caller should poll on execution, or register
 * a on_rx to get the signal.
 * 
 ************************************************************************************/

int gap8_udma_rx_start(struct gap8_udma_peripheral *instance,
                   uint8_t *buff, uint32_t size, int count);

/************************************************************************************
 * Name: gap8_udma_tx_poll
 * 
 * Description:
 *   Return OK if tx finished.
 * 
 ************************************************************************************/

int gap8_udma_tx_poll(struct gap8_udma_peripheral *instance);

/************************************************************************************
 * Name: gap8_udma_rx_poll
 * 
 * Description:
 *   Return OK if rx finished.
 * 
 ************************************************************************************/

int gap8_udma_rx_poll(struct gap8_udma_peripheral *instance);

/************************************************************************************
 * Name: gap8_udma_doirq
 * 
 * Description:
 *   uDMA ISR
 * 
 ************************************************************************************/

void gap8_udma_doirq(uint32_t irqn);

#endif