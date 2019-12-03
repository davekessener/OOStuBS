#include "machine/panic_gate.h"

#include "panic.h"

namespace oostubs {

void PanicGate::doPrologue(uint slot)
{
	PANIC("UNHANDLED INT #", slot, "!");
}

}

