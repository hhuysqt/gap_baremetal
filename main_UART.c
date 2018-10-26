/****************************************************************************/
// no printf support
#define printf(...)


/* Demo utlities includes. */
#include "GAP8.h"

#include "gap8_gpio.h"
#include "gap8_uart.h"
#include "gap8_interrupt.h"

/* Place a dummy debug_struct for plpbridge tool */
typedef struct _debug_struct {
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

} debug_struct_t;
#define GAP_DEBUG_STRUCT_INIT {0, 1, 0 ,0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0, 0, 0, 0, 0, 0}
debug_struct_t Debug_Struct = GAP_DEBUG_STRUCT_INIT;

/****************************************************************************/

/* Test task to test FreeRTOS port. */
void vTestUART( void *parameters );


/****************************************************************************/

/* Variables used. */

struct gap8_uart_t *uart0;

/****************************************************************************/


uint8_t buf[] = "hello world\r\n";
uint8_t getbuf[10];
int main(void)
{
    uint32_t baudrate = 115200;
    uint8_t get = 'a';

  SCBC->ICACHE_ENABLE = 0xFFFFFFFF;
    up_irqinitialize();

    /* Serial pins init */
    uart0 = gap8_uart_initialize(0);
    gap8_uart_setbaud(uart0, 115200, 50000000);

    //UART_TransferSendBlocking(uart_addrs[0], buf, strlen(buf));
    gap8_uart_sendbytes(uart0, buf, strlen(buf));
    while (1)
    {
        gap8_uart_recvbytes(uart0, getbuf, 1);
        gap8_uart_sendbytes(uart0, getbuf, 1);
    }

    /* Free serial. */
    //UART_Deinit( uart_addrs[0] );

    return 0;
}
/*-----------------------------------------------------------*/
