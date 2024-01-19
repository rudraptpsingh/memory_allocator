#include <iostream>
#include <memory>
#include <list>

using namespace std;

class HeapAllocator {
public:

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

    void* HAmalloc(size_t size);

    void HAfree(void* ptr);

    void* HArealloc(void* ptr, size_t size);

protected:
    size_t capacity;
    void* heap_start_addr;
};