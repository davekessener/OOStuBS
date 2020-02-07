#ifndef OOSTUBS_LIB_LOCKABLE_H
#define OOSTUBS_LIB_LOCKABLE_H

#include "panic.h"

namespace oostubs
{
	class Lockable
	{
		public:
			Lockable( ) : mState(true) { }

			void lock( )
			{
				ASSERT(available());

				mState = false;
			}

			void unlock( )
			{
				ASSERT(!available());

				mState = true;
			}

			bool available( ) const { return mState; }

		private:
			bool mState;
	};

	class Locker
	{
		public:
			Locker(Lockable *lock) : mLock(lock) { mLock->lock(); }
			~Locker( ) { mLock->unlock(); }

		private:
			Lockable *mLock;
	};
}

#endif

