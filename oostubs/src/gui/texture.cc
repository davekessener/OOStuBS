#include "gui/texture.h"

#include "lib/memory.h"

#define FLAG_ALPHA 0xFF000000

namespace oostubs {

Texture::Texture(size_t w, size_t h)
	: mTexture(new u32[w * h])
	, mWidth(w)
	, mHeight(h)
{
	memset(mTexture, 0, mWidth * mHeight * sizeof(u32));
}

Texture::~Texture(void)
{
	delete[] mTexture;
	mTexture = nullptr;
}

void Texture::blt(const Texture& src, int sx, int sy, uint w, uint h, int dx, int dy, bool a)
{
	int tw = w, th = h;
	int sw = src.width(), sh = src.height();
	int dw = mWidth, dh = mHeight;

	if(sx < 0)
	{
		tw += sx;
		sx = 0;
	}

	if(sy < 0)
	{
		th += sy;
		sy = 0;
	}

	if(dx < 0)
	{
		tw += dx;
		dx = 0;
	}

	if(dy < 0)
	{
		th += dy;
		dy = 0;
	}

	if(tw <= 0 || th <= 0)
		return;

	if(sx + tw > sw)
	{
		tw = sw - sx;
	}

	if(sy + th > sh)
	{
		th = sh - sy;
	}

	if(dx + tw > dw)
	{
		tw = dw - dx;
	}

	if(dy + th > dh)
	{
		th = dh - dy;
	}

	if(tw <= 0 || th <= 0)
		return;
	
	unchecked_blt(src, sx, sy, tw, th, dx, dy, a);
}

void Texture::unchecked_blt(const Texture& src, int sx, int sy, uint w, uint h, int dx, int dy, bool alpha)
{
	if(alpha)
	{
		for(uint cy = 0 ; cy < h ; ++cy)
		{
			for(uint cx = 0 ; cx < w ; ++cx)
			{
				u32 v = src.at(sx + cx, sy + cy);

				if(v & FLAG_ALPHA)
				{
					at(dx + cx, dy + cy) = v;
				}
			}
		}
	}
	else
	{
		for(uint cy = 0 ; cy < h ; ++cy)
		{
			const u8 *p1 = (const u8 *) &src.at(sx, sy + cy);
			u8 *p2 = (u8 *) &at(dx, dy + cy);

			memmove(p2, p1, w * sizeof(u32));
		}
	}
}

void Texture::fill_rect(uint x, uint y, uint w, uint h, u32 c)
{
	if(x >= mWidth || y >= mHeight)
		return;
	
	if(!w || !h)
		return;
	
	if(x + w > mWidth) w = mWidth - x;
	if(y + h > mHeight) h = mHeight - y;

	for(uint dy = 0 ; dy < h ; ++dy)
	{
		for(uint dx = 0 ; dx < w ; ++dx)
		{
			at(x + dx, y + dy) = c;
		}
	}
}

}

