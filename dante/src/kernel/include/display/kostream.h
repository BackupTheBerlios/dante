#ifndef __DISPLAY_KOSTREAM_H
#define __DISPLAY_KOSTREAM_H

#include <stdint.h>

class kostream
{
    public:
	virtual void clear() = 0;
	virtual kostream& operator<<(const char *) = 0;
	virtual kostream& operator<<(const uint32_t) = 0;
};

#endif
