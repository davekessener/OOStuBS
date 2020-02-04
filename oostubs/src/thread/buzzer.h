#ifndef OOSTUBS_THREAD_BUZZER_H
#define OOSTUBS_THREAD_BUZZER_H

#include "thread/bell.h"
#include "thread/waitingroom.h"

namespace oostubs
{
	class Buzzer : private Bell, private Waitingroom
	{
		public:
			Buzzer( ) : mCustomer(nullptr) { }
			
			void sleep(uint);

		protected:
			void doAdd(Customer *) override;
			void doRemove(Customer *) override;
			void doRing( ) override;

		private:
			Customer *mCustomer;
	};
}

#endif

