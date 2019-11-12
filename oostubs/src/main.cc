#include "aux.h"
#include "system.h"

#include "utils/shutdown_service.h"

#ifndef NOKERNEL

int main(void)
{
	using namespace oostubs;

	System::instance().run();

	ShutdownService::instance().shutdown();

	return 0;
}

#endif

