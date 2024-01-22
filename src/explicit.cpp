#include "../inc/explicit.h"

void* ExplicitAllocator::HAmalloc(size_t alloc_size_in_bytes) {
    if (alloc_size_in_bytes == 0) {
        cerr << "Incorrect allocation size\n";
        return nullptr;
    }

    // since free block contains prev and next pointer to free blocks, the payload should be atleast 2*sizeof(size_t)
    alloc_size_in_bytes = HeapAllocator::RoundUp(max(alloc_size_in_bytes, 2*sizeof(size_t)));
    void* alloc_addr = nullptr;
    void* free_block_addr = GetFreeBlock(alloc_size_in_bytes);
    alloc_addr = AllocateFromFreeBlock(free_block_addr, alloc_size_in_bytes);

    if (alloc_addr == nullptr) {
        cerr << "Failed to allocate memory as no free memory block found\n";
    }

    return alloc_addr;
}

void ExplicitAllocator::HAfree(void* addr) {
    if (addr == nullptr) {
        cerr << "Incorrect allocation size\n";
        return;
    }

    BlockHeader* curr_block = (BlockHeader*)((char*)addr - sizeof(BlockHeader));
    BlockHeader* next_block = nullptr;
    curr_block->allocation_status = BlockAllocationStatus::available;
    if ((char*)addr + curr_block->block_size + sizeof(FreeBlockListNode) <= (char*)start_addr + capacity) {
        next_block = curr_block + sizeof(BlockHeader) + curr_block->block_size;
    }

    CoalesceAndInsertBlockInFreeList((FreeBlockListNode*)curr_block, (FreeBlockListNode*)next_block);
}

void* ExplicitAllocator::HArealloc(void* addr, size_t alloc_size_in_bytes) {
    if (addr == nullptr || alloc_size_in_bytes == 0) {
        cerr << "Incorrect input parameters\n";
        return nullptr;
    }

    // since free block contains prev and next pointer to free blocks, the payload should be atleast 2*sizeof(size_t)
    alloc_size_in_bytes = HeapAllocator::RoundUp(max(alloc_size_in_bytes, 2*sizeof(size_t)));
    BlockHeader* curr_block = (BlockHeader*)((char*)addr - sizeof(BlockHeader));
    BlockHeader* next_block = nullptr;
    void* realloc_addr = nullptr;
    void* free_block_addr = nullptr;
    size_t remaining_size_in_bytes = 0;

    if (alloc_size_in_bytes <= curr_block->block_size) {
        // shrinking the allocation. Add the remaining block to free list.
        remaining_size_in_bytes = curr_block->block_size - alloc_size_in_bytes;
        curr_block->block_size = alloc_size_in_bytes;
        if (remaining_size_in_bytes >= (sizeof(BlockHeader) + 2*sizeof(size_t))) {
            void* new_free_block_addr = (char*)curr_block + sizeof(BlockHeader) + curr_block->block_size;
            CreateFreeBlockAndInsertInFreeList(new_free_block_addr, remaining_size_in_bytes, nullptr, this->free_blocks);
        }

        realloc_addr = addr;
    } else {
        // growing allocation. check if the next block is free and if it can be used along with the current allocation.
        // If not, allocate from an available block in free list and add the current allocation to free list.
        remaining_size_in_bytes = alloc_size_in_bytes - curr_block->block_size;
        if ((char*)addr + curr_block->block_size + sizeof(FreeBlockListNode) <= (char*)start_addr + capacity) {
            next_block = curr_block + sizeof(BlockHeader) + curr_block->block_size;
        }

        if (next_block == nullptr || (next_block->allocation_status == HeapAllocator::allocated) ||
            ((next_block->allocation_status == HeapAllocator::available) &&
            (sizeof(BlockHeader) + next_block->block_size < remaining_size_in_bytes))) {
            // find another free block and copy contents of the input allocation to it, and then free the input allocation.
            free_block_addr = GetFreeBlock(alloc_size_in_bytes);
            realloc_addr = AllocateFromFreeBlock(free_block_addr, alloc_size_in_bytes);
            if (realloc_addr == nullptr) {
                cerr << "Failed to allocate memory as no free memory block found\n";
            }
        } else if (next_block != nullptr && (next_block->allocation_status == HeapAllocator::available) &&
            (sizeof(BlockHeader) + next_block->block_size >= remaining_size_in_bytes)) {
            // Extend to the next block and update the remaining block of next available block in the free list.
            curr_block->block_size = alloc_size_in_bytes;
            remaining_size_in_bytes = sizeof(BlockHeader) + next_block->block_size - remaining_size_in_bytes;
            void* new_free_block_addr = (char*)curr_block + sizeof(BlockHeader) + curr_block->block_size;
            CreateFreeBlockAndInsertInFreeList(new_free_block_addr, remaining_size_in_bytes, nullptr, this->free_blocks);
            realloc_addr = addr;
        }
    }

    return realloc_addr;
}

void* ExplicitAllocator::CreateFreeBlockListNode(void *addr, size_t alloc_size_in_bytes) {
    FreeBlockListNode* free_block_listnode = (FreeBlockListNode*)HeapAllocator::CreateFreeBlock(addr, alloc_size_in_bytes);
    if (free_block_listnode != nullptr) {
        free_block_listnode->prev = nullptr;
        free_block_listnode->next = nullptr;
    }

    return free_block_listnode;
}

