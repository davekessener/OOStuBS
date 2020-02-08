#ifndef OOSTUBS_GUI_TEXTURE_H
#define OOSTUBS_GUI_TEXTURE_H

#include "aux.h"

namespace oostubs
{
	class Texture
	{
		struct bounds
		{
			int sx, sy;
			uint w, h;
			int tx, ty;

			bounds(int sx, int sy, uint w, uint h, int tx, int ty)
				: sx(sx), sy(sy), w(w), h(h), tx(tx), ty(ty) { }
		};

		public:
			Texture(size_t, size_t);
			Texture(Texture&& tx) : mTexture(tx.mTexture), mWidth(tx.mWidth), mHeight(tx.mHeight) { tx.mTexture = nullptr; }
			~Texture( );

			void blt(const Texture& src, int dx, int dy, bool a = true)
				{ blt(src, 0, 0, src.width(), src.height(), dx, dy, a); }
			void blt(const Texture&, int, int, uint, uint, int, int, bool = true);
			void unchecked_blt(const Texture&, int, int, uint, uint, int, int, bool = true);
			void blend_blt(const Texture& src, int dx, int dy) { blend_blt(src, 0, 0, src.width(), src.height(), dx, dy); }
			void blend_blt(const Texture&, int, int, uint, uint, int, int);

			void fill_rect(uint, uint, uint, uint, u32);
			void vflip( );
			void hflip( );

			u32 *begin( ) { return mTexture; }
			u32 *end( ) { return mTexture + mWidth * mHeight; }
			size_t width( ) const { return mWidth; }
			size_t height( ) const { return mHeight; }
			u32& at(uint x, uint y) { return mTexture[x + y * mWidth]; }
			const u32& at(uint x, uint y) const { return mTexture[x + y * mWidth]; }

		private:
			bool check_bounds(const Texture&, bounds&);

		private:
			u32 *mTexture;
			size_t mWidth, mHeight;
	};
}

#endif

