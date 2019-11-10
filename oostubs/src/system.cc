/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                         A P P L I C A T I O N                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/

#include "system.h"

#include "device/cga_stream.h"

namespace oostubs {
         
void SystemImpl::run(void)
{
	io::CGAStream os;

	uint8_t c = 1;

	for(int j = 0 ; true ; ++j)
	{
		os << io::set_foreground(static_cast<CGAScreen::Color>(c))
		   << "Hello, World!"
		   << io::endl;

		++c;

		for(uint i = ((uint)-1) / 20 ; i ; --i)
		{
		}
	}
}

}

