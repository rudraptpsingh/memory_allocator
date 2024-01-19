#include "heapallocator.h"

using namespace std;

class BumpAllocator : public HeapAllocator{
public:
    BumpAllocator(size_t _capacity) : HeapAllocator(_capacity) {
        start_addr = static_cast<char*>(heap_start_addr);
        curr_addr = start_addr;
        capacity = HeapAllocator::capacity;
    }

    void* HAmalloc(size_t alloc_size);

    void HAfree();

    void* HArealloc(void* addr, size_t alloc_size);

private:
    char* start_addr;
    char* curr_addr;
    size_t capacity;
};