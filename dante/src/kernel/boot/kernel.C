/* kernel.c */

//#include "multibootstructs.h"

#include <display/textStream.h>
#include <boot/multiBoot.h>

extern "C"
void KernelMain (void * i_bootHeader)
{
    //do static constructors.
    kout = TextStream();
    //end static constructors.
    
    kout.clear();

    kout << "Dante Kernel v0.1" << endl;
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
