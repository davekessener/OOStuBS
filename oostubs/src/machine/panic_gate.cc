#include "machine/panic_gate.h"

#include "panic.h"

namespace oostubs {

void PanicGate::doTrigger(uint slot)
{
	PANIC("UNHANDLED INT #", slot, "!");
}

}

