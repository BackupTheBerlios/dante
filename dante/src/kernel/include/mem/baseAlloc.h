#ifndef __MEM_BASEALLOC_H
#define __MEM_BASEALLOC_H

#include <mem/allocator.h>
#include <stdint.h>

class BaseAllocator : public Allocator
{
    public:
	BaseAllocator();
	BaseAllocator(Allocator * i_oldAllocator);
	
	virtual void * allocate(uint32_t i_size);
	virtual void release(void * i_ptr);

	virtual void * alloc_aligned(uint32_t i_size, uint32_t i_align);
	virtual void * alloc_page(uint32_t i_count);

    private:
	Allocator * cv_oldAlloc;
	static const uint32_t cv_baseSize = 1024*32;
	
	uint8_t cv_memory[cv_baseSize];
	uint8_t cv_usage[cv_baseSize/128];

	uint32_t cv_position;

	inline void initialize()
	    {
		for(int i = 0; i < (cv_baseSize/128); i++)
		    cv_usage[i] = 0;
		cv_position = 0;
	    };

};

#endif
