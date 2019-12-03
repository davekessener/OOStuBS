/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                             C G A _ S C R E E N                           */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Mit Hilfe dieser Klasse kann man auf den Bildschirm des PCs zugreifen.    */
/* Der Zugriff erfolgt direkt auf der Hardwareebene, d.h. ueber den Bild-    */
/* schirmspeicher bzw. die I/O-Ports der Grafikkarte.                        */
/*****************************************************************************/

#ifndef OOOSTUBS_MACHINE_CGASCREEN_H
#define OOOSTUBS_MACHINE_CGASCREEN_H

#include "machine/io_port.h"

#include "mpl/singleton.h"

namespace oostubs
{
	namespace impl
	{
		template<typename T>
		inline constexpr uint16_t cgascreen_construct(T a, T b)
		{
			return (static_cast<uint16_t>(b) << 4) | static_cast<uint16_t>(a);
		}
	}

	class CGAScreen
	{
		public:
		enum class Color : uint8_t
		{
			BLACK = 0,
			BLUE,
			GREEN,
			CYAN,
			RED,
			MAGENTA,
			BROWN,
			GRAY,
			DARK_GRAY,
			BRIGHT_BLUE,
			BRIGHT_GREEN,
			BRIGHT_CYAN,
			BRIGHT_RED,
			BRIGHT_MAGENTA,
			YELLOW,
			WHITE
		};

		static constexpr uint WIDTH = 80;
		static constexpr uint HEIGHT = 25;

		typedef uint8_t attrib_t;

		static constexpr attrib_t attribute(Color fgc, Color bgc)
			{ return impl::cgascreen_construct(fgc, bgc); }

		static constexpr attrib_t DEFAULT_ATTRIBUTE = impl::cgascreen_construct(Color::WHITE, Color::BLACK);

		public:
			CGAScreen( );
			void setCursor(uint, uint);
			void getCursor(uint *, uint *);
			void putc(char, attrib_t = DEFAULT_ATTRIBUTE);
			void putc_at(uint, uint, char, attrib_t = DEFAULT_ATTRIBUTE);
			void puts_n(char *, int, attrib_t = DEFAULT_ATTRIBUTE);
			void puts(char *s, attrib_t a = DEFAULT_ATTRIBUTE) { puts_n(s, -1, a); }
			void wputs_n(uint16_t *, uint);
			void scroll( );
			void clear( );

		private:
			CGAScreen(const CGAScreen&) = delete;

			void update( );
			void putc_impl(char, attrib_t);

			uint mPosX, mPosY;
			IO_Port mControlPort, mDataPort;
	};

	typedef mpl::SingletonHolder<CGAScreen> Screen;
}

#endif

