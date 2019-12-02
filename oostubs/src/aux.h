#ifndef OOSTUBS_AUX_H
#define OOSTUBS_AUX_H

#include <stdint.h>

#define double ::oostubs::Double

typedef unsigned uint;
typedef decltype(sizeof(0)) size_t;

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
		PIC_DATA_S = 0xA1
	};
}

#endif

