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
		Thread( ) : Coroutine(mStack + sizeof(mStack)) { }

	protected:
		void doRun( ) override
		{
			while(true)
			{
				{
					Guard::Lock lock(GuardManager::instance());

					kout << "Thread " << I << io::endl;
				}

				for(uint i = 0 ; i < 0x5000000 ; ++i);
			}
		}

	private:
		uint8_t mStack[0x1000];
};

System::System(void)
{
	PlugboxManager::instance().assign(IRQ::KEYBOARD, &mKeyboardDriver);
	PlugboxManager::instance().assign(IRQ::TIMER, &mSchedulerDriver);
}

System::~System(void)
{
	CPUManager::instance().halt();
}

void System::run(void)
{
	Thread<1> t1;
	Thread<2> t2;
	Thread<3> t3;
	Thread<4> t4;
	Thread<5> t5;

	GuardManager::instance().enter();

	SchedulerManager::instance().add(&t1);
	SchedulerManager::instance().add(&t2);
	SchedulerManager::instance().add(&t3);
	SchedulerManager::instance().add(&t4);
	SchedulerManager::instance().add(&t5);

	PICManager::instance().enable(PIC::Device::TIMER);
	PICManager::instance().enable(PIC::Device::KEYBOARD);

	CPUManager::instance().enable_int();

	SchedulerManager::instance().schedule();
}

}

