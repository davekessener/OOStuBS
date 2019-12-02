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
		enum class Device : uint8_t
		{
			TIMER = 0x01,
			KEYBOARD = 0x02,
			CASCADE = 0x04
		};

		public:
			PIC( );

			void enable(Device);
			void disable(Device);
			bool is_disabled(Device d) const { return mMask & static_cast<uint8_t>(d); }

		private:
			PIC(const PIC&) = delete;

			uint8_t mMask;
			IO_Port mIMR;
	};

	typedef mpl::SingletonHolder<PIC> PICManager;
}

#endif

