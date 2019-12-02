/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              G U A R D I A N                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Zentrale Unterbrechungsbehandlungsroutine des Systems.                    */
/* Der Parameter gibt die Nummer des aufgetretenen Interrupts an.            */
/*****************************************************************************/

#include "aux.h"

extern "C" void guardian(unsigned int slot);

namespace oostubs {

void guardian_impl(uint slot)
{
}

}

void guardian(unsigned int slot)
{
	oostubs::guardian_impl(slot);
}

