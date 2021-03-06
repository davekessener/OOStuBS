#ifndef OOSTUBS_GUI_SCREEN_H
#define OOSTUBS_GUI_SCREEN_H

#include "lib.h"

#include "gui/texture.h"

#include "mpl/singleton.h"

namespace oostubs
{
	class Screen
	{
		public:
		static constexpr size_t WIDTH = 800, HEIGHT = 600;

		public:
			Screen( );

			void sync( );
			void clear( );

			Texture& buffer( ) { return mBuffer; }

		private:
			Texture mBuffer;
			u8 *mFramebuffer;
			uint mPitch;
	};

	typedef mpl::SingletonHolder<Screen> FramebufferManager;
}

#endif

