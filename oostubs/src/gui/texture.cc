#include "gui/texture.h"

#include "lib/memory.h"

#include "panic.h"

#define FLAG_ALPHA 0xFF000000

namespace oostubs {

namespace
{
	inline void swap(u32& a, u32& b)
	{
		u32 t = a; a = b; b = t;
	}

	inline u32 blend(u32 c1, u32 c2)
	{
		bool p1 = (c1 & 0xFF000000);
		bool p2 = (c2 & 0xFF000000);

		if(!p1 && !p2)
			return 0;

		if(!p1)
			return c2;

		if(!p2)
			return c1;

		uint a1 = ((c1 >> 24) & 0xFF);
		uint a2 = ((c2 >> 24) & 0xFF);
		
		uint r1 = ((c1 >> 16) & 0xFF);
		uint g1 = ((c1 >> 8) & 0xFF);
		uint b1 =  (c1 & 0xFF);

		uint r2 = ((c2 >> 16) & 0xFF);
		uint g2 = ((c2 >> 8) & 0xFF);
		uint b2 =  (c2 & 0xFF);

		uint f = 0x10000- ((0x100 - a1) * (0x100 - a2));
		uint f1 = (a1 << 16) / (f >> 8);
		uint f2 = 0x10000 - f1;

		uint a = f >> 8;
		uint r = (f1 * r1 + f2 * r2) >> 16;
		uint g = (f1 * g1 + f2 * g2) >> 16;
		uint b = (f1 * b1 + f2 * b2) >> 16;

		return ((a << 24) | (r << 16) | (g << 8) | b);
	}
}

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
	bounds b(sx, sy, w, h, dx, dy);

	if(check_bounds(src, b))
	{
		unchecked_blt(src, b.sx, b.sy, b.w, b.h, b.tx, b.ty, a);
	}
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

void Texture::blend_blt(const Texture& src, int sx, int sy, uint w, uint h, int tx, int ty)
{
	bounds b(sx, sy, w, h, tx, ty);

	if(check_bounds(src, b))
	{
		for(uint dy = 0 ; dy < b.h ; ++dy)
		{
			for(uint dx = 0 ; dx < b.w ; ++dx)
			{
				auto& bg{at(b.tx + dx, b.ty + dy)};
				const auto& fg{src.at(b.sx + dx, b.sy + dy)};

				bg = blend(fg, bg);
			}
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

void Texture::vflip(void)
{
	for(uint y = 0 ; y < mHeight / 2 ; ++y)
	{
		for(uint x = 0 ; x < mWidth ; ++x)
		{
			swap(at(x, y), at(x, mHeight-1 - y));
		}
	}
}

void Texture::hflip(void)
{
	for(uint y = 0 ; y < mHeight ; ++y)
	{
		for(uint x = 0 ; x < mWidth / 2 ; ++x)
		{
			swap(at(x, y), at(mWidth-1 - x, y));
		}
	}
}

bool Texture::check_bounds(const Texture& src, bounds& b)
{
	int sx = b.sx, sy = b.sy;
	int dx = b.tx, dy = b.ty;
	int tw = b.w, th = b.h;
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
		return false;

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
		return false;
	
	b.sx = sx;
	b.sy = sy;
	b.w = tw;
	b.h = th;
	b.tx = dx;
	b.ty = dy;

	return true;
}

}

