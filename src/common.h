//-----------------------------------------------------------------------------
/*

   Common Definitions

 */
//-----------------------------------------------------------------------------

#ifndef COMMON_H
#define COMMON_H

//-----------------------------------------------------------------------------
// Re-define PROGMEM (http://gcc.gnu.org/bugzilla/show_bug.cgi?id=34734)

#undef PROGMEM
#define PROGMEM __attribute__((section(".progmem.data")))

//-----------------------------------------------------------------------------

#define INIT(function) \
	if (function() != 0) { \
		printf_P(PSTR("%s(): fail\n"), #function); \
		init_fails += 1; \
	}

//-----------------------------------------------------------------------------
// bit operations on ports

// set bit
#define sbi(port, bit) ((port) |= (1 << (bit)))
// clear bit
#define cbi(port, bit) ((port) &= ~(1 << (bit)))
// read bit
#define rbi(port, bit) (((port) & (1 << (bit))) != 0)

//-----------------------------------------------------------------------------

#define min(a, b) ((a < b) ? (a) : (b))
#define max(a, b) ((a > b) ? (a) : (b))

//-----------------------------------------------------------------------------

#endif // COMMON_H

//-----------------------------------------------------------------------------
