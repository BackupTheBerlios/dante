#ifndef __DISPLAY_NULLSTREAM_H
#define __DISPLAY_NULLSTREAM_H

#include <stdint.h>
#include <display/kostream.h>

class NullStream : public kostream
{
    public:
	inline NullStream() {};
	inline NullStream& operator<<(const char *) { return *this; };
	inline NullStream& operator<<(const uint32_t) { return *this; };

	inline void clear() { return; };

};

#endif
