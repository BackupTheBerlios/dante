#include <mem/pageDir.h>
#include <mem/init.h>
#include <stdint.h>

#include <display/textStream.h>

PageDir g_kernelPageDirectory(g_pageDirectory, g_virtualPageDirectory);

PageDir::PageDir()
{
    // allocate memory for page table from kernel table.
};

PageDir::PageDir(const PageDir&)
{
    // allocate memory for page table.

    // copy from kernel page table.
};

PageDir::PageDir(uint32_t * i_pageDirAddr, uint32_t * i_pageDirVirtAddr)
{
    cv_pageDir = i_pageDirAddr;
    cv_virtualPageDir = i_pageDirVirtAddr;

};

PageDir::~PageDir()
{
    // need to return this page table's memory chunk to the kernel.
};

void PageDir::info()
{
   kout << "Page directory at: " << (uint32_t)cv_pageDir << "(" 
	 << (uint32_t)cv_virtualPageDir << ")" << endl;
}

uint32_t PageDir::findFreeAddress(bool i_high)
{
    uint32_t i_min = (false == i_high ? 0 : 768);
    uint32_t i_max = (false == i_high ? 768 : 1024);
    
    for (int i = i_min; i < i_max; i++)
    {
	if(cv_virtualPageDir[i] != 0)
	{
	    // Comparing with i_min prevents usage of page 0,
	    // kernel uses page table 0 and page table 768 as the same.
	    for (int j = (i_min == i ? 1 : 0); j < 1024; j++)
		if (((uint32_t *)(cv_virtualPageDir[i]&0xFFFFFF00))[j] == 0)
		    return (i << 10 | j) << 12;
	}
    }

    return 0;
}

