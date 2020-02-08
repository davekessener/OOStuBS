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
		CGA_CTRL    = 0x03D4,
		CGA_DATA    = 0x03D5,
		PIC_CTRL_M  = 0x0020,
		PIC_DATA_M  = 0x0021,
		PIC_CTRL_S  = 0x00A0,
		PIC_DATA_S  = 0x00A1,
		PIT1_COUNT0 = 0x0040,
		PIT1_COUNT1 = 0x0041,
		PIT1_COUNT2 = 0x0042,
		PIT1_CTRL   = 0x0043,
		KB_CTRL     = 0x0064,
		KB_DATA     = 0x0060,
		DSP_RESET   = 0x0226,
		DSP_READ    = 0x022A,
		DSP_WRITE   = 0x022C,
		DSP_ACK     = 0x022E
	};

	enum class IRQ : uint8_t
	{
		TIMER = 0x20,
		KEYBOARD = 0x21,
		MOUSE = 0x2C
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

