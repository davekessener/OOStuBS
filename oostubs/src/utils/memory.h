#ifndef OOSTUBS_UTILS_MEMORY_H
#define OOSTUBS_UTILS_MEMORY_H

#include "mpl/singleton.h"

void * operator new(size_t) noexcept(false);
void operator delete(void *) noexcept;

namespace oostubs
{
	template<size_t N>
	class SimpleMemoryManager
	{
		typedef SimpleMemoryManager<N> Self;
		typedef mpl::singleton_policies::OneshotCreationPolicy<Self> CreationPolicy;

		#pragma pack(1)
		struct __attribute__((__packed__)) entry_t
		{
			uint32_t size;
			uint32_t previous;
			uint32_t next;
		};

		static constexpr size_t OVERHEAD = sizeof(uint32_t);

		static_assert(sizeof(entry_t) == 3 * sizeof(uint32_t), "entry not packed!");

		public:
		typedef mpl::SingletonHolder<Self> Singleton;

		static constexpr size_t CAPACITY = N;

		public:
			void *alloc(size_t) noexcept;
			void free(void *) noexcept;

		private:
			SimpleMemoryManager( );

			entry_t& entry(uint32_t i) { return *((entry_t *) (mBuf + i)); }
			void *to_address(entry_t& e) { return ((uint8_t *) &e) + OVERHEAD; } 
			uint32_t from_address(void *p) { return (((uint8_t *) p) - OVERHEAD) - mBuf; }
			uint32_t endof(entry_t& e) { return from_address(((uint8_t *) &e) + e.size); }

		private:
			uint8_t mBuf[CAPACITY];
			uint32_t mFirst;

			friend CreationPolicy;
	};

	typedef SimpleMemoryManager<0x100000>::Singleton MemoryManager;

	template<size_t N>
	SimpleMemoryManager<N>::SimpleMemoryManager(void)
	{
		entry_t& root{entry(mFirst = 0)};

		root.size = CAPACITY;
		root.previous = root.next = 0;
	}

	template<size_t N>
	void *SimpleMemoryManager<N>::alloc(size_t n)
	{
		entry_t& root{entry(mFirst)};
		entry_t *e = &root;

		while(true)
		{
			if(e->size >= n + OVERHEAD)
			{
				entry_t *prev = &entry(e->previous);
				entry_t *next = &entry(e->next);

				if(e->size - n - OVERHEAD <= 3 * OVERHEAD)
				{
					prev->next = e->next;
					next->previous = e->previous;

					return to_address(*e);
				}
				else
				{
					uint32_t a = prev->next + n + OVERHEAD;
					entry_t *s = &entry(a);

					s->size = e->size - n - OVERHEAD;
					e->size = n + OVERHEAD;

					s->next = e->next;
					s->previous = e->previous;
					
					prev->next = next->previous = a;

					return to_address(*e);
				}
			}

			e = &entry(e->next);

			if(e == &root)
			{
				// TODO error handling

				return nullptr;
			}
		}
	}

	template<size_t N>
	void SimpleMemoryManager<N>::free(void *p)
	{
		uint32_t a = from_address(p);
		entry_t& d{entry(a)};
		uint32_t o = endof(d);
		entry_t& root{entry(mFirst)};
		entry_t *e = &root;

		do
		{
			if(from_address(e) == o)
			{
				if(endof(entry(e->previous)) == a)
				{
					entry_t& p{entry(e->previous)};

					p.size += d.size + e->size;
					p.next = e->next;
					entry(e->next).previous = e->previous;

					return;
				}
				else
				{
					entry(e->previous).next = a;
					d.size += e->size;
					entry(e->next).previous = e->previous;

					return;
				}
			}
			else if(endof(*e) == a)
			{
				if(e->next == o)
				{
					entry_t& n{entry(e->next)};

					e->size += d.size + n.size;
					e->next = n.next;
					entry(n.next).previous = from_address(e);

					return;
				}
				else
				{
					e->size += d.size;

					return;
				}
			}
			else if((from_address(e) < a && e->next > o) || e->next < from_address(e))
			{
				if(from_address(e) < a)
				{
					entry(e->next).previous = a;
					d.next = e->next;
					e->next = a;
					d.previous = from_address(e);

					return;
				}
				else
				{
					entry(e->previous).next = a;
					d.previous = e->previous;
					e->previous = a;
					d.next = from_address(e);

					return;
				}
			}

			e = &entry(e->next);
		}
		while(e != &root);

		// TODO error handling
	}
}

#endif

