#ifndef OOSTUBS_THREAD_GUARDEDSEMAPHORE_H
#define OOSTUBS_THREAD_GUARDEDSEMAPHORE_H

#include "thread/semaphore.h"

#include "machine/guard.h"

namespace oostubs
{
	class GuardedSemaphore : private Semaphore
	{
		public:
			GuardedSemaphore(uint c = 1) : Semaphore(c) { }

			void p( ) { Guard::Lock lock(GuardManager::instance()); Semaphore::p(); }
			void v( ) { Guard::Lock lock(GuardManager::instance()); Semaphore::v(); }

			void wait( ) { p(); }
			void signal( ) { v(); }
	};
}

#endif

