/* kernel.c */

//#include "multibootstructs.h"

#include <mem/init.h>
#include <display/textStream.h>
#include <boot/multiBoot.h>
#include <mem/allocator.h>

#include <sys/gdt.h>

extern "C"
void KernelMain (void * i_bootHeader)
{
    initializePaging();

    //patch up address.
    asm volatile("ljmp $0x08, $main_patch\nmain_patch:" : :);
    
    //do static constructors.
    extern void (* __CTOR_LIST__)();
    void (** l_ctor)() = &__CTOR_LIST__;
    int l_ctorCount = *(int *)l_ctor;
    l_ctor++;
    while(l_ctorCount)
    {
	(*l_ctor)();
	l_ctorCount--;
	l_ctor++;
    }
    //end static constructors. 
    kout.clear();

    kout << endl << "Dante -- Inferno Kernel v0.1" << endl;
    kout << "\tKernelMain loaded at " << (unsigned int) &KernelMain << ".";
    kout << endl << endl;
    
    kout << "GDT " << (unsigned int) g_gdt << endl;
    for (int i = 0; i < GDT_DESC_SIZE; i++)
	kout << ((uint32_t *) &g_gdt[i])[0] << ((uint32_t *) &g_gdt[i])[1] << endl;
    kout << endl;

    g_multiBoot = MultiBootParser(i_bootHeader);

    kout << endl << "Loading...";
     
    kout << "|";
    while (1)
    {
#define SLEEP_A_BIT for(int i = 0; i < 1000000; i++)
	kout << "\b/";
	SLEEP_A_BIT;
	kout << "\b-";
	SLEEP_A_BIT;
	kout << "\b\\";
	SLEEP_A_BIT;
	kout << "\b|";
	SLEEP_A_BIT;
    }
}
