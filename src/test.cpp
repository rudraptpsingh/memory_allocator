#include <iostream>
#include <cassert>
#include "../inc/bumpallocator.h"

using namespace std;

int main() {
    size_t capacity = 80;
    size_t chunk_size = 2;
    BumpAllocator ba(capacity);
    vector<void*> allocations;
    for (size_t i = 1; i <= capacity+1; i++) {
        void* alloc_addr = ba.HAmalloc(chunk_size);
        if (alloc_addr != nullptr) {
            *(int*)alloc_addr = i;
            allocations.push_back(alloc_addr);
        }
    }

    assert(allocations.size() == capacity/chunk_size && "Expected chunks not allocated!\n");
    return 0;
}