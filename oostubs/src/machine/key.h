/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                K E Y                                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Taste, bestehend aus ASCII Code, Scancode und Modifier-Bits.              */
/*****************************************************************************/

#ifndef OOSTUBS_MACHINE_KEY_H
#define OOSTUBS_MACHINE_KEY_H

#include "aux.h"

namespace oostubs
{
	class Key
	{
		public:
	
		// Bit-Masken fuer die Modifier-Tasten
		struct mbit
		{
			static constexpr int SHIFT       = 1;
			static constexpr int ALT_LEFT    = 2;
			static constexpr int ALT_RIGHT   = 4;
			static constexpr int CTRL_LEFT   = 8;
			static constexpr int CTRL_RIGHT  = 16;
			static constexpr int CAPS_LOCK   = 32;
			static constexpr int NUM_LOCK    = 64;
			static constexpr int SCROLL_LOCK = 128;
		};
			
		// Scan-Codes einiger spezieller Tasten
		struct scan_code
		{ 
			static constexpr int F1    = 0x3B;
			static constexpr int DEL   = 0x53;
			static constexpr int UP    = 72;
			static constexpr int DOWN  = 80;
			static constexpr int LEFT  = 75;
			static constexpr int RIGHT = 77;
			static constexpr int DIV   = 0x8;
		};
	
		public:
			// DEFAULT-KONSTRUKTOR: setzt ASCII, Scancode und Modifier auf 0
			//                      und bezeichnet so einen ungueltigen Tastencode
			Key() : mAscii(0), mScan(0), mModi(0) {}
			
			// VALID: mit Scancode = 0 werden ungueltige Tasten gekennzeichnet.
			bool valid()      { return mScan != 0; }
			
			// INVALIDATE: setzt den Scancode auf Null und sorgt somit fuer einen
			//             ungueltigen Tastencode.
			void invalidate() { mScan = 0; }
			
			// ASCII, SCANCODE: Setzen und Abfragen von Ascii und Scancode
			void ascii(char a) { mAscii = a; }
			void scancode(uint8_t s) { mScan = s; }
			char ascii() { return mAscii; }
			uint8_t scancode() { return mScan; }
			
			// Funktionen zum Setzen und Loeschen von SHIFT, ALT, CTRL usw.
			void shift(bool pressed)
				{ mModi = pressed ? mModi | mbit::SHIFT : mModi & ~mbit::SHIFT; }
	
			void alt_left(bool pressed)
				{ mModi = pressed ? mModi | mbit::ALT_LEFT : mModi & ~mbit::ALT_LEFT; }
	
			void alt_right(bool pressed)
				{ mModi = pressed ? mModi | mbit::ALT_RIGHT : mModi & ~mbit::ALT_RIGHT; }
	
			void ctrl_left(bool pressed)
				{ mModi = pressed ? mModi | mbit::CTRL_LEFT : mModi & ~mbit::CTRL_LEFT; }
	
			void ctrl_right(bool pressed)
				{ mModi = pressed ? mModi | mbit::CTRL_RIGHT : mModi & ~mbit::CTRL_RIGHT; }
	
			void caps_lock(bool pressed)
				{ mModi = pressed ? mModi | mbit::CAPS_LOCK : mModi & ~mbit::CAPS_LOCK; }
	
			void num_lock(bool pressed)
				{ mModi = pressed ? mModi | mbit::NUM_LOCK : mModi & ~mbit::NUM_LOCK; }
	
			void scroll_lock(bool pressed)
				{ mModi = pressed ? mModi | mbit::SCROLL_LOCK : mModi & ~mbit::SCROLL_LOCK; }
			
			// Funktionen zum Abfragen von SHIFT, ALT, CTRL usw.
			bool shift()       { return mModi & mbit::SHIFT; }
			bool alt_left()    { return mModi & mbit::ALT_LEFT; }
			bool alt_right()   { return mModi & mbit::ALT_RIGHT; }
			bool ctrl_left()   { return mModi & mbit::CTRL_LEFT; }
			bool ctrl_right()  { return mModi & mbit::CTRL_RIGHT; }
			bool caps_lock()   { return mModi & mbit::CAPS_LOCK; }
			bool num_lock()    { return mModi & mbit::NUM_LOCK; }
			bool scroll_lock() { return mModi & mbit::SCROLL_LOCK; }
			bool alt()         { return alt_left()  | alt_right(); }
			bool ctrl()        { return ctrl_left() | ctrl_right(); }
			
			operator char()    { return(char) mAscii; }
	
		private:
			char mAscii;
			uint8_t mScan;
			uint8_t mModi;
	};
}

#endif
