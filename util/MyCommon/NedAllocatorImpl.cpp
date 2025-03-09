#include "NedAllocatorImpl.h"

void* allocBytes(size_t count)
{
    void* ptr = nullptr;

    ptr = malloc(count);
	return ptr;
}

void deallocBytes(void* ptr)
{
	if(!ptr)
		return;

    free(ptr);
}

void* allocBytesAligned(size_t align, size_t count)
{
	//const size_t SIMD_ALIGNMENT = 16;
    void* ptr =  nullptr;

#if ENALBE_NEDALLOC == 1
    ptr = align ? nedalloc::nedmemalign(align, count)
               : nedalloc::nedmemalign(SIMD_ALIGNMENT, count);
#endif

	return ptr;
}

void deallocBytesAligned(size_t align, void* ptr)
{
	if (!ptr)
		return;
    free(ptr);
}

void* MemoryMalloc(size_t size)
{
    void* ptr = nullptr;

    ptr = malloc(size);

    return ptr;
 }

void MemoryFree( void* ptr )
{
    if(!ptr)
        return;

    free(ptr);
}

void *Memorycalloc( size_t num,size_t size )
{
    void* ptr = nullptr;

    ptr = calloc(num,size);
    return ptr;
}

void *Memoryrealloc( void* memblock, size_t size )
{
    if(!memblock)
        return nullptr;
    void* ptr = nullptr;

    ptr = realloc(memblock,size);
    return ptr;
}
