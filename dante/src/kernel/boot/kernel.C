/* kernel.c */

//#include "multibootstructs.h"

#include <display/textStream.h>
#include <boot/multiBoot.h>
#include <mem/allocator.h>

extern "C"
void KernelMain (void * i_bootHeader)
{

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
