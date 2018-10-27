/***************************************************************************
 * Basic driver tests: FLL, FC TIMER, uDMA, UART
 * 
 * Author: hhuysqt <1020988872@qq.com>
 * 
 ***************************************************************************/

/* Demo utlities includes. */
#include "GAP8.h"
#include <stdio.h>

#include "gap8_gpio.h"
#include "gap8_uart.h"
#include "gap8_interrupt.h"
#include "gap8_tim.h"
#include "gap8_fll.h"

/* Place a dummy debug_struct for plpbridge tool */
struct _debug_struct {
    /* Used by external debug bridge to get exit status when using the board */
    uint32_t exitStatus;

    /* Printf */
    uint32_t useInternalPrintf;
    uint32_t putcharPending;
    uint32_t putcharCurrent;
    uint8_t putcharBuffer[128];

    /* Debug step, used for showing progress to host loader */
    uint32_t debugStep;
    uint32_t debugStepPending;

    // Requests
    uint32_t firstReq;
    uint32_t lastReq;
    uint32_t firstBridgeReq;

    uint32_t notifReqAddr;
    uint32_t notifReqValue;

    uint32_t bridgeConnected;
} Debug_Struct = {
  .useInternalPrintf = 1,
};


struct gap8_uart_t *uart0;
int cnt = 0;

/* IO buffer at L2 RAM */
uint8_t buf[] = "hello world\r\n";
uint8_t getbuf[10];
uint8_t cntbuf[20];

static void on_timer(void *arg)
{
  cnt++;
}

/* FC core clock */
#define TARGET_CLK_HZ 200000000

int main(void)
{
  SCBC->ICACHE_ENABLE = 0xFFFFFFFF;
  up_irqinitialize();
  gap8_setfreq(TARGET_CLK_HZ);

  /* Serial pins init */
  uart0 = gap8_uart_initialize(0);
  gap8_uart_setbaud(uart0, 115200, TARGET_CLK_HZ);

  gap8_uart_sendbytes(uart0, buf, strlen(buf));
  sprintf(cntbuf, "%dHz\r\n", gap8_getfreq());
  gap8_uart_sendbytes(uart0, cntbuf, strlen(cntbuf));

  gap8_timer_initialize(TARGET_CLK_HZ, 1);
  gap8_register_timercallback(on_timer, 0);
  while (1)
  {
      gap8_uart_recvbytes(uart0, getbuf, 1);
      sprintf(cntbuf, "%02d\r\n", cnt);
      gap8_uart_sendbytes(uart0, cntbuf, strlen(cntbuf));
  }

  return 0;
}
