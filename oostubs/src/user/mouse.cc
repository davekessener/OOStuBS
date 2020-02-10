#include "user/mouse.h"

#include "machine/guard.h"

#include "gui/screen.h"

namespace oostubs {

namespace
{
	inline u32 clamp(int lo, int v, int hi)
	{
		return (v < lo ? lo : (v > hi ? hi : v));
	}
}

Mouse::Mouse(void)
	: mX(Screen::WIDTH / 2)
	, mY(Screen::HEIGHT / 2)
	, mButtons(0)
	, mSemaphore(0)
{
}

void Mouse::accept(uint btns, i32 dx, i32 dy)
{
	mX = clamp(0, mX + dx, Screen::WIDTH);
	mY = clamp(0, mY - dy, Screen::HEIGHT);

	if(btns != mButtons)
	{
		uint change = mButtons ^ btns;

		for(int i = 0 ; i < 3 ; ++i)
		{
			if(change & (1<<i))
			{
				mBuffer.push({static_cast<Button>(1<<i), (btns & (1<<i)), mX, mY});

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
	}

	mButtons = btns;
}

Mouse::MouseEvent Mouse::get(void)
{
	Guard::Lock lock(GuardManager::instance());

	mSemaphore.wait();

	return mBuffer.pop();
}

}

