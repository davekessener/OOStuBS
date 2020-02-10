#ifndef OOSTUBS_SYS_SCREEN_H
#define OOSTUBS_SYS_SCREEN_H

#include "machine/cga_screen.h"

#include "gui/terminal.h"

#include "mpl/singleton.h"

namespace oostubs
{
//	typedef mpl::SingletonHolder<CGAScreen> ScreenManager;
	typedef mpl::SingletonHolder<Terminal> ScreenManager;
}

#endif

