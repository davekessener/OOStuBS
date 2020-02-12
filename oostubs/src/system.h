#ifndef OOSTUBS_SYSTEM_H
#define OOSTUBS_SYSTEM_H

#include "lib.h"

#include "mpl/singleton.h"

#include "machine/scheduler_gate.h"
#include "machine/keyboard_gate.h"

namespace oostubs
{
	class System
	{
		public:
		typedef mpl::singleton_policies::OneshotCreationPolicy<System> CreationPolicy;
		typedef mpl::SingletonHolder<System> Singleton;

		public:
			void run( );
	
		private:
			System( );
			~System( );
			System(const System&) = delete;

			SchedulerGate mSchedulerDriver;
			KeyboardGate mKeyboardDriver;

			friend CreationPolicy;
	};

	typedef System::Singleton SystemManager;
}

#endif

