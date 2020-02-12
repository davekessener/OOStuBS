#ifndef OOSTUBS_THREAD_WAITINGROOM_H
#define OOSTUBS_THREAD_WAITINGROOM_H

#include "lib.h"

#include "lib/fixed_ringbuffer.h"

namespace oostubs
{
	class Customer;

	class Waitingroom
	{
		public:
			virtual ~Waitingroom( ) { }

			void add(Customer& c) { doAdd(&c); }
			void remove(Customer& c) { doRemove(&c); }

		protected:
			virtual void doAdd(Customer *) = 0;
			virtual void doRemove(Customer *) = 0;
	};

	class BasicWaitingroom : public Waitingroom
	{
		public:
			~BasicWaitingroom( );

		protected:
			void doAdd(Customer *) override;
			void doRemove(Customer *) override;

			Customer *pop( ) { return mCustomers.pop(); }
			uint size( ) const { return mCustomers.size(); }

		private:
			FixedRingbuffer<Customer *, 0x100> mCustomers;
	};
}

#endif

