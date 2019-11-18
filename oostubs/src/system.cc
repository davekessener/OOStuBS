/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                         A P P L I C A T I O N                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/

#include "system.h"

#include "memory.h"

#include "io.h"

#include "mpl/ftor.h"

#include "machine/cga_screen.h"
#include "machine/keyboard_controller.h"

namespace oostubs {

int testf(int a, int b) { return a + b * b; }

struct Test
{
	Test() { kout << "Constuctor " << this << "\n"; }
	~Test() { kout << "Destructor " << this << "\n"; }
};

Key getc()
{
	Key k;
	
	while(!k.valid())
	{
		k = KeyboardController::instance().key_hit();
	}

	return k;
}

void update_line(const char *s)
{
	uint c = 0;

	while(*s) { kout << *s++; ++c; }
	kout << ' ';
	for(uint i = 0 ; i < c + 1 ; ++i) { kout << '\b'; }
}

void toggle_key_speed(void)
{
	static bool fast = false;

	KeyboardController::instance().set_repeat_rate((fast ? 0 : 31), (fast ? 0 : 3));

	Screen::instance().putc_at(
		CGAScreen::WIDTH - 1, 0,
		(fast ? 'F' : 'S'),
		CGAScreen::attribute(
			CGAScreen::Color::BLACK,
			(fast ? CGAScreen::Color::GREEN : CGAScreen::Color::RED)));

	fast = !fast;
}

void SystemImpl::run(void)
{
	typedef CGAScreen::Color Color;

	typedef mpl::FTor<int, int, int> f_t;

	Test *ptest = new Test[3];

	delete[] ptest; ptest = nullptr;

	f_t f1{&testf};
	f_t f2{[](int a, int b) { return a * a + b; }};

	uint8_t fg = 1, bg = 0;

	kout << "f1(3, 7) = " << f1(3, 7) << "\n"
	     << "f2(3, 7) = " << f2(3, 7) << io::endl;

	for(int j = 0 ; j < 4 ; ++j)
	{
		kout << io::set_attribute((bg << 4) | fg)
		     << "Hello, World! "
		     << io::endl;

		if(++fg == 0x10)
		{
			fg = 0;
			bg = (bg + 1) & 0x0F;
		}

//		for(uint i = ((uint)-1) / 20 ; i ; --i)
//		{
//		}
	}

	toggle_key_speed();

	kout << io::set_foreground(Color::WHITE)
		 << io::set_background(Color::BLACK)
		 << "\n> "
		 << io::flush;
	
	char buf[128];
	for(uint i = 0 ; i < sizeof(buf) ; ++i) { buf[i] = '\0'; }
	uint i = 0;
	while(true)
	{
		Key k = getc();

		switch(k.ascii())
		{
			case 0:
				switch(k.scancode())
				{
					case Key::scan_code::UP:
						toggle_key_speed();
						break;

					case Key::scan_code::DOWN:
						break;

					case Key::scan_code::LEFT:
						if(i > 0)
						{
							--i;
							kout << '\b' << io::flush;
						}
						break;

					case Key::scan_code::RIGHT:
						if(buf[i])
						{
							kout << buf[i] << io::flush;
							++i;
						}
						break;

					case Key::scan_code::DEL:
						if(buf[i])
						{
							for(uint j = i ; true ; ++j)
							{
								buf[j] = buf[j + 1];

								if(!buf[j]) break;
							}
							update_line(buf + i);
							kout << io::flush;
						}
						break;
					
					default:
						break;
				}
				break;

			case '\n':
				kout << '\n' << buf << "\n> " << io::flush;
				for(uint j = 0 ; j < sizeof(buf) ; ++j) { buf[j] = '\0'; }
				i = 0;
				break;

			case '\b':
				if(i > 0)
				{
					for(uint j = i ; buf[j-1] ; ++j)
					{
						buf[j-1] = buf[j];
					}

					--i;

					kout << '\b';
					update_line(buf + i);
					kout << io::flush;
				}
				break;

			default:
				if(buf[i])
				{
					uint j = sizeof(buf);

					while(!buf[--j]);

					for(; j >= i ; --j)
					{
						buf[j + 1] = buf[j];

						if(!j) break;
					}
				}
				buf[i++] = k.ascii();
				update_line(buf + i - 1);
				kout << buf[i-1] << io::flush;
				break;
		}
	}

	while(true);
}

}

