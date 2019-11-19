#ifndef OOSTUBS_MPL_ALGORITHM_H
#define OOSTUBS_MPL_ALGORITHM_H

namespace oostubs
{
	namespace mpl
	{
		template<typename I, typename T>
		void fill(I i1, I i2, const T& v)
		{
			for(; i1 != i2 ; ++i1)
			{
				*i1 = v;
			}
		}
	}
}

#endif

