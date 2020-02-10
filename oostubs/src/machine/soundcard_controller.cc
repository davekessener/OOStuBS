#include "machine/soundcard_controller.h"

#include "panic.h"

#define MAGIC_PRESENT 0xAA
#define IRQ5 1

namespace oostubs {

SoundcardController::SoundcardController(void)
	: mReset(Port::DSP_RESET)
	, mRead(Port::DSP_READ)
	, mWrite(Port::DSP_WRITE)
	, mAck(Port::DSP_ACK)
	, mMixerCtrl(Port::DSP_MIX_CTRL)
	, mMixerData(Port::DSP_MIX_DATA)
{
	PANIC("SHOULD NOT BE HERE!");
	mIsPresent = compute_present();
}

void SoundcardController::play(const initrd::Node *f)
{
	ASSERT(f);

	mDMA.setup(f->content(), f->size());

	mMixerCtrl.outb(0x80);
	mMixerData.outb(IRQ5);
	mMixerCtrl.outb(0x81);
	mMixerData.outb(1<<DMAController::CHANNEL);
	mMixerCtrl.outb(0x22);
	mMixerData.outb(0xDD);

	write(0xD1);
	write(0x41);
	write(44000 >> 8);
	write(44000 & 0xFF);
	write(0xCE);
	write(0x10);

	update();
}

bool SoundcardController::compute_present(void)
{
	mReset.outb(1);
	idle_for(0x100000);
	mReset.outb(0);
	idle_for(0x10000);

	return read() == MAGIC_PRESENT;
}

void SoundcardController::update(void)
{
	uint l = mDMA.send_next();

	mAck.inb();

	if(l)
	{
		write(max(DMAController::BUFFER_SIZE / 2, l));
	}
	else
	{
		write(0xDA);
	}
}

void SoundcardController::write(IO_Port& io, u8 v)
{
	TRY_FOR(0x100000, io.inb());

	io.outb(v);
}

u8 SoundcardController::read(void)
{
	TRY_FOR(0x100000, !(mAck.inb() & 0x80));

	return mRead.inb();
}

}

