#include "machine/soundcard_controller.h"

#include "panic.h"

#define MAGIC_PRESENT 0xAA

namespace oostubs {

//SoundcardController::SoundcardController(void)
//	: mReset(Port::DSP_RESET)
//	, mRead(Port::DSP_READ)
//	, mWrite(Port::DSP_WRITE)
//	, mAck(Port::DSP_ACK)
//{
//	PANIC("FUCK");
//	kout << "WHATEVER" << io::endl;
//	mIsPresent = compute_present();
//}

bool SoundcardController::compute_present(void)
{
	mReset.outb(1);
	
	idle_for(0x100000);

	mReset.outb(0);

	TRY_FOR(0x1000000, !(mAck.inb() & 0x80));

	u8 v = mRead.inb();

	kout << "v: " << (uint) v << io::endl;

	return v == MAGIC_PRESENT;
//	return mRead.inb() == MAGIC_PRESENT;
}

}

