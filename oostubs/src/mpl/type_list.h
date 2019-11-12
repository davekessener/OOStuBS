#ifndef OOSTUBS_MPL_TYPELIST_H
#define OOSTUBS_MPL_TYPELIST_H

namespace oostubs
{
	namespace mpl
	{
		template<typename H, typename T>
		struct Cons
		{
			typedef H Head;
			typedef T Tail;
		};

		struct Nil{ };

		template<typename ...>
		struct MakeList;

		template<typename H, typename ... T>
		struct MakeList<H, T...>
		{
			typedef Cons<H, typename MakeList<T...>::Type> Type;
		};

		template<>
		struct MakeList<>
		{
			typedef Nil Type;
		};

		template<typename ... T>
		using make_list = typename MakeList<T...>::Type;

		template<typename C>
		struct Car
		{
			typedef typename C::Head Type;
		};

		template<typename C>
		struct Cdr
		{
			typedef typename C::Tail Type;
		};

		template<typename, typename>
		struct Map_Get;

		template<typename K1, typename V1, typename T, typename K>
		struct Map_Get<Cons<Cons<K1, V1>, T>, K>
			: Map_Get<T, K>
		{
		};

		template<typename K, typename V, typename T>
		struct Map_Get<Cons<Cons<K, V>, T>, K>
		{
			typedef V Type;
		};

		template<typename M, typename K>
		using map_get = typename Map_Get<M, K>::Type;

		template<typename T>
		struct Decay
		{
			typedef T Type;
		};

		template<typename T>
		struct Decay<const T> : Decay<T>
		{
		};

		template<typename T>
		struct Decay<volatile T> : Decay<T>
		{
		};

		template<typename T>
		struct Decay<T&> : Decay<T>
		{
		};

		template<typename T>
		struct Decay<T&&> : Decay<T>
		{
		};

		template<typename T>
		using decay = typename Decay<T>::Type;
	}
}

#endif

