#include "gui/screen.h"

#include "lib/memory.h"

namespace oostubs {

Screen::Screen(void *fb, uint p)
	: mBuffer((p % sizeof(u32)) ? WIDTH : (p/sizeof(u32)), HEIGHT)
	, mFramebuffer((u8 *) fb)
	, mPitch((p % sizeof(u32)) ? p : 0)
{
	sync();
}

void Screen::sync(void)
{
	if(mPitch)
	{
		uint w = min(WIDTH, mBuffer.width());

		for(uint y = 0 ; y < HEIGHT ; ++y)
		{
			memmove(mFramebuffer + y * mPitch, mBuffer.begin() + y * mBuffer.width(), w * sizeof(u32));
		}
	}
	else
	{
		memmove(mFramebuffer, mBuffer.begin(), mBuffer.width() * HEIGHT * sizeof(u32));
	}
}

void Screen::clear(void)
{
	memset(mBuffer.begin(), 0, mBuffer.width() * HEIGHT * sizeof(u32));
}

}

