#ifndef OOSTUBS_AUX_H
#define OOSTUBS_AUX_H

#include <stdint.h>

#define double ::oostubs::Double

typedef unsigned uint;
typedef decltype(sizeof(0)) size_t;

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
		CGA_CTRL = 0x03D4,
		CGA_DATA = 0x03D5,
		PIC_CTRL_M = 0x20,
		PIC_DATA_M = 0x21,
		PIC_CTRL_S = 0xA0,
		PIC_DATA_S = 0xA1,
		PIT1_COUNT0 = 0x40,
		PIT1_COUNT1 = 0x41,
		PIT1_COUNT2 = 0x42,
		PIT1_CTRL = 0x43,
		KB_CTRL = 0x64,
		KB_DATA = 0x60
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
}

#endif

