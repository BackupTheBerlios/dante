#ifndef __SYS_CPU_H
#define __SYS_CPU_H

#include <stdint.h>

struct TaskDescriptor;

struct CPUDescriptor
{
    void * cv_kernelStack;
    uint32_t cv_kernelPageDir; // cr3.
    TaskDescriptor * cv_currentTask;
    
    uint32_t cv_cpuID;
    
    // need some bucket for tasks.
} __attribute__((__packed__));

#endif
