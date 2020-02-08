#include "app/monitor.h"

#include "io.h"

#include "user/keyboard.h"

namespace oostubs {

void sleepms()
{ 
	for(uint i = 0 ; i < 0x1000 ; ++i);
}

void Monitor::execute(void)
{
	kout << "Hello, World!" << io::endl;

	auto& kb{KeyboardManager::instance()};

	KeyboardControllerManager::instance().install_mouse();

	kout << "Installed mouse!" << io::endl;

	while(true)
	{
		kout << kb.getc().ascii() << io::flush;
	}
}

}

