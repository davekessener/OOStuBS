#include "lib.h"

#include "machine/plugbox.h"
#include "machine/guard.h"

extern "C" void guardian(unsigned int slot);

namespace oostubs {

void guardian_impl(uint slot)
{
	Gate& gate{PlugboxManager::instance().request(static_cast<IRQ>(slot))};

	gate.prologue(slot);

	if(gate.has_epilogue())
	{
		GuardManager::instance().relay(&gate);
	}
}

}

void guardian(unsigned int slot)
{
	oostubs::guardian_impl(slot);
}

