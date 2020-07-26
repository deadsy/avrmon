//-----------------------------------------------------------------------------
/*

   UART Driver

 */
//-----------------------------------------------------------------------------

#ifndef UART_H
#define UART_H

//-----------------------------------------------------------------------------
// UART baud rate

#define UART_BAUD 115200

//-----------------------------------------------------------------------------

void uart_rx_isr(void);
void uart_tx_isr(void);

// API
int uart_init(void);
void uart_tx(uint8_t c);
uint8_t uart_rx(void);
int uart_test_rx(void);
int uart_test_tx(void);

//-----------------------------------------------------------------------------

#endif // UART_H

//-----------------------------------------------------------------------------
