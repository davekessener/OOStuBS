#ifndef OOSTUBS_THREAD_SCHEDULER_H
#define OOSTUBS_THREAD_SCHEDULER_H

#include "aux.h"

#include "lib/fixed_ringbuffer.h"

#include "thread/coroutine.h"

namespace oostubs
{
	class Scheduler
	{
		public:
			Scheduler( ) : mActive(nullptr) { }

			void schedule( );
			void resume(bool = true);
			void add(Coroutine&);
			void exit( );
			void kill(Coroutine&);

		protected:
			Coroutine *active( ) { return mActive; }

		private:
			Coroutine *mActive;
			FixedRingbuffer<Coroutine *, 0x100> mBacklog;
	};
}

#endif

