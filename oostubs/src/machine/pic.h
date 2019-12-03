#ifndef OOSTUBS_MACHINE_PIC_H
#define OOSTUBS_MACHINE_PIC_H

#include "aux.h"

#include "mpl/singleton.h"

#include "machine/io_port.h"

namespace oostubs
{
	class PIC
	{
		public:
		enum class Device : uint16_t
		{
			TIMER = 0x01,
			KEYBOARD = 0x02,
			CASCADE = 0x04
		};

		class Lock
		{
			public:
				Lock(PIC& pic, Device d)
					: mPIC(&pic)
					, mDevice(d)
					, mState(!pic.is_disabled(d))
						{ if(mState) mPIC->disable(mDevice); }
				~Lock( )
					{ if(mState) mPIC->enable(mDevice); }

			private:
				PIC *mPIC;
				Device mDevice;
				bool mState;
		};

		public:
			PIC( );

			void enable(Device);
			void disable(Device);
			bool is_disabled(Device d) const { return mMask & static_cast<uint16_t>(d); }

		private:
			PIC(const PIC&) = delete;

			uint16_t mMask;
			IO_Port mIMR_M, mIMR_S;
	};

	typedef mpl::SingletonHolder<PIC> PICManager;
}

#endif

