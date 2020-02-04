#ifndef OOSTUBS_THREAD_CUSTOMER_H
#define OOSTUBS_THREAD_CUSTOMER_H

#include "aux.h"

#include "thread/coroutine.h"
#include "thread/waitingroom.h"

namespace oostubs
{
	class Customer : public Coroutine
	{
		public:
			Customer(void *s) : Coroutine(s), mWR(nullptr) { }

			void waitingroom(Waitingroom *);
			Waitingroom *waitingroom( ) { return mWR; }

		private:
			Waitingroom *mWR;
	};
}

#endif

