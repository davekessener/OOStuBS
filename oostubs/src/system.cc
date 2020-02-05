#include "system.h"

#include "io.h"

#include "machine/plugbox.h"
#include "machine/pic.h"
#include "machine/cpu.h"
#include "machine/cga_screen.h"
#include "machine/guard.h"
#include "machine/real.h"

#include "thread/customer.h"
#include "thread/semaphore.h"
#include "thread/bell.h"
#include "thread/bellringer.h"

#include "sys/scheduler.h"

#include "user/mutex.h"
#include "user/keyboard.h"
#include "user/sleeper.h"

namespace oostubs {

#define CGA_SCREEN ((uint16_t *) 0xB8000)

class Clock : public Customer
{
	public:
		Clock(uint i, uint f, uint m) : Customer(mStack + sizeof(mStack)), mI(i), mC(0), mF(f), mM(m) { }

	protected:
		void doRun( ) override
		{
			while(true)
			{
				CGA_SCREEN[80 - mI] = 0x0F00 | ('0' + (mC++ % mM));

				buzzer.sleep(50 * mF);
			}
		}

	private:
		uint mI, mC, mF, mM;
		Sleeper buzzer;
		uint8_t mStack[0x1000];
};

class Application : public Customer
{
	public:
		Application(uint i, Mutex& s)
			: Customer(mStack + sizeof(mStack))
			, mI(i)
			, mMtx(&s)
		{ }

	protected:
		void doRun( ) override
		{
			char c = mI + '0';

			while(true)
			{
				mMtx->lock();

				for(uint i = 0 ; i < 10 ; ++i)
				{
					for(uint j = 0 ; j < 0x1000000 ; ++j);

					kout << c << io::flush;
				}

				kout << io::endl;

				mMtx->unlock();

				Key k = KeyboardManager::instance().getc();

				ASSERT(k.valid());

				c = k.ascii();
			}
		}

	private:
		uint mI;
		Mutex *mMtx;
		uint8_t mStack[0x1000];
};

System::System(void)
{
	KeyboardManager::instance();
}

System::~System(void)
{
	CPUManager::instance().halt();
}

void System::run(void)
{
	Mutex mutex;

	Clock c1(1, 1, 10);
	Clock c2(2, 10, 6);
	Clock c3(4, 60, 10);
	Clock c4(5, 600, 6);

	Application a1(1, mutex);
	Application a2(2, mutex);
	Application a3(3, mutex);

	CGA_SCREEN[80 - 3] = 0x0F00 | ':';

	GuardManager::instance().enter();

	SchedulerManager::instance().add(a1);
	SchedulerManager::instance().add(a2);
	SchedulerManager::instance().add(a3);
	SchedulerManager::instance().add(c1);
	SchedulerManager::instance().add(c2);
	SchedulerManager::instance().add(c3);
	SchedulerManager::instance().add(c4);

	realmode_info rmi;
	toc_t toc;

	enter_real(&rmi, &toc);
	leave_real(&rmi, &toc);

	PICManager::instance().enable(PIC::Device::TIMER);
	PICManager::instance().enable(PIC::Device::KEYBOARD);

	CPUManager::instance().enable_int();

	SchedulerManager::instance().schedule();
}

}

