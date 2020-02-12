#ifndef OOSTUBS_MACHINE_CPU_H
#define OOSTUBS_MACHINE_CPU_H

#include "lib.h"

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
			CPU( ) : mEnabled(false) { }

			bool enabled( ) const { return mEnabled; }

			void enable_int( );
			void disable_int( );
			void idle( );
			void halt( );

		private:
			bool mEnabled;

			CPU(const CPU&) = delete;
			CPU& operator=(const CPU&) = delete;
	};
}

#endif
