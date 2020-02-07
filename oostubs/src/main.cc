#include "aux.h"
#include "system.h"
#include "mboot.h"

#include "utils/shutdown_service.h"

#include "machine/cga_screen.h"
#include "machine/cpu.h"
#include "machine/pic.h"
#include "machine/plugbox.h"

#ifndef NOKERNEL

volatile int gdb = 1;

int main(void)
{
	using namespace oostubs;

	while(gdb);

	ShutdownService::instance();
	CPUManager::instance();
	PICManager::instance();
	PlugboxManager::instance();
	Screen::instance();

	SystemManager::instance().run();

	ShutdownService::instance().shutdown();

	return 0;
}

#endif

