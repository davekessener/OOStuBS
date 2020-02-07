#ifndef OOSTUBS_MBOOT_H
#define OOSTUBS_MBOOT_H

#include "aux.h"

#define MBOOT_FLAG_MEM (1<<0)
#define MBOOT_FLAG_MODS (1<<3)

extern "C" {

struct mboot_aout
{
	u32 tabsize;
	u32 strsize;
	u32 addr;
	u32 reserved;
} __attribute__((packed));

struct mboot_elf_header
{
	u32 num;
	u32 size;
	u32 addr;
	u32 shndx;
} __attribute__((packed));

struct mboot_fb_palette
{
	u32 addr;
	u16 num_colors;
} __attribute__((packed));

struct mboot_fb_color_rgb
{
	u8 red_pos;
	u8 red_mask_size;
	u8 green_pos;
	u8 green_mask_size;
	u8 blue_pos;
	u8 blue_mask_size;
} __attribute__((packed));

struct mboot_info
{
	u32 flags;

	u32 mem_lower;
	u32 mem_upper;

	u32 boot_device;

	u32 cmdline;

	u32 mods_count;
	u32 mods_addr;

	union
	{
		struct mboot_aout aout;
		struct mboot_elf_header elf_header;
	} syms;

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
	union
	{
		struct mboot_fb_palette palette;
		struct mboot_fb_color_rgb rgb;
	} color_info;
} __attribute__((packed));

struct mboot_module
{
	u32 mod_start;
	u32 mod_end;

	u32 string;

	u32 reserved;
} __attribute__((packed));

extern struct mboot_info *mboot_info_ptr;

}

static_assert(sizeof(mboot_info) == 116);
static_assert(sizeof(mboot_module) == 16);

#endif

