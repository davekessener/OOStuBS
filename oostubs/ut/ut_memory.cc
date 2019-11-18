#include <iomanip>

#include "ut.h"

#include "utils/memory.h"

UT_SUIT("Static Memory Dealer")
{
	typedef SimpleMemoryManager<100> mm_t;

	UT_DATA
	{
		mm_t *mem = nullptr;

		size_t get_a(const void *p)
		{
			return (size_t) (((uint8_t *) p) - ((uint8_t *) mem));
		}

		void print()
		{
			std::cout << "\n";

			uint8_t *p = (uint8_t *) mem;

			for(size_t i = 0 ; i < sizeof(mm_t) ; ++i)
			{
				std::cout << std::hex << std::setw(2) << std::setfill('0') << (int) p[i] << " ";

				if((i + 1) % 0x10 == 0)
				{
					std::cout << "\n";
				}
			}

			std::cout << std::endl;
		}
	};

	UT_BEFORE
	{
		data.mem = new mm_t;
	};

	UT_AFTER
	{
		delete data.mem;
		data.mem = nullptr;
	};

	UT_TEST("simple alloc")
	{
		void *p = data.mem->alloc(10);

		UT_ASSERT(data.get_a(p) == 4);
	};

	UT_TEST("free")
	{
		void *p1 = data.mem->alloc(25);
		void *p2 = data.mem->alloc(25);
		void *p3 = data.mem->alloc(25);

		UT_ASSERT_EQ(4, data.get_a(p1));
		UT_ASSERT_EQ(4 + 25 + 4, data.get_a(p2));
		UT_ASSERT_EQ(4 + 25 + 4 + 25 + 4, data.get_a(p3));

		data.mem->free(p2);

		p2 = data.mem->alloc(20);

		UT_ASSERT_EQ(4 + 25 + 4, data.get_a(p2));
	};

	UT_TEST("re-empty")
	{
		data.print();

		void *p = data.mem->alloc(30);

		data.print();

		data.mem->free(p);

		data.print();
	};
};

