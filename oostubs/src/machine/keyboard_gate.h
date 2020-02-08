#ifndef OOSTUBS_MACHINE_KEYBOARDGATE_H
#define OOSTUBS_MACHINE_KEYBOARDGATE_H

#include "machine/key.h"
#include "machine/gate.h"

namespace oostubs
{
	class KeyboardGate : public Gate
	{
		public:
			KeyboardGate( );

		protected:
			void doPrologue(uint) override;
			void doEpilogue( ) override;

			virtual void onKeyHit(Key) = 0;
	};
}

#endif

