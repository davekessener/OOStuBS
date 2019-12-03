#ifndef OOSTUBS_MACHINE_GATE_H
#define OOSTUBS_MACHINE_GATE_H

#include "aux.h"

namespace oostubs
{
	class Gate
	{
		public:
			virtual ~Gate( ) { }

			void trigger(uint slot) { doTrigger(slot); }

		protected:
			virtual void doTrigger(uint) = 0;

		private:
	};
}

#endif

