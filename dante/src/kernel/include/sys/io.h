#ifndef __SYS_IO_H
#define __SYS_IO_H

#define outb(port,value)\
	asm volatile("outb %%al,%%dx": :"d" (port), "a" (value))


#endif
