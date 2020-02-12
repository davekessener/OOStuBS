#ifndef OOSTUBS_MACHINE_PCI_H
#define OOSTUBS_MACHINE_PCI_H

#include "lib.h"

#include "mpl/utils.h"

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
			struct
			{
				u32 bar[6];
				u32 cis_ptr;
				u16 subsys_vendor_id, subsys_device_id;
				u32 expansion_addr;
				u8 capabilities;
				u8 reserved[7];
				u8 int_line, int_pin, min_grant, max_latency;
			} base;
		} body;
	} __attribute__((packed));

	static_assert(sizeof(pci_header) == 64, "");
	static_assert(sizeof(mpl::declval<pci_header>().body.unknown) == 48, "");
	static_assert(sizeof(mpl::declval<pci_header>().body.base) == 48, "");

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
			void write(uint, u32);

		private:
			static u32 read(uint, uint, uint, uint);
			static void write(uint, uint, uint, uint, u32);
			static bool read_header(uint, uint, uint, pci_header *);

		private:
			uint mBus, mDevice, mFunction;
			pci_header mHeader;

			static IO_Port sCfgAddr, sCfgData;
	};
}

#endif

