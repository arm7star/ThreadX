#include <stdio.h>

#define PL011_UARTDR 		(*((volatile unsigned int *) 0x101f1000))
#define PL011_UARTFR 		(*((volatile unsigned int *) 0x101f1018))

int __putchar(int c)
{
	while (PL011_UARTFR & (1 << 5));
	PL011_UARTDR = c & 0xff;
}
