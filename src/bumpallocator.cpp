#include <iostream>
#include <cassert>
#include "../inc/bumpallocator.h"

using namespace std;

void* BumpAllocator::HAmalloc(size_t alloc_size) {
    void* alloc_addr = nullptr;
    if (curr_addr + alloc_size <= start_addr + capacity) {
        alloc_addr = static_cast<void*>(curr_addr);
        curr_addr += alloc_size;
        cout << "Allocating memory of " << alloc_size << " bytes at address: " << alloc_addr << endl;
        return alloc_addr;
    }
    
    cerr << "Failed to allocate memory as capacity reached" << endl;
    return alloc_addr;
}