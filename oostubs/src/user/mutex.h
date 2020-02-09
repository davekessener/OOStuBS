#ifndef OOSTUBS_USER_MUTEX_H
#define OOSTUBS_USER_MUTEX_H

#include "thread/guarded_semaphore.h"

namespace oostubs
{
	class Mutex
	{
		public:
			Mutex( ) : mS(1) { }

			void lock( ) { mS.wait(); }
			void unlock( ) { mS.signal(); }
			void unlock_all( ) { while(!available()) unlock(); }
			bool available( ) const { return mS.counter() > 0; }

		private:
			GuardedSemaphore mS;
	};
}

#endif

