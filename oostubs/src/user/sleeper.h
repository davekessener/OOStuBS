#ifndef OOSTUBS_USER_SLEEPER_H
#define OOSTUBS_USER_SLEEPER_H

#include "thread/buzzer.h"

#include "machine/guard.h"

namespace oostubs
{
	class Sleeper : private Buzzer
	{
		public:
			void sleep(uint t) { Guard::Lock lock(GuardManager::instance()); Buzzer::sleep(t); }
	};
}

#endif

