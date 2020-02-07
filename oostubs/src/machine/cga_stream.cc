#include "machine/cga_stream.h"

#include "sys/screen.h"

namespace oostubs { namespace io {

void CGAStream::doFlush(void)
{
	ScreenManager::instance().wputs_n(data(), size());
}

void CGAStream::doPutc(Super::value_type c)
{
	Super::doPutc((mAttrib << 8) | c);
}

}}

