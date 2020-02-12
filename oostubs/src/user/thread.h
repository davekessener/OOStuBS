#ifndef OOSTUBS_USER_THREAD_H
#define OOSTUBS_USER_THREAD_H

#include "lib.h"

#include "thread/customer.h"
#include "thread/guarded_semaphore.h"

namespace oostubs
{
	class Thread : private Customer
	{
		public:
			Thread( );
			~Thread( );

			void start( );
			void stop( ) { join(); }
			void join( );

		protected:
			virtual void execute( ) = 0;

		private:
			void doRun( ) override final;

		private:
			u8 *mStack;
			GuardedSemaphore mStatus;
	};
}

#endif

