#ifndef __DISPLAY_TEXTSTREAM_H
#define __DISPLAY_TEXTSTREAM_H

#include <stdint.h>
#include <display/textDriver.h>
#include <display/kostream.h>

#include <lib/Pair.h>

class TextStream : public kostream
{
    public:
	TextStream();
	TextStream(TextDriver& i_driver);

	void clear();
	TextStream& operator<<(const char * i_string);
	TextStream& operator<<(const uint32_t i_val);
	
    private:
	TextDriver cv_driver;
};

extern char * endl;
extern TextStream kout;

#endif
