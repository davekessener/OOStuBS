#include "machine/keyboard_gate.h"

#include "machine/keyboard_controller.h"
#include "machine/cga_screen.h"
#include "machine/plugbox.h"

#include "user/keyboard.h"
#include "user/mouse.h"

namespace oostubs {

KeyboardGate::KeyboardGate(void)
	: Gate(true)
{
	PlugboxManager::instance().assign(IRQ::KEYBOARD, this);
	PlugboxManager::instance().assign(IRQ::MOUSE, this);
}

void KeyboardGate::doEpilogue(void)
{
	KeyboardController::Response r;

	if(KeyboardControllerManager::instance().key_hit(&r))
	{
		if(r.is_mouse)
		{
			MouseManager::instance().accept(r.mouse.pressed, r.mouse.dx, r.mouse.dy);
		}
		else
		{
			ASSERT(r.key.valid());

			if(r.key.scancode() == Key::scan_code::DEL && r.key.alt() && r.key.ctrl())
			{
				KeyboardControllerManager::instance().reboot();
			}

			KeyboardManager::instance().accept(r.key);
		}
	}
}

}

