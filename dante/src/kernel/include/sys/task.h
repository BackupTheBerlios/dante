#ifndef __SYS_TASK_H
#define __SYS_TASK_H

#include <stdint.h>

#include <sys/cpu.h>

struct TaskDescriptor
{
    CPUDescriptor * cv_currentCPU;

    // registers..
    uint32_t	cr3, esp, eflags;
    uint32_t	eax, ebx, ecx, edx;
    uint32_t	ebp, esi, edi;
     
} __attribute__((__packed__));

void initializeTasking();

#endif
