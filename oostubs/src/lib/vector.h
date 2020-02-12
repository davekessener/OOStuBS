#ifndef OOSTUBS_LIB_VECTOR_H
#define OOSTUBS_LIB_VECTOR_H

#include "aux.h"
#include "panic.h"

#include "mpl/utils.h"

namespace oostubs
{
	template<typename T>
	class Vector
	{
		public:
		typedef T value_type;
		typedef T *iterator;
		typedef const T *const_iterator;

		public:
			Vector(uint = 1);

			value_type& at(uint i) { ASSERT(i < mSize); return mContent[i]; }
			const value_type& at(uint i) const { ASSERT(i < mSize); return mContent[i]; }
			iterator begin( ) { return mContent; }
			iterator end( ) { return mContent + mSize; }
			const_iterator begin( ) const { return mContent; }
			const_iterator end( ) const { return mContent + mSize; }
			void push_back(const value_type& t)
				{ emplace_back(t); }
			template<typename ... TT>
				void emplace_back(TT&& ... a)
					{ emplace(end(), mpl::forward<TT>(a)...); }
			void insert(const_iterator i, const T& v)
				{ emplace(i, v); }
			template<typename ... TT>
				void emplace(const_iterator, TT&& ...);
			iterator remove(iterator);
			const_iterator remove(const_iterator i)
				{ return remove(const_cast<iterator>(i)); }

			uint size( ) const { return mSize; }
			uint capacity( ) const { return mCapacity; }
			bool empty( ) const { return !mSize; }

		private:
			void resize( );

		private:
			value_type *mContent;
			uint mSize, mCapacity;
	};

	template<typename T>
	Vector<T>::Vector(uint c)
		: mContent(nullptr)
		, mSize(0)
		, mCapacity(0)
	{
		if(c)
		{
			mContent = (value_type *) new u8[(mCapacity = c) * sizeof(value_type)];
		}
	}

	template<typename T>
	template<typename ... TT>
	void Vector<T>::emplace(const_iterator ii, TT&& ... a)
	{
		ASSERT(begin() <= ii && ii <= end());

		resize();

		value_type *i = const_cast<iterator>(ii);

		for(auto i1 = i, i2 = end() ; i1 != i2 ;)
		{
			auto j = i2; --j;

			new(i2) value_type(mpl::move(*j));
			j->~value_type();

			i2 = j;
		}

		new(i) value_type(mpl::forward<TT>(a)...);

		++mSize;
	}

	template<typename T>
	typename Vector<T>::iterator Vector<T>::remove(iterator i)
	{
		ASSERT(!empty());
		ASSERT(begin() <= i && i < end());

		resize();

		for(auto i1 = i, i2 = end() ; true ;)
		{
			auto j = i1; ++j;

			i1->~value_type();
			if(i1 == i2) break;
			new(i1) value_type(mpl::move(*j));

			i1 = j;
		}

		--mSize;
	}

	template<typename T>
	void Vector<T>::resize(void)
	{
		uint c = mCapacity;

		if(mSize == mCapacity)
		{
			c = 2 * mCapacity + 1;
		}
		else if(mSize < mCapacity / 4)
		{
			c = mCapacity / 2;
		}

		if(c != mCapacity)
		{
			value_type *t = (value_type *) new u8[mCapacity];

			for(uint i = 0 ; i < mSize ; ++i)
			{
				new(&t[i]) value_type(mpl::move(mContent[i]));

				mContent[i].~value_type();
			}

			delete[] (u8 *) mContent;

			mContent = t;
		}
	}
}

#endif

