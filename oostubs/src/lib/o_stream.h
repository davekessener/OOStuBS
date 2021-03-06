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
				void write(const char *p, uint c) { write((const u8 *) p, c); }
				void write(const u8 *p, uint c) { while(c--) StringBuffer<N, T>::putc(*p++); }

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

			struct ostream_mem_impl
			{
				ostream_mem_impl(const volatile void *src, uint l) : src(src), len(l) { }

				const volatile void *src;
				uint len;
			};

			template<typename S>
			using ostream_meta_ops = mpl::make_list<
				mpl::Cons<ostream_endl, ostream_endl_impl<S>>,
				mpl::Cons<ostream_flush, ostream_flush_impl<S>>
			>;
		}

		inline impl::ostream_endl endl( ) { return {}; }
		inline impl::ostream_flush flush( ) { return {}; }
		inline impl::ostream_mem_impl memory(const volatile void *src, uint len) { return {src, len}; }
		inline impl::ostream_mem_impl memory(const void *src, uint len) { return {(const volatile void *) src, len}; }

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
		S& operator<<(S& os, const volatile void *p)
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
		S& operator<<(S& os, const void *p)
		{
			return os << static_cast<const volatile void *>(p);
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

				*--p = static_cast<char>(d + (d < 10 ? '0' : ('A' - 10)));
				v /= base;
			}

			while(os.width() && e - p <= os.width())
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

		template<typename S, typename = mpl::enable_if<impl::is_an_ostream<S>>>
		S& operator<<(S& os, const impl::ostream_mem_impl& mem)
		{
			u64 base = ((u64) mem.src) & ~0xF;
			const u8 *p = (const u8 *) mem.src;
			uint h = (mem.len + 0x10 + (base - ((u64) mem.src)) - 1) / 0x10;

			Format fmt = os.format();
			uint w = os.width();
			char f = os.fill();

			os.format(Format::HEX);
			os.width(2);
			os.fill('0');

			for(uint dy = 0 ; dy < h ; ++dy)
			{
				os << &p[dy * 0x10] << ": ";

				for(uint dx = 0 ; dx < 0x10 ; ++dx)
				{
					const u8 *pp = p + dx + dy * 0x10;

					if(pp < p || p >= p + mem.len)
					{
						os << "   ";
					}
					else
					{
						os << *pp << " ";
					}
				}

				os << "\n";
			}

			os.format(fmt);
			os.width(w);
			os.fill(f);

			return os;
		}
	}
}

#undef MXT_CBUF_MAX

#endif

