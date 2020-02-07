#ifndef OOSTUBS_MBOOT_H
#define OOSTUBS_MBOOT_H

#include "aux.h"

extern "C" {

struct mboot_info
{
	u32 flags;

	u32 mem_lower;
	u32 mem_upper;

	u32 boot_device;

	u32 cmdline;

	u32 mods_count;
	u32 mods_addr;

	u32 syms[3];

	u32 mmap_length;
	u32 mmap_addr;

	u32 drives_length;
	u32 drives_addr;

	u32 config_table;

	u32 boot_loader_name;

	u32 apm_table;

	u32 vbe_control_info;
	u32 vbe_mode_info;
	u16 vbe_mode;
	u16 vbe_interface_seq;
	u16 vbe_interface_off;
	u16 vbe_interface_len;

	u64 framebuffer_addr;
	u32 framebuffer_pitch;
	u32 framebuffer_width;
	u32 framebuffer_height;
	u8  framebuffer_bpp;
	u8  framebuffer_type;
	u8  color_info[5];
};

extern struct mboot_info *mboot_info_ptr;

}

#endif

