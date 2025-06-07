#ifndef MEMORY_TRACKER_HPP
#define MEMORY_TRACKER_HPP

#include <iostream>
#include <cstdlib>

//---------------- Memory Tracker Class ----------------//
class MemoryTracker
{
public:
    static inline size_t allocatedMemory = 0;
    static inline size_t deallocatedMemory = 0;

    static void report()
    {
        std::cout << "Allocated Memory: " << allocatedMemory << " bytes\n";
        std::cout << "Deallocated Memory: " << deallocatedMemory << " bytes\n";
        if (allocatedMemory == deallocatedMemory)
        {
            std::cout << "No memory leaks detected.\n";
        }
        else
        {
            std::cout << "Memory leak detected: "
                      << (allocatedMemory - deallocatedMemory)
                      << " bytes not freed.\n";
        }
    }
};

//---------------- Platform-specific Allocation Size ----------------//
#if defined(_WIN32)
#include <malloc.h>
#define GET_ALLOCATED_SIZE(ptr) _msize(ptr)
#elif defined(__linux__)
#include <malloc.h>
#define GET_ALLOCATED_SIZE(ptr) malloc_usable_size(ptr)
#else
#error "Unsupported platform"
#endif

//---------------- Overloading new/delete ----------------//
// Single Object Allocation
void *operator new(size_t size)
{
    MemoryTracker::allocatedMemory += size;
    return malloc(size);
}

void operator delete(void *memory) noexcept
{
    if (!memory)
        return;
    size_t size = GET_ALLOCATED_SIZE(memory);
    MemoryTracker::deallocatedMemory += size;
    free(memory);
}

// Array Allocation
void *operator new[](size_t size)
{
    MemoryTracker::allocatedMemory += size;
    return malloc(size);
}

void operator delete[](void *memory) noexcept
{
    if (!memory)
        return;
    size_t size = GET_ALLOCATED_SIZE(memory);
    MemoryTracker::deallocatedMemory += size;
    free(memory);
}

#endif // MEMORY_TRACKER_HPP
