#include "thread/scheduler.h"

#include "machine/guard.h"
#include "machine/cpu.h"

#include "panic.h"

namespace oostubs {

void Scheduler::schedule(void)
{
	mActive = mBacklog.pop();

	mActive->go();
}

void Scheduler::resume(bool f)
{
	if(f)
	{
		add(*mActive);
	}

	Coroutine *thread = mActive;

	while(mBacklog.empty())
	{
		GuardManager::instance().leave();
		CPUManager::instance().idle();
		GuardManager::instance().enter();
	}

	mActive = mBacklog.pop();

	thread->resume(*mActive);
}

void Scheduler::add(Coroutine& thread)
{
	mBacklog.push(&thread);
}

void Scheduler::exit(void)
{
	resume(false);
}

void Scheduler::kill(Coroutine& thread)
{
	ASSERT(mBacklog.remove(&thread));
}

}

