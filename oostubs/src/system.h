/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                         A P P L I C A T I O N                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/

#ifndef OOSTUBS_SYSTEM_H
#define OOSTUBS_SYSTEM_H

#include "mpl/singleton.h"

namespace oostubs
{
	class SystemImpl
	{
		public:
		typedef mpl::singleton_policies::OneshotCreationPolicy<SystemImpl> CreationPolicy;
		typedef mpl::SingletonHolder<SystemImpl> Singleton;

		public:
			void run( );
	
		private:
			SystemImpl( ) { }
			~SystemImpl( ) { }
			SystemImpl(const SystemImpl&) = delete;

			friend CreationPolicy;
	};

	typedef SystemImpl::Singleton System;
}

#endif

