#ifndef __MEM_PAGEDIR_H
#define __MEM_PAGEDIR_H

#include <stdint.h>

class PageDir
{
    public:
	PageDir();
	PageDir(uint32_t * i_pageDirAddr, 
		uint32_t * i_pageDirVirt, 
		uint32_t * i_pageDirVirtAddr);
	
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
	uint32_t findFreePageTableAddress(bool i_high = false);

	uint32_t readPageFlags(uint32_t i_pageAddress);
	void writePageFlags(uint32_t i_pageAddress, uint32_t i_flags);
	bool checkPageFlags(uint32_t i_pageAddress, uint32_t i_flags)
	    {
		if (i_flags & this->readPageFlags(i_pageAddress))
		    return true;
		return false;
	    };

	uint32_t readPageTableFlags(uint32_t i_idx);
	void writePageTableFlags(uint32_t i_idx, uint32_t i_flags);
	bool checkPageTableFlags(uint32_t i_idx, uint32_t i_flags)
	    {
		if (i_flags & this->readPageTableFlags(i_idx))
		    return true;
		return false;
	    }


	enum PageFlags
	{
	    PRESENT = 		0x001,
	    WRITE = 		0x002,
	    SUPERVISOR = 	0x004,
	    WRITE_THROUGH =	0x008,
	    CACHE_DISABLE = 	0x010,
	    ACCESSED = 		0x020,
	    DIRTY = 		0x040,
	    PAGESIZE = 		0x080,
	    GLOBAL = 		0x100,
	    USER1 = 		0x200,
	    USER2 =		0x400,
	    USER3 =		0x800,
	};
    
    void mapPage(uint32_t i_virtAddr, uint32_t i_physAddr);
    void mapPageTable(uint32_t i_idx, uint32_t i_virtAddr, uint32_t i_physAddr);
	
	
    private:
	uint32_t * cv_pageDir;
	uint32_t * cv_pageDirVirt;
	uint32_t * cv_virtualPageDir;
};

extern PageDir g_kernelPageDirectory;


#endif
