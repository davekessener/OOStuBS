#include "machine/plugbox.h"

#include "panic.h"

#include "mpl/algorithm.h"

namespace oostubs {

Plugbox::Plugbox(void)
{
	mpl::fill(mGates, mGates + NGATES, &mPanic);
}

void Plugbox::assign(IRQ id, Gate *gate)
{
	mGates[static_cast<uint8_t>(id)] = gate;
}

Gate& Plugbox::request(IRQ id)
{
	return *mGates[static_cast<uint8_t>(id)];
}

}

