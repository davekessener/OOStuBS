#include "gui/terminal.h"

#include "panic.h"
#include "initrd.h"

#include "gui/dif.h"

#define CPL 16

namespace oostubs {

Terminal::Terminal(void)
	: mCharset(dif_load_from_file(initrd_root->find("textures/charset_8x16.dif")))
	, mCX(0)
	, mCY(0)
{
}

void Terminal::setCursor(uint x, uint y)
{
	ASSERT(x < WIDTH && y < HEIGHT);

	mCX = x;
	mCY = y;
}

void Terminal::getCursor(uint *x, uint *y)
{
	*x = mCX;
	*y = mCY;
}

void Terminal::putc_impl(uint x, uint y, char c)
{
	auto& screen{FramebufferManager::instance().buffer()};

	switch(c)
	{
		case '\n':
		{
			mCX = 0;
			if(++mCY == HEIGHT)
			{
				scroll();
			}
		}
		break;

		default:
		{
			uint cx = c % CPL, cy = c / CPL;

			ASSERT(cx < CPL && cy < CPL);

			screen.blt(mCharset, cx * CHARW, cy * CHARH, CHARW, CHARH, x * CHARW, y * CHARH, false);
		}
		break;
	}
}

void Terminal::scroll_impl(void)
{
	auto& screen{FramebufferManager::instance().buffer()};

	if(mCY) --mCY;

	screen.blt(screen, 0, CHARH, screen.width(), screen.height() - CHARH, 0, 0, false);
}

void Terminal::putc_impl(char c)
{
	putc_impl(mCX, mCY, c);

	if(++mCX == WIDTH)
	{
		mCY = 0;

		if(++mCY == HEIGHT)
		{
			scroll_impl();
		}
	}
}

void Terminal::sync(void)
{
	FramebufferManager::instance().sync();
}

void Terminal::putc(char c, attrib_t a)
{
	putc_impl(c);

	sync();
}

void Terminal::putc_at(uint x, uint y, char c, attrib_t a)
{
	putc_impl(x, y, c);

	sync();
}

void Terminal::puts_n(const char *s, int n, attrib_t a)
{
	while(n < 0 ? *s : n--) putc_impl(*s++);

	sync();
}

void Terminal::wputs_n(const uint16_t *s, uint n)
{
	while(n--) putc_impl((char) *s++);

	sync();
}

void Terminal::scroll(void)
{
	scroll_impl();

	sync();
}

void Terminal::clear(void)
{
	mCX = mCY = 0;
	FramebufferManager::instance().clear();

	sync();
}

}

