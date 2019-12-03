#ifndef OOSTUBS_MPL_ALGORITHM_H
#define OOSTUBS_MPL_ALGORITHM_H

namespace oostubs
{
	namespace mpl
	{
		template<typename I, typename T>
		I fill(I i1, I i2, const T& v)
		{
			while(i1 != i2)
			{
				*i1++ = v;
			}

			return i1;
		}

		template<typename I, typename O>
		O copy(I i1, I i2, O j)
		{
			while(i1 != i2)
			{
				*j++ = *i1++;
			}

			return j;
		}

		template<typename I, typename O, typename P>
		O copy_if(I i1, I i2, O j, P&& f)
		{
			for(; i1 != i2 ; ++j, ++i1)
			{
				if(f(*i1))
				{
					*j = *i1;
				}
			}

			return j;
		}
	}
}

#endif

