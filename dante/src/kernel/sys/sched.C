#include <sys/sched.h>
#include <display/textStream.h>
#include <sys/io.h>

Scheduler g_scheduler;

extern uint8_t * g_idleStack;

void Scheduler::handle(int i_interrupt, int i_value)
{
	register void * l_newStack;
	
	asm volatile("mov %%esp, %0" : "=r" (l_newStack));
	CURRENT_CPU->cv_currentTask->cv_stack = l_newStack;
	asm volatile("mov %%ebp, %0" : "=r" (l_newStack));
	CURRENT_CPU->cv_currentTask->cv_frameptr = l_newStack;
	CURRENT_CPU->cv_currentTask->cv_runtime++;
	
	cv_schedQueue.push(CURRENT_CPU->cv_currentTask);
	CURRENT_CPU->cv_currentTask = cv_schedQueue.pop();
	
	l_newStack = CURRENT_CPU->cv_currentTask->cv_stack;	
	if (0 == CURRENT_CPU->cv_currentTask->cv_runtime)
	{
	    asm volatile("mov %0, %%esp" : : "r" (l_newStack));
	    if (0x30 == i_interrupt)
		outb(0x20, 0x20);
	    asm volatile("iret");
	}
	else
	{
	    asm volatile("mov %0, %%esp" : : "r" (l_newStack));
	}
	l_newStack = CURRENT_CPU->cv_currentTask->cv_frameptr;
	asm volatile("mov %0, %%ebp" : : "r" (l_newStack));
	if (0x30 == i_interrupt)
	    outb(0x20, 0x20);
	
};

void Scheduler::addTask(TaskDescriptor * i_task)
{
    cv_schedQueue.push(i_task);
};
