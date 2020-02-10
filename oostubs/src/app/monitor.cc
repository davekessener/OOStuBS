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

//	SoundcardController& sound{SoundcardControllerManager::instance()};

	PCIDevice hda;

	bool found = PCIControllerManager::instance().search([](const PCIDevice& pci) {
		return pci.header().class_code == 4 && pci.header().subclass == 3;
	}, &hda);

	if(found)
	{
		kout << "found HDA @" << hda.bus() << "," << hda.slot() << io::endl;
	}

	while(true)
	{
	}
}

}

