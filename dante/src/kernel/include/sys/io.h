#ifndef __SYS_IO_H
#define __SYS_IO_H

#define inb(port,var) \
	asm volatile("inb %%dx, %%al" : "=a" (var) : "d" (port))

#define outb(port,value)\
	asm volatile("outb %%al,%%dx": :"d" (port), "a" (value))



#endif
