#include "aux.h"

#include "machine/plugbox.h"

extern "C" void guardian(unsigned int slot);

namespace oostubs {

void guardian_impl(uint slot)
{
	PlugboxManager::instance().request(static_cast<IRQ>(slot)).trigger(slot);
}

}

void guardian(unsigned int slot)
{
	oostubs::guardian_impl(slot);
}

