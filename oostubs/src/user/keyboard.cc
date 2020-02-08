#include "user/keyboard.h"

#include "machine/guard.h"

#include "sys/cpu.h"

namespace oostubs {

Keyboard::Keyboard(void)
	: mBuffer(true)
{
}

Key Keyboard::getc(void)
{
	while(true)
	{
		Guard::Lock lock(GuardManager::instance());

		if(!mBuffer.empty())
		{
			return mBuffer.pop();
		}

		mSemaphore.wait();
	}
}

Key Keyboard::agetc(void)
{
	Guard::Lock lock(GuardManager::instance());

	Key key;

	if(!mBuffer.empty())
	{
		key = mBuffer.pop();
	}

	return key;
}

void Keyboard::accept(Key key)
{
	mBuffer.push(key);

	if(mSemaphore.counter() < 0)
	{
		mSemaphore.signal();
	}
}

}

