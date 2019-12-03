#ifndef OOSTUBS_MACHINE_PANICGATE_H
#define OOSTUBS_MACHINE_PANICGATE_H

#include "aux.h"

#include "machine/gate.h"

namespace oostubs
{
	class PanicGate : public Gate
	{
		public:
			PanicGate( ) { }

		protected:
			void doPrologue(uint) override;

		private:
			PanicGate(const PanicGate&) = delete;
	};
}

#endif

