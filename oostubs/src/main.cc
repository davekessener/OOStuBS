#include "aux.h"
#include "system.h"

#include "utils/shutdown_service.h"

struct Test
{
	~Test(void)
	{
		*((uint16_t *) 0xB8000) = 0x0100 | '!';
	}
};

int main(void)
{
	using namespace oostubs;

	static Test s;

	System::instance().run();

	ShutdownService::instance().shutdown();

	return 0;
}

