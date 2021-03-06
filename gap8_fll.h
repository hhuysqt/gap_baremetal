/************************************************************************************
 * GAP8 FLL clock generater
 *  FC can run up to 250MHz@1.2V, and 150MHz@1.0V. While the default voltage of PMU
 *  is 1.2V, it's okay to boost up without considering PMU.
 * 
 * Author: hhuysqt <1020988872@qq.com>
 * 
 ************************************************************************************/

#ifndef GAP8_FLL_H
#define GAP8_FLL_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include "GAP8.h"

/************************************************************************************
 * Public Function
 ************************************************************************************/

/****************************************************************************
 * Name: gap8_setfreq
 *
 * Description:
 *   Set frequency up to 250MHz. Input frequency in Hz.
 *
 ****************************************************************************/

void gap8_setfreq(uint32_t frequency);

/****************************************************************************
 * Name: gap8_getfreq
 *
 * Description:
 *   Get current system clock frequency in Hz.
 *
 ****************************************************************************/

uint32_t gap8_getfreq(void);

#endif