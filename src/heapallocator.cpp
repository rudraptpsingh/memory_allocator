#include "../inc/heapallocator.h"

void* HeapAllocator::CreateFreeBlock(void* addr, size_t alloc_size) {
    BlockHeader* block = nullptr;
    size_t free_size_in_bytes = HeapAllocator::RoundUp(alloc_size - sizeof(BlockHeader));

    if (free_size_in_bytes < sizeof(size_t)) {
        return nullptr;
    }

    block = (BlockHeader*)addr;
    block->allocation_status = BlockAllocationStatus::available;
    block->block_size = free_size_in_bytes;
    return addr;
}

bool HeapAllocator::IsAddressInRange(void* addr, void* start_addr, size_t range_size_in_bytes) {
    if (addr >= start_addr && addr <= (char*)start_addr + range_size_in_bytes) {
        return true;
    }

    return false;
}