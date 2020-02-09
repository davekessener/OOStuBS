#include "system.h"

#include "aux.h"
#include "io.h"
#include "mboot.h"
#include "initrd.h"

#include "machine/plugbox.h"
#include "machine/pic.h"
#include "machine/cpu.h"
#include "machine/guard.h"
#include "machine/real.h"
#include "machine/keyboard_controller.h"
//#include "machine/soundcard_controller.h"

#include "thread/customer.h"
#include "thread/semaphore.h"
#include "thread/bell.h"
#include "thread/bellringer.h"

#include "sys/scheduler.h"

#include "user/mutex.h"
#include "user/keyboard.h"
#include "user/mouse.h"
#include "user/sleeper.h"

#include "gui/texture.h"
#include "gui/screen.h"

#include "app/screensaver.h"
#include "app/monitor.h"

namespace oostubs {

class SystemThread : public Customer
{
	public:
		SystemThread( ) : Customer(mStack + sizeof(mStack)) { }

	protected:
		void doRun( ) override;

	private:
		u8 mStack[0x1000];
};

void SystemThread::doRun(void)
{
	FramebufferManager::instance();

//	ScreensaverThread app;
	Monitor app;

	app.start();
	app.join();

	{
		Guard::Lock lock(GuardManager::instance());

		SchedulerManager::instance().exit();
	}
}

System::System(void)
{
	KeyboardManager::instance();
	MouseManager::instance();
}

System::~System(void)
{
	CPUManager::instance().halt();
}

using namespace dave;

void System::run(void)
{
	SystemThread sys;

	ASSERT(mboot_info_ptr->mods_count > 0);

	initrd_root = (const initrd::Node *) (u64) ((mboot_module *) (u64) mboot_info_ptr->mods_addr)->mod_start;

	KeyboardControllerManager::instance();
//	SoundcardControllerManager::instance();

	GuardManager::instance().enter();

	SchedulerManager::instance().add(sys);

	PICManager::instance().enable(PIC::Device::TIMER);
	PICManager::instance().enable(PIC::Device::KEYBOARD);
	PICManager::instance().enable(PIC::Device::MOUSE);

	CPUManager::instance().enable_int();

	SchedulerManager::instance().schedule();
}

}

