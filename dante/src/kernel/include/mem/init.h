#ifndef __MEM_INIT_H
#define __MEM_INIT_H

#include <stdint.h>
#include <lib/RLEQueue.h>

extern "C" uint32_t * g_pageDirectory; // page directory containing real-addrs
extern "C" uint32_t * g_pageDirVirt; // virt address of real page directory.
extern "C" uint32_t * g_virtualPageDirectory; // page directory containing
					      // virtual addrs of tables.
extern RLEQueue g_freePageList;

void initializePaging();

#endif

