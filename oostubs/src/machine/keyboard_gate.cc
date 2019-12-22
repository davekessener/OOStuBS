#include "machine/keyboard_gate.h"

#include "machine/keyboard_controller.h"
#include "machine/cga_screen.h"

#include "io.h"

namespace oostubs {

void KeyboardGate::doPrologue(uint slot)
{
	mKey = KeyboardControllerManager::instance().key_hit();

	if(mKey.valid() && mKey.scancode() == Key::scan_code::DEL && mKey.alt() && mKey.ctrl())
	{
		KeyboardControllerManager::instance().reboot();
	}
}

void KeyboardGate::doEpilogue(void)
{
}

}

