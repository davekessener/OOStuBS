#ifndef OOSTUBS_MACHINE_REAL_H
#define OOSTUBS_MACHINE_REAL_H

extern "C" {

struct realmode_info
{
	uint16_t ss;
	uint32_t stacksize;
	uint16_t ds;
	uint8_t gdt[10];
};

void enter_real(struct realmode_info *);
void leave_real(struct realmode_info *);

}

#endif

