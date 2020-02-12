#ifndef OOSTUBS_LIB_RNG_H
#define OOSTUBS_LIB_RNG_H

#include "lib.h"

namespace oostubs
{
	template<u32 A = 6700417, u32 C = 524287>
	class RNG
	{
		public:
			RNG(u32 seed = 0x12345678) : mSeed(seed) { }

			u32 operator()( );

			void seed(u32 seed) { mSeed = seed; }
			u32 seed( ) const { return mSeed; }

		private:
			u32 mSeed;
	};

	typedef RNG<> rng_t;

	template<u32 A, u32 C>
	u32 RNG<A, C>::operator()(void)
	{
		u64 v = ((u64) mSeed) * A + (u64) C;

		return mSeed = (u32) (v >> 8);
	}
}

#endif

