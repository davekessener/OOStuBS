#ifndef OOSTUBS_LIB_MEMORY_H
#define OOSTUBS_LIB_MEMORY_H

#include "aux.h"

extern "C" {

void *memset(void *, int, long unsigned int);
void *memmove(void *, const void *, long unsigned int);

}

#endif

