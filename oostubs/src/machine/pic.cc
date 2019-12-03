#include "machine/pic.h"

#include "io.h"

namespace oostubs {

PIC::PIC(void)
	: mIMR_M(Port::PIC_DATA_M)
	, mIMR_S(Port::PIC_DATA_S)
{
	mMask = (mIMR_S.inb() << 8) | mIMR_M.inb();
}

void PIC::enable(Device d)
{
	if(is_disabled(d))
	{
		uint16_t v = static_cast<uint16_t>(d);

		mMask &= ~v;

		((v & 0xFF) ? mIMR_M : mIMR_S).outb(mMask);
	}
}

void PIC::disable(Device d)
{
	if(!is_disabled(d))
	{
		uint16_t v = static_cast<uint16_t>(d);

		mMask |= v;

		((v & 0xFF) ? mIMR_M : mIMR_S).outb(mMask);
	}
}

}

