#include "gui/dif.h"

#include "panic.h"

#define MAGIC 0x7EDA

namespace oostubs {

Texture dif_load_from_file(const initrd::Node *f)
{
	ASSERT(f->size() > 10);

	const u8 *base = f->content();

	ASSERT(*((const u16 *) base) == MAGIC);

	u32 w = *((const u32 *) (base + 2));
	u32 h = *((const u32 *) (base + 6));

	ASSERT(w && h);
	ASSERT(f->size() == 10 + w * h * sizeof(u32));

	Texture tx(w, h);
	const u32 *image = (const u32 *) (base + 10);

	for(uint y = 0 ; y < h ; ++y)
	{
		for(uint x = 0 ; x < w ; ++x)
		{
			tx.at(x, y) = image[y + x * h];
		}
	}

	return tx;
}

}

