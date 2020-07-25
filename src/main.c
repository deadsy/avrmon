//-----------------------------------------------------------------------------
/*


 */
//-----------------------------------------------------------------------------

#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <math.h>
#include <avr/interrupt.h>
//#include <avr/pgmspace.h>

#include "common.h"
#include "uart.h"

//-----------------------------------------------------------------------------

static void avrmon(void){
	char c = 'a';

	while (1) {
		putc(c++, stdout);
		if (c > 'z') {
			c = 'a';
		}
	}
}

//-----------------------------------------------------------------------------
// Use UART for stdio

static struct uart_drv uart0;

static struct uart_cfg uart0_cfg = {
	baud: 115200,
	data: 8,
	parity: 0,
	stop: 1,
};

static int serial_putc(char c, FILE *stream){
	return uart_putc(&uart0, c);
}
static int serial_getc(FILE *stream){
	return uart_getc(&uart0);
}

static FILE uart_stream;

static void uart_stdio(void){
	uart_init(&uart0, &uart0_cfg);
	uart_stream.put = serial_putc;
	uart_stream.get = serial_getc;
	uart_stream.flags = _FDEV_SETUP_RW;
	uart_stream.udata = 0;
	stdout = stdin = stderr = &uart_stream;
}

//-----------------------------------------------------------------------------

int main(void){

	uart_stdio();
	sei();
	putc('\n', stdout);

	// initialisation
	int init_fails = 0;
	//INIT(uart_init);
	if (init_fails != 0) {
		// loop forever...
		while (1);
	}

	avrmon();

	// loop forever...
	while (1);

	return 0;
}

//-----------------------------------------------------------------------------
