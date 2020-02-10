#include "lib/memory.h"

extern "C" {

void *memset(void *dst, int v, long unsigned int c)
{
	u8 *p = (u8 *) dst;

	while(c--) *p++ = v;

	return p;
}

void *memmove(void *dst, const void *src, long unsigned int c)
{
	const u8 *p1 = (const u8 *) src;
	u8 *p2 = (u8 *) dst;

	while(c--) *p2++ = *p1++;

	return p2;
}

}

namespace oostubs {

void slow_memset(void *p, u8 v, uint l)
{
	volatile u8 *pp = (u8 *) p;
	
	while(l--) *pp++ = v;
}

}

