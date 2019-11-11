#include "utils/memory.h"

void * operator new(size_t n)
{
	return oostubs::MemoryManager::instance().alloc(n);
}

void operator delete(void *p)
{
	oostubs::MemoryManager::instance().free(p);
}

