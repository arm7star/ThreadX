/* --------------------------------------
 * void * memcpy(...)
 * -------------------------------------- */
void * memcpy(void * dest,const void *src,unsigned int count)
{
	char *tmp = (char *) dest, *s = (char *) src;

	while (count--)
		*tmp++ = *s++;

	return dest;
}

/* --------------------------------------
 * int strlen(const char * s)
 * -------------------------------------- */
int strlen(const char * s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}

/* --------------------------------------
 * int strnlen(const char *s, int count)
 * -------------------------------------- */
int strnlen(const char *s, int count)
{
	const char *sc;
	for (sc = s; *sc != '\0' && count--; ++sc);
		return sc - s;
}
