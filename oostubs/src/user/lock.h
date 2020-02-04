#ifndef OOSTUBS_USER_LOCK_H
#define OOSTUBS_USER_LOCK_H

namespace oostubs
{
	template<typename M>
	class Lock
	{
		public:
		typedef M mutex_type;

		public:
			Lock(mutex_type& mtx) : mMtx(mtx) { mMtx.lock(); }
			~Lock( ) { mMtx.unlock(); }

		private:
			mutex_type& mMtx;
	};
}

#endif

