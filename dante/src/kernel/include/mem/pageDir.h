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
	
    private:
	uint32_t * cv_pageDir;
	uint32_t * cv_virtualPageDir;
};

extern PageDir g_kernelPageDirectory;


#endif
