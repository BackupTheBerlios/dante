#include <display/textDriver.h>
#include <sys/io.h>

TextDriver::TextDriver()
{
    cv_buffer = (char *) 0xC00B8000;
    cv_pos = 0;
}

TextDriver::TextDriver(char * i_buffer)
{
    cv_buffer = i_buffer;
    cv_pos = 0;
}

void TextDriver::print(const char * i_string)
{
    int i = 0;
    while ('\0' != i_string[i])
    {
	switch (i_string[i])
	{
	    case '\t':
		cv_pos += 3;
		break;
	    case '\b': // need to add checking of cv_prev.
		if (cv_pos != 0)
		{
		    cv_pos -= 2;
		    cv_buffer[((cv_pos+1) << 1)] = ' ';
		}
		break;
	    case '\n':
		cv_pos = cv_pos - (cv_pos % 80) + 79;
		break;
	    default:
		cv_buffer[(cv_pos << 1)] = i_string[i];
		cv_buffer[(cv_pos << 1) | 0x1] = '\7';
		break;
	}
	cv_prev = i_string[i];

	cv_pos++;
	i++;

	while (cv_pos >= (80*25))
	{
	    for (int i = 0; i < (160*24); i++)
	    {
		cv_buffer[i] = cv_buffer[i+160];
	    }
	    for (int i = (160*24); i < (160*25); i++)
	    {
		cv_buffer[i] = '\0';
	    }
	    cv_pos -= 80;
	}
	
    }
    //update cursor
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char) cv_pos & 0xFF);
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char) (cv_pos >> 8) & 0xFF);
}

void TextDriver::clear(void)
{
    for (int i = 0; i < (80*25); i++)
    {
	cv_buffer[i<<1] = '\0';
    }
    cv_pos = 0;
};
