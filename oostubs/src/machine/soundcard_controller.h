#ifndef OOSTUBS_MACHINE_SOUNDCARD_H
#define OOSTUBS_MACHINE_SOUNDCARD_H

#include "aux.h"

#include "machine/io_port.h"
#include "machine/dma.h"

#include "mpl/singleton.h"

namespace oostubs
{
	class SoundcardController
	{
		public:
			SoundcardController( );

			bool is_present( ) const { return mIsPresent; }

		private:
			bool compute_present( );

		private:
			IO_Port mReset, mRead, mWrite, mAck;
			bool mIsPresent;
//			DMAController mDMA;
	};

	typedef mpl::SingletonHolder<SoundcardController> SoundcardControllerManager;
}

#endif

