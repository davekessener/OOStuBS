#include "thread/waitingroom.h"

#include "panic.h"

#include "thread/customer.h"

#include "sys/scheduler.h"

namespace oostubs {

BasicWaitingroom::~BasicWaitingroom(void)
{
	while(!mCustomers.empty())
	{
		auto *c = mCustomers.pop();

		c->waitingroom(nullptr);
		SchedulerManager::instance().add(*c);
	}
}

void BasicWaitingroom::doAdd(Customer *thread)
{
	mCustomers.push(thread);
}

void BasicWaitingroom::doRemove(Customer *thread)
{
	ASSERT(mCustomers.remove(thread));
}

}

