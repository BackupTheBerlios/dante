#ifndef __SYS_INTERRUPT_H
#define __SYS_INTERRUPT_H

#include <stdint.h>

extern uint32_t g_idt[512];

void initializeInterrupts();
void initializePIC();

class InterruptHandler
{
    public:
	virtual void handle(int i_interrupt, int i_value);
};

extern InterruptHandler * g_interruptHandlers[256];

#define kpanic(string) \
    kout << __FILE__ << __LINE__ << ": " << string << endl; \
    asm volatile("cli"); \
    while(1) asm volatile("hlt")
    

#endif
