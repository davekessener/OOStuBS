#include "utils/memory.h"

#ifndef NOKERNEL

void * operator new(size_t n)
{
	return oostubs::MemoryManager::instance().alloc(n);
}

void operator delete(void *p)
{
	oostubs::MemoryManager::instance().free(p);
}

void * operator new[](size_t n)
{
	return oostubs::MemoryManager::instance().alloc(n);
}

void operator delete[](void *p, size_t n)
{
	oostubs::MemoryManager::instance().free(p);
}

#endif

