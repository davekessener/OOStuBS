#include "user/keyboard.h"

#include "machine/guard.h"

namespace oostubs {

Keyboard::Keyboard(void)
{
	PlugboxManager::instance().assign(IRQ::KEYBOARD, this);
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

void Keyboard::onKeyHit(Key key)
{
	mBuffer.push(key);

	if(mSemaphore.counter() < 0)
	{
		mSemaphore.signal();
	}
}

}

