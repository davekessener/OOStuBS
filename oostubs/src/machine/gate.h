#ifndef OOSTUBS_MACHINE_GATE_H
#define OOSTUBS_MACHINE_GATE_H

#include "aux.h"

#include "machine/pic.h"

namespace oostubs
{
	class Gate
	{
		public:
			explicit Gate(bool f = false) : mHasEpi(f) { }
			virtual ~Gate( ) { }

			void prologue(uint slot) { doPrologue(slot); }
			virtual void epilogue( ) { doEpilogue(); }

			bool has_epilogue( ) const { return mHasEpi; }

		protected:
			virtual void doPrologue(uint) = 0;
			virtual void doEpilogue( ) { }

		private:
			bool mHasEpi;
	};

	class DeviceGate : public Gate
	{
		public:
			DeviceGate(PIC::Device d, bool f = false) : Gate(f), mDevice(d) { }

			void epilogue( ) override
			{
				PIC::Lock lock(PICManager::instance(), mDevice);

				Gate::epilogue();
			}

		private:
			PIC::Device mDevice;
	};
}

#endif

