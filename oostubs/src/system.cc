#include "system.h"

#include "io.h"

#include "machine/plugbox.h"
#include "machine/pic.h"
#include "machine/cpu.h"
#include "machine/cga_screen.h"
#include "machine/guard.h"

#include "thread/coroutine.h"
#include "thread/scheduler.h"

namespace oostubs {

template<uint I>
class Thread : public Coroutine
{
	public:
		Thread( ) : Coroutine(mStack) { }

	protected:
		void doRun( ) override
		{
			kout << "Thread " << I << io::endl;

			SchedulerManager::instance().resume();
		}

	private:
		uint8_t mStack[0x1000];
};

System::System(void)
{
	PlugboxManager::instance().assign(IRQ::KEYBOARD, &mKeyboardDriver);
}

System::~System(void)
{
	CPUManager::instance().halt();
}

void System::run(void)
{
//	PICManager::instance().enable(PIC::Device::KEYBOARD);

//	CPUManager::instance().enable_int();

	Thread<1> t1;
	Thread<2> t2;
	Thread<3> t3;

	SchedulerManager::instance().add(&t1);
	SchedulerManager::instance().add(&t2);
	SchedulerManager::instance().add(&t3);

	SchedulerManager::instance().schedule();
}

}

