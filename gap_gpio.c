/************************************************************************************
 * PIN driver for GAP8
 *  GAP8 has only 1 port. Each pin could be configured to GPIO or alternative
 *  functions.
 * 
 * Author: hhuysqt <1020988872@qq.com>
 * 
 ************************************************************************************/

#include "gap_gpio.h"

#define OK    0
#define ERROR -1

/************************************************************************************
 * Name: gap8_configpin
 *
 * Description:
 *   Configure a pin based on bit-encoded description of the pin.
 * 
 * GPIO software abstraction: bitmap configuration of pins
 * 
 * |31 18|   17  |     16     |15              8|7      0|
 * | --- | drive | pull-up/OD | alt-func-number | pinnum |
 * | --- | 1-bit |   1-bit    |      2-bit      |  8-bit |
 *
 * Returned Value:
 *   OK on success
 *   ERROR on invalid pin.
 *
 ************************************************************************************/

int gap8_configpin(uint32_t cfgset)
{
  uint32_t pinnum = cfgset & 0xff;
  uint32_t altfunc = (cfgset >> 8) & 0x3;
  uint32_t pin_dr_pu = (cfgset >> 16) & 0x3;
  uint32_t port_cfg_reg, pin_alt_reg;
  int shiftcnt;

  if (pinnum > MAX_PIN_NUM)
    {
      return ERROR;
    }

  /* Set pin drive strength (or pin speed in other words) and pulling */

  port_cfg_reg = PORTA->PADCFG[pinnum >> 2];
  shiftcnt = (pinnum & 0x3) << 3;
  port_cfg_reg &=     ~(0x3 << shiftcnt);
  port_cfg_reg |= pin_dr_pu << shiftcnt;
  PORTA->PADCFG[pinnum >> 2] = port_cfg_reg;

  /* Set pin alternative function */

  pin_alt_reg = PORTA->PADFUN[pinnum >> 4];
  shiftcnt = (pinnum & 0xf) << 1;
  pin_alt_reg &=   ~(0x3 << shiftcnt);
  pin_alt_reg |= altfunc << shiftcnt;
  PORTA->PADFUN[pinnum >> 4] = pin_alt_reg;

  return OK;
}

/************************************************************************************
 * Name: gap8_gpiowrite
 *
 * Description:
 *   Write one or zero to the selected GPIO pin
 *
 ************************************************************************************/

void gap8_gpiowrite(uint32_t pinset, bool value)
{

}

/************************************************************************************
 * Name: gap8_gpioread
 *
 * Description:
 *   Read one or zero from the selected GPIO pin
 *
 ************************************************************************************/

bool gap8_gpioread(uint32_t pinset)
{

}

/************************************************************************************
 * Name: gap8_gpioirqset
 *
 * Description:
 *   Enable or disable interrupt on GPIO
 *
 ************************************************************************************/

bool gap8_gpioirqset(uint32_t pinset, bool enable)
{
  
}

