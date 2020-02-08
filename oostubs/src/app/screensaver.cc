#include "app/screensaver.h"

#include "mboot.h"

#include "gui/texture.h"
#include "gui/screen.h"
#include "gui/dif.h"

namespace oostubs {

namespace
{
	u32 rgb(u32 r, u32 g, u32 b)
	{	
		return 0xFF000000 | (r << 16) | (g << 8) | b;
	}
}

void ScreensaverThread::execute(void)
{
	int w = mboot_info_ptr->framebuffer_width;
	int h = mboot_info_ptr->framebuffer_height;

	if(!w) w = Screen::WIDTH;
	if(!h) h = Screen::HEIGHT;

	int r = 8;
	Texture bg(w, h);
	Texture ball(2 * r, 2 * r);
	Texture icon{dif_load_from_file(initrd_root->find("textures/diablo.dif"))};

	Screen& screen{FramebufferManager::instance()};

	for(int y = 0 ; y < h ; ++y)
	{
		for(int x = 0 ; x < w ; ++x)
		{
			bg.at(x, y) = rgb(0xFF * x * y / (w-1) / (h-1), 0xFF * x / (w-1), 0xFF * y / (h-1));
		}
	}

	for(uint y = 0 ; y < ball.height() ; ++y)
	{
		for(uint x = 0 ; x < ball.width() ; ++x)
		{
			int dx = ((int) x) - r, dy = ((int) y) - r;

			if(dx * dx + dy * dy < r * r)
			{
				ball.at(x, y) = rgb(0xFF, 0xFF, 0xFF);
			}
		}
	}

	int dx = 9, dy = 2;
	int x = 0, y = 0;
	while(true)
	{
		screen.buffer().blt(bg, 0, 0);
		screen.buffer().blt(icon, (w - icon.width()) / 2, (h - icon.height()) / 2);
		screen.buffer().blt(ball, x, y);

		x += dx;
		y += dy;

		if(x < 0 || x + (int) ball.width() >= w) dx = -dx;
		if(y < 0 || y + (int) ball.height() >= h) dy = -dy;

		screen.sync();
	}
}

}

