#ifndef __SYS_SCHED_H 
#define __SYS_SCHED_H

#include <sys/task.h>
#include <lib/Queue.h>
#include <sys/interrupt.h>

class Scheduler : public InterruptHandler
{
    public:
	void handle(int i_interrupt, int i_value);
	
	void addTask(TaskDescriptor * i_task);

    private:
	Queue<TaskDescriptor *> cv_schedQueue;
	
};

extern Scheduler g_scheduler; 

#endif
