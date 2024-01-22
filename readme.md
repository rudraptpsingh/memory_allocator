For reference, I used the following lecture
https://web.stanford.edu/class/archive/cs/cs107/cs107.1196/lectures/15/Lecture15.pdf
and followed the following assignment
https://web.stanford.edu/class/archive/cs/cs107/cs107.1196/assign7/


Heap allocator: set of functions that fulfills requests for heap memory

Heap init: initialize with starting address and size of large continuous block of memory(the heap)

Heap allocator functions:
1. void* malloc(size_t size);
2. void free(void *ptr);
3. void* realloc(void *ptr, size_t size);

Heap allocator must:
1. manage this memory as clients request or no longer need pieces of it(can't assume about the order of allocation and free requests)
2. keep track of memory allocated and which is available(mark memory regions as allocated and available)
3. decide which memory to provide to fulfill an allocation request(based on variety of factors)
4. immediately respond to requests without delay(no priority order)
5. return addresses are 8-byte aligned(must be multiples of 8)

Heap allocator should maximize thoughput and memory utilization. These are conflicting goals as it may sometimes take longer to better plan out heap memory use for each request. So heap allocators should find a balance between the two.

Bump allocator:
A heap allocator design that simply allocates the next available memory address upon an allocate request and does nothing in a free request.
We use each memory block at most once. No freeing at all, so no memory is ever reused.
It optimizes only throughput and not utilization.

Questions to consider:
1. How to keep track of free blocks?
2. How to choose and appropriate free block in which to place a newly allocated block?
3. After placing the newly allocated block in free block, what do we do with the remainder of the free block?
4. What do we do with the block that has just been freed?

Implicit Free List Allocator:
1. In order to reuse blocks, we need a way to track which blocks are allocated and which are free.
2. We could store this information in a separate global structure but this is inefficient.
3. Let's allocate extra space before each block for a header storing its payload size and whether it is allocated or free.
4. When we allocate a block, we look through blocks to find a free one, and we update its header to reflect its allocated size and that it is now allocated.
5. When we free a block, we update its header to reflect it is now free.
6. The header should be 8 bytes or larger.

malloc/realloc use size_t for sizes.
block size will always be a multiple of 8.
Least significant 3 bits will be unused. use one of them to store free/allocated status.

How can we choose a free block to use for an allocation request?
1. First fit: search the list from the beginning each time and choose first free block that fits.
2. Next fit: instead of starting at the beginning, continue where previous search left off.
3. Best fit: examine every free block and choose the one with the smallest size that fits.

Can we do better?
1. Can we avoid searching all block for free blocks to reuse?
2. Can we merge adjacent free blocks to keep large spaces available?
3. can we avoid always copying/moving data during realloc?

The payloads of the free blocks aren't being used. Since the pointers for next and previous free blocks are required, store them in the first 16 bytes of each free block's payload.

Explicit Free List Allocator:
This design builds on the implicit allocator, but also stores pointers to the next and previous free block inside each free block's payload.
When we allocate a block, we look through just the free blocks using our linked list to find a free one and we update its header and the linked list to reflect its allocated size and that it is now allocated.
When we free a block, we update its header to reflect it is now free and update the linked list.

Use a double-linked list to avoid searching all blocks for free blocks to reuse.
Merge adjacent free blocks to keep large spaces available (Coalescing).
Coalesce if possible when a block is freed. Coalesce the most immediate right neighbor.

In-place realloc:
We may be able to keep the date in same place when
1. size is growing, but we added padding to the the block and can use that.
2. size is shrinking, so we can use the existing block.
3. size is growing, and current block isn't big enough, but adjacent blocks are free.
Combine with the right neighbors as much as possible until we get enough space, or until we know we can't get enough space.

Testing:
The following command is run to check for no memory leaks.
leaks --atExit -- ./obj/test.o