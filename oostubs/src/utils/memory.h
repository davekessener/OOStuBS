#ifndef OOSTUBS_UTILS_MEMORY_H
#define OOSTUBS_UTILS_MEMORY_H

#include "mpl/singleton.h"

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
			SimpleMemoryManager( );

			void *alloc(size_t) noexcept;
			void free(void *) noexcept;

		private:
			entry_t& entry(uint32_t i) { return *((entry_t *) (mBuf + i)); }
			void *to_address(entry_t& e) { return ((uint8_t *) &e) + OVERHEAD; } 
			uint32_t get_address(entry_t& e) { return ((uint8_t *) &e) - mBuf; }
			uint32_t from_address(void *p) { return (((uint8_t *) p) - OVERHEAD) - mBuf; }
			uint32_t endof(entry_t& e) { return ((uint8_t *) &e) + e.size - mBuf; }

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

		for(uint i = 0 ; i < N ; ++i)
		{
			mBuf[i] = 0;
		}

		root.size = CAPACITY;
		root.previous = root.next = 0;
	}

	template<size_t N>
	void *SimpleMemoryManager<N>::alloc(size_t n) noexcept
	{
		entry_t& root{entry(mFirst)};
		entry_t *e = &root;

		if(n < 3 * OVERHEAD)
		{
			n = 3 * OVERHEAD;
		}

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

					if(mFirst == prev->next)
					{
						mFirst = a;
					}

					s->size = e->size - n - OVERHEAD;
					e->size = n + OVERHEAD;
					
					prev->next = next->previous = a;

					s->next = e->next;
					s->previous = e->previous;

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
	void SimpleMemoryManager<N>::free(void *p) noexcept
	{
		uint32_t a = from_address(p);
		entry_t& d{entry(a)};
		uint32_t o = endof(d);
		entry_t& root{entry(mFirst)};
		entry_t *e = &root;

		do
		{
			uint32_t ea = get_address(*e);
			uint32_t ee = endof(*e);

			if(ea == o)
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
					d.size += e->size;

					if(ea != e->next)
					{
						d.previous = e->previous;
						d.next = e->next;
						entry(d.previous).next = a;
						entry(d.next).previous = a;
					}
					else
					{
						d.next = d.previous = a;
					}

					if(ea == mFirst)
					{
						mFirst = a;
					}

					return;
				}
			}
			else if(ee == a)
			{
				if(e->next == o)
				{
					entry_t& n{entry(e->next)};

					e->size += d.size + n.size;
					e->next = n.next;
					entry(n.next).previous = ea;

					return;
				}
				else
				{
					e->size += d.size;

					return;
				}
			}
			else if((ea < a && e->next > o) || e->next < ee)
			{
				if(ea < a)
				{
					entry(e->next).previous = a;
					d.next = e->next;
					e->next = a;
					d.previous = ea;

					return;
				}
				else
				{
					entry(e->previous).next = a;
					d.previous = e->previous;
					e->previous = a;
					d.next = ea;

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

