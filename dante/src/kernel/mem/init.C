#include <mem/init.h>
#include <stdint.h>
#include <sys/gdt.h>
#include <sys/task.h>
#include <lib/RLEQueue.h>
#include <lib/Algo.h>

RLEQueue g_freePageList;

static uint32_t g_chunkOfMem[1024*3] __attribute__((__aligned__(4096)));
uint32_t * g_pageDirectory;
uint32_t * g_pageDirVirt;
uint32_t * g_virtualPageDirectory;

GDTDescriptor g_gdt[GDT_DESC_SIZE] = { 
			    MakeDescriptor(0,0,0), //NULL descriptor
			    MakeDescriptor(0x0,0xFFFFF, attr_Segment | attr_Gran_4k | attr_Big_Addr | attr_Present | attr_Code | attr_Code_Read), //Code descriptor - RNG0
			    MakeDescriptor(0x0,0xFFFFF, attr_Segment | attr_Gran_4k | attr_Big_Addr | attr_Present | attr_Data_Write), //Data descriptor - RNG0
			    MakeDescriptor(0,0,0),
			    MakeDescriptor(0,0,0),
			    MakeDescriptor(0,0,0)
			};

void initializePaging()
{
#define LOWPTR(x) ((uint32_t *) (((uint32_t) (x)) ^ 0xC0000000))
    extern uint32_t __KERNEL_END__;
    uint32_t * l_pageTable;
    l_pageTable = LOWPTR(&g_chunkOfMem[0]);
    LOWPTR(&g_pageDirectory)[0]
		= (uint32_t)(l_pageTable) + 0x1000;
    LOWPTR(&g_virtualPageDirectory)[0]
		= 0x2000 + (uint32_t)(l_pageTable); 
    l_pageTable[0] = 0 | 02; // Don't want to access page 0. (NULL)
    // map in kernel and bottom 1mg. (max 4mg)
    uint32_t l_topKernel = align<4096>((uint32_t)LOWPTR(&__KERNEL_END__)) >> 12;
    for (int i = 0; i < l_topKernel; i++)
    {
	l_pageTable[i] = (uint32_t)((i * 4096) | 03);
    }
    
    for (int i = 0; i < 1024; i++) 
    {
	(*(uint32_t **)LOWPTR(&g_pageDirectory))[i] = 0 | 02;
	(*(uint32_t **)LOWPTR(&g_virtualPageDirectory))[i] = 0;
    }
    (*(uint32_t **)LOWPTR(&g_pageDirectory))[0x0] = ((uint32_t)(l_pageTable)) | 03;
    (*(uint32_t **)LOWPTR(&g_virtualPageDirectory))[0x0] = (uint32_t)g_chunkOfMem;
    (*(uint32_t **)LOWPTR(&g_pageDirectory))[0x300] = ((uint32_t)(l_pageTable)) | 03;
    (*(uint32_t **)LOWPTR(&g_virtualPageDirectory))[0x300] = (uint32_t)g_chunkOfMem;
    
    // We want the virtual page pointer to be a virtual address, so fix.
    LOWPTR(&g_virtualPageDirectory)[0]
		= 0x2000 + (uint32_t)(g_chunkOfMem); 
    LOWPTR(&g_pageDirVirt)[0] = 0x1000 + (uint32_t)(g_chunkOfMem);
     
    // start using page directory.
    register uint32_t reg_a = (uint32_t)(*LOWPTR(&g_pageDirectory));
    asm volatile("mov %0, %%cr3" : :"r" (reg_a));
    asm volatile("mov %%cr0, %0" : "=r" (reg_a) :);
    reg_a |= 0x80000000;
    asm volatile("mov %0, %%cr0" : :"r" (reg_a));
    
    // load GDT 
    g_gdt[5] = (GDTDescriptor) MakeDescriptor(((uint32_t)&g_initialTSS), sizeof(TSSDescriptor), attr_TSS | attr_Present | 0x100);
    GDTPtr l_tablePtr = { GDT_DESC_SIZE*8 , g_gdt };
    asm volatile("lgdt %0" : : "m" (l_tablePtr));
    asm volatile("ljmp $0x08, $cs_finalize\n"
    		 "cs_finalize:"
    		 : : );
    
    // load SS, DS, ES, FS, GS.
    reg_a = 0x10; 
    asm volatile("mov %0, %%ss": : "r" (reg_a));
    asm volatile("mov %0, %%ds": : "r" (reg_a));
    asm volatile("mov %0, %%es": : "r" (reg_a));
    asm volatile("mov %0, %%fs": : "r" (reg_a));
    asm volatile("mov %0, %%gs": : "r" (reg_a)); 
    
    return;
}
