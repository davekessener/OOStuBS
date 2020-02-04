#include "thread/customer.h"

namespace oostubs {

void Customer::waitingroom(Waitingroom *wr)
{
	ASSERT(mWR ? !wr : !!wr);

	mWR = wr;
}

}

