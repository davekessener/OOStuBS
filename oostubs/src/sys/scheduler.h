#ifndef OOSTUBS_SYS_SCHEDULER_H
#define OOSTUBS_SYS_SCHEDULER_H

#include "aux.h"

#include "thread/organizer.h"

#include "mpl/singleton.h"

namespace oostubs
{
	typedef mpl::SingletonHolder<Organizer> SchedulerManager;
}

#endif

