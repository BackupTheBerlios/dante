#include <stdint.h>
#include <mem/allocator.h>

void * __dso_handle = (void *) &__dso_handle;
extern "C" int __cxa_atexit ( void (*f)(void *), void *p, void *d )
{
    // figure out what this should do...for kernel it is probably not needed.
    return 0;
}
extern "C" void __cxa_pure_virtual() { while(1); }

void * operator new(uint32_t i_size)
{
    if (NULL != g_Allocator)
	return g_Allocator->allocate(i_size);
    return NULL;
}

void * operator new[](uint32_t i_size)
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

void operator delete[](void * i_ptr)
{
    if (NULL != g_Allocator)
	g_Allocator->release(i_ptr);
}
