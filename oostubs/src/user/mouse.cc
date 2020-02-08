#include "user/mouse.h"

#include "gui/screen.h"

namespace oostubs {

namespace
{
	inline u32 clamp(int lo, int v, int hi)
	{
		return (v < lo ? lo : (v > hi ? hi : v));
	}
}

Mouse::Mouse(void)
	: mX(Screen::WIDTH / 2)
	, mY(Screen::HEIGHT / 2)
	, mButtons(0)
{
}

void Mouse::accept(uint btns, i32 dx, i32 dy)
{
	mButtons = btns;
	mX = clamp(0, mX + dx, Screen::WIDTH);
	mY = clamp(0, mY - dy, Screen::HEIGHT);
}

}

