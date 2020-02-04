#include "thread/buzzer.h"

#include "thread/bellringer.h"

#include "sys/scheduler.h"

namespace oostubs {

void Buzzer::sleep(uint t)
{
	BellringerManager::instance().schedule(*this, t);
	SchedulerManager::instance().block(*this);
}

void Buzzer::doAdd(Customer *c)
{
	ASSERT(!mCustomer);

	mCustomer = c;
}

void Buzzer::doRemove(Customer *c)
{
	ASSERT(mCustomer == c);

	mCustomer = nullptr;
}

void Buzzer::doRing(void)
{
	SchedulerManager::instance().add(*mCustomer);
}

}

