#include <iostream>
#include <cassert>
#include <vector>
#include "../inc/bumpallocator.h"
#include "../inc/implicit.h"
#include "../inc/explicit.h"

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

    cout << "\nTesting malloc\n";
    for (size_t i = 1; i <= total_chunks+1; i++) {
        void* alloc_addr = ia.HAmalloc(chunk_size);
        if (alloc_addr != nullptr) {
            *(int*)alloc_addr = i;
            allocations.push_back(alloc_addr);
        }
    }

    cout << "\nTesting free\n";
    for (auto allocation: allocations) {
        ia.HAfree(allocation);
    }

    cout << "\nTesting realloc\n";
    ImplicitAllocator ia2(256);
    void* addr = ia2.HAmalloc(30);
    addr = ia2.HArealloc(addr, 20);

    if (allocations.size() == total_chunks) {
        cout << "Test passed!\n";
    } else {
        cout << "Test failed: Expected chunks not allocated!\n";
    }

    return;
}

void TestExplicitAllocator(size_t capacity) {
    cout << "\nTesting Explicit Allocator\n";

    ExplicitAllocator ea(capacity);
    size_t chunk_size = 2;
    vector<void*> allocations;
    size_t aligned_chunk_size = HeapAllocator::RoundUp(chunk_size);
    size_t total_chunks = capacity/(sizeof(ImplicitAllocator::BlockHeader) + max(aligned_chunk_size, 2*sizeof(size_t)));

    cout << "\nTesting malloc\n";
    for (size_t i = 1; i <= total_chunks+1; i++) {
        void* alloc_addr = ea.HAmalloc(chunk_size);
        if (alloc_addr != nullptr) {
            *(int*)alloc_addr = i;
            allocations.push_back(alloc_addr);
        }
    }

    cout << "\nTesting free\n";
    for (auto allocation: allocations) {
        ea.HAfree(allocation);
    }

    cout << "\nTesting realloc\n";
    ExplicitAllocator ea2(256);
    void* addr = ea2.HAmalloc(30);
    addr = ea2.HArealloc(addr, 20);

    if (allocations.size() == total_chunks) {
        cout << "Test passed!\n";
    } else {
        cout << "Test failed: Expected chunks not allocated! Expected: " << total_chunks << " , Allocated: " << allocations.size() << endl;
    }
    
    return;
}

int main() {
    size_t capacity = 1024;
    size_t chunk_size = 16;

    TestBumpAllocator(capacity, chunk_size);
    TestImplicitAllocator(capacity);
    TestExplicitAllocator(capacity);

    return 0;
}