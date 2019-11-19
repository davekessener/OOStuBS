#ifndef OOSTUBS_MPL_FTOR_H
#define OOSTUBS_MPL_FTOR_H

#include "aux.h"

#include "mpl/utils.h"
#include "mpl/type_list.h"

namespace oostubs
{
	namespace mpl
	{
		namespace impl
		{
			template<typename R, typename ... A>
			struct ftor_base
			{
				virtual ~ftor_base( ) { }

				virtual R operator()(A...) = 0;
			};

			template<typename R, typename ... A>
			struct ftor_fp : ftor_base<R, A...>
			{
				typedef R (*fn_t)(A...);

				ftor_fp(fn_t f) : fn(f) { }

				R operator()(A ... a)
				{
					return (*fn)(mpl::forward<A>(a)...);
				}

				fn_t fn;
			};

			template<typename C, typename R, typename ... A>
			struct ftor_member
			{
				typedef C Class;
				typedef R Return;
				typedef make_list<A...> Arguments;

				template<template <typename ...> class F>
				struct GetArguments
				{
					typedef F<A...> Type;
				};
			};

			template<typename C, typename R, typename ... A>
			inline ftor_member<C, R, A...> ftor_get_member(R (C::*)(A...) const)
			{
				return {};
			}

			template<typename F>
			struct ftor_ftor_info : decltype(ftor_get_member(&F::operator()))
			{
			};

			template<typename F>
			struct ftor_build_ftor
			{
				typedef ftor_ftor_info<F> Info;
				typedef typename Info::Return Return;

				template<typename ... A>
				struct ftor_impl : ftor_base<Return, A...>
				{
					template<typename T>
					ftor_impl(T&& f) : ftor(mpl::forward<T>(f)) { }

					Return operator()(A ... a)
					{
						return ftor(mpl::forward<A>(a)...);
					}

					F ftor;
				};

				typedef typename Info::template GetArguments<ftor_impl>::Type Type;
			};

			template<typename F>
			struct ftor_ftor : ftor_build_ftor<F>::Type
			{
				typedef typename ftor_build_ftor<F>::Type Super;

				template<typename T>
				ftor_ftor(T&& f) : Super(mpl::forward<T>(f)) { }
			};
		}

		template<typename R, typename ... A>
		class FTor
		{
			public:
				FTor(R (*f)(A...)) : mFun(new impl::ftor_fp<R, A...>(f)) { }
				template<typename F>
					FTor(F&& f) : mFun(new impl::ftor_ftor<decay<F>>(mpl::forward<F>(f))) { }

				~FTor( ) { delete mFun; }

				template<typename ... T>
				R operator()(T&& ... a)
				{
					return (*mFun)(mpl::forward<T>(a)...);
				}

			private:
				impl::ftor_base<R, A...> *mFun;
		};
	}
}

#endif

