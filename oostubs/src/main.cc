#include "aux.h"
#include "system.h"

#include "utils/shutdown_service.h"

#include "machine/cga_screen.h"

#ifndef NOKERNEL

int main(void)
{
	using namespace oostubs;

	ShutdownService::instance();
	Screen::instance();

	System::instance().run();

	ShutdownService::instance().shutdown();

	return 0;
}

#endif

