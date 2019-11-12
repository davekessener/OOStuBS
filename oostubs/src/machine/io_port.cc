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

extern "C" void outb(int port, int value);
extern "C" void outw(int port, int value);
extern "C" int inb(int port);
extern "C" int inw(int port);

namespace oostubs {

// OUTB: Byteweise Ausgabe eines Wertes ueber einen I/O-Port.
void IO_Port::outb(uint8_t val) const
{ 
	::outb(mAddress, val); 
};

// OUTW: Wortweise Ausgabe eines Wertes ueber einen I/O-Port.
void IO_Port::outw(uint16_t val) const
{ 
	::outw(mAddress, val); 
};

// INB: Byteweises Einlesen eines Wertes ueber einen I/O-Port.
uint8_t IO_Port::inb(void) const
{ 
	return ::inb(mAddress); 
};

// INW: Wortweises Einlesen eines Wertes ueber einen I/O-Port.
uint16_t IO_Port::inw(void) const
{ 
	return ::inw(mAddress); 
};

}

#endif

