/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                         A P P L I C A T I O N                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/

#include "system.h"

#include "memory.h"

#include "device/cga_stream.h"

namespace oostubs {
         
void SystemImpl::run(void)
{
	io::CGAStream os;

	uint8_t fg = 1, bg = 0;

	for(int j = 0 ; true ; ++j)
	{
		os << io::set_attribute((bg << 4) | fg)
		   << "Hello, World!"
		   << io::endl;

		if(++fg == 0x10)
		{
			fg = 0;
			bg = (bg + 1) & 0x0F;
		}

		for(uint i = ((uint)-1) / 20 ; i ; --i)
		{
		}
	}
}

}

