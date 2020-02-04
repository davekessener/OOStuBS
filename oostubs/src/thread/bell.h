#ifndef OOSTUBS_THREAD_BELL_H
#define OOSTUBS_THREAD_BELL_H

namespace oostubs
{
	class Bell
	{
		public:
			virtual ~Bell( ) { }

			void ring( ) { doRing(); }

		protected:
			virtual void doRing( ) = 0;
	};
}

#endif

