#include <iostream>
#include <cassert>
#include <vector>
#include <memory>
#include "../inc/bumpallocator.h"
#include "../inc/implicit.h"

using namespace std;

void TestBumpAllocator(size_t capacity, size_t chunk_size) {
    cout << "\nTesting Bump Allocator\n";

    BumpAllocator ba(capacity);
    vector<void*> allocations;
    size_t aligned_chunk_size = HeapAllocator::RoundUp(chunk_size);
    size_t total_chunks = capacity/aligned_chunk_size;
    for (size_t i = 1; i <= total_chunks+1; i++) {
        void* alloc_addr = ba.HAmalloc(chunk_size);
        if (alloc_addr != nullptr) {
            *(int*)alloc_addr = i;
            allocations.push_back(alloc_addr);
        }
    }

    if (allocations.size() == total_chunks) {
        cout << "Test passed!\n";
    } else {
        cout << "Test failed: Expected chunks not allocated!\n";
    }
    return;
}

void TestImplicitAllocator(size_t capacity) {
    cout << "\nTesting Implicit Allocator\n";

    ImplicitAllocator ia(capacity);
    size_t chunk_size = 2;
    vector<void*> allocations;
    size_t aligned_chunk_size = HeapAllocator::RoundUp(chunk_size);
    size_t total_chunks = capacity/(aligned_chunk_size + sizeof(ImplicitAllocator::BlockHeader));
    for (size_t i = 1; i <= total_chunks+1; i++) {
        void* alloc_addr = ia.HAmalloc(chunk_size);
        if (alloc_addr != nullptr) {
            *(int*)alloc_addr = i;
            allocations.push_back(alloc_addr);
        }
    }

    if (allocations.size() == total_chunks) {
        cout << "Test passed!\n";
    } else {
        cout << "Test failed: Expected chunks not allocated!\n";
    }return;
}

int main() {
    size_t capacity = 80;
    size_t chunk_size = 9;

    TestBumpAllocator(capacity, chunk_size);
    TestImplicitAllocator(capacity);

    return 0;
}