#ifndef OOSTUBS_GUI_TERMINAL_H
#define OOSTUBS_GUI_TERMINAL_H

#include "aux.h"

#include "machine/cga_screen.h"

#include "gui/screen.h"

namespace oostubs
{
	class Terminal
	{
		public:
		typedef CGAScreen::attrib_t attrib_t;

		static constexpr size_t CHARW = 8;
		static constexpr size_t CHARH = 16;
		static constexpr size_t WIDTH = Screen::WIDTH / CHARW;
		static constexpr size_t HEIGHT = Screen::HEIGHT / CHARH;

		static constexpr attrib_t DEFAULT_ATTRIBUTE = CGAScreen::DEFAULT_ATTRIBUTE;

		public:
			Terminal( );

			void setCursor(uint, uint);
			void getCursor(uint *, uint *);
			void putc(char, attrib_t = DEFAULT_ATTRIBUTE);
			void putc_at(uint, uint, char, attrib_t = DEFAULT_ATTRIBUTE);
			void puts_n(const char *, int, attrib_t = DEFAULT_ATTRIBUTE);
			void puts(const char *s, attrib_t a = DEFAULT_ATTRIBUTE) { puts_n(s, -1, a); }
			void wputs_n(const uint16_t *, uint);
			void scroll( );
			void clear( );

		private:
			void putc_impl(uint, uint, char);
			void putc_impl(char);
			void scroll_impl( );
			void sync( );

		private:
			Texture mCharset;
			uint mCX, mCY;
	};
}

#endif

