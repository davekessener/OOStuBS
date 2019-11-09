/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                            O _ S T R E A M                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Die Klasse O_Stream enthaelt die Definition des << Operators fuer die     */
/* wichtigsten der vordefinierten Datentypen und realisiert somit die        */
/* bekannte Ausgabefunktion der C++ iO_Stream Bibliothek. Zur Zeit wird die  */
/* Darstellung von Zeichen, Zeichenketten und ganzen Zahlen unterstuetzt.    */
/* Ein weiterer << Operator erlaubt die Verwendung von Manipulatoren.        */
/*                                                                           */
/* Neben der Klasse O_Stream sind hier auch die Manipulatoren hex, dec, oct  */
/* und bin fuer die Wahl der Basis bei der Zahlendarstellung, sowie endl     */
/* fuer den Zeilenumbruch definiert.                                         */
/*****************************************************************************/

#ifndef OOSTUBS_OBJECT_OSTREAM_H
#define OOSTUBS_OBJECT_OSTREAM_H

#include "object/string_buffer.h"

namespace oostubs
{
	class O_Stream
	{
		private:
			O_Stream(const O_Stream &) = delete;
	};
	
	/*---------------------------------------------------------------------------*/
	/*                                                                           */
	/*                          M A N I P U L A T O R E N                        */
	/*                                                                           */
	/*---------------------------------------------------------------------------*/
	/* Die folgenden Funktionen erhalten und liefern jeweils eine Referenz auf   */
	/* ein O_Stream Objekt. Da die Klasse O_Stream einen Operator << fuer        */
	/* derartige Funktionen definiert, koennen sie mit Hilfe dieses Operators    */
	/* aufgerufen und sogar in weitere Eingaben eingebettet werden.              */
	/* Aufgabe der Manipulatoren ist, die Darstellung der nachfolgenden Ausgaben */
	/* zu beeinflussen, z.B durch die Wahl des Zahlensystems.                    */
	/*---------------------------------------------------------------------------*/
	
	// ENDL: fuegt einen Zeilenumbruch in die Ausgabe ein.
	// TODO
	
	// BIN: waehlt das binaere Zahlensystem aus.
	// TODO
	        
	// OCT: waehlt das oktale Zahlensystem aus.
	// TODO
	        
	// DEC: waehlt das dezimale Zahlensystem aus.
	// TODO
	        
	// HEX: waehlt das hexadezimale Zahlensystem aus.
	// TODO
}

#endif
