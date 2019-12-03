#include "machine/guard.h"

namespace oostubs {

void Guard::leave(void)
{
	while(!mBacklog.empty())
	{
		mBacklog.pop()->epilogue();
	}

	unlock();
}

void Guard::relay(Gate *gate)
{
	if(available())
	{
		enter();

		CPUManager::instance().enable_int();

		gate->epilogue();

		leave();
	}
	else
	{
		mBacklog.push(gate);
	}
}

}

