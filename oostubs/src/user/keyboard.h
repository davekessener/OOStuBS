#ifndef OOSTUBS_USER_KEYBOARD_H
#define OOSTUBS_USER_KEYBOARD_H

#include "thread/semaphore.h"

#include "machine/plugbox.h"
#include "machine/keyboard_gate.h"
#include "machine/keyboard_controller.h"

#include "lib/fixed_ringbuffer.h"

#include "mpl/singleton.h"

namespace oostubs
{
	class Keyboard : private KeyboardGate
	{
		public:
			Keyboard( );

			Key getc( );
			Key agetc( );

		protected:
			void onKeyHit(Key) override;

		private:
			FixedRingbuffer<Key, 0x100> mBuffer;
			Semaphore mSemaphore;
	};

	typedef mpl::SingletonHolder<Keyboard> KeyboardManager;
}

#endif

