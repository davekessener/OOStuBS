#ifndef OOSTUBS_LIB_FIXEDRINGBUFFER_H
#define OOSTUBS_LIB_FIXEDRINGBUFFER_H

#include "aux.h"
#include "panic.h"

#include "mpl/utils.h"

namespace oostubs
{
	template
	<
		typename T,
		size_t N
	>
	class FixedRingbuffer
	{
		typedef FixedRingbuffer<T, N> Self;

		public:
		typedef T value_type;
		typedef value_type *pointer;
		typedef const value_type *const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;

		static constexpr size_t SIZE = N;

		public:
			FixedRingbuffer( ) : mReadIdx(0), mWriteIdx(0) { }
			
			void push(const_reference);
			value_type pop( );

			bool empty( ) const { return mReadIdx == mWriteIdx; }
			bool full( ) const { return mReadIdx == ((mWriteIdx + 1) % (N+1)); }
			size_t size( ) const { return (mWriteIdx + N+1 - mReadIdx) % (N+1); }

		private:
			value_type mContent[N+1];
			uint mReadIdx, mWriteIdx;
	};

	template<typename T, size_t N>
	void FixedRingbuffer<T, N>::push(const_reference v)
	{
		ASSERT(!full());

		mContent[mWriteIdx] = v;
		mWriteIdx = (mWriteIdx + 1) % (N+1);
	}

	template<typename T, size_t N>
	T FixedRingbuffer<T, N>::pop(void)
	{
		ASSERT(!empty());

		value_type r{mpl::move(mContent[mReadIdx])};

		mReadIdx = (mReadIdx + 1) % (N+1);

		return r;
	}
}

#endif

