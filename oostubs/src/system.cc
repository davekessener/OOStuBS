/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                         A P P L I C A T I O N                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/

#include "system.h"

#include "machine/cga_screen.h"
#include "device/cga_stream.h"

namespace oostubs {
         
void SystemImpl::run(void)
{
	CGAScreen screen;

	for(int j = 0 ; true ; ++j)
	{
		screen.puts("Hello, World!");

		if(j >= 5 * 23)
		for(uint i = ((uint)-1) / 20 ; i ; --i)
		{
		}
	}
}

}

