#include "NedAllocatedObject.h"
#include "NedAllocatorImpl.h"

void* NedAllocatedObject::operator new(size_t sz)
{
	return allocBytes(sz);
}

/// placement operator new
void* NedAllocatedObject::operator new(size_t sz, void* ptr)
{
	return ptr;
}

void* NedAllocatedObject::operator new[] ( size_t sz )
{
	return allocBytes(sz);
}

void NedAllocatedObject::operator delete( void* ptr )
{
	deallocBytes(ptr);
}

void NedAllocatedObject::operator delete( void* ptr, void* )
{
	deallocBytes(ptr);
}

void NedAllocatedObject::operator delete[] ( void* ptr )
{
	deallocBytes(ptr);
}
