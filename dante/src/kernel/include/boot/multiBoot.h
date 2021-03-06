#ifndef __BOOT_MULTIBOOT_H
#define __BOOT_MULTIBOOT_H

#include <stdint.h>
#include <lib/Queue.h>

class MultiBootParser
{
    public:
	MultiBootParser(void);
	MultiBootParser(void * i_bootHeader);
	void parseHeader(void * i_bootHeader);

	enum imageType
	{
	    UNKNOWN = 0,
	    AOUT = 1,
	    ELF = 2
	};

	struct Module
	{
	    void * cv_start, * cv_end;
	};

    private:
	bool cv_verboseParsing;
	
	uint32_t cv_flags;
	char * cv_cmdLine;
	
	void parseMemRange(void *);
	uint32_t cv_memLow;
	uint32_t cv_memHigh;

	void parseBootDevice(void *);
	uint8_t cv_bootDevice[4];

	void parseModules(void *);
	Queue<Module> cv_modules;
	uint32_t cv_modCount;
	void * cv_modAddress;

	void parseSymbolTable(void *, imageType = UNKNOWN);
	imageType cv_imageType;
	void * cv_symbolAddress;

	void parseMemMap(void *);
	uint32_t cv_mapLength;
	void * cv_mapAddress;
		

};


extern MultiBootParser g_multiBoot;
    

#endif
