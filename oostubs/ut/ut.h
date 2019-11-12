#ifndef OOSTUBS_UT_H
#define OOSTUBS_UT_H

#include <vector>
#include <map>
#include <functional>
#include <string>
#include <sstream>

namespace oostubs {

struct the_data_t_
{
};

template<typename S>
void stringify_impl(S&)
{
}

template<typename S, typename T, typename ... TT>
void stringify_impl(S& ss, const T& o, const TT& ... a)
{
	ss << o;

	stringify_impl(ss, a...);
}

template<typename ... T>
std::string stringify(const T& ... a)
{
	std::stringstream ss;

	stringify_impl(ss, a...);

	return ss.str();
}

namespace ut_impl
{
	template<typename C, typename R, typename ... A>
	struct mem_ftor_info
	{
		template<template <typename ...> class F>
		struct GetArguments
		{
			typedef F<A...> Type;
		};
	};

	template<typename C, typename R, typename ... A>
	inline mem_ftor_info<C, R, A...> get_mem_ftor_info(R (C::*)(A...) const)
	{
		return {};
	}

	template<size_t, typename ...>
	struct get_nth;

	template<size_t N, typename H, typename ... T>
	struct get_nth<N, H, T...> : get_nth<(N-1), T...>
	{
	};

	template<typename H, typename ... T>
	struct get_nth<0, H, T...>
	{
		typedef H Type;
	};

	template<typename F>
	struct get_second_arg
	{
		typedef decltype(get_mem_ftor_info(&F::operator())) info_t;

		template<typename ... A>
		struct Apply
		{
			typedef typename get_nth<1, A...>::Type Type;
		};

		typedef typename info_t::template GetArguments<Apply>::Type tmp_t;
		typedef typename tmp_t::Type Type;
	};
}

struct UTManager
{
	typedef std::function<void(std::string&, void *)> fn_t;

	struct wrapper
	{
		wrapper(fn_t& f) : fun(f) { }

		template<typename F>
		wrapper& operator=(F f)
		{
			typedef typename std::decay<
				typename ut_impl::get_second_arg<F>::Type
			>::type T;

			fun = [f](std::string& e, void *p) {
				f(e, *((T *) p));
			};

			return *this;
		}

		fn_t& fun;
	};

	struct Suit
	{
		std::string name;
		fn_t setup, teardown;
		std::vector<std::pair<std::string, fn_t>> tests;
		void *data = nullptr;

		wrapper make_setup() { return {setup}; }
		wrapper make_teardown() { return {teardown}; }

		wrapper make_test(const std::string& d)
		{
			tests.emplace_back(d, [](std::string&, void *) { });

			return {tests.back().second};
		}

		Suit& operator=(std::function<void(Suit&)> f)
		{
			f(*this);

			return *this;
		}
	};

	static UTManager& instance()
	{
		static UTManager i;

		return i;
	}

	Suit& make_suit(const std::string& n)
	{
		suits.emplace_back();

		Suit& s{suits.back()};

		s.name = n;

		return s;
	}

	UTManager& operator=(std::function<void(void)> f)
	{
		f();

		return *this;
	}

	void run(void)
	{
		for(auto& s : suits)
		{
			std::cout << "Running suit '" << s.name << "':\n";

			for(auto& e : s.tests)
			{
				if(static_cast<bool>(s.setup))
				{
					std::string r;

					s.setup(r, s.data);
				}

				std::cout << "\t" << e.first << ": ";

				std::string r;

				e.second(r, s.data);

				if(static_cast<bool>(s.teardown))
				{
					std::string r;

					s.teardown(r, s.data);
				}

				if(r.empty())
				{
					std::cout << "T\n";
				}
				else
				{
					std::cout << "F " << r << "\n";
				}
			}
		}
	}

	std::vector<Suit> suits;
};

}

#define UT_SUIT(name) \
UTManager::instance().make_suit(name) = [](UTManager::Suit& the_suit_)

#define UT_BEFORE \
the_suit_.make_setup() = [](std::string&, the_data_t_& data)

#define UT_AFTER \
the_suit_.make_teardown() = [](std::string&, the_data_t_& data)

#define UT_TEST(d) \
if(!the_suit_.data) { \
the_suit_.data = new the_data_t_; } \
the_suit_.make_test(d) = [](std::string& the_error_, the_data_t_& data)

#define UT_DATA \
struct the_data_t_

#define UT_ASSERT(e) \
do { if(!(e)) { \
the_error_ = stringify("[", __FILE__, ":", __LINE__, "] ", #e ); \
return; \
} } while(false)

#define UT_ASSERT_EQ(a,b) \
do { \
auto rb{b}; \
decltype(rb) ra{a}; \
if(ra != rb) { \
the_error_ = stringify("[", __FILE__, ":", __LINE__, "] ", #a, " == ", #b, " | ", ra, " == ", rb); \
return; \
} } while(false)

#endif

