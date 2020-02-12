#ifndef OOSTUBS_USER_MOUSE_H
#define OOSTUBS_USER_MOUSE_H

#include "lib.h"

#include "machine/keyboard_gate.h"

#include "mpl/singleton.h"

#include "thread/semaphore.h"

#include "lib/fixed_ringbuffer.h"

namespace oostubs
{
	class Mouse
	{
		public:
		enum class Button : u8
		{
			PRIMARY   = (1<<0),
			SECONDARY = (1<<1),
			TERTIARY  = (1<<2)
		};

		struct Position
		{
			Position( ) : x(0), y(0) { }
			Position(u32 x, u32 y) : x(x), y(y) { }

			u32 x, y;
		};

		struct MouseEvent
		{
			Button button;
			bool pressed;
			uint x, y;

			MouseEvent( ) { }
			MouseEvent(Button btn, bool f, uint x, uint y)
				: button(btn), pressed(f), x(x), y(y) { }
		};

		public:
			Mouse( );

			Position position( ) const { return {mX, mY}; }
			bool pressed(Button b) const { return mButtons & static_cast<u8>(b); }
			MouseEvent get( );
			uint size( ) const { return mBuffer.size(); }

		private:
			void accept(uint, i32, i32);

		private:
			u32 mX, mY;
			uint mButtons;
			Semaphore mSemaphore;
			FixedRingbuffer<MouseEvent, 0x100> mBuffer;

			friend class KeyboardGate;
	};

	typedef mpl::SingletonHolder<Mouse> MouseManager;
}

#endif

