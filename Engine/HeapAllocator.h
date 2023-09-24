#pragma once
#include <Windows.h>

struct BlockDescriptor {
	void* startAddress;
	size_t size;
	BlockDescriptor* prev;
	BlockDescriptor* next;
};

class HeapManager {
private:
	BlockDescriptor* freeBlocks;
	BlockDescriptor* outstandingBlocks;
	void* heapStart;
	size_t heapSize;
public:
	HeapManager(size_t size);
	~HeapManager();
	void* allocate(size_t size);
	void deallocate(void* ptr);
};