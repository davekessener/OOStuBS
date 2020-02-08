#ifndef OOSTUBS_USER_KEYBOARD_H
#define OOSTUBS_USER_KEYBOARD_H

#include "thread/semaphore.h"

#include "machine/keyboard_gate.h"

#include "lib/fixed_ringbuffer.h"

#include "mpl/singleton.h"

namespace oostubs
{
	class Keyboard
	{
		public:
			Keyboard( );

			Key getc( );
			Key agetc( );

		private:
			void accept(Key);

		private:
			FixedRingbuffer<Key, 0x100> mBuffer;
			Semaphore mSemaphore;

			friend class KeyboardGate;
	};

	typedef mpl::SingletonHolder<Keyboard> KeyboardManager;
}

#endif

