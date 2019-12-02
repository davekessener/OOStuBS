#include "machine/cpu.h"

namespace oostubs {

extern "C" void int_enable(void);
extern "C" void int_disable(void);
extern "C" void cpu_idle(void);
extern "C" void cpu_halt(void);

void CPU::enable_int(void)
{
	int_enable();
}

void CPU::disable_int(void)
{
	int_disable();
}

void CPU::idle(void)
{
	cpu_idle();
}

void CPU::halt(void)
{
	cpu_halt();
}

}

