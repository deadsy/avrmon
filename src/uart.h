//-----------------------------------------------------------------------------
/*

   UART Driver

 */
//-----------------------------------------------------------------------------

#ifndef UART_H
#define UART_H

//-----------------------------------------------------------------------------

#define UART_TXBUF_SIZE 32              // must be a power of 2
#define UART_RXBUF_SIZE 32              // must be a power of 2

struct uart_cfg {
	unsigned long baud;               // baud rate
	int data;               // data bits
	int parity;             // parity bits
	int stop;               // stop bits
};

struct uart_drv {
	uint8_t txbuf[UART_TXBUF_SIZE];
	uint8_t rxbuf[UART_RXBUF_SIZE];
	volatile int tx_rd, tx_wr;
	volatile int rx_rd, rx_wr;
	// stats
	uint16_t rx_ints;
	uint16_t rx_bytes;
	uint16_t rx_parity_error;
	uint16_t rx_framing_error;
	uint16_t rx_overrun_error;
	uint16_t rx_overflow_error;
	uint16_t tx_ints;
	uint16_t tx_bytes;
};

//-----------------------------------------------------------------------------

int uart_init(struct uart_drv *uart, struct uart_cfg *cfg);

// stdio functions
int uart_putc(struct uart_drv *uart, char c);
void uart_flush(struct uart_drv *uart);
int uart_tstc(struct uart_drv *uart);
int uart_getc(struct uart_drv *uart);

//-----------------------------------------------------------------------------

#endif // UART_H

//-----------------------------------------------------------------------------
