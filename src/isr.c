//-----------------------------------------------------------------------------
/*

   ISR Entry Points

 */
//-----------------------------------------------------------------------------

#include <avr/interrupt.h>

#include "uart.h"

//-----------------------------------------------------------------------------
// ISR Entry Points

ISR(USART_RX_vect) {
	uart_rx_isr();
}

ISR(USART_UDRE_vect) {
	uart_tx_isr();
}

//-----------------------------------------------------------------------------
