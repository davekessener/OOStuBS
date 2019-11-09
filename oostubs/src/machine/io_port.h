/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              I O _ P O R T                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Diese Klasse dient dem Zugriff auf die Ein-/Ausgabe Ports des PCs. Beim   */
/* PC gibt es einen gesonderten I/O-Adressraum, der nur mittels der Maschi-  */
/* neninstruktionen 'in' und 'out' angesprochen werden kann. Ein IO_Port-    */
/* Objekt wird beim Erstellen an eine Adresse des I/O-Adressraums gebunden   */
/* und kann dann fuer byte- oder wortweise Ein- oder Ausgaben verwendet wer- */
/* den.                                                                      */
/*****************************************************************************/

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
			uint8_t inb() const;
			uint16_t inw() const;
		
		private:
			// Adresse im I/O-Adressraum
			uint16_t mAddress;
	};
}

#endif
