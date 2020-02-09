#include "machine/dma.h"

#include "machine/plugbox.h"

#include "lib/memory.h"

#include "panic.h"

#define CHANNEL 5

namespace oostubs {

namespace
{
//	u8 dma_buffer[DMAController::BUFFER_SIZE << 1];

	u8 *find_dma_buffer(void)
	{
		u8 *base = nullptr;//dma_buffer;

		uint p1 = (((u64) base) >> 16);
		uint p2 = (((u64) base + DMAController::BUFFER_SIZE - 1) >> 16);

		if(p1 != p2)
		{
			return (u8 *) (((u64) p2) << 16);
		}
		else
		{
			return base;
		}
	}
}

DMAController::DMAController(void)
	: mAddr(Port::DMA_ADDR)
	, mCount(Port::DMA_COUNT)
	, mPage(Port::DMA_PAGE)
	, mMask(Port::DMA_MASK)
	, mMode(Port::DMA_MODE)
	, mClear(Port::DMA_CLEAR)
{
	u8 *buf = find_dma_buffer();

	mBuffer[0] = buf;
	mBuffer[1] = buf + BUFFER_SIZE / 2;
	mIdx = 0;

	mSource = nullptr;
	mSent = mSize = 0;
}

void DMAController::setup(const u8 *data, uint l)
{
	ASSERT(l);
	ASSERT(data);
	ASSERT(!mSource);

	mSource = data;
	mSize = l;
	mSent = 0;
	mIdx = 0;
}

uint DMAController::send_next(void)
{
	if(mSent == mSize)
	{
		mMask.outb(4 | (CHANNEL & 3));

		return 0;
	}
	else
	{
		uint l = min(BUFFER_SIZE / 2, mSize - mSent);
		bool start = !mSent;

		memmove(mBuffer[mIdx], mSource + mSent, l);

		mSent += l;
		mIdx ^= 1;

		if(start)
		{
			mMask.outb(4 | (CHANNEL & 3));
			mClear.outb(1);
			mMode.outb(0x58 | (CHANNEL & 3));
			mAddr.outb(0);
			mAddr.outb(0);
			mCount.outb((BUFFER_SIZE-1) & 0xFF);
			mCount.outb((BUFFER_SIZE-1) >> 8);
			mPage.outb(((u64) mBuffer[0]) >> 16);
			mMask.outb(CHANNEL & 3);
		}

		return l;
	}
}

}

