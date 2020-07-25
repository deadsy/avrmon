//-----------------------------------------------------------------------------
/*

   UART Driver

 */
//-----------------------------------------------------------------------------

#include <string.h>
#include <inttypes.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "common.h"
#include "uart.h"

//-----------------------------------------------------------------------------

// increment modulus (size is a power of 2)
static inline uint8_t inc_mod(uint8_t idx, uint8_t size) {
	return (idx + 1) & (size - 1);
}

//-----------------------------------------------------------------------------

int uart_init(struct uart_drv *uart, struct uart_cfg *cfg) {

	memset(uart, 0, sizeof(struct uart_drv));

	UBRR0L = (F_CPU / (8UL * 19200)) - 1;
	UBRR0H = 0;
	// double speed
	UCSR0A = _BV(U2X0);
	// 8 data, no parity, 1 stop
	UCSR0C = (3 << 1);
	UCSR0B = _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0);
	return 0;
}

//-----------------------------------------------------------------------------
// USART Rx Complete

void uart_rx_isr(struct uart_drv *uart){

	uint8_t status;

	uart->rx_ints++;

	while (((status = UCSR0A) & _BV(RXC0)) != 0) {
		uint8_t c = UDR0;

		// Check errors
		if (status & _BV(UPE0)) {
			uart->rx_parity_error++;
			break;
		}

		if (status & _BV(FE0)) {
			uart->rx_framing_error++;
			break;
		}

		if (status & _BV(DOR0)) {
			uart->rx_overrun_error++;
			break;
		}

		// If we have space, put it in the rx buffer.
		uint8_t next = inc_mod(uart->rx_wr, UART_RXBUF_SIZE);
		if (next != uart->rx_rd) {
			uart->rxbuf[uart->rx_wr] = c;
			uart->rx_wr = next;
			uart->rx_bytes++;
		}else{
			uart->rx_overflow_error++;
		}
	}
}

//-----------------------------------------------------------------------------
// USART, Data Register Empty

void uart_tx_isr(struct uart_drv *uart){
	uart->tx_ints++;
	if (uart->tx_rd != uart->tx_wr) {
		uart->tx_bytes++;
		UDR0 = uart->txbuf[uart->tx_rd];
		uart->tx_rd = inc_mod(uart->tx_rd, UART_TXBUF_SIZE);
	}else{
		// No more tx data, disable the tx interrupt.
		UCSR0B &= (uint8_t) ~_BV(UDRIE0);
	}
}

//-----------------------------------------------------------------------------
// Receive a character from the serial port. Don't return until we have one.

static int uart_rx_empty(struct uart_drv *uart){
	return uart->rx_rd == uart->rx_wr;
}

static uint8_t uart_rx(struct uart_drv *uart){
	uint8_t c;
	// Wait for a character in the Rx buffer.
	while (uart_rx_empty(uart));
	cli();
	c = uart->rxbuf[uart->rx_rd];
	uart->rx_rd = inc_mod(uart->rx_rd, UART_RXBUF_SIZE);
	sei();
	return c;
}

//-----------------------------------------------------------------------------
// Transmit a character on the serial port.

static int uart_tx_full(struct uart_drv *uart){
	return inc_mod(uart->tx_wr, UART_TXBUF_SIZE) == uart->tx_rd;
}

static int uart_tx_empty(struct uart_drv *uart){
	return uart->tx_rd == uart->tx_wr;
}

static void uart_tx(struct uart_drv *uart, uint8_t c){
	// Wait for a some space in the Tx buffer.
	while (uart_tx_full(uart));
	cli();
	if (uart->tx_wr == uart->tx_rd) {
		// Single byte in buffer, turn on tx interrupts.
		UCSR0B |= (uint8_t)_BV(UDRIE0);
	}
	// Put the character into the Tx buffer.
	uart->txbuf[uart->tx_wr] = c;
	uart->tx_wr = inc_mod(uart->tx_wr, UART_TXBUF_SIZE);
	sei();
}

//-----------------------------------------------------------------------------
// stdio compatible putc/getc

int uart_putc(struct uart_drv *uart, char c) {
	uart_tx(uart, c);
	return 0;
}

void uart_flush(struct uart_drv *uart) {
	while(!uart_tx_empty(uart));
}

int uart_tstc(struct uart_drv *uart) {
	return !uart_rx_empty(uart);
}

int uart_getc(struct uart_drv *uart) {
	return (int)uart_rx(uart);
}

//-----------------------------------------------------------------------------
