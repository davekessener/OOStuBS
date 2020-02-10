#include "user/keyboard.h"

#include "machine/guard.h"

#include "sys/cpu.h"

namespace oostubs {

Keyboard::Keyboard(void)
	: mSemaphore(0)
{
}

Key Keyboard::getc(void)
{
	Guard::Lock lock(GuardManager::instance());

	mSemaphore.wait();

	return mBuffer.pop();
}

Key Keyboard::agetc(void)
{
	Guard::Lock lock(GuardManager::instance());

	Key key;

	if(!mBuffer.empty())
	{
		mSemaphore.wait();

		key = mBuffer.pop();
	}

	return key;
}

void Keyboard::accept(Key key)
{
	mBuffer.push(key);

	if(mBuffer.full())
	{
		mBuffer.pop();
	}
	else
	{
		mSemaphore.signal();
	}
}

}

