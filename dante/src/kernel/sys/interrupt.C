#include <stdint.h>

#include <sys/interrupt.h>
#include <display/textStream.h>
#include <sys/gdt.h>
#include <sys/task.h>

uint32_t g_idt[512] __attribute__((__aligned__(4096)));

InterruptHandler g_defaultInterruptHandler;
InterruptHandler * g_interruptHandlers[256];

extern "C" void interrupt_handler(uint32_t i_int, uint32_t i_value) 
{
    //kout << "Interrupt " << i_int << ": " << i_value << endl;
    g_interruptHandlers[i_int]->handle(i_int, i_value);
}

void InterruptHandler::handle(int i_interrupt, int i_value)
{
    kout << "Un-handled interrupt occured. " << i_interrupt << ": " 
	 << i_value << endl;
}

#define INTERRUPT_WITH_ERROR(number) \
    extern "C" void __interrupt_##number (); \
    asm (".global __interrupt_"#number); \
    asm (".text"); \
    asm ("__interrupt_"#number":"); \
    asm ("	pushl $"#number); \
    asm ("	jmp __interrupt_tail");
 

#define INTERRUPT_WITHOUT_ERROR(number) \
    extern "C" void __interrupt_##number (); \
    asm (".global __interrupt_"#number); \
    asm (".text"); \
    asm ("__interrupt_"#number":"); \
    asm ("	pushl $0xBADF00D"); \
    asm ("	pushl $"#number); \
    asm ("	jmp __interrupt_tail");

INTERRUPT_WITHOUT_ERROR(0);
INTERRUPT_WITHOUT_ERROR(1);
INTERRUPT_WITHOUT_ERROR(2);
INTERRUPT_WITHOUT_ERROR(3);
INTERRUPT_WITHOUT_ERROR(4);
INTERRUPT_WITHOUT_ERROR(5);
INTERRUPT_WITHOUT_ERROR(6);
INTERRUPT_WITHOUT_ERROR(7);
INTERRUPT_WITHOUT_ERROR(8); // Why is this without? Seem to get spurious.
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

asm (".global __interrupt_tail");
asm (".text");
asm ("__interrupt_tail:");
asm ("pusha");
asm ("pushl 0x24(%esp)");
asm ("pushl 0x24(%esp)");
asm ("call interrupt_handler");
asm ("add $0x8, %esp");
asm ("popa");
asm ("	add $0x8, %esp");
asm ("	iret");


#define INTERRUPT_DESCRIPTOR_ENTRY(index, address) \
    g_idt[index*2] = (((uint32_t) address) & 0xffff) | 0x080000; \
    g_idt[index*2+1] = 0x8E00 | (((uint32_t) address) & 0xffff0000)

void initializeInterrupts()
{
    INTERRUPT_DESCRIPTOR_ENTRY(0, __interrupt_0);
    for (int i = 0; i < 256; i++)
    {
	g_idt[i] = g_idt[0];
	g_idt[i+1] = g_idt[1];
	g_interruptHandlers[i] = &g_defaultInterruptHandler;
    }
    
    INTERRUPT_DESCRIPTOR_ENTRY(0, __interrupt_0);
    INTERRUPT_DESCRIPTOR_ENTRY(1, __interrupt_1);
    INTERRUPT_DESCRIPTOR_ENTRY(2, __interrupt_2);
    INTERRUPT_DESCRIPTOR_ENTRY(3, __interrupt_3);
    INTERRUPT_DESCRIPTOR_ENTRY(4, __interrupt_4);
    INTERRUPT_DESCRIPTOR_ENTRY(5, __interrupt_5);
    INTERRUPT_DESCRIPTOR_ENTRY(6, __interrupt_6);
    INTERRUPT_DESCRIPTOR_ENTRY(7, __interrupt_7);
    INTERRUPT_DESCRIPTOR_ENTRY(8, __interrupt_8);
    INTERRUPT_DESCRIPTOR_ENTRY(9, __interrupt_9);
    INTERRUPT_DESCRIPTOR_ENTRY(10, __interrupt_10);
    INTERRUPT_DESCRIPTOR_ENTRY(11, __interrupt_11);
    INTERRUPT_DESCRIPTOR_ENTRY(12, __interrupt_12);
    INTERRUPT_DESCRIPTOR_ENTRY(13, __interrupt_13);
    INTERRUPT_DESCRIPTOR_ENTRY(14, __interrupt_14);
    INTERRUPT_DESCRIPTOR_ENTRY(16, __interrupt_16);
    INTERRUPT_DESCRIPTOR_ENTRY(17, __interrupt_17);
    INTERRUPT_DESCRIPTOR_ENTRY(18, __interrupt_18);
    INTERRUPT_DESCRIPTOR_ENTRY(19, __interrupt_19);

    
    GDTPtr l_tablePtr = { 512*4, (GDTDescriptor *) g_idt };
    asm volatile("lidt %0" : : "m" (l_tablePtr));
    
    for (int i = 0; i < sizeof(TSSDescriptor); i++)
	((uint8_t *) &g_initialTSS)[i] = 0;

    register uint32_t reg_a; 
    g_initialTSS.cv_ss0 = 0x0010;
    asm volatile("mov %%esp, %0" : "=r" (reg_a)); 
    g_initialTSS.cv_esp0 = reg_a;
    asm volatile("mov %%cr3, %0" : "=r" (reg_a));
    g_initialTSS.cv_cr3 = reg_a;
    
    // Load initial TSS. 
    asm volatile("mov $0x28, %%eax; ltr %%ax" : : : "eax");
    asm volatile("loaded_tss:");
    
    // Turn on interrupts now.
    asm volatile("sti");
    
}
    
