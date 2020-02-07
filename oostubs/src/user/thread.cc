#include "user/thread.h"

#include "machine/guard.h"

#include "user/mutex.h"
#include "user/lock.h"

#include "sys/scheduler.h"

#define STACK_SIZE 0x10000
#define THREAD_COUNT 50

namespace oostubs {

namespace
{
	struct StackManager
	{
		StackManager( );

		u8 *allocate( );
		void free(u8 *);

		u8 *stack_at(uint i) { return stack + i * STACK_SIZE; }

		u8 stack[STACK_SIZE * THREAD_COUNT];
		uint idx;
		Mutex mutex;
	};

	StackManager::StackManager(void)
	{
		idx = 0;

		for(uint i = 0 ; i < THREAD_COUNT ; ++i)
		{
			*stack_at(i) = (i + 1) % THREAD_COUNT;
		}
	}

	u8 *StackManager::allocate()
	{
		Lock<Mutex> lock(mutex);

		u8 *s = stack_at(idx);

		idx = *s;

		return s;
	}

	void StackManager::free(u8 *s)
	{
		Lock<Mutex> lock(mutex);

		uint i = (s - stack) / STACK_SIZE;

		*s = idx;
		idx = i;
	}

	StackManager stack_manager;
}

Thread::Thread(void)
	: Customer((mStack = stack_manager.allocate()) + STACK_SIZE)
	, mStatus(1)
{
}

Thread::~Thread(void)
{
	join();
}

void Thread::start(void)
{
	ASSERT(mStack);

	mStatus.wait();

	{
		Guard::Lock lock(GuardManager::instance());

		SchedulerManager::instance().add(*this);
	}
}

void Thread::join(void)
{
	if(mStack)
	{
		mStatus.wait();
	}
}

void Thread::doRun(void)
{
	execute();

	mStatus.signal();

	stack_manager.free(mStack);

	mStack = nullptr;

	{
		Guard::Lock lock(GuardManager::instance());

		SchedulerManager::instance().exit();
	}
}

}

