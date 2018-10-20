/****************************************************************************/
// no printf support
#define printf(...)


/* Demo utlities includes. */
#include "gap_common.h"
#include "gap_uart.h"
//#include "pinmap.h"
//#include "PeripheralPins.h"

#include "gap_gpio.h"

/* Place a dummy debug_struct for plpbridge tool */
debug_struct_t HAL_DEBUG_STRUCT_NAME = GAP_DEBUG_STRUCT_INIT;

/****************************************************************************/

/* Test task to test FreeRTOS port. */
void vTestUART( void *parameters );


/****************************************************************************/

/* Variables used. */
static UART_reg_t *const uart_addrs[] = UART_BASE_PTRS;

/****************************************************************************/


static void serial_init(void)
{
    uart_config_t config;

    UART_GetDefaultConfig( &config );

    UART_Init( uart_addrs[0], &config, SystemCoreClock );

    //pinmap_pinout(tx, PinMap_UART_TX);
    //pinmap_pinout(rx, PinMap_UART_RX);
    gap8_configpin(GAP8_PIN_A7_UART_TX | GAP8_PIN_PULL_UP);
    gap8_configpin(GAP8_PIN_B6_UART_RX | GAP8_PIN_PULL_UP);

    UART_EnableTx( uart_addrs[0], true );
    UART_EnableRx( uart_addrs[0], true );
}

static void serial_format( uart_bit_length_t data_bits, uart_parity_mode_t parity, uart_stop_bit_count_t stop_bits )
{
    /* Set bit count and parity mode. */
    uart_addrs[0]->SETUP |= ~( UART_SETUP_STOP_BITS_MASK |
                      UART_SETUP_BIT_LENGTH_MASK |
                      UART_SETUP_PARITY_ENA_MASK );
    uart_addrs[0]->SETUP |= ( UART_SETUP_PARITY_ENA( parity ) |
                     UART_SETUP_STOP_BITS( stop_bits ) |
                     UART_SETUP_BIT_LENGTH( data_bits ) );
}

int main(void)
{
    uint32_t baudrate = 115200;
    uint8_t get = 'a';
    const char *buf = "abcd";

    /* Serial pins init */
    serial_init();

    /* Serial format 8N1 configuration : 8 bits, parity disabled, 2 stop bits. */
    serial_format( uUART_8bits, uUART_ParityDisabled, uUART_OneStopBit );

    /* Serial baudrate 115200 configuration */
    UART_SetBaudRate( uart_addrs[0], baudrate, SystemCoreClock );

    UART_TransferSendBlocking(uart_addrs[0], buf, 4);
    while(1) {
        //UART_TransferSendBlocking(uart_addrs[0], buf, 4);
        get = UART_ReadByte(uart_addrs[0]);
        UART_WriteByte(uart_addrs[0], get);
    }

    /* Free serial. */
    UART_Deinit( uart_addrs[0] );

    return 0;
}
/*-----------------------------------------------------------*/
