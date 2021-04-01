#include <stdarg.h>
#include <stdio.h>

/* --------------------------------------
 * int printf(const char *fmt, ...)
 * -------------------------------------- */
int printf(const char *format, ...)
{
	va_list ap;
	char buffer[128], *s;
	va_start(ap, format);
	sprintf(buffer, format, ap);
	va_end(ap);

	s = buffer;
	while (*s)
	{
		__putchar(*s++);
	}
	
	return 0;
}
