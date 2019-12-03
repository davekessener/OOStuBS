#include "machine/keyboard_gate.h"

#include "machine/keyboard_controller.h"
#include "machine/cga_screen.h"

#include "io.h"

namespace oostubs {

void KeyboardGate::doTrigger(uint slot)
{
	Key k = KeyboardControllerManager::instance().key_hit();

	if(k.valid())
	{
		uint cx = 0, cy = 0;

		Screen::instance().getCursor(&cx, &cy);
		Screen::instance().setCursor(10, 10);

		kout << k.ascii() << io::flush;

		Screen::instance().setCursor(cx, cy);
	}
}

}

