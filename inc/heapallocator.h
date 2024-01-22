#include <iostream>
#include <cassert>

using namespace std;

#pragma once

class HeapAllocator {
public:

    enum BlockAllocationStatus {
        available = 0x0,
        allocated = 0x1,
    };

    // TODO: move the isFree function as it is currently a part of the block header.
    struct BlockHeader {
        size_t allocation_status : 3;
        size_t block_size: 61;
        bool isFree() {
            return (allocation_status & BlockAllocationStatus::allocated) == 0;
        }
    };

    HeapAllocator(size_t _capacity) : capacity(_capacity) {
        heap_start_addr = malloc(capacity);
        cout << "Heap memory initialized: capacity = " << capacity << " bytes and at address: " << std::hex << heap_start_addr << std::dec << endl;
    }

    HeapAllocator() = delete;
    HeapAllocator(const HeapAllocator&) = delete;
    HeapAllocator& operator=(const HeapAllocator&) = delete;

    virtual ~HeapAllocator() {
        cout << "Freeing allocated heap memory" << endl;
        free(heap_start_addr);
        heap_start_addr = nullptr;
    }

    static size_t RoundUp(size_t size_in_bytes) {
        return (1 + (size_in_bytes-1)/alignment_in_bytes)*alignment_in_bytes;
    }

    void* HAmalloc(size_t alloc_size);

    void HAfree(void* addr);

    void* HArealloc(void* addr, size_t alloc_size);

protected:

    void* CreateFreeBlock(void* addr, size_t alloc_size);

    static bool IsAddressInRange(void* addr, void* start_addr, size_t range_size_in_bytes);

    static const size_t alignment_in_bytes = sizeof(size_t);
    size_t capacity;
    void* heap_start_addr;
};