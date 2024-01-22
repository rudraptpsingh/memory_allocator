#include "../inc/bumpallocator.h"

void* BumpAllocator::HAmalloc(size_t alloc_size_in_bytes) {
    if (alloc_size_in_bytes == 0) {
        cerr << "Incorrect allocation size\n";
        return nullptr;
    }

    void* alloc_addr = nullptr;
    size_t aligned_size_in_bytes = HeapAllocator::RoundUp(alloc_size_in_bytes);
    if ((char*)curr_addr + aligned_size_in_bytes <= (char*)start_addr + capacity) {
        alloc_addr = static_cast<void*>(curr_addr);
        curr_addr = (char*)curr_addr + aligned_size_in_bytes;
    }
    
    if (alloc_addr == nullptr) {
        cerr << "Failed to allocate memory as capacity reached" << endl;
    } 
    //} else {
    //    cout << "Allocating memory of " << alloc_size_in_bytes << " bytes at address: " << alloc_addr << endl;
    //}
    
    return alloc_addr;
}