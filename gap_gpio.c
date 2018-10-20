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
 * |31 18|   17  |     16     |  15 |14   10|9     8|7      0|
 * | --- | drive | pull-up/OD | I/O | GPIOn |  alt  | pinnum |
 * | --- | 1-bit |   1-bit    | 1-b | 5-bit | 2-bit |  8-bit |
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

  /* Set pin drive strength (or pin speed in other words) and pulling
   * If it's a GPIO, set input or output.
   *
   * Note that GPIO and non-GPIO uses different register sets...
   * And all the GPIO functions are mapped to ALT-1, and ALT-1 contains
   * only GPIO functions...
   **/

  port_cfg_reg = PORTA->PADCFG[pinnum >> 2];
  shiftcnt = (pinnum & 0x3) << 3;
  port_cfg_reg &=     ~(0x3 << shiftcnt);
  port_cfg_reg |= pin_dr_pu << shiftcnt;
  PORTA->PADCFG[pinnum >> 2] = port_cfg_reg;

  if (altfunc == 1)
    {
      uint32_t gpio_n = (cfgset >> 10) & 0x1f;
      uint32_t gpio_dir = (cfgset >> 15) & 0x1;
      uint32_t tmp;

      /* It must be a GPIO */

      GPIOA->EN |= (1L << gpio_n);

      tmp = GPIOA->PADCFG[gpio_n >> 2];
      shiftcnt = (gpio_n & 0x3) << 3;
      tmp &=     ~(0x3 << shiftcnt);
      tmp |= pin_dr_pu << shiftcnt;
      GPIOA->PADCFG[gpio_n >> 2] = tmp;

      tmp = GPIOA->DIR;
      tmp &= ~(1L << gpio_n);
      tmp |= gpio_dir << gpio_n;
      GPIOA->DIR = tmp;
    }

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
 * Bit encoded pinset:
 * 
 * |31 15|14   10|9   0|
 * | --- | GPIOn | --- |
 * | --- | 5-bit | --- |
 *
 ************************************************************************************/

void gap8_gpiowrite(uint32_t pinset, bool value)
{
  uint32_t gpio_n = (pinset >> 10) & 0x1f;
  if (value)
    {
      GPIOA->OUT |= (1L << gpio_n);
    }
  else
    {
      GPIOA->OUT &= ~(1L << gpio_n);
    }
}

/************************************************************************************
 * Name: gap8_gpioread
 *
 * Description:
 *   Read one or zero from the selected GPIO pin
 *
 * Bit encoded pinset:
 * 
 * |31 15|14   10|9   0|
 * | --- | GPIOn | --- |
 * | --- | 5-bit | --- |
 *
 ************************************************************************************/

bool gap8_gpioread(uint32_t pinset)
{
  uint32_t gpio_n = (pinset >> 10) & 0x1f;
  return (GPIOA->IN >> gpio_n) & 0x1;
}

/************************************************************************************
 * Name: gap8_gpioirqset
 *
 * Description:
 *   Enable or disable interrupt on GPIO
 * 
 * Bit encoded pinset:
 * 
 * |31 20|19     18|17 15|14   10|9   0|
 * | --- | int-typ | --- | GPIOn | --- |
 * | --- |  2-bit  | --- | 5-bit | --- |
 *
 ************************************************************************************/

void gap8_gpioirqset(uint32_t pinset, bool enable)
{
  uint32_t gpio_n = (pinset >> 10) & 0x1f;
  uint32_t int_type = (pinset >> 18) * 0x3;
  uint32_t tmp, shitfcnt;

  if (enable)
    {
      shitfcnt = (gpio_n & 0xf) << 1;
      tmp = GPIOA->INTCFG[gpio_n >> 4];
      tmp &=    ~(0x3 << shitfcnt);
      tmp |= int_type << shitfcnt;
      GPIOA->INTCFG[gpio_n >> 4] = tmp;
      
      GPIOA->INTEN |= (1L << gpio_n);
    }
}

