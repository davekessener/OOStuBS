#include "thread/coroutine.h"

namespace oostubs {

void Coroutine::go(void)
{
	toc_go(&mTOC);
}

void Coroutine::resume(Coroutine& other)
{
	toc_switch(&mTOC, &other.mTOC);
}

void Coroutine::kickoff(Coroutine *self)
{
	self->run();
}

}

