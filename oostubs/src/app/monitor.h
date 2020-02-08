#ifndef OOSTUBS_APP_MONITOR_H
#define OOSTUBS_APP_MONITOR_H

#include "user/thread.h"

namespace oostubs
{
	class Monitor : public Thread
	{
		public:
		protected:
			void execute( ) override;

		private:
	};
}

#endif

