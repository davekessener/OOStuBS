#ifndef OOSTUBS_MPL_PAIR_H
#define OOSTUBS_MPL_PAIR_H

#include "mpl/utils.h"

namespace oostubs
{
	namespace mpl
	{
		template<typename U, typename V>
		struct Pair
		{
			U first;
			V second;

			Pair( ) { }
			template<typename UU, typename VV>
				Pair(UU&& u, VV&& v) : first(forward<UU>(u)), second(forward<VV>(v)) { }
			Pair(const Pair<U, V>& p) : first(p.first), second(p.second) { }
			Pair(Pair<U, V>&& p) : first(move(p.first)), second(move(p.second)) { }
		};

		template<typename U, typename V>
		Pair<decay<U>, decay<V>> make_pair(U&& a, V&& b)
		{
			return {forward<U>(a), forward<V>(b)};
		}
	}
}

#endif

