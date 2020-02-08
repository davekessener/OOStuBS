#include "machine/keyboard_gate.h"

#include "machine/keyboard_controller.h"
#include "machine/cga_screen.h"

#include "io.h"

namespace oostubs {

KeyboardGate::KeyboardGate(void)
	: Gate(true)
{
}

void KeyboardGate::doPrologue(uint slot)
{
}

void KeyboardGate::doEpilogue(void)
{
	Key k = KeyboardControllerManager::instance().key_hit();

	if(k.valid() && k.scancode() == Key::scan_code::DEL && k.alt() && k.ctrl())
	{
		KeyboardControllerManager::instance().reboot();
	}

	if(k.valid())
	{
		onKeyHit(k);
	}
}

}

