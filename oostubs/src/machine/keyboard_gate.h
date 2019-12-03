#ifndef OOSTUBS_MACHINE_KEYBOARDGATE_H
#define OOSTUBS_MACHINE_KEYBOARDGATE_H

#include "machine/gate.h"

namespace oostubs
{
	class KeyboardGate : public Gate
	{
		public:
		protected:
			void doTrigger(uint) override;

		private:
	};
}

#endif

