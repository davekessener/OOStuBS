#ifndef OOSTUBS_MACHINE_CPU_H
#define OOSTUBS_MACHINE_CPU_H

#include "aux.h"

#include "mpl/singleton.h"

namespace oostubs
{
	class CPU
	{
		public:
		class Lock
		{
			public:
				Lock(CPU& cpu) : mCPU(&cpu) { mCPU->disable_int(); }
				~Lock( ) { mCPU->enable_int(); }

			private:
				CPU *mCPU;
		};

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
