#ifndef __MEM_ALLOCATOR_H
#define __MEM_ALLOCATOR_H

#include <stdint.h>

class Allocator
{
    public:
	Allocator(Allocator * i_oldAllocator) = 0;
	
	virtual void * allocate(uint32_t i_size) = 0;
	virtual void release(void * i_ptr) = 0;

	virtual void * alloc_aligned(uint32_t i_size, uint32_t i_align) = 0;
	virtual void * alloc_page(uint32_t i_count) = 0;

}

#endif
