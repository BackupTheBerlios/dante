/* kernel.c */

//#include "multibootstructs.h"

#include <mem/init.h>
#include <mem/pageDir.h>
#include <display/textStream.h>
#include <boot/multiBoot.h>
#include <mem/allocator.h>

#include <sys/gdt.h>
#include <sys/task.h>
#include <sys/interrupt.h>

#include <sys/io.h>

extern "C"
void KernelMain (void * i_bootHeader)
{
    initializePaging();

    register uint32_t reg_a;
    
    //patch up address.
    asm volatile("ljmp $0x08, $main_patch\nmain_patch:" : :);
    asm volatile("mov %%esp, %0" : "=r" (reg_a));
    reg_a |= 0xC0000000;
    asm volatile("mov %0, %%esp" : : "r" (reg_a));
    asm volatile("mov %%ebp, %0" : "=r" (reg_a));
    reg_a |= 0xC0000000;
    asm volatile("mov %0, %%ebp" : : "r" (reg_a));

    
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
    
    initializePIC(); // GRUB maps these to a low interrupt, map high before
		     // enabling interrupts.
    initializeTasking();
    initializeInterrupts();
    initializePIC(); // Re-initialize in case we lost the timer.
    
    kout.clear();
   
    extern uint32_t __KERNEL_END__;
    
    kout << endl << "Dante -- Inferno Kernel v0.1" << endl;
    kout << "\tKernelMain loaded at " << (uint32_t) &KernelMain << "." << endl;
    kout << "\tKernel end at " << (uint32_t) &__KERNEL_END__ << "." << endl;
    kout << endl << endl;
     
    g_multiBoot = MultiBootParser(i_bootHeader);
    
    g_kernelPageDirectory.info();

    kout << endl << "Loading...";
    beginIdleThread();
    
    kout << "|";
    while (1)
    {
#define SLEEP_A_BIT asm volatile("int $42")//for(int i = 0; i < 1000000; i++)
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
