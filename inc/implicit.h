#include "heapallocator.h"

class ImplicitAllocator : public HeapAllocator {
public:

    ImplicitAllocator(size_t _capacity) :
    HeapAllocator(_capacity),
    capacity(_capacity) {
        start_addr = CreateFreeBlock(heap_start_addr, capacity);
        if (start_addr == nullptr) {
            throw invalid_argument("capacity too low.");
        }
    }

    void* HAmalloc(size_t alloc_size_in_bytes);

    void HAfree(void* addr);

    void* HArealloc(void* addr, size_t alloc_size_in_bytes);

private:

    void* AllocateFromFreeBlock(void* addr, BlockHeader* block, size_t alloc_size);

    void* start_addr;
    size_t capacity;
};