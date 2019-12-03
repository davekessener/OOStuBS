#include "system.h"

#include "io.h"

#include "machine/plugbox.h"
#include "machine/pic.h"
#include "machine/cpu.h"
#include "machine/cga_screen.h"

namespace oostubs {

System::System(void)
{
	PlugboxManager::instance().assign(IRQ::KEYBOARD, &mKeyboardDriver);
}

System::~System(void)
{
	CPUManager::instance().halt();
}

void System::run(void)
{
	PICManager::instance().enable(PIC::Device::KEYBOARD);

	CPUManager::instance().enable_int();

	for(uint i = 0 ; true ; ++i)
	{
		uint cx = 0, cy = 0;

		Screen::instance().getCursor(&cx, &cy);
		Screen::instance().setCursor(5, 5);

		kout << ((i >> 15) % 10) << io::flush;

		Screen::instance().setCursor(cx, cy);
	}
}

}

