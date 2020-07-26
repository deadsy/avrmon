//-----------------------------------------------------------------------------
/*

   AVR Monitor

 */
//-----------------------------------------------------------------------------

#include <inttypes.h>
#include <avr/interrupt.h>

#include "common.h"
#include "uart.h"

//-----------------------------------------------------------------------------

static int putchar(int c) {
	uart_tx(c);
	return (int)c;
}

static int puts(const char *s) {
	while (*s != 0) {
		uart_tx(*s);
		s++;
	}
	return 0;
}

//-----------------------------------------------------------------------------

static void put_nybble(int x) {
	x &= 15;
	putchar((x <= 9) ? ('0' + x) : ('a' + x - 10));
}

static void put_hex8(uint8_t x) {
	put_nybble(x >> 4);
	put_nybble(x);
}

static void put_hex16(uint16_t x) {
	put_hex8(x >> 8);
	put_hex8(x);
}

static void put_hex32(uint32_t x) {
	put_hex16(x >> 16);
	put_hex16(x);
}

static char *itoa(int x, char *s) {
	unsigned int ux = (x >= 0) ? x : -x;
	int i = 0;
	do {
		s[i++] = '0' + (ux % 10);
		ux /= 10;
	} while (ux != 0);
	if (x < 0) {
		s[i++] = '-';
	}
	s[i] = 0;
	i--;
	int j = 0;
	while (j < i) {
		char tmp = s[j];
		s[j] = s[i];
		s[i] = tmp;
		j++;
		i--;
	}
	return s;
}

static void put_dec(int x) {
	char tmp[32];
	puts(itoa(x, tmp));
}

//-----------------------------------------------------------------------------

#define BYTES_PER_LINE 16

static void mem_display8(void *ptr, unsigned n) {
	char ascii[BYTES_PER_LINE + 1];
	int ofs = 0;

	n = (n + BYTES_PER_LINE - 1) & ~(BYTES_PER_LINE - 1);
	ascii[BYTES_PER_LINE] = 0;

	while (n > 0) {
		put_hex16((uint16_t)ptr + ofs);
		puts(": ");
		for (int i = 0; i < BYTES_PER_LINE; i++) {
			char c = ((uint8_t *)ptr)[ofs + i];
			put_hex8(c);
			putchar(' ');
			// ascii string
			if ((c >= 0x20) && (c <= 0x7e)) {
				ascii[i] = c;
			} else {
				ascii[i] = '.';
			}
			n--;
		}
		puts(ascii);
		puts("\r\n");
		ofs += BYTES_PER_LINE;
	}
}

//-----------------------------------------------------------------------------

static void avrmon(void) {
	puts("avrmon 1.0\r\n");

	put_dec(-1234);
	puts("\r\n");

	put_hex32(0xdeadbeef);
	puts("\r\n");

	mem_display8((void *)0, 1 << 10);
	puts("done\r\n");

	while(1);
}

//-----------------------------------------------------------------------------

int main(void){
	uart_init();
	sei();

	puts("\r\n");

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
