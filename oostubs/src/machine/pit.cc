#include "machine/pit.h"

#define PIT_COUNTER0  (0<<6)
#define PIT_COUNTER1  (1<<6)
#define PIT_COUNTER2  (2<<6)
#define PIT_LATCH     (0<<4)
#define PIT_LO        (1<<4)
#define PIT_HI        (2<<4)
#define PIT_ONESHOT   (0<<1)
#define PIT_HWONESHOT (1<<1)
#define PIT_PERIODIC  (2<<1)
#define PIT_SQUARE    (3<<1)
#define PIT_BINARY    (0<<0)
#define PIT_BCD       (1<<0)

#define PIT_BASET 838

#define LO(v) ((v)&0xFF)
#define HI(v) (((v)>>8)&0xFF)

namespace oostubs {

PIT::PIT(void)
	: mInterval(0)
	, mCtrl(Port::PIT1_CTRL)
	, mCounter(Port::PIT1_COUNT0)
{
}

void PIT::interval(uint t)
{
	mInterval = t;

	t = (t * 1000) / PIT_BASET;

	mCtrl.outb(PIT_COUNTER0 | PIT_LO | PIT_HI | PIT_PERIODIC | PIT_BINARY);
	mCounter.outb(LO(t));
	mCounter.outb(HI(t));
}

}

