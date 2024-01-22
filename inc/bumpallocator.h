#include "heapallocator.h"

class BumpAllocator : public HeapAllocator{
public:
    BumpAllocator(size_t _capacity) :
    HeapAllocator(_capacity),
    capacity(_capacity) {
        start_addr = static_cast<size_t*>(heap_start_addr);
        curr_addr = start_addr;
    }

    void* HAmalloc(size_t alloc_size_in_bytes);

    void HAfree();

    void* HArealloc(void* addr, size_t alloc_size_in_bytes);

private:
    void* start_addr;
    void* curr_addr;
    size_t capacity;
};