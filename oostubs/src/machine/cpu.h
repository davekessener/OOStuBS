#ifndef OOSTUBS_MACHINE_CPU_H
#define OOSTUBS_MACHINE_CPU_H

#include "aux.h"

#include "mpl/singleton.h"

namespace oostubs
{
	class CPU
	{
		public:
			CPU( ) { }

			void enable_int( );
			void disable_int( );
			void idle( );
			void halt( );

		private:
			CPU(const CPU&) = delete;
			CPU& operator=(const CPU&) = delete;
	};

	typedef mpl::SingletonHolder<CPU> CPUManager;
}

#endif
