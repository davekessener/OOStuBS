#include "gui/screen.h"

#include "lib/memory.h"

#include "mboot.h"

namespace oostubs {

namespace
{
	inline u8 *framebuffer(void)
	{
		return (u8 *) (u64) mboot_info_ptr->framebuffer_addr;
	}

	inline uint pitch(void)
	{
		return mboot_info_ptr->framebuffer_pitch;
	}
}

Screen::Screen(void)
	: mBuffer((pitch() % sizeof(u32)) ? WIDTH : (pitch() / sizeof(u32)), HEIGHT)
	, mFramebuffer(framebuffer())
	, mPitch((pitch() % sizeof(u32)) ? pitch() : 0)
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

