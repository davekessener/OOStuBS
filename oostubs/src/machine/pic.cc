#include "machine/pic.h"

#include "io.h"

namespace oostubs {

PIC::PIC(void)
	: mIMR(Port::PIC_DATA_M)
{
	mMask = mIMR.inb();
}

void PIC::enable(Device d)
{
	if(is_disabled(d))
	{
		mMask &= ~static_cast<uint8_t>(d);

		kout << "\nSending " << io::Format::HEX << mMask << " to " << mIMR.port() << io::Format::DEC << io::endl;

		mIMR.outb(mMask);
	}
}

void PIC::disable(Device d)
{
	if(!is_disabled(d))
	{
		mMask |= static_cast<uint8_t>(d);

		mIMR.outb(mMask);
	}
}

}

