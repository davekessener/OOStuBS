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

#include "mpl/ftor.h"

namespace oostubs {

int testf(int a, int b) { return a + b * b; }

void SystemImpl::run(void)
{
	io::CGAStream os;

	typedef mpl::FTor<int, int, int> f_t;

	f_t f1{&testf};
	f_t f2{[](int a, int b) { return a * a + b; }};

	uint8_t fg = 1, bg = 0;

	os << "f1(3, 7) = " << f1(3, 7) << "\n"
	   << "f2(3, 7) = " << f2(3, 7) << io::endl;

	for(int j = 0 ; true ; ++j)
	{
		os << io::set_attribute((bg << 4) | fg)
		   << "Hello, World! "
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

