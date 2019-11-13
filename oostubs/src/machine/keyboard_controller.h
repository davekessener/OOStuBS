/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                       K E Y B O A R D _ C O N T R O L L E R               */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Tastaturcontroller des PCs.                                               */
/*****************************************************************************/

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

	class KeyboardControllerImpl
	{
		public:
		typedef mpl::SingletonHolder<KeyboardControllerImpl> Singleton;

			// KEYBOARD_CONTROLLER: Initialisierung der Tastatur: alle LEDs werden
			//                      ausgeschaltet und die Wiederholungsrate auf
			//                      maximale Geschwindigkeit eingestellt.
			KeyboardControllerImpl();
			
			// KEY_HIT: Dient der Tastaturabfrage nach dem Auftreten einer Tastatur-
			//          unterbrechung. Wenn der Tastendruck abgeschlossen ist und
			//          ein Scancode, sowie gegebenenfalls ein ASCII Code emittelt
			//          werden konnte, werden diese in Key zurueckgeliefert. Anderen-
			//          falls liefert key_hit() einen ungueltigen Wert zurueck, was
			//          mit Key::valid() ueberprueft werden kann.
			Key key_hit();
			
			// REBOOT: Fuehrt einen Neustart des Rechners durch. Ja, beim PC macht
			//         das der Tastaturcontroller.
			void reboot();
			
			// SET_REPEAT_RATE: Funktion zum Einstellen der Wiederholungsrate der
			//                  Tastatur. delay bestimmt, wie lange eine Taste ge-
			//                  drueckt werden muss, bevor die Wiederholung einsetzt.
			//                  Erlaubt sind Werte zwischen 0(minimale Wartezeit)
			//                  und 3(maximale Wartezeit). speed bestimmt, wie
			//                  schnell die Tastencodes aufeinander folgen soll.
			//                  Erlaubt sind Werte zwischen 0(sehr schnell) und 31
			//                 (sehr langsam).
			void set_repeat_rate(int speed, int delay);
			
			// SET_LED: setzt oder loescht die angegebene Leuchtdiode.
			void set_led(KeyboardData::LED, bool);

			bool get_led(KeyboardData::LED led) const { return mLEDs & static_cast<uint8_t>(led); }
	
		private:
			KeyboardControllerImpl(const KeyboardControllerImpl &) = delete;
	
			// KEY_DECODED: Interpretiert die Make und Break-Codes der Tastatur und
			//              liefert den ASCII Code, den Scancode und Informationen
			//              darueber, welche zusaetzlichen Tasten wie Shift und Ctrl
			//              gedrueckt wurden. Ein Rueckgabewert true bedeutet, dass
			//              das Zeichen komplett ist, anderenfalls fehlen noch Make
			//              oder Breakcodes.
			bool key_decoded();
			
			// GET_ASCII_CODE: ermittelt anhand von Tabellen aus dem Scancode und
			//                 den gesetzten Modifier-Bits den ASCII Code der Taste.
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
		
			// Benutzte Ports des Tastaturcontrollers
			const IO_Port mCtrlPort; // Status-(R) u. Steuerregister(W)
			const IO_Port mDataPort; // Ausgabe-(R) u. Eingabepuffer(W)
	};

	typedef KeyboardControllerImpl::Singleton KeyboardController;
}

#endif
