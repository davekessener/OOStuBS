#include "thread/scheduler.h"

#include "machine/guard.h"

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
		add(mActive);
	}

	Coroutine *thread = mActive;

	mActive = mBacklog.pop();

	thread->resume(*mActive);
}

void Scheduler::add(Coroutine *thread)
{
	mBacklog.push(thread);
}

void Scheduler::exit(void)
{
	resume(false);
}

}

