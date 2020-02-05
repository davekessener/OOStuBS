#ifndef OOSTUBS_OBJECT_OSTREAM_H
#define OOSTUBS_OBJECT_OSTREAM_H

#include "lib/string_buffer.h"

#include "mpl/enable_if.h"
#include "mpl/type_list.h"

#define MXT_CBUF_MAX (64+1)

namespace oostubs
{
	namespace io
	{
		enum class Format
		{
			DEC = 10,
			HEX = 16,
			OCT = 8,
			BIN = 2
		};

		template<size_t N, typename T>
		class OStream;

		template<size_t N, typename T = char>
		class OStream : public StringBuffer<N, T>
		{
			static_assert(sizeof(unsigned long) >= sizeof(void *), "pointers have to fit inside unsigned long");

			typedef OStream<N, T> Self;

			public:
			typedef T value_type;
			static constexpr size_t CAPACITY = N;

			public:
				OStream( ) : mFormat(Format::DEC), mFill('\0'), mWidth(0) { }

				Format format( ) const { return mFormat; }
				void format(Format f) { mFormat = f; }
				char fill( ) const { return mFill; }
				void fill(char f) { mFill = f; }
				uint width( ) const { return mWidth; }
				void width(uint w) { mWidth = w; }

			private:
				Format mFormat;
				char mFill;
				uint mWidth;
		};

		namespace impl
		{
			template<typename S>
			struct is_an_ostream
			{
				struct yes { char _[1]; };
				struct no  { char _[2]; };

				template<size_t N, typename T>
				static yes test(OStream<N, T> *);

				static no  test(...);

				static constexpr bool Value = (sizeof(test((S *) nullptr)) == sizeof(yes));
			};

			struct ostream_endl{ };
			struct ostream_flush{ };

			struct ostream_f
			{
				ostream_f(char f) : fill(f) { }

				char fill;
			};

			struct ostream_w
			{
				ostream_w(uint w) : width(w) { }

				uint width;
			};

			template<typename S>
			struct ostream_endl_impl
			{
				static void impl(S& os)
				{
					os << '\n';

					os.flush();
				}
			};

			template<typename S>
			struct ostream_flush_impl
			{
				static void impl(S& os)
				{
					os.flush();
				}
			};

			template<typename S>
			using ostream_meta_ops = mpl::make_list<
				mpl::Cons<ostream_endl, ostream_endl_impl<S>>,
				mpl::Cons<ostream_flush, ostream_flush_impl<S>>
			>;
		}

		inline impl::ostream_endl endl( ) { return {}; }
		inline impl::ostream_flush flush( ) { return {}; }

		template<typename S, typename T, typename = mpl::enable_if<impl::is_an_ostream<S>>>
		S& operator<<(S& os, T (*)(void))
		{
			typedef mpl::map_get<impl::ostream_meta_ops<S>, T> impl_t;

			impl_t::impl(os);

			return os;
		}

		template<typename S, typename = mpl::enable_if<impl::is_an_ostream<S>>>
		S& operator<<(S& os, void (*f)(S&))
		{
			(*f)(os);

			return os;
		}

		template<typename S, typename = mpl::enable_if<impl::is_an_ostream<S>>>
		S& operator<<(S& os, Format f)
		{
			os.format(f);

			return os;
		}

		template<typename S, typename = mpl::enable_if<impl::is_an_ostream<S>>>
		S& operator<<(S& os, const impl::ostream_f& f)
		{
			os.fill(f.fill);

			return os;
		}

		inline impl::ostream_f set_fill(char f)
		{
			return {f};
		}

		template<typename S, typename = mpl::enable_if<impl::is_an_ostream<S>>>
		S& operator<<(S& os, const impl::ostream_w& w)
		{
			os.width(w.width);

			return os;
		}

		inline impl::ostream_w set_width(uint w)
		{
			return {w};
		}

		template<typename S, typename = mpl::enable_if<impl::is_an_ostream<S>>>
		S& operator<<(S& os, void *p)
		{
			Format fmt = os.format();
			uint w = os.width();
			char f = os.fill();

			os << "@$";

			os.format(Format::HEX);
			os.width(sizeof(p) * 2);
			os.fill('0');

			os << reinterpret_cast<unsigned long>(p);

			os.format(fmt);
			os.width(w);
			os.fill(f);

			return os;
		}

		template<typename S, typename = mpl::enable_if<impl::is_an_ostream<S>>>
		S& operator<<(S& os, char c)
		{
			os.putc(c);

			return os;
		}

		template<typename S, typename = mpl::enable_if<impl::is_an_ostream<S>>>
		S& operator<<(S& os, long v)
		{
			if(v < 0)
			{
				return os << '-' << (unsigned long) -v;
			}
			else
			{
				return os << (unsigned long) v;
			}
		}

		template<typename S, typename = mpl::enable_if<impl::is_an_ostream<S>>>
		S& operator<<(S& os, const char *s)
		{
			while(*s)
			{
				os << *s++;
			}

			return os;
		}

		template<typename S, typename = mpl::enable_if<impl::is_an_ostream<S>>>
		S& operator<<(S& os, unsigned long v)
		{
			uint base = static_cast<uint>(os.format());
			char buf[MXT_CBUF_MAX + 1];
			char *e = &buf[MXT_CBUF_MAX + 1];
			char *p = e;

			*--p = '\0';

			if(!v)
			{
				*--p = '0';
			}
			else while(v)
			{
				uint d = v % base;

				*--p = static_cast<char>(d + (d < 10 ? '0' : ('a' - 10)));
				v /= base;
			}

			while(e - p < os.width())
			{
				*--p = os.fill();
			}

			return os << p;
		}

		template<typename S, typename = mpl::enable_if<impl::is_an_ostream<S>>>
		S& operator<<(S& os, unsigned char c)
		{
			return os << (unsigned long) c;
		}

		template<typename S, typename = mpl::enable_if<impl::is_an_ostream<S>>>
		S& operator<<(S& os, short c)
		{
			return os << (long) c;
		}

		template<typename S, typename = mpl::enable_if<impl::is_an_ostream<S>>>
		S& operator<<(S& os, unsigned short c)
		{
			return os << (unsigned long) c;
		}

		template<typename S, typename = mpl::enable_if<impl::is_an_ostream<S>>>
		S& operator<<(S& os, int c)
		{
			return os << (long) c;
		}

		template<typename S, typename = mpl::enable_if<impl::is_an_ostream<S>>>
		S& operator<<(S& os, unsigned int c)
		{
			return os << (unsigned long) c;
		}
	}
}

#undef MXT_CBUF_MAX

#endif

