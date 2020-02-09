#include "lib/promise.h"

#include "user/lock.h"

namespace oostubs {

Promise::Promise(void)
	: mImpl(new Block)
{
}

Promise::Promise(Promise& p)
	: mImpl(nullptr)
{
	Lock<Mutex> lock(p.mImpl->sync);

	mImpl = p.mImpl;
	++mImpl->c;
}

Promise::~Promise(void)
{
	Lock<Mutex> lock(mImpl->sync);

	if(!--(mImpl->c))
	{
		delete mImpl;
		mImpl = nullptr;
	}
}

Promise& Promise::operator=(Promise& p)
{
	Promise t(p);

	swap(t);

	return *this;
}

void Promise::swap(Promise& p)
{
	Lock<Mutex> lock1(mImpl->sync);
	Lock<Mutex> lock2(p.mImpl->sync);

	Block *i = mImpl; mImpl = p.mImpl; p.mImpl = i;
}

bool Promise::done(void) const
{
	return mImpl->mtx.available();
}

void Promise::wait(void)
{
	mImpl->mtx.lock();
}

void Promise::fulfill(void)
{
	mImpl->mtx.unlock_all();
}

}

