#include "machine/keyboard_gate.h"

#include "machine/keyboard_controller.h"
#include "machine/cga_screen.h"

#include "io.h"

namespace oostubs {

extern bool use_sync;

void KeyboardGate::doPrologue(uint slot)
{
	mKey = KeyboardControllerManager::instance().key_hit();

	if(mKey.valid() && mKey.scancode() == Key::scan_code::DEL && mKey.alt() && mKey.ctrl())
	{
		KeyboardControllerManager::instance().reboot();
	}

	if(mKey.valid() && mKey.scancode() == Key::scan_code::ESC)
	{
		use_sync = !use_sync;
	}
}

void KeyboardGate::doEpilogue(void)
{
	auto& screen{Screen::instance()};

	if(mKey.valid())
	{
		uint cx = 0, cy = 0;

		screen.getCursor(&cx, &cy);
		screen.setCursor(10, 10);
		kout << mKey.ascii() << io::flush;
		screen.setCursor(cx, cy);
	}
}

}

