#ifndef OOSTUBS_MACHINE_IOPORT_H
#define OOSTUBS_MACHINE_IOPORT_H

#include "aux.h"

namespace oostubs
{
	class IO_Port
	{
		public:
			IO_Port(uint16_t a) : mAddress(a) { };
			IO_Port(Port p) : mAddress(static_cast<uint16_t>(p)) { }
	
			void outb(uint8_t) const;
			void outw(uint16_t) const;
			void outl(uint32_t) const;
			uint8_t inb() const;
			uint16_t inw() const;
			uint32_t inl() const;

			uint16_t address( ) const { return mAddress; }
			uint16_t port( ) const { return address(); }
		
		private:
			uint16_t mAddress;
	};
}

#endif
