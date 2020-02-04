#include "machine/scheduler_gate.h"

#include "thread/bellringer.h"

namespace oostubs {

SchedulerGate::SchedulerGate(void)
	: Gate(true)
{
	PITManager::instance().interval(PIT_INTERVAL);
	PlugboxManager::instance().assign(IRQ::TIMER, this);
}

void SchedulerGate::doEpilogue(void)
{
	BellringerManager::instance().tick();
	SchedulerManager::instance().resume();
}

}

