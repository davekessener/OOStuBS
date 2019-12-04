#include "machine/guard.h"

#include "machine/cpu.h"

namespace oostubs {

void Guard::leave(void)
{
	while(true)
	{
		if(mBacklog.empty())
		{
			CPU::Lock lock(CPUManager::instance());

			if(mBacklog.empty())
			{
				unlock();

				return;
			}
		}

		mBacklog.pop()->epilogue();
	}
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

