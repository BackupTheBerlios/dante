#include <mem/pageDir.h>
#include <mem/init.h>
#include <stdint.h>

#include <display/textStream.h>
#include <sys/interrupt.h>

PageDir g_kernelPageDirectory(g_pageDirectory, g_pageDirVirt, g_virtualPageDirectory);

PageDir::PageDir()
{
    // allocate memory for page table from kernel table.
};

PageDir::PageDir(const PageDir&)
{
    // allocate memory for page table.

    // copy from kernel page table.
};

PageDir::PageDir(uint32_t * i_pageDirAddr, 
		 uint32_t * i_pageDirVirt, 
		 uint32_t * i_pageDirVirtAddr)
{
    cv_pageDir = i_pageDirAddr;
    cv_pageDirVirt = i_pageDirVirt;
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

uint32_t PageDir::readPageFlags(uint32_t i_pageAddress)
{
    uint32_t l_indexI, l_indexJ;

    l_indexI = i_pageAddress >> 22;
    l_indexJ = i_pageAddress >> 12 & 0x3FF;

    uint32_t * l_pageTable = (uint32_t *)(cv_virtualPageDir[l_indexI]);

    if (NULL == l_pageTable)
    {
	// need to map it in.
	kpanic("Page table is not mapped into virtual memory.");
	return 0; // for now.
    }
    else
    {
	return l_pageTable[l_indexJ] & 0x0FFF;
    }
};

void PageDir::writePageFlags(uint32_t i_pageAddress, uint32_t i_flags)
{
    uint32_t l_indexI, l_indexJ;

    l_indexI = i_pageAddress >> 22;
    l_indexJ = i_pageAddress >> 12 & 0x3FF;

    uint32_t * l_pageTable = (uint32_t *)(cv_virtualPageDir[l_indexI]);

    if (NULL == l_pageTable)
    {
	// need to map it in.
	kpanic("Page table is not mapped into virtual memory.");
    }
    else
    {
	l_pageTable[l_indexJ] = (l_pageTable[l_indexJ] & 0xFFFFF000) 
			      | (i_flags & 0x0FFF);
    }
    return;
};

uint32_t PageDir::readPageTableFlags(uint32_t i_idx)
{
    return cv_virtualPageDir[i_idx] & 0x0FFF;
}

void PageDir::writePageTableFlags(uint32_t i_idx, uint32_t i_flags)
{
    if (cv_pageDirVirt == NULL)
    {
	// map in page directory.
	kpanic("Page directory is not mapped into virtual memory");
    }
    
    cv_pageDirVirt[i_idx] = i_flags
			  | (cv_pageDirVirt[i_idx] & 0xFFFFF000);
    cv_virtualPageDir[i_idx] = i_flags 
			     | (cv_virtualPageDir[i_idx] & 0xFFFFF000);

    return;
}

void PageDir::mapPage(uint32_t i_virtAddr, uint32_t i_physAddr)
{
    uint32_t l_indexI, l_indexJ;

    l_indexI = i_virtAddr >> 22;
    l_indexJ = i_virtAddr >> 12 & 0x3FF;

    i_physAddr = i_physAddr & 0xFFFFF000;

    uint32_t * l_pageTable = (uint32_t *)(cv_virtualPageDir[l_indexI]);

    if (NULL == l_pageTable)
    {
	// need to map it in.
	kpanic("Page table is not mapped into virtual memory");
    }
    else
    {
	l_pageTable[l_indexJ] = i_physAddr | (l_pageTable[l_indexJ] & 0x0FFF);
    }
    return;
}

void PageDir::mapPageTable(uint32_t i_idx, 
			   uint32_t i_virtAddr, 
			   uint32_t i_physAddr)
{
    if (!cv_pageDirVirt)
    {
	// map in page table.
	kpanic("Page directory not mapped into virtual memory.");
    }

    cv_pageDirVirt[i_idx] = i_physAddr | (cv_pageDirVirt[i_idx] & 0x0FFF);
    cv_virtualPageDir[i_idx] = i_virtAddr | (cv_pageDirVirt[i_idx] & 0x0FFF);

}
