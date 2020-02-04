#include "thread/semaphore.h"

#include "sys/scheduler.h"

#include "panic.h"

namespace oostubs {

Semaphore::Semaphore(uint i)
	 : mCounter(i)
{
	ASSERT(mCounter > 0);
}

void Semaphore::p(void)
{
	--mCounter;

	if(mCounter < 0)
	{
		SchedulerManager::instance().block(*this);
	}
}

void Semaphore::v(void)
{
	++mCounter;

	if(mCounter <= 0)
	{
		Customer *c = pop();

		ASSERT(c->waitingroom() == this);

		c->waitingroom(nullptr);

		SchedulerManager::instance().add(*c);
	}
}

}

