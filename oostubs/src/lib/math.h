#ifndef LIB_MATH_H
#define LIB_MATH_H

namespace oostubs
{
	template<typename T>
	T sqrt(T n)
	{
		T x = n / 2;

		while(true)
		{
			T y = (x + n / x) / 2;

			if(x == y)
				break;

			x = y;
		}

		return x;
	}
}

#endif

