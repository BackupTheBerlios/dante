#include <stdint.h>
#include <sys/timer.h>
#include <sys/io.h>

#include <display/textStream.h>

void programPIT(int i_timer, int i_timeHz)
{
    uint8_t l_register = 0x36 | (i_timer << 5);
    outb(0x43, l_register);
    asm volatile("nop; nop;");
    outb((0x40 + i_timer), (1193180/i_timeHz) & 0x00FF);
    outb((0x40 + i_timer), ((1193180/i_timeHz) >> 8) & 0x00FF);
};

Timer_InterruptHandler g_globalTimer;

void Timer_InterruptHandler::handle(int i_interrupt, int i_value)
{
   cv_ticks++;
    
   if (0 == (cv_ticks % 0x20))
       kout << "Time: " << (cv_ticks * 5) / 91 << endl;
   
   outb(0x20, 0x20); // clear interrupt.

}; 
