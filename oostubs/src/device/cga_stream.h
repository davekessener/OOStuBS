/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                         C G A _ S T R E A M                               */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Die Klasse CGA_Stream ermoeglicht die Ausgabe verschiedener Datentypen    */
/* als Zeichenketten auf dem CGA Bildschirm eines PCs.                       */
/* Fuer weitergehende Formatierung oder spezielle Effekte stehen die         */
/* Methoden der Klasse CGA_Screen zur Verfuegung.                            */
/*****************************************************************************/

#ifndef OOSTUBS_DEVICE_CGASTREAM_H
#define OOSTUBS_DEVICE_CGASTREAM_H

#include "object/o_stream.h"

#include "machine/cga_screen.h"

namespace oostubs
{
	class CGAStream
	{
		private:
			CGAStream(const CGAStream&) = delete;
	};
}

#endif

