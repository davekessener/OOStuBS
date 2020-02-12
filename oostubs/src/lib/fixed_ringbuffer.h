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
			explicit FixedRingbuffer(bool ow = false) : mReadIdx(0), mWriteIdx(0), mOverwrite(ow) { }
			
			void push(const_reference);
			value_type pop( );
			bool remove(const_reference);
			value_type& top( );

			bool empty( ) const { return mReadIdx == mWriteIdx; }
			bool full( ) const { return mReadIdx == ((mWriteIdx + 1) % (N+1)); }
			size_t size( ) const { return (mWriteIdx + N+1 - mReadIdx) % (N+1); }

		private:
			value_type mContent[N+1];
			uint mReadIdx, mWriteIdx;
			bool mOverwrite;
	};

	template<typename T, size_t N>
	void FixedRingbuffer<T, N>::push(const_reference v)
	{
		if(!mOverwrite)
		{
			ASSERT(!full());
		}
		else
		{
			if(full())
			{
				pop();
			}
		}

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

	template<typename T, size_t N>
	T& FixedRingbuffer<T, N>::top(void)
	{
		ASSERT(!empty());

		return mContent[mReadIdx];
	}

	template<typename T, size_t N>
	bool FixedRingbuffer<T, N>::remove(const_reference v)
	{
		bool found = false;

		for(uint i = mReadIdx, last = 0 ; i != mWriteIdx ; i = (i + 1) % (N+1))
		{
			if(found)
			{
				mContent[last] = mpl::move(mContent[i]);
			}
			else if(mContent[i] == v)
			{
				found = true;
			}

			last = i;
		}

		if(found)
		{
			mWriteIdx = (mWriteIdx + N+1 - 1) % (N+1);
		}

		return found;
	}
}

#endif

