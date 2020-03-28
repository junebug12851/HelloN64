#ifndef MEMORY_H
#define MEMORY_H

#define HEAP_SIZE (1024 * 512)

extern char mem_heap[HEAP_SIZE];

u8 memory_initHeap();

#endif
