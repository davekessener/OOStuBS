#ifndef OOSTUBS_OBJECT_STRINGBUFFER_H
#define OOSTUBS_OBJECT_STRINGBUFFER_H

#include "machine/cga_screen.h"

namespace oostubs
{
	template<size_t N, typename T = char>
	class StringBuffer
	{
		public:
		typedef T value_type;
		static constexpr size_t CAPACITY = N;

		public:
			StringBuffer( ) : mSize(0) { }
			virtual ~StringBuffer( ) { }
	
			void flush( );
			void putc(value_type);
			void puts(const value_type *, uint);
			void puts(const char *s) { while(*s) putc(*s++); }

		protected:
			value_type *data( ) { return mBuffer; }
			size_t size( ) { return mSize; }

			virtual void doFlush( ) = 0;
			virtual void doPutc(value_type);
	
		private:
			value_type mBuffer[N];
			size_t mSize;
	};

	template<size_t N, typename T>
	void StringBuffer<N, T>::flush(void)
	{
		doFlush();

		mSize = 0;
	}

	template<size_t N, typename T>
	void StringBuffer<N, T>::putc(value_type c)
	{
		if(mSize == N)
		{
			flush();
		}

		doPutc(c);
	}

	template<size_t N, typename T>
	void StringBuffer<N, T>::puts(const value_type *s, uint l)
	{
		while(l--)
		{
			putc(*s++);
		}
	}

	template<size_t N, typename T>
	void StringBuffer<N, T>::doPutc(value_type c)
	{
		mBuffer[mSize++] = c;
	}
}

#endif

