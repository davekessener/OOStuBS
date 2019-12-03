#include "system.h"

#include "io.h"

#include "machine/plugbox.h"
#include "machine/pic.h"
#include "machine/cpu.h"
#include "machine/cga_screen.h"
#include "machine/guard.h"

namespace oostubs {

volatile bool use_sync = false;

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
	auto& screen{Screen::instance()};

	PICManager::instance().enable(PIC::Device::KEYBOARD);

	CPUManager::instance().enable_int();

	bool f = !use_sync;
	for(uint i = 0 ; true ; ++i)
	{
		if(f != use_sync)
		{
			*((uint16_t *) 0xB8000) = ((f = use_sync) ? (0x0200 | 'O') : (0x7400 | 'X'));
		}

		if(f)
		{
			GuardManager::instance().enter();
		}

		uint cx = 0, cy = 0;

		screen.getCursor(&cx, &cy);
		screen.setCursor(5, 5);
		kout << ((i >> 15) % 10) << io::flush;
		screen.setCursor(cx, cy);

		if(f)
		{
			GuardManager::instance().leave();
		}
	}
}

}

