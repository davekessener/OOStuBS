#ifndef OOSTUBS_LIB_STRING_H
#define OOSTUBS_LIB_STRING_H

namespace oostubs
{
	template<typename T = char>
	class String
	{
		typedef String<T> Self;

		public:
		typedef T value_type;
		typedef value_type *pointer;
		typedef const value_type *const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;

		public:
			String( ) : mContent(nullptr), mSize(0) { }
			String(const_pointer);
			String(const Self&);
			String(Self&&);
			~String( );

			size_t size( ) const { return mSize; }
			
			explicit operator pointer( ) { return mContent; }
			explicit operator const_pointer( ) const { return mContent; }
			reference operator[](int i) { return mContent[i]; }
			const_reference operator[](int i) const { return mContent[i]; }

			void swap(Self&) noexcept;
			Self& operator=(const Self&);
			Self& operator=(Self&&);
			void clear( );

		private:
			T *mContent;
			size_t mSize;
	};

	template<typename T>
	String<T>::String(const_pointer p)
	{
		auto t = p;
		size_t n = 0;

		while(*t++) ++n;

		mSize = n;
		mContent = new value_type[mSize + 1];

		mpl::copy(p, p + n, mContent);
		mContent[mSize] = 0;
	}

	template<typename T>
	String<T>::String(const Self& s)
		: mContent(nullptr)
		, mSize(0)
	{
		*this = s;
	}

	template<typename T>
	String<T>::String(Self&& s)
		: mContent(nullptr)
		, mSize(0)
	{
		*this = mpl::move(s);
	}

	template<typename T>
	String<T>::~String(void)
	{
		clear();
	}

	template<typename T>
	void String<T>::swap(Self& o) noexcept
	{
		pointer t1 = mContent; mContent = o.mContent; o.mContent = t1;
		size_t t2 = mSize; mSize = o.mSize; o.mSize = t2;
	}

	template<typename T>
	String<T>::Self& String<T>::operator=(const Self& s)
	{
		clear();

		mSize = s.size();
		mContent = new value_type[mSize + 1];

		mpl::copy(s.mContent, c.mContent + mSize, mContent);
		mContent[mSize] = 0;

		return *this;
	}

	template<typename T>
	String<T>::Self& String<T>::operator=(Self&& s)
	{
		clear();

		swap(s);

		return *this;
	}

	template<typename T>
	void String<T>::clear(void)
	{
		if(mContent)
		{
			delete[] mContent;
			mContent = nullptr;
			mSize = 0;
		}
	}
}

#endif

