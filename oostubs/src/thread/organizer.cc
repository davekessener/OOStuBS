#include "thread/organizer.h"

namespace oostubs {

void Organizer::add(Customer& thread)
{
	Waitingroom *wr = thread.waitingroom();

	if(wr)
	{
		wr->remove(thread);
		thread.waitingroom(nullptr);
	}

	Scheduler::add(thread);
}

void Organizer::kill(Customer& thread)
{
	Waitingroom *wr = thread.waitingroom();

	if(wr)
	{
		wr->remove(thread);
		thread.waitingroom(nullptr);
	}
	else
	{
		Scheduler::kill(thread);
	}
}

void Organizer::block(Waitingroom& wr)
{
	Customer *c = static_cast<Customer *>(Scheduler::active());

	ASSERT(c);

	c->waitingroom(&wr);
	wr.add(*c);

	Scheduler::exit();
}

}

