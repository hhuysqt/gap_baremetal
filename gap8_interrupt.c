/************************************************************************************
 * GAP8 event system
 *  GAP8 features a FC controller and a 8-core cluster. IRQ from peripherals have 
 *  unique ID, which are dispatched to the FC or cluster by the SOC event unit, and
 *  then by the FC event unit or cluster event unit, and finally to FC or cluster.
 *  Peripherals share the same IRQ entry.
 * 
 *  We won't exhaust all the features of GAP8.
 * 
 * Author: hhuysqt <1020988872@qq.com>
 * 
 ************************************************************************************/

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include "gap8_interrupt.h"
#include "gap8_udma.h"

/************************************************************************************
 * Public Function
 ************************************************************************************/

/****************************************************************************
 * Name: up_irqinitialize
 *
 * Description:
 *   Initialize the IRQ on FC.
 *
 ****************************************************************************/

void up_irqinitialize(void)
{
  /* Deactivate all the soc events */

  SOC_EU->FC_MASK_MSB = 0xFFFFFFFF;
  SOC_EU->FC_MASK_LSB = 0xFFFFFFFF;

  /* enable soc peripheral interrupt */
  up_enable_irq(GAP8_IRQ_FC_UDMA);
  up_irq_enable();
}

/****************************************************************************
 * Name: gap8_dispatch_irq
 *
 * Description:
 *   Called from IRQ vectors. Input vector id. Return SP pointer, modified
 *   or not.
 *
 ****************************************************************************/

void* gap8_dispatch_irq(uint32_t vector, void *current_regs)
{
  // TODO: call nuttx core functions
  if (vector == GAP8_IRQ_FC_UDMA)
    {
      // udma peripheral interrupt

      /* Get current event */
      uint32_t event = SOC_EVENTS->CURRENT_EVENT & 0xff;

      EU_SW_EVNT_TRIG->TRIGGER_SET[3] = 0;
      
      /* Clear IRQ pending */
      FCEU->BUFFER_CLEAR = (1 << GAP8_IRQ_FC_UDMA);

      if (event < GAP8_UDMA_MAX_EVENT)
        {
          gap8_udma_doirq(event);
        }
    }
    
  return current_regs;
}