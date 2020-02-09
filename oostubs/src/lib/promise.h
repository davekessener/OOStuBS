#ifndef OOSTUBS_LIB_PROMISE_H
#define OOSTUBS_LIB_PROMISE_H

#include "aux.h"

#include "user/mutex.h"

namespace oostubs
{
	class Promise
	{
		struct Block
		{
			Mutex mtx, sync;
			int c;

			Block( ) { c = 1; mtx.lock(); }
		};

		public:
			Promise( );
			Promise(Promise&);
			~Promise( );
			Promise& operator=(Promise&);
			void swap(Promise&) noexcept;

			bool done( ) const;
			void wait( );
			void fulfill( );

		private:
			Block *mImpl;
	};
}

#endif

