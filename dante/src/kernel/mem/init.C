#include <mem/init.h>
#include <stdint.h>
#include <sys/gdt.h>

#include <display/textStream.h>
uint32_t g_chunkOfMem[1024*3] __attribute__((__aligned__(4096)));
uint32_t * g_pageDirectory;
uint32_t * g_idt;
GDTDescriptor g_gdt[6];

void initializePaging()
{
#define LOWPTR(x) ((uint32_t *) (((uint32_t) (x)) ^ 0xC0000000))
//#define LOWPTR(x) ((uint32_t *) (x)) 
    uint32_t * l_pageTable;
    l_pageTable = LOWPTR(&g_chunkOfMem[0]);
    LOWPTR(&g_pageDirectory)[0]
		= (uint32_t) (((uint32_t)(l_pageTable)) + 0x1000);
    LOWPTR(&g_idt)[0] = (uint32_t) (((uint32_t)(l_pageTable)) + 0x2000);
    
    l_pageTable[0] = 0 | 02; // Don't want to access page 0. (NULL)
    // set up rest of first 4MB.
    for (int i = 0; i < 1024; i++)
    {
	l_pageTable[i] = (uint32_t)((i * 4096) | 03);
    }
    
    for (int i = 0; i < 1024; i++) 
	(*(uint32_t **)LOWPTR(&g_pageDirectory))[i] = 0 | 02;
    (*(uint32_t **)LOWPTR(&g_pageDirectory))[0x0] = ((uint32_t)(l_pageTable)) | 03;
    (*(uint32_t **)LOWPTR(&g_pageDirectory))[0x300] = ((uint32_t)(l_pageTable)) | 03;
     
    // start using page directory.
    register uint32_t reg_a = (uint32_t)(*LOWPTR(&g_pageDirectory));
    asm("mov %0, %%cr3" : :"r" (reg_a));
    asm("mov %%cr0, %0" : "=r" (reg_a) :);
    reg_a |= 0x80000000;
    asm("mov %0, %%cr0" : :"r" (reg_a));
    
    // clear IDT.
    for(int i = 0; i < 512; i++)
	g_idt[i] = 0;
     
    return;
}
