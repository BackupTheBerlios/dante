#include <mem/baseAlloc.h>
#include <stdint.h>

#include <display/textStream.h>

BaseAllocator g_baseAllocator;
Allocator * g_Allocator = &g_baseAllocator;

BaseAllocator::BaseAllocator(Allocator * i_oldAllocator)
{
    cv_oldAlloc = i_oldAllocator;
    initialize();
}

BaseAllocator::BaseAllocator()
{
    initialize();
}

void * BaseAllocator::allocate(uint32_t i_size)
{
    uint32_t l_blocks = (i_size-1)/128 + 1;
    uint32_t l_position = cv_position;

    if((cv_position + l_blocks) >= (cv_baseSize/128))
    {
	cv_position = 0; l_position = 0;
    }
    
    bool l_foundBlock;
    do
    {
	l_foundBlock = true;
	for(int i=0; i < l_blocks; i++)
	    if (0x00 != cv_usage[l_position+i])
	    {
		l_foundBlock = false;
		i = l_blocks;
	    }
	l_position++;
	if ((l_position + l_blocks) >= (cv_baseSize/128))
	    l_position = 0;
    }
    while((false == l_foundBlock) && (l_position != cv_position));

    if (false == l_foundBlock)
    {
	return NULL;
    }

    cv_position = (l_position + (cv_baseSize/128) - 1) % (cv_baseSize/128);
    for (int i = 0; i < (l_blocks-1); i++)
    {
	cv_usage[cv_position+i] = 0xFF;
    }
    cv_usage[cv_position+(l_blocks-1)] = (i_size % 128 == 0 ?
					    128
					    :
					    (i_size) % 128
					 );
    
    kout << "Allocated " << i_size << " bytes at 0x";
    kout << (uint32_t) &cv_memory[128 * cv_position] << endl;

    return &cv_memory[128 * cv_position];
}

void BaseAllocator::release(void * i_ptr)
{
    uint32_t l_pos = (((uint32_t)i_ptr) - ((uint32_t)cv_memory)) / 128;
    uint32_t l_size = 0;
    
    if ((i_ptr < (void *)cv_memory) || (i_ptr > (void *)&cv_memory[cv_baseSize]))
    {
	if (NULL == cv_oldAlloc)
	{
	    kout << "Bad ptr: " << (uint32_t) i_ptr << endl;
	}
	else
	{
	    cv_oldAlloc->release(i_ptr);
	}
	return;
    }
    
    while((cv_usage[l_pos] == 0xFF) && ((cv_baseSize/128) > l_pos))
    {
	    cv_usage[l_pos] = 0;
	    l_pos++;
	    l_size+=128;
    }
    if ((cv_baseSize/128) > l_pos)
    {
	l_size += cv_usage[l_pos];
	cv_usage[l_pos] = 0;
    }

    kout << "Freed " << l_size << " bytes at " << (uint32_t) i_ptr << endl;
    
    return;
}

void * BaseAllocator::alloc_aligned(uint32_t i_size, uint32_t i_align)
{
    return NULL;
}

void * BaseAllocator::alloc_page(uint32_t i_count)
{
    return NULL;
}

