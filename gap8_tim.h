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

#ifndef GAP8_BASIC_TIMER
#define GAP8_BASIC_TIMER

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include "GAP8.h"

/************************************************************************************
 * Public Function Prototypes
 ************************************************************************************/

/****************************************************************************
 * Name: gap8_timer_initialize
 *
 * Description:
 *   Initialize the timer based on the frequency of source clock and ticks
 *   per second.
 *
 ****************************************************************************/

void gap8_timer_initialize(uint32_t source_clock, uint32_t tick_per_second);

/****************************************************************************
 * Name: gap8_register_timercallback
 *
 * Description:
 *   Register a callback function called on timer IRQ
 *
 ****************************************************************************/

void gap8_register_timercallback(void (*on_timer)(void*arg), void *arg);

/****************************************************************************
 * Name: gap8_timer_isr
 *
 * Description:
 *   ISR for timer
 *
 ****************************************************************************/

void gap8_timer_isr(void);

#endif
