#ifndef OOSTUBS_LIB_MEMORY_H
#define OOSTUBS_LIB_MEMORY_H

#include "lib.h"

extern "C" {

void *memset(void *, int, long unsigned int);
void *memmove(void *, const void *, long unsigned int);

}

namespace oostubs
{
	void slow_memset(void *, u8, uint);
}

#endif

