#ifndef OOSTUBS_MACHINE_SOUNDCARD_H
#define OOSTUBS_MACHINE_SOUNDCARD_H

#include "aux.h"
#include "initrd.h"

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

			void play(const initrd::Node *);

		private:
			bool compute_present( );
			void update( );
			void write(IO_Port&, u8);
			void write(u8 v) { write(mWrite, v); }
			u8 read( );

		private:
			IO_Port mReset, mRead, mWrite, mAck;
			IO_Port mMixerCtrl, mMixerData;
			bool mIsPresent;
			DMAController mDMA;
	};

	typedef mpl::SingletonHolder<SoundcardController> SoundcardControllerManager;
}

#endif

