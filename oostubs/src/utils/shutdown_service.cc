#include "utils/shutdown_service.h"

#ifndef NOKERNEL

extern "C" {

int atexit(void (*f)(void))
{
	oostubs::ShutdownService::instance().register_callback(f);

	return 0;
}

}

#endif

namespace oostubs {

ShutdownService *ShutdownService::sInstance = nullptr;

ShutdownService& ShutdownService::instance(void)
{
	static uint8_t buf[sizeof(ShutdownService)];

	if(!sInstance)
	{
		new(buf) ShutdownService;

		sInstance = reinterpret_cast<ShutdownService *>(buf);
	}

	return *sInstance;
}

void ShutdownService::register_callback(callback_fn f, Priority p)
{
	for(auto i1 = mEntries.cbegin(), i2 = mEntries.cend() ; i1 != i2 ; ++i1)
	{
		if(static_cast<uint32_t>(i1->first) > static_cast<uint32_t>(p))
		{
			mEntries.insert(i1, mpl::make_pair(p, f));

			return;
		}
	}

	mEntries.emplace_back(mpl::make_pair(p, f));
}

void ShutdownService::shutdown(void)
{
	while(!mEntries.empty())
	{
		decltype(mEntries) backlog(mEntries);

		mEntries.clear();

		for(const auto& p : backlog)
		{
			p.second();
		}
	}
}

}

