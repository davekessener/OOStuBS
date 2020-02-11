#include "app/monitor.h"

#include "machine/pci_controller.h"

#include "io.h"

#include "user/keyboard.h"
#include "user/mouse.h"

namespace oostubs {

void Monitor::execute(void)
{
	kout << "Hello, World!" << io::endl;

//	auto& kb{KeyboardManager::instance()};

//	new(&SoundcardControllerManager::instance()) SoundcardController;

	while(true)
	{
	}
}

}

