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
				Lock(CPU& cpu) : mCPU(&cpu), mEnabled(cpu.enabled()) 
					{ 
						if(mEnabled)
							mCPU->disable_int(); 
					}

				~Lock( ) { if(mEnabled) mCPU->enable_int(); }

			private:
				CPU *mCPU;
				bool mEnabled;
		};

		public:
			CPU( ) : mCounter(0) { }

			bool enabled( ) const { return mCounter > 0; }

			void enable_int( );
			void disable_int( );
			void idle( );
			void halt( );

		private:
			int mCounter;

			CPU(const CPU&) = delete;
			CPU& operator=(const CPU&) = delete;
	};

	typedef mpl::SingletonHolder<CPU> CPUManager;
}

#endif
