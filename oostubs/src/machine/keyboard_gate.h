#ifndef OOSTUBS_MACHINE_KEYBOARDGATE_H
#define OOSTUBS_MACHINE_KEYBOARDGATE_H

#include "machine/key.h"
#include "machine/gate.h"

namespace oostubs
{
	class KeyboardGate : public DeviceGate
	{
		public:
			KeyboardGate( ) : DeviceGate(PIC::Device::KEYBOARD, true) { }

		protected:
			void doPrologue(uint) override;
			void doEpilogue( ) override;

		private:
			Key mKey;
	};
}

#endif

