#ifndef OOSTUBS_AUX_H
#define OOSTUBS_AUX_H

#include <stdint.h>

typedef unsigned uint;
typedef decltype(sizeof(0)) size_t;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

inline void *operator new(size_t, void *p)
{
	return p;
}

namespace oostubs
{
	enum class Port : uint16_t
	{
		PIC_CTRL_M   = 0x0020,
		PIC_DATA_M   = 0x0021,
		PIC_CTRL_S   = 0x00A0,
		PIC_DATA_S   = 0x00A1,
		PIT1_COUNT0  = 0x0040,
		PIT1_COUNT1  = 0x0041,
		PIT1_COUNT2  = 0x0042,
		PIT1_CTRL    = 0x0043,
		KB_DATA      = 0x0060,
		KB_CTRL      = 0x0064,
		DMA_PAGE     = 0x008B,
		DMA_ADDR     = 0x00C4,
		DMA_COUNT    = 0x00C6,
		DMA_MASK     = 0x00D4,
		DMA_MODE     = 0x00D6,
		DMA_CLEAR    = 0x00D8,
		DSP_MIX_CTRL = 0x0224,
		DSP_MIX_DATA = 0x0225,
		DSP_RESET    = 0x0226,
		DSP_READ     = 0x022A,
		DSP_WRITE    = 0x022C,
		DSP_ACK      = 0x022E,
		CGA_CTRL     = 0x03D4,
		CGA_DATA     = 0x03D5,
		PCI_CFG_ADDR = 0x0CF8,
		PCI_CFG_DATA = 0x0CFC
	};

	enum class IRQ : uint8_t
	{
		TIMER    = 0x20,
		KEYBOARD = 0x21,
		MOUSE    = 0x2C
	};

	template<typename A, typename B>
	auto min(const A& a, const B& b) -> decltype(a + b)
	{
		typedef decltype(a + b) R;

		return (a < b ? (R) a : (R) b);
	};

	template<typename A, typename B>
	auto max(const A& a, const B& b) -> decltype(a + b)
	{
		typedef decltype(a + b) R;

		return (a > b ? (R) a : (R) b);
	};

	inline void idle_for(volatile uint n) { while(n--); }
}

#endif

