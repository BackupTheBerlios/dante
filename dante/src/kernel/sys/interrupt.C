#include <stdint.h>

#include <sys/interrupt.h>
#include <display/textStream.h>
#include <sys/gdt.h>

uint32_t g_idt[512] __attribute__((__aligned__(4096)));

extern "C" void interrupt_handler() 
{
    kout << "Interrupt!" << endl;
}

#define INTERRUPT_WITH_ERROR(number) \
    extern "C" void __interrupt_##number (); \
    asm(".global __interrupt_"#number); \
    asm("__interrupt_"#number":"); \
    asm("	pushl $"#number); \
    asm("	jmp __interrupt_tail");
 

#define INTERRUPT_WITHOUT_ERROR(number) \
    extern "C" void __interrupt_##number (); \
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


#define INTERRUPT_DESCRIPTOR_ENTRY(index, address) \
    g_idt[index*2] = (((uint32_t) address) & 0xffff) | 0x100000; \
    g_idt[index*2+1] = 0x8E00 | (((uint32_t) address) & 0xffff0000)

void initializeInterrupts()
{
    for (int i = 0; i < 512; i++)
	g_idt[i] = 0;
    
    INTERRUPT_DESCRIPTOR_ENTRY(0, __interrupt_0);
    
    GDTPtr l_tablePtr = { 512*4, (GDTDescriptor *) g_idt };
    asm volatile("lidt %0" : : "m" (l_tablePtr));

}
    
