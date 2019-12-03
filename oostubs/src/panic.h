#ifndef OOSTUBS_PANIC_H
#define OOSTUBS_PANIC_H

#include "io.h"

#include "machine/cpu.h"

#define PANIC(...) ::oostubs::panic(__FILE__, __LINE__, __VA_ARGS__)
#define ASSERT(c) do { if(!(c)) { PANIC("ASSERTION FAILED: " #c ); } } while(false)

namespace oostubs
{
	namespace impl
	{
		inline void panic_impl(void) { }

		template<typename T, typename ... TT>
		void panic_impl(T&& v, TT&& ... a)
		{
			kout << v;

			panic_impl(mpl::forward<TT>(a)...);
		}
	}

	template<typename ... T>
	void panic(const char *f, int l, T&& ... a)
	{
		CPUManager::instance().disable_int();

		Screen::instance().clear();

		impl::panic_impl(
			io::set_color(CGAScreen::Color::WHITE, CGAScreen::Color::RED),
			"[PANIC in ", f, ": ", l, "]",
			io::set_color(CGAScreen::Color::RED, CGAScreen::Color::WHITE),
			" ", mpl::forward<T>(a)...,
			io::endl
		);

		CPUManager::instance().halt();
	}
}

#endif

