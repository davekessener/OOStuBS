#ifndef OOSTUBS_THREAD_ORGANIZER_H
#define OOSTUBS_THREAD_ORGANIZER_H

#include "aux.h"

#include "thread/scheduler.h"
#include "thread/waitingroom.h"
#include "thread/customer.h"

namespace oostubs
{
	class Organizer : private Scheduler
	{
		public:
			void schedule( ) { Scheduler::schedule(); }
			void resume(bool f = true) { Scheduler::resume(f); }
			void exit( ) { Scheduler::exit(); }

			void add(Customer&);
			void kill(Customer&);

			void block(Waitingroom&);
	};
}

#endif

