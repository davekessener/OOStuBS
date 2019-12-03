#ifndef OOSTUBS_DEVICE_CGASTREAM_H
#define OOSTUBS_DEVICE_CGASTREAM_H

#include "lib/o_stream.h"

#include "machine/cga_screen.h"

namespace oostubs
{
	namespace io
	{
		namespace impl
		{
			struct cgastream_a
			{
				cgastream_a(uint8_t a, uint8_t m) : attrib(a), mask(m) { }

				uint8_t attrib, mask;
			};
		}

		class CGAStream : public OStream<(CGAScreen::WIDTH * CGAScreen::HEIGHT), uint16_t>
		{
			typedef OStream<(CGAScreen::WIDTH * CGAScreen::HEIGHT), uint16_t> Super;

			public:
			typedef CGAScreen::Color Color;
			typedef CGAScreen::attrib_t attrib_t;

			public:
				CGAStream( ) : mAttrib(CGAScreen::DEFAULT_ATTRIBUTE) { }
				Color foreground_color( ) const { return static_cast<Color>(mAttrib & 0x0F); }
				Color background_color( ) const { return static_cast<Color>((mAttrib >> 4) & 0x0F); }
				void foreground_color(Color c) { mAttrib = (mAttrib & ~0x0F) | static_cast<uint8_t>(c); }
				void background_color(Color c) { mAttrib = (mAttrib & ~0xF0) | (static_cast<uint8_t>(c) << 4); }
				attrib_t attribute( ) const { return mAttrib; }
				void attribute(attrib_t a) { mAttrib = a; }

			protected:
				void doFlush( ) override;
				void doPutc(Super::value_type) override;

			private:
				attrib_t mAttrib;
		};

		inline impl::cgastream_a set_foreground(CGAStream::Color c)
		{
			return {static_cast<uint8_t>(c), 0x0F};
		}

		inline impl::cgastream_a set_background(CGAStream::Color c)
		{
			return {static_cast<uint8_t>(static_cast<uint8_t>(c) << 4), 0xF0};
		}

		inline impl::cgastream_a set_color(CGAStream::Color fg, CGAStream::Color bg)
		{
			return {CGAScreen::attribute(fg, bg), 0xFF};
		}

		inline impl::cgastream_a set_attribute(uint8_t a)
		{
			return {a, 0xFF};
		}

		template<typename S, typename = mpl::enable_if<mpl::IsSupertypeOf<CGAStream, S>>>
		S& operator<<(S& os, const impl::cgastream_a& a)
		{
			os.attribute((os.attribute() & ~a.mask) | (a.attrib & a.mask));

			return os;
		}
	}
}

#endif

