#ifndef __MEM_INIT_H
#define __MEM_INIT_H

#include <stdint.h>
#include <lib/RLEQueue.h>

extern "C" uint32_t * g_pageDirectory;
extern RLEQueue g_freePageList;

void initializePaging();

#endif

