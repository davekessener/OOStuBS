#ifndef OOSTUBS_MACHINE_PLUGBOX_H
#define OOSTUBS_MACHINE_PLUGBOX_H

#include "lib.h"

#include "mpl/singleton.h"

#include "machine/gate.h"
#include "machine/panic_gate.h"

namespace oostubs
{
	class Plugbox
	{
		static constexpr size_t NGATES = 64;

		public:
			Plugbox( );

			void assign(IRQ, Gate *);
			Gate& request(IRQ);

		private:
			Plugbox(const Plugbox&) = delete;

			Gate *mGates[NGATES];
			PanicGate mPanic;
	};

	typedef mpl::SingletonHolder<Plugbox> PlugboxManager;
}

#endif

