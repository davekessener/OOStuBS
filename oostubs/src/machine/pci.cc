#include "machine/pci.h"

#include "lib/memory.h"

#include "panic.h"

namespace oostubs {

IO_Port PCIDevice::sCfgAddr(Port::PCI_CFG_ADDR);
IO_Port PCIDevice::sCfgData(Port::PCI_CFG_DATA);

PCIDevice::PCIDevice(void)
{
	mBus = mDevice = mFunction = -1;
	slow_memset(&mHeader, 0xFF, sizeof(mHeader));
}

PCIDevice::PCIDevice(uint bus, uint device, uint func)
	: mBus(bus)
	, mDevice(device)
	, mFunction(func)
{
	ASSERT(bus == (bus & 0xFF));
	ASSERT(device == (device & 0x1F));
	ASSERT(func == (func & 0x07));

	slow_memset(&mHeader, 0xFF, sizeof(mHeader));

	read_header(bus, device, func, &mHeader);
}

u32 PCIDevice::read(uint a)
{
	return read(mBus, mDevice, mFunction, a);
}

void PCIDevice::write(uint a, u32 v)
{
	write(mBus, mDevice, mFunction, a, v);
}

u32 PCIDevice::read(uint b, uint d, uint f, uint a)
{
	ASSERT(a == (a & 0xFC));

	sCfgAddr.outl(0x80000000 | (b << 16) | (d << 11) | (f << 8) | a);

	return sCfgData.inl();
}

void PCIDevice::write(uint b, uint d, uint f, uint a, u32 v)
{
	ASSERT(a == (a & 0xFC));

	sCfgAddr.outl(0x80000000 | (b << 16) | (d << 11) | (f << 8) | a);
	sCfgData.outl(v);
}

bool PCIDevice::read_header(uint b, uint d, uint f, pci_header *h)
{
	u32 t = read(b, d, f, 0);

	if(t == 0xFFFFFFFF)
		return false;
	
	u32 *p = (u32 *) h;

	*p++ = t;

	for(uint i = 1 ; i < (sizeof(*h) / sizeof(u32)) ; ++i)
	{
		*p++ = read(b, d, f, i * sizeof(u32));
	}

	return true;
}

}

