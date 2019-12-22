#ifndef OOSTUBS_THREAD_COROUTINE_H
#define OOSTUBS_THREAD_COROUTINE_H

#include "machine/toc.h"

namespace oostubs
{
	class Coroutine
	{
		public:
			Coroutine(void *s) { toc_settle(&mTOC, s, this); }
			virtual ~Coroutine( ) { }

			void go( );
			void resume(Coroutine&);
			void run( ) { doRun(); }

			static void kickoff(toc_t *);

		protected:
			virtual void doRun( ) = 0;

		private:
			toc_t mTOC;
	};
}

#endif

