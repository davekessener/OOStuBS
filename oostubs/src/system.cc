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

#include "thread/customer.h"
#include "thread/semaphore.h"
#include "thread/bell.h"
#include "thread/bellringer.h"

#include "sys/scheduler.h"

#include "user/mutex.h"
#include "user/keyboard.h"
#include "user/sleeper.h"

#include "gui/texture.h"
#include "gui/screen.h"

#include "app/screensaver.h"

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
	ScreensaverThread screensaver;

	screensaver.start();
	screensaver.join();

	{
		Guard::Lock lock(GuardManager::instance());

		SchedulerManager::instance().exit();
	}
}

System::System(void)
{
	KeyboardManager::instance();
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

	GuardManager::instance().enter();

	SchedulerManager::instance().add(sys);

	PICManager::instance().enable(PIC::Device::TIMER);
	PICManager::instance().enable(PIC::Device::KEYBOARD);

	CPUManager::instance().enable_int();

	SchedulerManager::instance().schedule();
}

//	kout << "Kernel loaded" << io::endl;
//
//	kout << (void *) (u64) mboot_info_ptr->flags << io::endl;
//	kout << mboot_info_ptr->mods_count << " modules loaded " << (void *) (u64) mboot_info_ptr->mods_addr << io::endl;
//
//	ASSERT(mboot_info_ptr->mods_count > 0);
//
//	mboot_module *modules = (mboot_module *) (u64) mboot_info_ptr->mods_addr;
//
//	for(uint i = 0 ; i < mboot_info_ptr->mods_count ; ++i)
//	{
//		kout << "Module " << i << ": '" << (const char *) (u64) modules[i].string << "' sized " << (modules[i].mod_end - modules[i].mod_start) << "B" << io::endl;
//	}
//
//	const initrd::Node *root = (const initrd::Node *) (u64) modules[0].mod_start;
//
//	print_info(root, 0);
//
//	kout << io::endl;
//
//	const initrd::Node *testf = root->find("sample/test.txt");
//
//	ASSERT(testf);
//
//	kout.write(testf->content(), testf->size());
//
//	kout << io::endl;
//
//	u8 *fb = (u8 *) (u64) mboot_info_ptr->framebuffer_addr;
//
//	Screen screen(fb, mboot_info_ptr->framebuffer_pitch);
//	Texture tx(100, 100);
//
//	uint w = tx.width() - 1, h = tx.height() - 1;
//
//	for(uint y = 0 ; y < tx.height() ; ++y)
//	{
//		for(uint x = 0 ; x < tx.width() ; ++x)
//		{
//			tx.at(x, y) = tx.at(x, y) = 0xFF000000 | ((0xFF * x / w) << 16) | ((0xFF * y / h) << 8) | (0xFF * x * y / w / h);
//		}
//	}
//
//	int x = 0, y = 0;
//	int max_x = Screen::WIDTH - tx.width(), max_y = Screen::HEIGHT - tx.height();
//	int dx = 5, dy = 1;
//
//	while(true)
//	{
////		for(uint i = 0 ; i < 0x10000 ; ++i);
//
//		x += dx;
//		y += dy;
//
//		if(x < 0 || x >= max_x) dx = -dx;
//		if(y < 0 || y >= max_y) dy = -dy;
//
//		screen.clear();
//		screen.buffer().fast_blt(tx, x, y);
//		screen.sync();
//	}
//
//	while(true);
//	Mutex mutex;
//
//	Clock c1(1, 1, 10);
//	Clock c2(2, 10, 6);
//	Clock c3(4, 60, 10);
//	Clock c4(5, 600, 6);
//
//	Application a1(1, mutex);
//	Application a2(2, mutex);
//	Application a3(3, mutex);
//
//	CGA_SCREEN[80 - 3] = 0x0F00 | ':';
//
//	GuardManager::instance().enter();
//
//	SchedulerManager::instance().add(a1);
//	SchedulerManager::instance().add(a2);
//	SchedulerManager::instance().add(a3);
//	SchedulerManager::instance().add(c1);
//	SchedulerManager::instance().add(c2);
//	SchedulerManager::instance().add(c3);
//	SchedulerManager::instance().add(c4);
//
//	realmode_info rmi;
//
//	enter_real(&rmi);
//	leave_real(&rmi);
//
//	PICManager::instance().enable(PIC::Device::TIMER);
//	PICManager::instance().enable(PIC::Device::KEYBOARD);
//
//	CPUManager::instance().enable_int();
//
//	SchedulerManager::instance().schedule();

}

