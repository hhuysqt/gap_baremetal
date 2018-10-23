/************************************************************************************
 * uDMA driver for GAP8
 *  GAP8 features a simple uDMA subsystem. Peripherals including UART, SPI, I2C, I2S,
 *  CPI, LVDS, Hyperbus, have config registers memory-mapped, but not data registers.
 *  The only way to send or receive data is using the uDMA.
 * 
 *  Those peripherals share the same uDMA ISR.
 * 
 * Author: hhuysqt <1020988872@qq.com>
 * 
 ************************************************************************************/

#include "gap8_udma.h"
#include "gap_soc_eu.h"
#include <stddef.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define CHECK_CHANNEL_ID(INSTANCE) \
  if ((INSTANCE) == NULL || \
      (INSTANCE)->id < 0 || (INSTANCE)->id >= GAP8_UDMA_NR_CHANNELS) \
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

/* Statically allocated request channels */

static struct __udma_queue _requests[8] = {
  { .next = &_requests[1], },
  { .next = &_requests[2], },
  { .next = &_requests[3], },
  { .next = &_requests[4], },
  { .next = &_requests[5], },
  { .next = &_requests[6], },
  { .next = &_requests[7], },
  { .next = NULL }
};
static struct __udma_queue *aval_req = &_requests[0];

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void _dma_txstart(struct gap8_udma_peripheral *the_peri)
{
  the_peri->regs->TX_SADDR = (uint32_t)the_peri->tx->buff;
  the_peri->regs->TX_SIZE  = (uint32_t)the_peri->tx->block_size;
  the_peri->regs->TX_CFG   = UDMA_CFG_EN(1);
}

static void _dma_rxstart(struct gap8_udma_peripheral *the_peri)
{
  the_peri->regs->RX_SADDR = (uint32_t)the_peri->rx->buff;
  the_peri->regs->RX_SIZE  = (uint32_t)the_peri->rx->block_size;
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
      SOC_EU_SetFCMask(1 + (instance->id << 1));
    }
  else
    {
      SOC_EU_ClearFCMask(1 + (instance->id << 1));
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
      SOC_EU_SetFCMask(instance->id << 1);
    }
  else
    {
      SOC_EU_ClearFCMask(instance->id << 1);
    }

  return OK;
}

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
                   uint8_t *buff, uint32_t size, uint32_t count)
{
  struct __udma_queue *thisreq;

  CHECK_CHANNEL_ID(instance)
  
  // TODO: lock the aval_req

  if (aval_req == NULL)
    {
      /* Cannot allocate __udma_queue. The caller should try again later */

      // TODO: unlock the aval_req
      return ERROR;
    }

  thisreq = aval_req;
  aval_req = aval_req->next;

  // TODO: unlock the aval_req

  thisreq->buff = buff;
  thisreq->block_size = size;
  thisreq->block_count = count;
  thisreq->next = NULL;

  /* Enqueue */

  // TODO: lock the tx queue

  if (instance->txtail == NULL)
    {
      instance->txtail = thisreq;
    }
  else
    {
      instance->txtail->next = thisreq;
    }

  if (instance->tx == NULL)
    {
      /* It's the first transmittion, or the ISR routine has just
       * finished the whole queue, making it the first enqueued element
       **/
      instance->tx = instance->txtail = thisreq;

      _dma_txstart(instance);
    }

  // TODO: unlock the tx queue

  return OK;
}

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
                   uint8_t *buff, uint32_t size, uint32_t count)
{
  struct __udma_queue *thisreq;

  CHECK_CHANNEL_ID(instance)
  
  // TODO: lock the aval_req

  if (aval_req == NULL)
    {
      /* Cannot allocate __udma_queue. The caller should try again later */

      // TODO: unlock the aval_req
      return ERROR;
    }

  thisreq = aval_req;
  aval_req = aval_req->next;

  // TODO: unlock the aval_req

  thisreq->buff = buff;
  thisreq->block_size = size;
  thisreq->block_count = count;
  thisreq->next = NULL;

  /* Enqueue */

  // TODO: lock the rx queue

  if (instance->rxtail == NULL)
    {
      instance->rxtail = thisreq;
    }
  else
    {
      instance->rxtail->next = thisreq;
    }

  if (instance->rx == NULL)
    {
      /* It's the first transmittion, or the ISR routine has just
       * finished the whole queue, making it the first enqueued element
       **/
      instance->rx = instance->rxtail = thisreq;

      _dma_rxstart(instance);
    }

  // TODO: unlock the rx queue

  return OK;
}

/************************************************************************************
 * Name: gap8_udma_tx_poll
 * 
 * Description:
 *   Return OK if the buffer is not in the tx pending list.
 * 
 ************************************************************************************/

int gap8_udma_tx_poll(struct gap8_udma_peripheral *instance, 
                    uint8_t *buff)
{
  struct __udma_queue *reqlist;

  CHECK_CHANNEL_ID(instance)

  /* Iterate through the linked list */

  for (reqlist = instance->tx; 
       reqlist != NULL && reqlist->buff != buff; 
       reqlist = reqlist->next);

  return reqlist == NULL ? OK : ERROR;
}

/************************************************************************************
 * Name: gap8_udma_rx_poll
 * 
 * Description:
 *   Return OK if the buffer is not in the rx pending list.
 * 
 ************************************************************************************/

int gap8_udma_rx_poll(struct gap8_udma_peripheral *instance, 
                    uint8_t *buff)
{
  struct __udma_queue *reqlist;

  CHECK_CHANNEL_ID(instance)

  /* Iterate through the linked list */

  for (reqlist = instance->rx; 
       reqlist != NULL && reqlist->buff != buff; 
       reqlist = reqlist->next);

  return reqlist == NULL ? OK : ERROR;
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

  if (irqn < GAP8_UDMA_MIN_EVENT || irqn > GAP8_UDMA_MAX_EVENT)
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
      struct __udma_queue *thisqueue = the_peripheral->tx;

      /* tx channel */

      if (thisqueue == NULL)
        {
          /* Invalid use case */

          the_peripheral->txtail = NULL;
          return;
        }
      
      thisqueue->block_count--;
      thisqueue->buff += thisqueue->block_size;
      if (thisqueue->block_count <= 0)
        {
          /* Next node */

          the_peripheral->tx = thisqueue->next;
          thisqueue->next = aval_req;
          aval_req = thisqueue;
        }

      if (the_peripheral->tx != NULL)
        {
          _dma_txstart(the_peripheral);
        }
      else
        {
          /* The queue is exhausted */

          the_peripheral->txtail = NULL;
        }

      /* Forward to peripheral's driver */

      if (the_peripheral->on_tx)
        the_peripheral->on_tx(the_peripheral);
    }
  else
    {
      struct __udma_queue *thisqueue = the_peripheral->rx;

      /* rx channel */

      if (thisqueue == NULL)
        {
          the_peripheral->rxtail = NULL;
          return;
        }

      thisqueue->block_count--;
      thisqueue->buff += thisqueue->block_size;
      if (thisqueue->block_count <= 0)
        {
          /* Next node */

          the_peripheral->rx = thisqueue->next;
          thisqueue->next = aval_req;
          aval_req = thisqueue;
        }

      if (the_peripheral->rx != NULL)
        {
          _dma_rxstart(the_peripheral);
        }
      else
        {
          /* The queue is exhausted */

          the_peripheral->rxtail = NULL;
        }

      /* Forward to peripheral's driver */
      
      if (the_peripheral->on_rx)
        the_peripheral->on_rx(the_peripheral);
    }
}
