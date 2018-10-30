/************************************************************************************
 * uDMA driver for GAP8
 *  GAP8 features a simple uDMA subsystem. Peripherals including UART, SPI, I2C, I2S,
 *  CPI, LVDS, Hyperbus, have config registers memory-mapped, but not data registers.
 *  The only way to send or receive data is using the uDMA. Those peripherals share 
 *  the same uDMA ISR.
 * 
 *  Note that uDMA can only recognize L2 RAM. So data must not be stored at L1, which
 *  means that if you link the stack at L1, you cannot use local buffers as data src 
 *  or destination.
 * 
 * Author: hhuysqt <1020988872@qq.com>
 * 
 ************************************************************************************/

#include "gap8_udma.h"
#include <stddef.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define CHECK_CHANNEL_ID(INSTANCE) \
  if ((INSTANCE) == NULL || \
      (INSTANCE)->id >= GAP8_UDMA_NR_CHANNELS) \
    { \
      return ERROR; \
    }

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* uDMA peripheral instances
 * The peripheral driver instanciate it and register through _init()
 **/
static struct gap8_udma_peripheral *_peripherals[GAP8_UDMA_NR_CHANNELS] = { 0 };


/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void _dma_txstart(struct gap8_udma_peripheral *the_peri)
{
  the_peri->regs->TX_SADDR = (uint32_t)the_peri->tx.buff;
  the_peri->regs->TX_SIZE  = (uint32_t)the_peri->tx.block_size;
  the_peri->regs->TX_CFG   = UDMA_CFG_EN(1);
}

static void _dma_rxstart(struct gap8_udma_peripheral *the_peri)
{
  the_peri->regs->RX_SADDR = (uint32_t)the_peri->rx.buff;
  the_peri->regs->RX_SIZE  = (uint32_t)the_peri->rx.block_size;
  the_peri->regs->RX_CFG   = UDMA_CFG_EN(1);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

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

int gap8_udma_init(struct gap8_udma_peripheral *instance)
{
  uint32_t id;

  CHECK_CHANNEL_ID(instance)

  id = instance->id;
  _peripherals[id] = instance;

  /* Enable clock gating */

  UDMA_GC->CG |= (1L << id);
}

/************************************************************************************
 * Name: gap8_udma_deinit
 * 
 * Description:
 *   Deinit a udma peripheral
 * 
 ************************************************************************************/

int gap8_udma_deinit(struct gap8_udma_peripheral *instance)
{
  uint32_t id;

  CHECK_CHANNEL_ID(instance)

  id = instance->id;
  _peripherals[id] = NULL;

  /* Disable clock gating */

  UDMA_GC->CG &= ~(1L << id);
}

/************************************************************************************
 * Name: gap8_udma_tx_setirq
 *
 * Description:
 *   Enable or disable the tx interrupt.
 * 
 ************************************************************************************/

int gap8_udma_tx_setirq(struct gap8_udma_peripheral *instance, bool enable)
{
  CHECK_CHANNEL_ID(instance)

  /* The irq enable bit happened to be 2*id + 1 */

  if (enable)
    {
      up_enable_event(1 + (instance->id << 1));
    }
  else
    {
      up_disable_event(1 + (instance->id << 1));
    }

  return OK;
}

/************************************************************************************
 * Name: gap8_udma_rx_setirq
 *
 * Description:
 *   Enable or disable the rx interrupt.
 * 
 ************************************************************************************/

int gap8_udma_rx_setirq(struct gap8_udma_peripheral *instance, bool enable)
{
  CHECK_CHANNEL_ID(instance)

  /* The irq enable bit happened to be 2*id */

  if (enable)
    {
      up_enable_event(instance->id << 1);
    }
  else
    {
      up_disable_event(instance->id << 1);
    }

  return OK;
}

/************************************************************************************
 * Name: gap8_udma_tx_start
 * 
 * Description:
 *   Send size * count bytes non-blocking.
 * 
 * This function may be called on ISR, so it cannot be blocked. The caller should
 * manage the muxing.
 * 
 ************************************************************************************/

int gap8_udma_tx_start(struct gap8_udma_peripheral *instance, 
                   uint8_t *buff, uint32_t size, int count)
{
  CHECK_CHANNEL_ID(instance)
  
  instance->tx.buff = buff;
  instance->tx.block_size = size;
  instance->tx.block_count = count;

  _dma_txstart(instance);

  return OK;
}

/************************************************************************************
 * Name: gap8_udma_rx_start
 * 
 * Description:
 *   Receive size * count bytes
 * 
 * This function may be called on ISR, so it cannot be blocked. The caller should
 * manage the muxing.
 * 
 ************************************************************************************/

int gap8_udma_rx_start(struct gap8_udma_peripheral *instance,
                   uint8_t *buff, uint32_t size, int count)
{
  struct __udma_queue *thisreq;

  CHECK_CHANNEL_ID(instance)
  
  instance->rx.buff = buff;
  instance->rx.block_size = size;
  instance->rx.block_count = count;

  _dma_rxstart(instance);

  return OK;
}

/************************************************************************************
 * Name: gap8_udma_tx_poll
 * 
 * Description:
 *   Return OK if the buffer is not in the tx pending list.
 * 
 ************************************************************************************/

int gap8_udma_tx_poll(struct gap8_udma_peripheral *instance)
{
  CHECK_CHANNEL_ID(instance)

  return instance->tx.block_count <= 0 ? OK : ERROR;
}

/************************************************************************************
 * Name: gap8_udma_rx_poll
 * 
 * Description:
 *   Return OK if the buffer is not in the rx pending list.
 * 
 ************************************************************************************/

int gap8_udma_rx_poll(struct gap8_udma_peripheral *instance)
{
  CHECK_CHANNEL_ID(instance)

  return instance->rx.block_count <= 0 ? OK : ERROR;
}

/************************************************************************************
 * Name: gap8_udma_doirq
 * 
 * Description:
 *   uDMA ISR
 * 
 ************************************************************************************/

void gap8_udma_doirq(uint32_t irqn)
{
  struct gap8_udma_peripheral *the_peripheral;

  if (irqn > GAP8_UDMA_MAX_EVENT)
    {
      /* Bypass */
      return;
    }

  /* periheral id happeded to be half of irqn... */

  the_peripheral = _peripherals[irqn >> 1];
  if (the_peripheral == NULL)
    {
      return;
    }

  if (irqn & 0x1)
    {
      if (the_peripheral->tx.block_count == 1)
        {
          /* The buffer is exhausted. Forward to peripheral's driver*/

          the_peripheral->regs->TX_CFG = UDMA_CFG_CLR(1);
          the_peripheral->tx.block_count = 0;
          if (the_peripheral->on_tx)
            {
              the_peripheral->on_tx(the_peripheral);
            }
        }
      else if (the_peripheral->tx.block_count > 1)
        {
          the_peripheral->tx.block_count--;
          the_peripheral->tx.buff += the_peripheral->tx.block_size;
          _dma_txstart(the_peripheral);
        }
    }
  else
    {
      if (the_peripheral->rx.block_count == 1)
        {
          /* The buffer is exhausted. Forward to peripheral's driver*/

          the_peripheral->regs->RX_CFG = UDMA_CFG_CLR(1);
          the_peripheral->rx.block_count = 0;
          if (the_peripheral->on_rx)
            {
              the_peripheral->on_rx(the_peripheral);
            }
        }
      else if (the_peripheral->rx.block_count > 1)
        {
          the_peripheral->rx.block_count--;
          the_peripheral->rx.buff += the_peripheral->rx.block_size;
          _dma_rxstart(the_peripheral);
        }
    }
}