void* ExplicitAllocator::GetFreeBlock(size_t alloc_size_in_bytes) {
    alloc_size_in_bytes = HeapAllocator::RoundUp(alloc_size_in_bytes);
    FreeBlockListNode* start_node = this->free_blocks;
    while (start_node != nullptr) {
        if (start_node->block.block_size >= alloc_size_in_bytes) {
            return (void*)start_node;
        }

        start_node = start_node->next;
    }

    return nullptr;
}

void* ExplicitAllocator::AllocateFromFreeBlock(void* addr, size_t alloc_size_in_bytes) {
    if (addr == nullptr) {
        return addr;
    }

    // Update current block header based on allocation.
    // Create a new free block from the remaining block.
    // Update the pointers in the free list.
    FreeBlockListNode* curr_block_listnode = (FreeBlockListNode*)addr;
    FreeBlockListNode* prev_block_listnode = curr_block_listnode->prev;
    FreeBlockListNode* next_block_listnode = curr_block_listnode->next;
    size_t remaining_size_in_bytes = curr_block_listnode->block.block_size - alloc_size_in_bytes;

    curr_block_listnode->block.allocation_status = BlockAllocationStatus::allocated;
    curr_block_listnode->block.block_size = alloc_size_in_bytes;

    void* new_free_block_addr = (char*)curr_block_listnode + sizeof(BlockHeader) + curr_block_listnode->block.block_size;
    CreateFreeBlockAndInsertInFreeList(new_free_block_addr, remaining_size_in_bytes,prev_block_listnode, next_block_listnode);
    return (char*)addr + sizeof(BlockHeader);
}

void ExplicitAllocator::InsertBlockInFreeList(FreeBlockListNode* curr_block_listnode, FreeBlockListNode* prev_block_listnode, FreeBlockListNode* next_block_listnode) {
    // check if current, previous and next block addresses are valid. If not, assign them to null.
    if (!HeapAllocator::IsAddressInRange(curr_block_listnode, this->start_addr, this->capacity)) {
        curr_block_listnode = nullptr;
    }

    if (!HeapAllocator::IsAddressInRange(prev_block_listnode, this->start_addr, this->capacity)) {
        prev_block_listnode = nullptr;
    }

    if (!HeapAllocator::IsAddressInRange(next_block_listnode, this->start_addr, this->capacity)) {
        next_block_listnode = nullptr;
    }
    
    if (curr_block_listnode == nullptr) {
        if (prev_block_listnode == nullptr && next_block_listnode == nullptr) {
            // no free blocks exist.
            this->free_blocks = nullptr;
        } else if (prev_block_listnode == nullptr && next_block_listnode != nullptr) {
            next_block_listnode->prev = nullptr;
            this->free_blocks = next_block_listnode;
        } else if (prev_block_listnode != nullptr && next_block_listnode == nullptr) {
            prev_block_listnode->next = nullptr;
        } else {
            prev_block_listnode->next = next_block_listnode;
            next_block_listnode->prev = prev_block_listnode;
        }
    } else {
        if (prev_block_listnode == nullptr && next_block_listnode == nullptr) {
            // no free blocks exist.
            this->free_blocks = curr_block_listnode;
        } else if (prev_block_listnode == nullptr && next_block_listnode != nullptr) {
            curr_block_listnode->prev = nullptr;
            curr_block_listnode->next = next_block_listnode;
            next_block_listnode->prev = curr_block_listnode;
            this->free_blocks = curr_block_listnode;
        } else if (prev_block_listnode != nullptr && next_block_listnode == nullptr) {
            prev_block_listnode->next = curr_block_listnode;
            curr_block_listnode->prev = prev_block_listnode;
            curr_block_listnode->next = nullptr;
        } else {
            prev_block_listnode->next = curr_block_listnode;
            curr_block_listnode->prev = prev_block_listnode;
            curr_block_listnode->next = next_block_listnode;
            next_block_listnode->prev = curr_block_listnode;
        }
    }
}

void ExplicitAllocator::CoalesceAndInsertBlockInFreeList(FreeBlockListNode* curr_block_listnode, FreeBlockListNode* next_block_listnode) {
    assert(curr_block_listnode->block.allocation_status == available);
    if (next_block_listnode == nullptr) {
        InsertBlockInFreeList(curr_block_listnode, nullptr, this->free_blocks);
    } else {
        FreeBlockListNode* prev_node = next_block_listnode->prev;
        FreeBlockListNode* next_node = next_block_listnode->next;
        curr_block_listnode->block.block_size += sizeof(BlockHeader) + next_block_listnode->block.block_size;
        InsertBlockInFreeList(curr_block_listnode, prev_node, next_node);
    }
}

void ExplicitAllocator::CreateFreeBlockAndInsertInFreeList(void* addr, size_t alloc_size_in_bytes, FreeBlockListNode* prev, FreeBlockListNode* next) {
    void* new_free_block_addr = CreateFreeBlockListNode(addr, alloc_size_in_bytes);
    InsertBlockInFreeList((FreeBlockListNode*)new_free_block_addr, prev, next);
}