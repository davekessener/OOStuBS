#ifndef OOSTUBS_MACHINE_SCHEDULERGATE_H
#define OOSTUBS_MACHINE_SCHEDULERGATE_H

#include "aux.h"

#include "thread/scheduler.h"

#include "machine/gate.h"
#include "machine/pit.h"

namespace oostubs
{
	class SchedulerGate : public Gate
	{
		static constexpr uint PIT_INTERVAL = 20000;

		public:
			SchedulerGate( ) : Gate(true) { PITManager::instance().interval(PIT_INTERVAL); }

		protected:
			void doPrologue(uint) override { }
			void doEpilogue( ) override { SchedulerManager::instance().resume(); }
	};
}

#endif

