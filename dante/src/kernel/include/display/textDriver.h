#ifndef __DISPLAY_TEXTDRIVER_H
#define __DISPLAY_TEXTDRIVER_H

class TextDriver
{
    public:
	TextDriver();
	TextDriver(char * i_buffer);

	void print(const char * i_string);
	void clear();
    private:
	volatile char * cv_buffer;
	int cv_pos;
	char cv_prev;
};		

#endif
