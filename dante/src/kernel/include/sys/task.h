#ifndef __SYS_TASK_H
#define __SYS_TASK_H

#include <stdint.h>

#include <sys/cpu.h>

struct TaskDescriptor
{
    CPUDescriptor * cv_currentCPU;

    // registers..
    uint32_t	eflags, cr3;
    uint32_t	eax, ecx, edx, ebx;
    uint32_t	esp, ebp, esi, edi;
     
} __attribute__((__packed__));

void initializeTasking();

#endif
