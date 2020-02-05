#ifndef OOSTUBS_SYS_CPU_H
#define OOSTUBS_SYS_CPU_H

#include "machine/cpu.h"

#include "mpl/singleton.h"

namespace oostubs
{
	typedef mpl::SingletonHolder<CPU> CPUManager;
}

#endif

