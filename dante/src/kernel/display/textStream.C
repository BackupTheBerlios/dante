#include <display/textStream.h>

char * endl = "\n";
TextStream kout;

TextStream::TextStream()
{
    cv_driver = TextDriver();
}

TextStream::TextStream(TextDriver& i_driver)
{
    cv_driver = i_driver;
}

TextStream& TextStream::operator<<(const char * i_string)
{
    cv_driver.print(i_string);
    return *this;
}

TextStream& TextStream::operator<<(const uint32_t i_val)
{
    char l_buf[9];
    l_buf[8] = '\0';
    (*this) << "0x";
    for(int i = 0; i < 8; i++)
    {
	char l_tmp = (i_val >> ((7-i) << 2)) & 0xF;
	if (l_tmp > 9)
	{
	    l_tmp = 'A' + l_tmp - 0xA;
	}
	else
	{
	    l_tmp = '0' + l_tmp;
	}
	l_buf[i] = l_tmp;
    }
    (*this) << l_buf;

    return *this;
}    
    

void TextStream::clear()
{
    cv_driver.clear();
}


