#ifndef OOSTUBS_MACHINE_PIT_H
#define OOSTUBS_MACHINE_PIT_H

#include "aux.h"

#include "machine/io_port.h"

#include "mpl/singleton.h"

namespace oostubs
{
	class PIT
	{
		public:
			PIT( );

			uint interval( ) const { return mInterval; }
			void interval(uint);

		private:
			uint mInterval;
			IO_Port mCtrl, mCounter;
	};

	typedef mpl::SingletonHolder<PIT> PITManager;
}

#endif

