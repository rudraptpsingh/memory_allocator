#include "../inc/implicit.h"

void* ImplicitAllocator::HAmalloc(size_t alloc_size_in_bytes) {
    void* alloc_addr = nullptr;
    void* curr_addr = (char*)start_addr - sizeof(BlockHeader);
    BlockHeader* curr_block;
    size_t aligned_size_in_bytes = HeapAllocator::RoundUp(alloc_size_in_bytes);
    while ((char*)curr_addr + sizeof(BlockHeader) + aligned_size_in_bytes <= (char*)heap_start_addr + capacity) {
        curr_block = (BlockHeader*)curr_addr;
        if (curr_block->isFree() && curr_block->block_size >= aligned_size_in_bytes) {
            // free block found. allocate
            alloc_addr = static_cast<void*>(AllocateFromFreeBlock(curr_addr, curr_block, aligned_size_in_bytes));
            break;
        } else {
            curr_addr = (char*)curr_addr + sizeof(BlockHeader) + curr_block->block_size;
        }
    }
    
    if (alloc_addr == nullptr) {
        cerr << "Failed to allocate memory as no free memory block found\n";
    } else {
        cout << "Allocating memory of " << alloc_size_in_bytes << " bytes at address: " << alloc_addr << endl;
    }

    return alloc_addr;
}

void ImplicitAllocator::HAfree(void* addr) {
    BlockHeader curr_block;
    memcpy(&curr_block, (char*)addr - sizeof(curr_block), sizeof(curr_block));
    // change the block allocation status to free.
    curr_block.allocation_status = BlockAllocationStatus::free;
    return;
}

void* ImplicitAllocator::HArealloc(void* addr, size_t alloc_size_in_bytes) {
    // get a new free block and copy the contents of the previous allocation to this new address.
    void* realloc_addr = HAmalloc(alloc_size_in_bytes);
    if (realloc_addr == nullptr) {
        // new free block could not be found. return.
        cerr << "Failed to reallocate memory. returning null address.\n";
        return realloc_addr;
    }

    BlockHeader curr_block;
    BlockHeader realloc_block;
    memcpy(&curr_block, (char*)addr - sizeof(curr_block), sizeof(curr_block));
    memcpy(&realloc_block, (char*)realloc_addr - sizeof(realloc_addr), sizeof(realloc_addr));
    
    // only copy required memory from the previously allocated buffer.
    memcpy(realloc_addr, addr, min(curr_block.block_size, realloc_block.block_size));
    // free the previous allocation now.
    HAfree(addr);
    return realloc_addr;
}

void* ImplicitAllocator::CreateFreeBlock(void* addr, size_t alloc_size) {
    BlockHeader* block = nullptr;
    size_t free_size_in_bytes = HeapAllocator::RoundUp(alloc_size - sizeof(BlockHeader));

    if (free_size_in_bytes < sizeof(size_t)) {
        return nullptr;
    }

    block = (BlockHeader*)addr;
    block->allocation_status = BlockAllocationStatus::free;
    block->block_size = free_size_in_bytes;
    return ((char*)addr + sizeof(BlockHeader));
}

void* ImplicitAllocator::AllocateFromFreeBlock(void* addr, BlockHeader* block, size_t alloc_size_in_bytes) {
    if (addr >= (char*)heap_start_addr + capacity) {
        return nullptr;
    }

    alloc_size_in_bytes = HeapAllocator::RoundUp(alloc_size_in_bytes);
    size_t remaining_block_size_in_bytes = block->block_size - alloc_size_in_bytes;
    if (remaining_block_size_in_bytes < 0) {
        return nullptr;
    }

    block->allocation_status = BlockAllocationStatus::allocated;
    block->block_size = alloc_size_in_bytes;

    if (remaining_block_size_in_bytes >= sizeof(BlockHeader) + alignment_in_bytes) {
        // we can allocate from the free memory block.
        // update the curr free memory block.
        void* new_free_block_addr = (char*)addr + sizeof(BlockHeader) + block->block_size;
        BlockHeader* new_free_block = (BlockHeader*)new_free_block_addr;
        new_free_block->allocation_status = BlockAllocationStatus::free;
        new_free_block->block_size = HeapAllocator::RoundUp(remaining_block_size_in_bytes - sizeof(BlockHeader));
    }

    return (char*)addr + sizeof(BlockHeader);
}