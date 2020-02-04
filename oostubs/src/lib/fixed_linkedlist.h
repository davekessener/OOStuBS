#ifndef OOSTUBS_LIB_FIXEDLINKEDLIST_H
#define OOSTUBS_LIB_FIXEDLINKEDLIST_H

#include "aux.h"

#include "panic.h"

namespace oostubs
{
	template
	<
		typename T,
		size_t N
	>
	class FixedLinkedList
	{
		struct Cell
		{
			T value;
			Cell *next;
			Cell *prev;
		};

		public:
		class iterator
		{
			public:
				iterator( ) : mSelf(nullptr) { }

				T& operator*( ) { return mSelf->value; }
				T *operator->( ) { return &mSelf->value; }
				explicit operator bool( ) const { return mSelf; }
				iterator& operator++( ) { mSelf = mSelf->next; return *this; }
				iterator operator++(int) { iterator i(*this); ++*this; return i; }
				iterator& operator--( ) { mSelf = mSelf->prev; return *this; }
				iterator operator--(int) { iterator i(*this); --*this; return i; }
				bool operator==(const iterator& i) const { return mSelf == i.mSelf; }
				bool operator!=(const iterator& i) const { return !(*this == i); }

			private:
				iterator(Cell *c) : mSelf(c) { }

			private:
				Cell *mSelf;

				friend class FixedLinkedList<T, N>;
		};

		public:
			FixedLinkedList( );

			uint size( ) const { return mSize; }
			bool empty( ) const { return !mSize; }
			bool full( ) const { return mSize == N; }

			void push(const T&);
			T pop( );
			bool remove(const T&);
			void insert(iterator, const T&);
			iterator remove(iterator);

			iterator begin( ) { return {mFirst}; }
			iterator end( ) { return {nullptr}; }

		private:
			Cell mContent[N];
			Cell *mEmpty;
			Cell *mFirst;
			Cell *mLast;
			uint mSize;
	};

	template<typename T, size_t N>
	FixedLinkedList<T, N>::FixedLinkedList(void)
	{
		mEmpty = &mContent[0];
		mFirst = mLast = nullptr;
		mSize = 0;

		for(uint i = 0 ; i < N - 1 ; ++i)
		{
			mContent[i].next = &mContent[i+1];
			mContent[i+1].prev = &mContent[i];
		}

		mContent[0].prev = nullptr;
		mContent[N-1].next = nullptr;
	}

	template<typename T, size_t N>
	void FixedLinkedList<T, N>::push(const T& v)
	{
		ASSERT(!full());

		if(empty())
		{
			mFirst = mLast = mEmpty;
			mEmpty = mEmpty->next;
			mFirst->next = mFirst->prev = nullptr;
			mFirst->value = v;
		}
		else
		{
			auto *c = mEmpty;

			mEmpty = mEmpty->next;

			mLast->next = c;

			c->next = nullptr;
			c->prev = mLast;
			c->value = v;

			mLast = c;
		}

		++mSize;
	}

	template<typename T, size_t N>
	T FixedLinkedList<T, N>::pop(void)
	{
		ASSERT(!empty());

		Cell *c = mFirst;

		remove(begin());

		return c->value;
	}

	template<typename T, size_t N>
	bool FixedLinkedList<T, N>::remove(const T& v)
	{
		for(auto i1 = begin(), i2 = end() ; i1 != i2 ; ++i1)
		{
			if(*i1 == v)
			{
				remove(i1);

				return true;
			}
		}

		return false;
	}

	template<typename T, size_t N>
	void FixedLinkedList<T, N>::insert(iterator i, const T& v)
	{
		if(empty() || !i.mSelf)
		{
			push(v);
		}
		else
		{
			ASSERT(!full());
			ASSERT(i.mSelf);

			Cell *c = mEmpty;

			mEmpty = mEmpty->next;

			c->prev = i.mSelf->prev;
			c->next = i.mSelf;
			if(i.mSelf->prev)
			{
				i.mSelf->prev->next = c;
			}
			else
			{
				mFirst = c;
			}
			i.mSelf->prev = c;

			c->value = v;

			++mSize;
		}
	}

	template<typename T, size_t N>
	typename FixedLinkedList<T, N>::iterator FixedLinkedList<T, N>::remove(iterator i)
	{
		Cell *c = i.mSelf;

		if(c == mFirst)
		{
			mFirst = mFirst->next;
		}

		Cell *p = c->prev;
		Cell *n = c->next;

		if(n) n->prev = p;
		if(p) p->next = n;

		if(!mFirst) mLast = nullptr;

		c->next = mEmpty;
		c->prev = nullptr;
		mEmpty->prev = c;
		mEmpty = c;

		--mSize;

		return {n};
	}
}

#endif

