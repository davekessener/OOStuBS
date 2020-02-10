#ifndef OOSTUBS_MACHINE_DMA_H
#define OOSTUBS_MACHINE_DMA_H

#include "aux.h"

#include "machine/io_port.h"

#include "user/mutex.h"

#include "lib/promise.h"

namespace oostubs
{
	class DMAController
	{
		struct entry
		{
			Promise promise;
			const u8 *data;
			uint size, i;
		};

		public:
		static constexpr uint BUFFER_SIZE = 0x04000;
		static constexpr uint CHANNEL = 5;

		public:
			DMAController( );

			void setup(const u8 *, uint);

			uint send_next( );

		private:
			Mutex mMutex;
			IO_Port mAddr, mCount, mPage, mMask, mMode, mClear;
			u8 *mBuffer[2];
			uint mIdx;
			const u8 *mSource;
			uint mSent, mSize;
	};
}

#endif

