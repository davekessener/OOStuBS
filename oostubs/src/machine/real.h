#ifndef OOSTUBS_MACHINE_REAL_H
#define OOSTUBS_MACHINE_REAL_H

#include "machine/toc.h"

extern "C" {

struct realmode_info
{
	uint16_t ss;
	uint32_t stacksize;
	uint16_t ds;
	uint8_t gdt[10];
};

extern void enter_real(struct realmode_info *, struct toc *);
extern void leave_real(struct realmode_info *, struct toc *);

}

#endif

