#ifndef OOSTUBS_MACHINE_PCI_H
#define OOSTUBS_MACHINE_PCI_H

#include "aux.h"

#include "machine/io_port.h"

namespace oostubs
{
	extern "C" struct pci_header
	{
		u16 vendor_id, device_id;
		u16 command, status;
		u8 revision_id, prog_if, subclass, class_code;
		u8 cache_line_size, latency_timer, header_type, bist;

		union
		{
			struct
			{
				u32 reserved[12];
			} unknown;
		} body;
	} __attribute__((packed));

	static_assert(sizeof(pci_header) == 64, "");

	class PCIDevice
	{
		public:
			PCIDevice( );
			PCIDevice(uint, uint, uint);

			const pci_header& header( ) const { return mHeader; }

			bool valid( ) const { return mHeader.device_id != 0xFFFF; }
			uint bus( ) const { return mBus; }
			uint slot( ) const { return mDevice; }
			uint fidx( ) const { return mFunction; }

			u32 read(uint);

		private:
			static u32 read(uint, uint, uint, uint);
			static bool read_header(uint, uint, uint, pci_header *);

		private:
			uint mBus, mDevice, mFunction;
			pci_header mHeader;

			static IO_Port sCfgAddr, sCfgData;
	};
}

#endif

