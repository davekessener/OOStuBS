#ifndef MACHINE_KEYBOARDCONTROLLER_H
#define MACHINE_KEYBOARDCONTROLLER_H

#include "machine/io_port.h"
#include "machine/key.h"

#include "mpl/singleton.h"

namespace oostubs
{
	struct KeyboardData
	{
		enum class LED : uint8_t
		{
			SCROLL_LOCK = 1,
			NUM_LOCK = 2,
			CAPS_LOCK = 4
		};
	};

	class KeyboardController
	{
		public:
		typedef mpl::SingletonHolder<KeyboardController> Singleton;

			KeyboardController();
			
			Key key_hit();
			Key get_key();
			void reboot();

			void set_repeat_rate(int speed, int delay);
			void set_led(KeyboardData::LED, bool);
			bool get_led(KeyboardData::LED led) const { return mLEDs & static_cast<uint8_t>(led); }
	
		private:
			KeyboardController(const KeyboardController &) = delete;

			bool key_decoded();
			void get_ascii_code();
			void wait_for_port_empty();
			void wait_for_ack();
	
		private:
			unsigned char mCode;
			unsigned char mPrefix;
			Key mGather;
			uint8_t mLEDs;
			
			static unsigned char normal_tab[];
			static unsigned char shift_tab[];
			static unsigned char alt_tab[];
			static unsigned char asc_num_tab[];
			static unsigned char scan_num_tab[];
		
			const IO_Port mCtrlPort; // Status-(R) u. Steuerregister(W)
			const IO_Port mDataPort; // Ausgabe-(R) u. Eingabepuffer(W)
	};

	typedef KeyboardController::Singleton KeyboardControllerManager;
}

#endif
