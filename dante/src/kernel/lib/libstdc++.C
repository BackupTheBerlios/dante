#include <stdint.h>
#include <mem/allocator.h>

extern "C" void __cxa_pure_virtual() { while(1); }

void * operator new(uint32_t i_size)
{
    if (NULL != g_Allocator)
	return g_Allocator->allocate(i_size);
    return NULL;
}

void operator delete(void * i_ptr)
{
    if (NULL != g_Allocator)
	g_Allocator->release(i_ptr);
}
