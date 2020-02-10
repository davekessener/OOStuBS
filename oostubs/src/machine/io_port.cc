#include "machine/io_port.h"

#ifdef NOKERNEL

#include <string>

namespace oostubs {

void IO_Port::outb(uint8_t v) const
{
	throw std::string("outb called!");
}

void IO_Port::outw(uint16_t v) const
{
	throw std::string("outw called!");
}

uint8_t IO_Port::inb(void) const
{
	throw std::string("inb called!");
}

uint16_t IO_Port::inw(void) const
{
	throw std::string("inw called!");
}

}

#else

extern "C" void out_b(int port, int value);
extern "C" void out_w(int port, int value);
extern "C" void out_l(int port, int value);
extern "C" int in_b(int port);
extern "C" int in_w(int port);
extern "C" int in_l(int port);

namespace oostubs {

void IO_Port::outb(uint8_t val) const
{ 
	out_b(mAddress, val); 
}

void IO_Port::outw(uint16_t val) const
{ 
	out_w(mAddress, val); 
}

void IO_Port::outl(uint32_t val) const
{
	out_l(mAddress, val);
}

uint8_t IO_Port::inb(void) const
{ 
	return in_b(mAddress); 
}

uint16_t IO_Port::inw(void) const
{ 
	return in_w(mAddress); 
}

uint32_t IO_Port::inl(void) const
{
	return in_l(mAddress);
}

}

#endif

