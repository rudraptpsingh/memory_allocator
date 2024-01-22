#include "heapallocator.h"

class ExplicitAllocator : public HeapAllocator {
public:

    struct FreeBlockListNode {
        BlockHeader block;
        FreeBlockListNode* prev;
        FreeBlockListNode* next;
    };

    ExplicitAllocator(size_t _capacity) :
    HeapAllocator(_capacity),
    start_addr(heap_start_addr),
    capacity(_capacity) {
        free_blocks = (FreeBlockListNode*)CreateFreeBlockListNode(start_addr, capacity);

        assert(free_blocks != nullptr && "Failed to initialize explicit allocator due to insufficient capacity");
    }

    void* HAmalloc(size_t alloc_size_in_bytes);

    void HAfree(void* addr);

    void* HArealloc(void* addr, size_t alloc_size_in_bytes);

private:

    void* CreateFreeBlockListNode(void *addr, size_t alloc_size_in_bytes);

    void* GetFreeBlock(size_t alloc_size_in_bytes);

    void* AllocateFromFreeBlock(void* addr, size_t alloc_size_in_bytes);

    void InsertBlockInFreeList(FreeBlockListNode* curr_block_listnode, FreeBlockListNode* prev_block_listnode, FreeBlockListNode* next_block_listnode);

    void CoalesceAndInsertBlockInFreeList(FreeBlockListNode* curr_block_listnode, FreeBlockListNode* next_block_listnode);

    void CreateFreeBlockAndInsertInFreeList(void* addr, size_t alloc_size_in_bytes, FreeBlockListNode* prev, FreeBlockListNode* next);

    FreeBlockListNode* free_blocks;
    void* start_addr;
    size_t capacity;
};