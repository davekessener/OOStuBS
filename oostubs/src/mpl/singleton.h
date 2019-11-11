#ifndef OOSTUBS_MPL_SINGLETON_H
#define OOSTUBS_MPL_SINGLETON_H

#include "aux.h"

#include "utils/shutdown_service.h"

namespace oostubs
{
	namespace mpl
	{
		namespace singleton_policies
		{
			template<typename T>
			struct OneshotCreationPolicy
			{
				public:
					static T *create( );
					static T *recreate( );
					static void destroy(T *);
			};
		}

		template
		<
			typename Singleton,
			template <typename> class CreationPolicy = singleton_policies::OneshotCreationPolicy
		>
		struct SingletonHolder
		{
			typedef SingletonHolder<Singleton, CreationPolicy> Self;
			typedef CreationPolicy<Singleton> Creator;

			public:
			typedef Singleton value_type;

			public:
				static value_type& instance( );

			private:
				static void destroy( );

				static value_type *sInstance;
				static bool sCreated;
		};

		template<typename T, template <typename> class CP>
		T *SingletonHolder<T, CP>::sInstance = nullptr;

		template<typename T, template <typename> class CP>
		bool SingletonHolder<T, CP>::sCreated = false;

		template<typename T, template <typename> class CP>
		T& SingletonHolder<T, CP>::instance(void)
		{
			if(!sInstance)
			{
				if(sCreated)
				{
					sInstance = Creator::recreate();
				}
				else
				{
					sInstance = Creator::create();
					sCreated = true;
				}

				ShutdownService::instance().register_callback(
					&Self::destroy,
					ShutdownService::Priority::LATER);
			}

			return *sInstance;
		}

		template<typename T, template <typename> class CP>
		void SingletonHolder<T, CP>::destroy(void)
		{
			Creator::destroy(sInstance);
			sInstance = nullptr;
		}

		namespace singleton_policies
		{
			template<typename T>
			T *OneshotCreationPolicy<T>::create(void)
			{
				static T sInstance;

				return &sInstance;
			}

			template<typename T>
			T *OneshotCreationPolicy<T>::recreate(void)
			{
				// TODO error handling

				return nullptr;
			}

			template<typename T>
			void OneshotCreationPolicy<T>::destroy(T *)
			{
			}
		}
	}
}

#endif

