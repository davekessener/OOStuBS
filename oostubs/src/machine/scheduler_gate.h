#ifndef OOSTUBS_MACHINE_SCHEDULERGATE_H
#define OOSTUBS_MACHINE_SCHEDULERGATE_H

#include "aux.h"

#include "sys/scheduler.h"

#include "machine/gate.h"
#include "machine/pit.h"
#include "machine/plugbox.h"

namespace oostubs
{
	class SchedulerGate : public Gate
	{
		static constexpr uint PIT_INTERVAL = 20000;

		public:
			SchedulerGate( );

		protected:
			void doPrologue(uint) override { }
			void doEpilogue( ) override;
	};
}

#endif

