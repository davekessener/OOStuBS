#ifndef LIB_FLOAT_H
#define LIB_FLOAT_H

#include "lib.h"

#include "lib/o_stream.h"

#define double ::oostubs::Double

namespace oostubs
{
	class Double
	{
	#define MXT_BSET(n) (((uint64_t)-1)>>(64-(n)))
		static constexpr uint64_t BL_S = 1;
		static constexpr uint64_t BL_E = 10;
		static constexpr uint64_t BL_M = 64 - BL_S - BL_E;
		static constexpr uint64_t BO_S = BL_M + BL_E;
		static constexpr uint64_t BO_E = BL_M;
		static constexpr uint64_t BO_M = 0;
		static constexpr uint64_t BM_S = MXT_BSET(BL_S) << (BL_E + BL_M);
		static constexpr uint64_t BM_E = MXT_BSET(BL_E) << BL_M;
		static constexpr uint64_t BM_M = MXT_BSET(BL_M);
		static constexpr  int64_t MAN_TO_32   = -(32 - BL_M);
		static constexpr  int64_t MAN_TO_64   =   64 - BL_M;
		static constexpr uint64_t BM_MM = MXT_BSET(BL_M) << (64 - BL_E - BL_S);
		static constexpr uint64_t BM_E_H = (1ull << (BL_E - 1));
		static constexpr uint64_t BM_E_L = (BM_E_H - 1);
		static constexpr uint64_t BM_M_H = (1ull << (BL_M - 1));
	#undef MXT_BSET
	
		static_assert(!~(BM_S|BM_E|BM_M), "");
	
		public:
			Double( ) { }
			Double(int64_t v) { update(false, 0, v); }
	
			bool sign( ) const { return (mData & BM_S) >> BO_S; }
	
			int64_t exponent( ) const
			{
				auto e{(mData & BM_E) >> BO_E};
				return (e & BM_E_H) ? -static_cast<int64_t>((~e + 1) & BM_E_L) : static_cast<int64_t>(e);
			}
	
			uint64_t mantissa( ) const { return (mData & BM_M) >> BO_M; }
	
			explicit operator int64_t( ) const;
			explicit operator int32_t( ) const { return (i32) static_cast<int64_t>(*this); }
			explicit operator int16_t( ) const { return (i16) static_cast<int64_t>(*this); }
			explicit operator int8_t( ) const { return (i8) static_cast<int64_t>(*this); }
			explicit operator uint64_t( ) const { return (u64) static_cast<int64_t>(*this); }
			explicit operator uint32_t( ) const { return (u32) static_cast<int64_t>(*this); }
			explicit operator uint16_t( ) const { return (u16) static_cast<int64_t>(*this); }
			explicit operator uint8_t( ) const { return (u8) static_cast<int64_t>(*this); }
	
			Double round(uint) const;
	
			Double abs( ) const { Double r{*this}; r.mData &= ~BM_S; return r; }
	
			Double& operator*=(const Double&);
			Double& operator/=(const Double&);
			Double& operator+=(const Double&);
			Double& operator-=(const Double& v) { Double t{v}; t.mData ^= BM_S; return *this += t; }
	
			bool operator<(const Double&) const;
			bool operator>(const Double& v) const { return v < *this; }
			bool operator==(const Double& v) const { return mData == v.mData; }
	
		private:
			void update(bool, int64_t, uint64_t);
	
		private:
			uint64_t mData;
	};
	
	void Double::update(bool s, int64_t e, uint64_t m)
	{
		while((m & BM_M) != m)
		{
			m >>= 1;
			++e;
		}
	
		while(m && !(m & BM_M_H))
		{
			m <<= 1;
			--e;
		}
	
		mData = (s ? BM_S : 0) | ((static_cast<uint64_t>(e) << BO_E) & BM_E) | (m & BM_M);
	}
	
	Double::operator int64_t(void) const
	{
		int64_t v{static_cast<int64_t>(mantissa())};
		int64_t e{exponent()};
	
		while(e > 0)
		{
			v <<= 1;
			--e;
		}
	
		while(e < 0)
		{
			v >>= 1;
			++e;
		}
	
		return sign() ? -v : v;
	}
	
	bool Double::operator<(const Double& v) const
	{
		if(sign() == v.sign())
		{
			return exponent() < v.exponent() || (exponent() == v.exponent() && mantissa() < v.mantissa());
		}
		else
		{
			return sign();
		}
	}
	
	Double& Double::operator*=(const Double& d)
	{
		auto s{sign() != d.sign()};
		auto m{(mantissa() >> MAN_TO_32) * (d.mantissa() >> MAN_TO_32)};
		auto e{exponent() + d.exponent() + 2 * MAN_TO_32};
	
		update(s, e, m);
	
		return *this;
	}
	
	Double& Double::operator/=(const Double& d)
	{
		auto s{sign() != d.sign()};
		auto m{(d.mantissa() << MAN_TO_64) >> BL_M};
		auto e{exponent() - d.exponent() - BL_M};
	
		if(!m)
		{
			// TODO NaN
		}
		else
		{
			m = (mantissa() << MAN_TO_64) / m;
	
			update(s, e, m);
		}
	
		return *this;
	}
	
	Double& Double::operator+=(const Double& d)
	{
		auto e1{exponent()}, e2{d.exponent()};
		auto m1{mantissa() << MAN_TO_64}, m2{d.mantissa() << MAN_TO_64};
		auto ex = ((e1 < e2) || (e1 == e2 && m1 < m2));
	
		if(ex)
		{
			auto e = e1; e1 = e2; e2 = e;
			auto m = m1; m1 = m2; m2 = m;
		}
	
		while(e2 < e1)
		{
			m2 >>= 1;
			++e2;
		}
	
		auto s{sign()};
		decltype(m1) m = 0;
		auto e = e1 - MAN_TO_64;
	
		if(sign() != d.sign())
		{
			s = (ex ? d.sign() : sign());
	
			m = m1 - m2;
		}
		else
		{
			m = (m1 >> 1) + (m2 >> 1) + 1;
	
			++e;
		}
	
		update(s, e, m);
	
		return *this;
	}
	
	Double operator*(const Double& v1, const Double& v2)
	{
		Double v{v1};
	
		return v *= v2;
	}
	
	Double operator/(const Double& v1, const Double& v2)
	{
		Double v{v1};
	
		return v /= v2;
	}
	
	Double operator+(const Double& v1, const Double& v2)
	{
		Double v{v1};
	
		return v += v2;
	}
	
	Double operator-(const Double& v1, const Double& v2)
	{
		Double v{v1};
	
		return v -= v2;
	}
	
	Double Double::round(uint n) const
	{
		int b = 10;
	
		while(n--)
		{
			b *= 10;
		}
	
		Double c{*this};
	
		c.mData &= ~BM_S;
	
		int64_t v = static_cast<int64_t>(c * b);
	
		v += (10 - (v % 10)) % 10;
	
		v /= 10;
		b /= 10;
	
		c = Double{v} / b;
	
		if(sign())
		{
			c.mData |= BM_S;
		}
	
		return c;
	}

	namespace io
	{
		template<typename S, typename = mpl::enable_if<impl::is_an_ostream<S>>>
		S& operator<<(S& os, const Double& v)
		{
			os << static_cast<int64_t>(v) << '.';

			int w = (int) os.width();
			
			if(!w) w = 3;

			int b = 1;
			for(int i = 0 ; i < w ; ++i)
			{
				b *= 10;

				os << (static_cast<int64_t>(v * b) % 10);
			}

			return os;
		}
	}
}

#endif

