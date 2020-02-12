#ifndef OOSTUBS_UTILS_FIXEDVECTOR_H
#define OOSTUBS_UTILS_FIXEDVECTOR_H

#include "lib.h"

#include "mpl/utils.h"

namespace oostubs
{
	template
	<
		size_t N,
		typename T
	>
	class FixedVector
	{
		public:
		typedef T value_type;
		typedef value_type *pointer;
		typedef const value_type *const_pointer;
		typedef pointer iterator;
		typedef const_pointer const_iterator;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		static constexpr size_t CAPACITY = N;

		public:
			FixedVector( ) : mSize(0) { }
			FixedVector(const FixedVector<N, T>&);
			~FixedVector( ) { clear(); }
			reference first( ) { return content(0); }
			const_reference first( ) const { return content(0); }
			reference last( ) { return content(mSize-1); }
			const_reference last( ) const { return content(mSize-1); }
			reference operator[](int i) { return content(i); }
			const_reference operator[](int i) const { return content(i); }
			iterator begin( ) { return &first(); }
			const_iterator begin( ) const { return &first(); }
			const_iterator cbegin( ) const { return &first(); }
			iterator end( ) { return &content(mSize); }
			const_iterator end( ) const { return &content(mSize); }
			const_iterator cend( ) const { return &content(mSize); }
			uint size( ) const { return mSize; }
			bool empty( ) const { return !mSize; }

			template<typename TT>
				void insert(const_iterator, TT&&);

			void remove(const_iterator);

			template<typename TT>
				void emplace_back(TT&& v)
					{ new(end()) value_type(mpl::forward<TT>(v)); ++mSize; }

			void push_back(const T& v)
				{ emplace_back(v); }

			template<typename TT>
				void emplace_front(TT&& v)
					{ insert(begin(), mpl::forward<TT>(v)); }

			void push_front(const T& v)
				{ emplace_front(v); }

			void clear( );

		private:
			reference content(int i) { return reinterpret_cast<pointer>(mContent)[i]; }
			const_reference content(int i) const { return reinterpret_cast<const_pointer>(mContent)[i]; }

		private:
			uint8_t mContent[N * sizeof(value_type)];
			uint mSize;
	};

	template<size_t N, typename T>
	FixedVector<N, T>::FixedVector(const FixedVector<N, T>& v)
	{
		mSize = v.mSize;

		for(uint i = 0 ; i < mSize ; ++i)
		{
			new(&content(i)) value_type(v[i]);
		}
	}

	template<size_t N, typename T>
	void FixedVector<N, T>::clear(void)
	{
		for(uint i = 0 ; i < mSize ; ++i)
		{
			content(i).~value_type();
		}

		mSize = 0;
	}

	template<size_t N, typename T>
	template<typename TT>
	void FixedVector<N, T>::insert(const_iterator i, TT&& v)
	{
		iterator j = end();

		for(; i != j ; --j)
		{
			new(j) value_type(mpl::move(j[-1]));
			j[-1].~value_type();
		}

		new(j) value_type(mpl::forward<TT>(v));

		++mSize;
	}

	template<size_t N, typename T>
	void FixedVector<N, T>::remove(const_iterator i)
	{
		if(i == end())
			return;

		i->~value_type();

		while(++i != end())
		{
			new(i-1) value_type(mpl::move(*i));
			i->~value_type();
		}

		--mSize;
	}
}

#endif

