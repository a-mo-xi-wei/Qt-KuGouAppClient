#include "NedAllocatorImpl.h"

#if ENALBE_NEDALLOC == 1
    #include "nedmalloc.c"
#endif

void* allocBytes(size_t count)
{
    void* ptr = nullptr;

#if ENALBE_NEDALLOC == 1
    ptr = nedalloc::nedmalloc(count);
#else
    ptr = malloc(count);
#endif

	return ptr;
}

void deallocBytes(void* ptr)
{
	if(!ptr)
		return;

#if ENALBE_NEDALLOC == 1
	nedalloc::nedfree(ptr);
#else
    free(ptr);
#endif
}

void* allocBytesAligned(size_t align, size_t count)
{
	const size_t SIMD_ALIGNMENT = 16;
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

#if ENALBE_NEDALLOC == 1
    nedalloc::nedfree(ptr);
#else
    free(ptr);
#endif
}

void* MemoryMalloc(size_t size)
{
    void* ptr = nullptr;

#if ENALBE_NEDALLOC == 1
    ptr = nedalloc::nedmalloc(size);
#else
    ptr = malloc(size);
#endif

    return ptr;
 }

void MemoryFree( void* ptr )
{
    if(!ptr)
        return;

#if ENALBE_NEDALLOC == 1
    nedalloc::nedfree(ptr);
#else
    free(ptr);
#endif
}

void *Memorycalloc( size_t num,size_t size )
{
    void* ptr = nullptr;

#if ENALBE_NEDALLOC == 1
    ptr = nedalloc::nedcalloc(num,size);
#else
    ptr = calloc(num,size);
#endif

    return ptr;
}

void *Memoryrealloc( void* memblock, size_t size )
{
    if(!memblock)
        return nullptr;
    void* ptr = nullptr;

#if ENALBE_NEDALLOC == 1
    ptr = nedalloc::nedrealloc(memblock, size);
#else
    ptr = realloc(memblock,size);
#endif

    return ptr;
}

void DestroyMemory()
{
#if ENALBE_NEDALLOC == 1
    nedalloc::neddestroysyspool();
#elif ENABLE_TCMALLOC == 1

#else

#endif
}
