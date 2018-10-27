/************************************************************************************
 * GAP8 basic timer
 *  GAP8 features a 64-bit basic timer, able to split into 2 32-bit timers, with
 *  identicle memory map and 2 IRQ channels, for both FC core and cluster. We would
 *  use it as system timer.
 * 
 *  We won't exhaust all the features of GAP8.
 * 
 * Author: hhuysqt <1020988872@qq.com>
 * 
 ************************************************************************************/

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include "gap8_tim.h"
#include "gap8_interrupt.h"

/****************************************************************************
 * Private Data
 ****************************************************************************/

struct gap8_tim_instance {
  BASIC_TIM_reg_t *reg;
  uint32_t core_clock;
  uint32_t tick_per_second;
  /* callback function */
  void (*on_timer)(void*arg);
  void *arg;
} fc_basic_timer = {
  .reg = BASIC_TIM,
  .core_clock = 50000000,
  .tick_per_second = 10,
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: gap8_timer_initialize
 *
 * Description:
 *   Initialize the timer based on the frequency of source clock and ticks
 *   per second.
 *
 ****************************************************************************/

void gap8_timer_initialize(uint32_t source_clock, uint32_t tick_per_second)
{
  /* Set input clock to 1MHz. FC won't exceed 250MHz */

  uint32_t prescaler = (source_clock / 1000000) & 0xff;
  uint32_t cmpval = 1000000 / tick_per_second;

  /* Initialize timer registers */

  fc_basic_timer.reg->CMP_LO = cmpval;
  fc_basic_timer.reg->CFG_REG_LO = (prescaler << 8) |
    BASIC_TIM_CLKSRC_FLL | BASIC_TIM_PRESC_ENABLE | BASIC_TIM_MODE_CYCL | 
    BASIC_TIM_IRQ_ENABLE | BASIC_TIM_RESET | BASIC_TIM_ENABLE;
  fc_basic_timer.reg->VALUE_LO = 0;

  fc_basic_timer.core_clock = source_clock;
  fc_basic_timer.tick_per_second = tick_per_second;

  up_enable_irq(GAP8_IRQ_FC_TIMER_LO);
}

/****************************************************************************
 * Name: gap8_register_callback
 *
 * Description:
 *   Register a callback function called on timer IRQ
 *
 ****************************************************************************/

void gap8_register_timercallback(void (*on_timer)(void*arg), void *arg)
{
  fc_basic_timer.on_timer = on_timer;
  fc_basic_timer.arg = arg;
}

/****************************************************************************
 * Name: gap8_timer_isr
 *
 * Description:
 *   ISR for timer
 *
 ****************************************************************************/

void gap8_timer_isr(void)
{
  if (fc_basic_timer.on_timer)
    fc_basic_timer.on_timer(fc_basic_timer.arg);
}

