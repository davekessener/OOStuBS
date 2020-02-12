#ifndef OOSTUBS_APP_SCREENSAVER_H
#define OOSTUBS_APP_SCREENSAVER_H

#include "lib.h"

#include "user/thread.h"

namespace oostubs
{
	class ScreensaverThread : public Thread
	{
		protected:
			void execute( ) override;
	};
}

#endif

