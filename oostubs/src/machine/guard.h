#ifndef OOSTUBS_MACHINE_GUARD_H
#define OOSTUBS_MACHINE_GUARD_H

#include "lib.h"

#include "lib/lockable.h"
#include "lib/fixed_ringbuffer.h"

#include "mpl/singleton.h"

#include "machine/gate.h"

namespace oostubs
{
	class Guard : private Lockable
	{
		public:
		class Lock
		{
			public:
				Lock(Guard& guard) : mGuard(&guard) { mGuard->enter(); }
				~Lock( ) { mGuard->leave(); }

			private:
				Guard *mGuard;
		};

		public:
			void enter( ) { lock(); }
			void leave( );
			void relay(Gate *);

		private:
			FixedRingbuffer<Gate *, 0x40> mBacklog;
	};

	typedef mpl::SingletonHolder<Guard> GuardManager;
}

#endif

