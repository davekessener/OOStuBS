#ifndef OOSTUBS_UTILS_SHUTDOWNSERVICE_H
#define OOSTUBS_UTILS_SHUTDOWNSERVICE_H

#include "aux.h"

#include "lib/fixed_vector.h"

namespace oostubs
{
	class ShutdownService
	{
		public:
		enum class Priority : uint32_t
		{
			FIRST,
			EARLY,
			NORMAL,
			LATER,
			LAST
		};

		private:
		typedef void (*callback_fn)(void); // TODO support ftors
		typedef std::pair<Priority, callback_fn> entry_t;

		public:
			static ShutdownService& instance( );

			void register_callback(callback_fn, Priority = Priority::NORMAL);
			void shutdown( );

		private:
			ShutdownService( ) { }

			static ShutdownService *sInstance;

		private:
			utils::FixedVector<10, entry_t> mEntries;
	};
}

#endif

