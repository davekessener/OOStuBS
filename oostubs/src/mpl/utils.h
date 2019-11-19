#ifndef OOSTUBS_MPL_UTILS_H
#define OOSTUBS_MPL_UTILS_H

namespace oostubs
{
	namespace mpl
	{
		template<typename T>
		struct Identity
		{
			typedef T Type;
		};

		template<typename T>
		using identity = typename Identity<T>::Type;

		template<typename T>
		struct RemoveReference
		{
			typedef T Type;
		};

		template<typename T>
		struct RemoveReference<T&>
			: RemoveReference<T>
		{
		};

		template<typename T>
		struct RemoveReference<T&&>
			: RemoveReference<T>
		{
		};

		template<typename T>
		using remove_reference = typename RemoveReference<T>::Type;

		template<typename T>
		struct RemoveConst
		{
			typedef T Type;
		};

		template<typename T>
		struct RemoveConst<const T>
			: RemoveConst<T>
		{
		};

		template<typename T>
		using remove_const = typename RemoveConst<T>::Type;

		template<typename T>
		struct Decay
		{
			typedef remove_const<remove_reference<T>> Type;
		};

		template<typename T>
		using decay = typename Decay<T>::Type;

		template<typename T>
		constexpr T&& forward(remove_reference<T>&& e) noexcept
		{
			return static_cast<T&&>(e);
		}

		template<typename T>
		constexpr T&& forward(remove_reference<T>& e) noexcept
		{
			return static_cast<T&&>(e);
		}

		template<typename T>
		remove_reference<T>&& move(T&& e) noexcept
		{
			return static_cast<remove_reference<T>&&>(e);
		}
	}
}

#endif

