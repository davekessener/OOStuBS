/* $Id: main.cc 8485 2017-03-27 11:50:06Z friesel $ */

#include "aux.h"

#define SCREEN ((uint16_t *) 0xB8000)

void puts(const char *s)
{
	uint16_t *scr = SCREEN;

	while(*s)
	{
		*scr++ = (0x0F00 | (uint8_t) *s++);
	}
}

int main()
{
	puts("Hello, World!");

	return 0;
}

