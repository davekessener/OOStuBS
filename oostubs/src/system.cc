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

class Application : public Coroutine
{
	public:
		Application(uint i) : Coroutine(mStack + sizeof(mStack)), mI(i) { }

	protected:
		void doRun( ) override
		{
			while(true)
			{
				{
					Guard::Lock lock(GuardManager::instance());

					kout << "Application " << mI << io::endl;
				}

				for(uint i = 0 ; i < 0x5000000 ; ++i);
			}
		}

	private:
		uint mI;
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
	Application a1(1);
	Application a2(2);
	Application a3(3);
	Application a4(4);
	Application a5(5);

	GuardManager::instance().enter();

	SchedulerManager::instance().add(&a1);
	SchedulerManager::instance().add(&a2);
	SchedulerManager::instance().add(&a3);
	SchedulerManager::instance().add(&a4);
	SchedulerManager::instance().add(&a5);

	PICManager::instance().enable(PIC::Device::TIMER);
	PICManager::instance().enable(PIC::Device::KEYBOARD);

	CPUManager::instance().enable_int();

	SchedulerManager::instance().schedule();
}

}

