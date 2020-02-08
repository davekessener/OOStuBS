#include "app/monitor.h"

#include "io.h"

#include "user/keyboard.h"
#include "user/mouse.h"

namespace oostubs {

void Monitor::execute(void)
{
	kout << "Hello, World!" << io::endl;

//	auto& kb{KeyboardManager::instance()};

	auto& mouse{MouseManager::instance()};

	while(true)
	{
		for(uint i = 0 ; i < 0x100000 ; );

		kout << "@(" << mouse.position().x << ", " << mouse.position().y << ")" << io::endl;
	}
}

}

