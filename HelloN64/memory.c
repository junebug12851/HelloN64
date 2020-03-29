#include <nusys.h>
#include "memory.h"

char memoryHeap[HEAP_SIZE];

u8 memory_initHeap()
{
	// Initialize the heap for dynamic memory allocation
	if (InitHeap(memoryHeap, sizeof(memoryHeap)) == -1)
		return -1; // or stop the program if something horrible happens.

	return 0;
}
