#ifndef __SYS_INTERRUPT_H
#define __SYS_INTERRUPT_H

#include <stdint.h>

extern uint32_t g_idt[512];

void initializeInterrupts();

#endif
