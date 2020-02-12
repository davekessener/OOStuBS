#ifndef OOSTUBS_MACHINE_PCICONTROLLER_H
#define OOSTUBS_MACHINE_PCICONTROLLER_H

#include "lib.h"
#include "panic.h"

#include "machine/pci.h"

#include "user/keyboard.h"

#include "mpl/singleton.h"

namespace oostubs
{
	class PCIController
	{
		public:
		static constexpr u8 FLAG_MULTIFUNCTION = (1<<7);

		public:
			template<typename F>
				bool search(F&&, PCIDevice *);

		private:
	};

	typedef mpl::SingletonHolder<PCIController> PCIControllerManager;

	template<typename F>
	bool PCIController::search(F&& f, PCIDevice *r)
	{
		for(uint bus = 0 ; bus < 256 ; ++bus)
		{
			for(uint slot = 0 ; slot < 32 ; ++slot)
			{
				PCIDevice d(bus, slot, 0);

				if(!d.valid()) continue;

				if(f(d))
				{
					*r = d;

					return true;
				}

				if(d.header().header_type & FLAG_MULTIFUNCTION)
				{
					for(uint i = 1 ; i < 8 ; ++i)
					{
						d = {bus, slot, i};

						if(!d.valid()) continue;

						if(f(d))
						{
							*r = d;

							return true;
						}
					}
				}
			}
		}

		return false;
	}
}

#endif

