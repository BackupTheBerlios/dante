#include <stdint.h>

#include <sys/interrupt.h>
#include <sys/task.h>
#include <sys/cpu.h>
#include <mem/init.h>
#include <sys/sched.h>

TaskDescriptor g_initialTask; // idle task after initialization.
CPUDescriptor g_initialCPU;   // cpu 0.

//static uint8_t g_kernelStack[8192] __attribute__((__aligned__(4096)));

TSSDescriptor g_initialTSS;

void initializeTasking()
{
    g_initialTask.cv_stack = 0;
    g_initialTask.cv_pageDir = &g_kernelPageDirectory;
    g_initialTask.cv_currentCPU = &g_initialCPU;
    
    g_initialCPU.cv_currentTask = &g_initialTask;
    g_initialCPU.cv_cpuID = 0;

    register uint32_t reg_a = (uint32_t) &g_initialCPU;
    asm volatile("mov %0, %%dr3" : : "r" (reg_a));

}

// Stuff for idle thread.  Need to make more standardized so we can create
// other tasks easily.
uint8_t g_idleStack[8192] __attribute__((__aligned__(4096)));
TaskDescriptor g_idleTask;

extern "C" void __begin_idle_thread();
void beginIdleThread()
{
    register uint8_t * l_oldStack; 
    register uint8_t * l_testStack = &g_idleStack[8188];
    asm volatile("mov %%esp, %0" : "=r" (l_oldStack));
    asm volatile("mov %0, %%esp" : : "r" (l_testStack));
    asm volatile("pushfl");
    asm volatile("push %cs");
    asm volatile("push %0" : : "a" (__begin_idle_thread));
    asm volatile("mov %0, %%esp" : : "r" (l_oldStack));
    
    g_idleTask.cv_stack = &g_idleStack[8188-12];
    g_idleTask.cv_pageDir = &g_kernelPageDirectory;
    g_idleTask.cv_runtime = 0;
    g_idleTask.cv_currentCPU = (CPUDescriptor *) NULL;
    g_scheduler.addTask(&g_idleTask);
    
    asm volatile("mov %0, %%eax\nint $42" : : "a" (&g_idleStack[8188-12]));
}

asm(".global __begin_idle_thread");
asm(".text");
asm("__begin_idle_thread:");
asm("	call idleThread");
asm("	jmp __begin_idle_thread");

extern "C" void idleThread()
{
    while(1)
    {
	kout << "Idle." << endl;
	asm volatile("hlt");
	asm volatile("int $42");
    }
}
