#include <stdint.h>

#include <display/textStream.h>

extern "C" void interrupt_handler() 
{
    kout << "Interrupt!" << endl;
}

#define INTERRUPT_WITH_ERROR(number) \
    asm(".global __interrupt_"#number); \
    asm("__interrupt_"#number":"); \
    asm("	pushl $"#number); \
    asm("	jmp __interrupt_tail");
 

#define INTERRUPT_WITHOUT_ERROR(number) \
    asm(".global __interrupt_"#number); \
    asm("__interrupt_"#number":"); \
    asm("	pushl $0"); \
    asm("	pushl $"#number); \
    asm("	jmp __interrupt_tail");

INTERRUPT_WITHOUT_ERROR(0);
INTERRUPT_WITHOUT_ERROR(1);
INTERRUPT_WITHOUT_ERROR(2);
INTERRUPT_WITHOUT_ERROR(3);
INTERRUPT_WITHOUT_ERROR(4);
INTERRUPT_WITHOUT_ERROR(5);
INTERRUPT_WITHOUT_ERROR(6);
INTERRUPT_WITHOUT_ERROR(7);
INTERRUPT_WITH_ERROR(8);
INTERRUPT_WITHOUT_ERROR(9);
INTERRUPT_WITH_ERROR(10);
INTERRUPT_WITH_ERROR(11);
INTERRUPT_WITH_ERROR(12);
INTERRUPT_WITH_ERROR(13);
INTERRUPT_WITH_ERROR(14);
INTERRUPT_WITHOUT_ERROR(16);
INTERRUPT_WITH_ERROR(17);
INTERRUPT_WITHOUT_ERROR(18);
INTERRUPT_WITHOUT_ERROR(19);

asm(".global __interrupt_tail");
asm("__interrupt_tail:");
asm("call interrupt_handler");
asm("	add $0x8, %esp");
asm("	iret");

