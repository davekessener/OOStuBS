#ifndef OOSTUBS_THREAD_SEMAPHORE_H
#define OOSTUBS_THREAD_SEMAPHORE_H

#include "aux.h"

#include "thread/waitingroom.h"

namespace oostubs
{
	class Semaphore : public BasicWaitingroom
	{
		public:	
			Semaphore(uint = 1);

			void p( );
			void v( );

			void wait( ) { p(); }
			void signal( ) { v(); }

			int counter( ) const { return mCounter; }

		private:
			int mCounter;
	};
}

#endif

