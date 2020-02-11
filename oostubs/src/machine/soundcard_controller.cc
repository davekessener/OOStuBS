#include "machine/soundcard_controller.h"

#include "lib/memory.h"

#include "panic.h"

#include "machine/pci_controller.h"

#define CORB_ADDR   0x40
#define CORB_LBA    (CORB_ADDR+0x00)
#define CORB_UBA    (CORB_ADDR+0x04)
#define CORB_WP     (CORB_ADDR+0x08)
#define CORB_RP     (CORB_ADDR+0x0A)
#define CORB_CTRL   (CORB_ADDR+0x0C)
#define CORB_STATUS (CORB_ADDR+0x0D)
#define CORB_SIZE   (CORB_ADDR+0x0E)

#define RIRB_ADDR   0x50
#define RIRB_LBA    (RIRB_ADDR+0x00)
#define RIRB_UBA    (RIRB_ADDR+0x04)
#define RIRB_WP     (RIRB_ADDR+0x08)
#define RIRB_INT    (RIRB_ADDR+0x0A)
#define RIRB_CTRL   (RIRB_ADDR+0x0C)
#define RIRB_STATUS (RIRB_ADDR+0x0D)
#define RIRB_SIZE   (RIRB_ADDR+0x0E)

namespace oostubs {

namespace
{
// because qemu can't do basic pointer arithmetic
//	inline u16 read16(const void *p) { return (((const u8 *) p)[1] << 8) | ((const u8 *) p)[0]; }
//	inline u32 read32(const void *p) { return ((read16(((const u8 *) p) + 2) << 16) | read16(p)); }
//	inline void write16(void *p, uint v) { ((u8 *) p)[0] = (v & 0xFF); ((u8 *) p)[1] = ((v >> 8) & 0xFF); }
//	inline void write32(void *p, uint v) { write16(p, (v & 0xFFFF)); write16(((u8 *) p) + 2, ((v >> 16) & 0xFFFF)); }
	inline u16 read16(volatile const void *p) { return *((const u16 *) p); }
	inline u32 read32(volatile const void *p) { return *((const u32 *) p); }
	inline void write16(volatile void *p, uint v) { *((u16 *) p) = (u16) v; }
	inline void write32(volatile void *p, uint v) { *((u32 *) p) = (u32) v; }
}

SoundcardController::RB::RB(uint s)
{
	base = nullptr;

	u8 *t = new u8[0x100 * s + 1024];

	u64 p = (u64) t;

	p = (p + 1023) & ~0x1FF;

	ASSERT((p - (u64) t) < 1024);

	buffer = (u32 *) p;
}

void SoundcardController::CORB::setup(volatile void *p)
{
	base = (volatile u8 *) p;

	kout << "setting up CORB " << buffer << io::endl;

	write32(base + CORB_LBA, (u64) buffer);
	write32(base + CORB_UBA, (u64) (buffer + 256 * sizeof(u32)));
	base[CORB_SIZE] = base[CORB_SIZE];
	write16(base + CORB_RP, 0x8000 | read16(base + CORB_RP));
	TRY_FOR(0x100000, !(read16(base + CORB_RP) & 0x8000));
	write16(base + CORB_RP, read16(base + CORB_RP) & 0x7FFF);
	TRY_FOR(0x100000, read16(base + CORB_RP) & 0x8000);
	write16(base + CORB_WP, 0);

	base[CORB_CTRL] = 2;
	TRY_FOR(0x100000, !(base[CORB_CTRL] & 2));
}

void SoundcardController::RIRB::setup(volatile void *p)
{
	base = (volatile u8 *) p;

	kout << "setting up RIRB " << buffer << io::endl;

	write32(base + RIRB_LBA, (u64) buffer);
	write32(base + RIRB_UBA, (u64) (buffer + 256 * sizeof(u64)));
	base[RIRB_SIZE] = base[RIRB_SIZE];

	base[RIRB_CTRL] = 2;
	TRY_FOR(0x100000, !(base[RIRB_CTRL] & 2));
}

void SoundcardController::CORB::write(uint codec, uint node, uint cmd, u8 data)
{
	ASSERT(codec == (codec & 0xF));
	ASSERT(node == (node & 0xFF));
	ASSERT(cmd == (cmd & 0xFFF));

	u32 v = ((codec << 28) | (node << 20) | (cmd << 8) | data);
	uint idx = (read16(base + CORB_WP) + 1) % 256;

	buffer[idx] = v;
	write16(base + CORB_WP, idx);
}

SoundcardController::SoundcardController(void)
{
	PCIControllerManager::instance().search([](const PCIDevice& pci) {
		return pci.header().class_code == 4 && pci.header().subclass == 3;
	}, &mHDA);

	if(present())
	{
		kout << "type=" << io::Format::HEX << mHDA.header().header_type << io::endl;

		for(uint i = 0 ; i < 6 ; ++i)
		{
			kout << "BAR" << i << ": " << (void *) (u64) mHDA.header().body.base.bar[i] << io::endl;
		}

		volatile u8 *base_addr = (volatile u8 *) (u64) mHDA.header().body.base.bar[0];

		kout << "Resetting HDA" << io::endl;

		base_addr[0x08] |= 1;
		while(!(base_addr[0x08] & 1));

		mVersion = read16(base_addr + 2);

		kout << "Version " << mVersion << io::endl;

		uint node_count = read16(base_addr + 0x0E);

		kout << "Nodes are " << node_count << io::endl;

		ASSERT(node_count > 0);

		mCORB.setup(base_addr);
		mRIRB.setup(base_addr);

		mCORB.write(0, 0, 0xF00, 4);

//		TRY_FOR(0x1000000, read16(base_addr + RIRB_INT) == 0);

		for(volatile uint i = 0 ; i < 0x1000000 ; ++i);

		u32 buf[0x80 / sizeof(u32)];

		for(uint i = 0 ; i < (0x80 / sizeof(u32)) ; ++i)
		{
			buf[i] = read32(base_addr + i * sizeof(u32));
		}

		kout << io::memory(buf, 0x80) << io::endl;

		kout << io::memory(mRIRB.buffer, 10 * sizeof(u64)) << io::endl;
	}
}

}

