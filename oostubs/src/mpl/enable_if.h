#ifndef OOSTUBS_MPL_ENABLEIF_H
#define OOSTUBS_MPL_ENABLEIF_H

#include "lib.h"

namespace oostubs
{
	namespace mpl
	{
		namespace impl
		{
			template<bool>
			struct EnableIfImpl
			{
			};

			template<>
			struct EnableIfImpl<true>
			{
				typedef void Type;
			};
		}

		template<typename C>
		using enable_if = typename impl::EnableIfImpl<C::Value>::Type;

		template<typename Super, typename Sub>
		struct IsSupertypeOf
		{
			struct yes { char _[1]; };
			struct no  { char _[2]; };

			static yes test(Super *);
			static no  test(...);

			static constexpr bool Value = (sizeof(test((Sub *) nullptr)) == sizeof(yes));
		};
	}
}

#endif

