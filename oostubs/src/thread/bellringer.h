#ifndef OOSTUBS_THREAD_BELLRINGER_H
#define OOSTUBS_THREAD_BELLRINGER_H

#include "thread/bell.h"

#include "lib/fixed_linkedlist.h"

#include "mpl/singleton.h"

namespace oostubs
{
	class Bellringer
	{
		struct cell
		{
			cell( ) : bell(nullptr), left(0) { }
			cell(Bell *b, uint t) : bell(b), left(t) { }

			Bell *bell;
			uint left;
		};

		public:
			void schedule(Bell&, uint);
			void cancel(Bell&);
			void tick( );

		private:
			FixedLinkedList<cell, 0x1000> mBells;
	};

	typedef mpl::SingletonHolder<Bellringer> BellringerManager;
}

#endif

