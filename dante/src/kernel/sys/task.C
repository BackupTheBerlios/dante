#include <stdint.h>

#include <sys/task.h>
#include <sys/cpu.h>
#include <mem/init.h>

static TaskDescriptor g_initialTask; // idle task after initialization.
static CPUDescriptor g_initialCPU;   // cpu 0.

static uint8_t g_kernelStack[8192] __attribute__((__aligned__(4096)));

void initializeTasking()
{
    g_initialCPU.cv_kernelStack = (void *) g_kernelStack;   
    g_initialCPU.cv_kernelPageDir = (uint32_t) g_pageDirectory;
    g_initialCPU.cv_currentTask = &g_initialTask;
    g_initialCPU.cv_cpuID = 0;

    g_initialTask.cv_currentCPU = &g_initialCPU;

    register uint32_t reg_a = (uint32_t) &g_initialCPU;
    asm volatile("mov %%dr3, %0" : : "r" (reg_a));
}
