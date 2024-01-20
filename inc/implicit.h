#include "heapallocator.h"

using namespace std;

class ImplicitAllocator : public HeapAllocator {
public:

    enum BlockAllocationStatus {
        free = 0x0,
        allocated = 0x1,
    };

    struct BlockHeader {
        size_t allocation_status : 3;
        size_t block_size: 61;
        bool isFree() {
            return (allocation_status & BlockAllocationStatus::allocated) == 0;
        }
    };

    ImplicitAllocator(size_t _capacity) :
    HeapAllocator(_capacity),
    capacity(_capacity) {
        start_addr = CreateFreeBlock(heap_start_addr, capacity);
    }

    void* HAmalloc(size_t alloc_size_in_bytes);

    void HAfree(void* addr);

    void* HArealloc(void* addr, size_t alloc_size_in_bytes);

private:

    void* CreateFreeBlock(void* addr, size_t alloc_size);

    void* AllocateFromFreeBlock(void* addr, BlockHeader* block, size_t alloc_size);

    void* start_addr;
    size_t capacity;
};