#ifndef __MEM_INIT_H
#define __MEM_INIT_H

#include <stdint.h>

extern "C" uint32_t * g_pageDirectory;

void initializePaging();

#endif

