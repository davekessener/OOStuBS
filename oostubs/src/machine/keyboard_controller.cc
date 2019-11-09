/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                       K E Y B O A R D _ C O N T R O L L E R               */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Tastaturcontroller des PCs.                                               */
/*****************************************************************************/

/* INCLUDES */

#include "machine/keyboard_controller.h"

namespace oostubs {

// Bits im Statusregister
enum { outb = 0x01, inpb = 0x02, auxb = 0x20 };

// Kommandos an die Tastatur
struct kbd_cmd
{
	enum { set_led = 0xed, set_speed = 0xf3 };
};
enum { cpu_reset = 0xfe };

// Namen der LEDs
struct led
{
	enum { caps_lock = 4, num_lock = 2, scroll_lock = 1 };
};

// Antworten der Tastatur
struct kbd_reply
{
	enum { ack = 0xfa };
}; 

// Konstanten fuer die Tastaturdekodierung
enum { break_bit = 0x80, prefix1 = 0xe0, prefix2   = 0xe1 };


unsigned char KeyboardController::normal_tab[] =
{
	0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 225, 39, '\b',
	0, 'q', 'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'p', 129, '+', '\n',
	0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 148, 132, '^', 0, '#',
	'y', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', 0,
	'*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-',
	0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, '<', 0, 0
};

unsigned char KeyboardController::shift_tab[] =
{
	0, 0, '!', '"', 21, '$', '%', '&', '/', '(', ')', '=', '?', 96, 0,
	0, 'Q', 'W', 'E', 'R', 'T', 'Z', 'U', 'I', 'O', 'P', 154, '*', 0,
	0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 153, 142, 248, 0, 39,
	'Y', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':', '_', 0,
	0, 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '>', 0, 0
};

unsigned char KeyboardController::alt_tab[] =
{
	0, 0, 0, 253, 0, 0, 0, 0, '{', '[', ']', '}', '\\', 0, 0,
	0, '@', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '~', 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 230, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '|', 0, 0
};

unsigned char KeyboardController::asc_num_tab[] =
{
	'7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', ','
};

unsigned char KeyboardController::scan_num_tab[] =
{
	8, 9, 10, 53, 5, 6, 7, 27, 2, 3, 4, 11, 51
};

/* PRIVATE METHODEN */

// KEY_DECODED: Interpretiert die Make und Break-Codes der Tastatur und
//              liefert den ASCII Code, den Scancode und Informationen
//              darueber, welche zusaetzlichen Tasten wie Shift und Ctrl
//              gedrueckt wurden. Ein Rueckgabewert true bedeutet, dass
//              das Zeichen komplett ist, anderenfalls fehlen noch Make
//              oder Breakcodes.
bool KeyboardController::key_decoded(void)
{
	bool done = false;
	
	// Die Tasten, die bei der MF II Tastatur gegenueber der aelteren
	// AT Tastatur hinzugekommen sind, senden immer erst eines von zwei
	// moeglichen Prefix Bytes.
	if(mCode == prefix1 || mCode == prefix2)
	{ 
		mPrefix = mCode;

		return false;
	}
	
	// Das Loslassen einer Taste ist eigentlich nur bei den "Modifier" Tasten
	// SHIFT, CTRL und ALT von Interesse, bei den anderen kann der Break Code
	// ignoriert werden.
	if(mCode & break_bit)
	{
		mCode &= ~break_bit;

		// Der Break Code einer Taste ist gleich dem
		// Make Code mit gesetzten break_bit.
		switch(mCode)
		{
			case 42:  
			case 54:
				mGather.shift(false);
				break;

			case 56:
				if(mPrefix == prefix1)
				{
					mGather.alt_right(false);
				}
				else
				{
					mGather.alt_left(false);
				}
				break;

			case 29:
				if(mPrefix == prefix1)
				{
					mGather.ctrl_right(false);
				}
				else
				{
					mGather.ctrl_left(false);
				}
				break;
		}
	
		// Ein Prefix gilt immer nur fuer den unmittelbar nachfolgenden Code.
		// Also ist es jetzt abgehandelt.
		mPrefix = 0;
		
		// Mit einem Break-Code kann man nichts anfangen, also false liefern.
		return false;
	}
	
	// Eine Taste wurde gedrueckt. Bei den Modifier Tasten wie SHIFT, ALT,
	// NUM_LOCK etc. wird nur der interne Zustand geaendert. Durch den
	// Rueckgabewert 'false' wird angezeigt, dass die Tastatureingabe noch
	// nicht abgeschlossen ist. Bei den anderen Tasten werden ASCII
	// und Scancode eingetragen und ein 'true' fuer eine erfolgreiche
	// Tastaturabfrage zurueckgegeben, obwohl genaugenommen noch der Break-
	// mCode der Taste fehlt.
	
	switch(mCode)
	{
		case 42:
		case 54:
			mGather.shift(true);
			break;

		case 56:
			if(mPrefix == prefix1)
			{
				mGather.alt_right(true);
			}
			else
			{
				mGather.alt_left(true);
			}
			break;

		case 29:
			if(mPrefix == prefix1)
			{
				mGather.ctrl_right(true);
			}
			else
			{
				mGather.ctrl_left(true);
			}
			break;

		case 58:
			mGather.caps_lock(!mGather.caps_lock());
			set_led(led::caps_lock, mGather.caps_lock());
			break;

		case 70:
			mGather.scroll_lock(!mGather.scroll_lock());
			set_led(led::scroll_lock, mGather.scroll_lock());
			break;

		case 69: // Numlock oder Pause ?
			if(mGather.ctrl_left())  // Pause Taste
			{
				// Auf alten Tastaturen konnte die Pause-Funktion wohl nur
				// ueber Ctrl+NumLock erreicht werden. Moderne MF-II Tastaturen
				// senden daher diese Codekombination, wenn Pause gemeint ist.
				// Die Pause Taste liefert zwar normalerweise keinen ASCII
				// Code, aber Nachgucken schadet auch nicht. In jedem Fall ist
				// die Taste nun komplett.
				get_ascii_code();
				done = true;
			}
			else // NumLock
			{
				mGather.num_lock(!mGather.num_lock());
				set_led(led::num_lock, mGather.num_lock());
			}
			break;
			
		default: // alle anderen Tasten
			// ASCII Codes aus den entsprechenden Tabellen auslesen, fertig.
			get_ascii_code();
			done = true;
			break;
	}
	
	// Ein Prefix gilt immer nur fuer den unmittelbar nachfolgenden Code.
	// Also ist es jetzt abgehandelt.
	mPrefix = 0;
	
	return done;
}

// GET_ASCII_CODE: ermittelt anhand von Tabellen aus dem Scancode und
//                 den gesetzten Modifier-Bits den ASCII Code der Taste.
void KeyboardController::get_ascii_code()
{
	// Sonderfall Scancode 53: Dieser Code wird sowohl von der Minustaste
	// des normalen Tastaturbereichs, als auch von der Divisionstaste des
	// Ziffernblocks gesendet. Damit in beiden Faellen ein Code heraus-
	// kommt, der der Aufschrift entspricht, muss im Falle des Ziffern-
	// blocks eine Umsetzung auf den richtigen Code der Divisionstaste
	// erfolgen.
	if(mCode == 53 && mPrefix == prefix1)  // Divisionstaste des Ziffernblocks
	{
		mGather.ascii('/');
		mGather.scancode(Key::scan_code::DIV);
	}
	
	// Anhand der Modifierbits muss die richtige Tabelle ausgewaehlt
	// werden. Der Einfachheit halber hat NumLock Vorrang vor Alt,
	// Shift und CapsLock. Fuer Ctrl gibt es keine eigene Tabelle.
	
	else if(mGather.num_lock() && !mPrefix && mCode >= 71 && mCode <= 83)
	{
		// Bei eingeschaltetem NumLock und der Betaetigung einer der
		// Tasten des separaten Ziffernblocks(Codes 71-83), sollen 
		// nicht die Scancodes der Cursortasten, sondern ASCII und
		// Scancodes der ensprechenden Zifferntasten geliefert werden.
		// Die Tasten des Cursorblocks(mPrefix == prefix1) sollen
		// natuerlich weiterhin zur Cursorsteuerung genutzt werden
		// koennen. Sie senden dann uebrigens noch ein Shift, aber das
		// sollte nicht weiter stoeren.
		mGather.ascii(asc_num_tab[mCode-71]);
		mGather.scancode(scan_num_tab[mCode-71]);
	}
	else if(mGather.alt_right())
	{
		mGather.ascii(alt_tab[mCode]);
		mGather.scancode(mCode);
	}
	else if(mGather.shift())
	{
		mGather.ascii(shift_tab[mCode]);
		mGather.scancode(mCode);
	}
	else if(mGather.caps_lock())
	{  // Die Umschaltung soll nur bei Buchstaben gelten 
		if((mCode>=16 && mCode<=26) ||(mCode>=30 && mCode<=40) ||(mCode>=44 && mCode<=50))
		{
			mGather.ascii(shift_tab[mCode]);
			mGather.scancode(mCode);
		}
		else
		{
			mGather.ascii(normal_tab[mCode]);
			mGather.scancode(mCode);
		}
	}
	else
	{
		mGather.ascii(normal_tab[mCode]);
		mGather.scancode(mCode);
	}
}
	
/* OEFFENTLICHE METHODEN */

// KEYBOARD_CONTROLLER: Initialisierung der Tastatur: alle LEDs werden
//                      ausgeschaltet und die Wiederholungsrate auf
//                      maximale Geschwindigkeit eingestellt.

KeyboardController::KeyboardController(void)
	: mCtrlPort(0x64), mDataPort(0x60)
{
	// alle LEDs ausschalten(bei vielen PCs ist NumLock nach dem Booten an)
	set_led(led::caps_lock, false);
	set_led(led::scroll_lock, false);
	set_led(led::num_lock, false);
	
	// maximale Geschwindigkeit, minimale Verzoegerung
	set_repeat_rate(0, 0);  
}

// REBOOT: Fuehrt einen Neustart des Rechners durch. Ja, beim PC macht
//         das der Tastaturcontroller.
void KeyboardController::reboot(void)
{
	int status;
	
	// Dem BIOS mitteilen, dass das Reset beabsichtigt war
	// und kein Speichertest durchgefuehrt werden muss.
	
	*(unsigned short*) 0x472 = 0x1234;
	
	// Der Tastaturcontroller soll das Reset ausloesen.
	do
	{
		status = mCtrlPort.inb();      // warten, bis das letzte Kommando
	}
	while(status & inpb);   // verarbeitet wurde.

	mCtrlPort.outb(cpu_reset);        // Reset
}

// SET_REPEAT_RATE: Funktion zum Einstellen der Wiederholungsrate der
//                  Tastatur. delay bestimmt, wie lange eine Taste ge-
//                  drueckt werden muss, bevor die Wiederholung einsetzt.
//                  Erlaubt sind Werte zwischen 0(minimale Wartezeit)
//                  und 3(maximale Wartezeit). speed bestimmt, wie
//                  schnell die Tastencodes aufeinander folgen soll.
//                  Erlaubt sind Werte zwischen 0(sehr schnell) und 31
//                 (sehr langsam).
void KeyboardController::set_repeat_rate(int speed, int delay)
{
	// TODO
}

// SET_LED: setzt oder loescht die angegebene Leuchtdiode

void KeyboardController::set_led(char led, bool on)
{
	// TODO
}

// KEY_HIT: Dient der Tastaturabfrage nach dem Auftreten einer Tastatur-
//          unterbrechung. Wenn der Tastendruck abgeschlossen ist und
//          ein Scancode, sowie gegebenenfalls ein ASCII Code emittelt
//          werden konnte, werden diese in Key zurueckgeliefert. Anderen-
//          falls liefert key_hit() einen ungueltigen Wert zurueck, was
//          mit Key::valid() ueberprueft werden kann.
Key KeyboardController::key_hit(void)
{
	Key invalid;  // nicht explizit initialisierte Tasten sind ungueltig
	
	// TODO

	return invalid;
}

}
