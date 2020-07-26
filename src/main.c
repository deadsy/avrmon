//-----------------------------------------------------------------------------
/*

   MIDI Lights

   Use MIDI notes to control a string of RGB modules.

 */
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "common.h"
#include "uart.h"

//-----------------------------------------------------------------------------

static void avrmon(void) {

	printf_P(PSTR("\nThe BFP"));
	printf_P(PSTR("\nVersion 1.0"));

	char c = 'A';

	while(1) {
		putc(c, stdout);
		c += 1;
		if (c > 'Z') {
			putc('\n', stdout);
			c = 'A';
		}
	}
}

//-----------------------------------------------------------------------------
// Use UART for stdio

static FILE uart_stream;

static int uart_putc(char c, FILE *stream){
	if (c == '\n')
		uart_tx('\r');
	uart_tx(c);
	return (int)c;
}

static int uart_getc(FILE *stream){
	return (int)uart_rx();
}

static void uart_stdio(void) {
	uart_init();
	uart_stream.put= uart_putc;
	uart_stream.get = uart_getc;
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
	if (init_fails != 0) {
		// loop forever...
		while(1);
	}

	avrmon();

	// loop forever...
	while(1);
	return 0;
}

//-----------------------------------------------------------------------------
