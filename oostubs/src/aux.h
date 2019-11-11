#ifndef OOSTUBS_AUX_H
#define OOSTUBS_AUX_H

#include <utility>
#include <new>

#include <stdint.h>

typedef unsigned uint;
typedef decltype(sizeof(0)) size_t;

namespace oostubs
{
	enum class Port : uint16_t
	{
		CGA_CTRL = 0x03D4,
		CGA_DATA = 0x03D5
	};
}

#endif

