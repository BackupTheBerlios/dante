#ifndef __MEM_PAGEDIR_H
#define __MEM_PAGEDIR_H

#include <stdint.h>

class PageDir
{
    public:
	PageDir();
	PageDir(uint32_t * i_pageDirAddr, uint32_t * i_pageDirVirtAddr);
	PageDir(const PageDir&);
	~PageDir();

	void info();
	void reloadPageDir()
	    {
		register uint32_t __cr3;
		asm volatile("mov %%cr3, %0" : "=r" (__cr3));
		asm volatile("mov %0, %%cr3" : : "r" (__cr3));
	    }

	uint32_t findFreeAddress(bool i_high = false);

	uint32_t readPageFlags(uint32_t i_pageAddress);
	uint32_t writePageFlags(uint32_t i_pageAddress, uint32_t i_flags);
	bool checkPageFlags(uint32_t i_pageAddress, uint32_t i_flags)
	    {
		if (i_flags & this->readPageFlags(i_pageAddress))
		    return true;
		return false;
	    }

	enum PageFlags
	{
	    PRESENT = 		0x001,
	    WRITE = 		0x002,
	    SUPERVISOR = 	0x004,
	    CACHE_DISABLE = 	0x008,
	    ACCESSED = 		0x010,
	    DIRTY = 		0x020,
	    PAGESIZE = 		0x040,
	    GLOBAL = 		0x080,
	    USER1 = 		0x100,
	    USER2 =		0x200,
	    USER3 =		0x400,
	};
	    
	
    private:
	uint32_t * cv_pageDir;
	uint32_t * cv_virtualPageDir;
};

extern PageDir g_kernelPageDirectory;


#endif
