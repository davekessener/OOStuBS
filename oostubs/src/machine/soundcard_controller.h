#ifndef OOSTUBS_MACHINE_SOUNDCARD_H
#define OOSTUBS_MACHINE_SOUNDCARD_H

#include "aux.h"
#include "initrd.h"

#include "machine/io_port.h"
#include "machine/pci.h"

#include "mpl/singleton.h"

namespace oostubs
{
	class SoundcardController
	{
		struct RB
		{
			RB(uint);

			volatile u8 *base;
			volatile u32 *buffer;
		};

		struct CORB : public RB
		{
			CORB( ) : RB(sizeof(u32)) { }

			void setup(volatile void *);

			void write(uint, uint, uint, u8);
		};

		struct RIRB : public RB
		{
			RIRB( ) : RB(sizeof(u64)) { }

			void setup(volatile void *);
		};

		public:
			SoundcardController( );
			bool present( ) const { return mHDA.valid(); }

		private:
			PCIDevice mHDA;
			u16 mVersion;
			CORB mCORB;
			RIRB mRIRB;
	};

	typedef mpl::SingletonHolder<SoundcardController> SoundcardControllerManager;
}

#endif

