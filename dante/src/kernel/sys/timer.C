#include <stdint.h>
#include <sys/timer.h>
#include <sys/io.h>

#include <display/textStream.h>

Timer_InterruptHandler g_globalTimer;

void Timer_InterruptHandler::handle(int i_interrupt, int i_value)
{
   cv_ticks++;
    
   if (0 == (cv_ticks % 0x100))
       kout << "Time: " << cv_ticks << endl;
   
   outb(0x20, 0x20); // clear interrupt.

}; 
