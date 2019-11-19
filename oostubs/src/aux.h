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
		CGA_DATA = 0x03D5
	};
}

#endif

