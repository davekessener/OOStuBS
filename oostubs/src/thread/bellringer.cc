#include "thread/bellringer.h"

namespace oostubs {

void Bellringer::schedule(Bell& bell, uint time)
{
	ASSERT(time);

	for(auto i1 = mBells.begin(), i2 = mBells.end() ; i1 != i2 ; ++i1)
	{
		if(i1->left > time)
		{
			i1->left -= time;

			mBells.insert(i1, {&bell, time});

			return;
		}
		else
		{
			time -= i1->left;
		}
	}

	mBells.push({&bell, time});
}

void Bellringer::cancel(Bell& bell)
{
	for(auto i1 = mBells.begin(), i2 = mBells.end() ; i1 != i2 ; ++i1)
	{
		if(i1->bell == &bell)
		{
			auto i = i1;

			++i;

			if(i != i2)
			{
				i->left += i1->left;
			}

			mBells.remove(i1);

			return;
		}
	}

	PANIC("NO SUCH BELL!");
}

void Bellringer::tick(void)
{
	if(mBells.empty()) return;

	auto i = mBells.begin();

	--i->left;

	while(i != mBells.end() && !i->left)
	{
		i->bell->ring();

		i = mBells.remove(i);
	}
}

}

