#include "machine/cga_screen.h"

#include "mpl/algorithm.h"

#define CGA_CURSOR_HI 0x0e
#define CGA_CURSOR_LO 0x0f

#define CGA_SCREEN ((uint16_t *) 0xB8000)

namespace oostubs {

CGAScreen::CGAScreen(void)
	: mPosX(0)
	, mPosY(0)
	, mControlPort(Port::CGA_CTRL)
	, mDataPort(Port::CGA_DATA)
{
	clear();
}

void CGAScreen::setCursor(uint x, uint y)
{
	mPosX = x;
	mPosY = y;

	update();
}

void CGAScreen::getCursor(uint *x, uint *y)
{
	*x = mPosX;
	*y = mPosY;
}

void CGAScreen::putc_impl(char c, attrib_t a)
{
	switch(c)
	{
		case '\b':
			if(mPosX)
			{
				--mPosX;
			}
			else
			{
				mPosX = WIDTH - 1;

				if(mPosY)
				{
					--mPosY;
				}
			}
			break;

		case '\n':
			mPosX = 0;

			if(++mPosY == HEIGHT)
			{
				scroll();
			}

			break;

		default:
			CGA_SCREEN[mPosX + mPosY * WIDTH] = (a << 8) | c;
		
			if(++mPosX == WIDTH)
			{
				mPosX = 0;
		
				if(++mPosY == HEIGHT)
				{
					scroll();
				}
			}

			break;
	}
}

void CGAScreen::putc(char c, attrib_t a)
{
	putc_impl(c, a);
	update();
}

void CGAScreen::putc_at(uint x, uint y, char c, attrib_t a)
{
	CGA_SCREEN[x + y * WIDTH] = (a << 8) | c;
}

void CGAScreen::puts_n(char *s, int n, attrib_t a)
{
	while((n == -1 && *s) || (n > 0 && --n))
	{
		putc_impl(*s++, a);
	}

	update();
}

void CGAScreen::wputs_n(uint16_t *s, uint n)
{
	while(n--)
	{
		uint16_t c = *s++;

		putc_impl(c & 0xFF, (c >> 8) & 0xFF);
	}

	update();
}

void CGAScreen::scroll(void)
{
	uint16_t *dst = &CGA_SCREEN[0];
	uint16_t *src = &CGA_SCREEN[WIDTH];
	const uint16_t * const end = &CGA_SCREEN[WIDTH * HEIGHT];

	while(dst != end)
	{
		*dst++ = (src >= end ? (DEFAULT_ATTRIBUTE << 8) : *src++);
	}

	if(mPosY > 0)
		--mPosY;
}

void CGAScreen::update(void)
{
	uint p = mPosX + mPosY * WIDTH;

	mControlPort.outb(CGA_CURSOR_HI);
	mDataPort.outb((p >> 8) & 0xFF);
	mControlPort.outb(CGA_CURSOR_LO);
	mDataPort.outb(p & 0xFF);
}

void CGAScreen::clear(void)
{
	mpl::fill(CGA_SCREEN, CGA_SCREEN + WIDTH * HEIGHT, 0);

	mPosX = mPosY = 0;

	update();
}

}

