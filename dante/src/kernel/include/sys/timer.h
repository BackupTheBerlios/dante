#ifndef __SYS_TIMER_H
#define __SYS_TIMER_H

#include <sys/interrupt.h>
#include <stdint.h>

class Timer_InterruptHandler : public InterruptHandler
{
    public:
	Timer_InterruptHandler() : cv_ticks(0) {};

	void handle(int i_interrupt, int i_value);

    private:
	uint32_t cv_ticks;
};

#endif
