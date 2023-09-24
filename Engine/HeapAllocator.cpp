#include "HeapAllocator.h"
HeapManager::HeapManager(size_t size) {
	heapStart = ::HeapAlloc(::GetProcessHeap(), 0, size);
	heapSize = size;
	freeBlocks = reinterpret_cast<BlockDescriptor*>(heapStart);
	freeBlocks->startAddress = static_cast<char*>(heapStart) + sizeof(BlockDescriptor); //operate memory in 1 byte(char)
	freeBlocks->size = size - sizeof(BlockDescriptor);
	freeBlocks->prev = nullptr;
	freeBlocks->next = nullptr;
	outstandingBlocks = nullptr;
} 
HeapManager::~HeapManager() {
	::HeapFree(::GetProcessHeap(), 0, heapStart);
}
void* HeapManager::allocate(size_t size) {
	BlockDescriptor* current = freeBlocks;
	while (current) {
		if (current->size >= size + sizeof(BlockDescriptor)) {
			BlockDescriptor* newBlock = reinterpret_cast<BlockDescriptor*>(current->startAddress);
			newBlock->startAddress = static_cast<char*>(current->startAddress) + sizeof(BlockDescriptor);
			newBlock->size = size;
			newBlock->next = outstandingBlocks;
			newBlock->prev = nullptr;
			if (outstandingBlocks) {
				outstandingBlocks->prev = newBlock;
			}
			outstandingBlocks = newBlock;

			current->startAddress = static_cast<char*>(current->startAddress) + size + sizeof(BlockDescriptor);
			current->size -= (size + sizeof(BlockDescriptor));
			return newBlock->startAddress;
		}
		current = current->next;
	}
	return nullptr;
}
void HeapManager::deallocate(void* ptr) {
	//Find the corresponding block in the outstandingBlocks list.
	BlockDescriptor* current = outstandingBlocks;
	while (current) {
		if (current->startAddress == ptr) {
			// Remove this block from the outstandingBlocks list
			if (current->prev) {
				current->prev->next = current->next;
			}
			else {
				outstandingBlocks = current->next;
			}
			if (current->next) {
				current->next->prev = current->prev;
			}

			// Add this block back to freeBlocks list
			// for simplicity, insert it at the front
			current->next = freeBlocks;
			current->prev = nullptr;
			if (freeBlocks) {
				freeBlocks->prev = current;
			}
			freeBlocks = current;

			//Attempt to coalesce this block with adjancent blocks in freeBlocks.
			BlockDescriptor* tmp = freeBlocks;
			while (tmp) {
				char* tmpEndAddress = static_cast<char*>(tmp->startAddress) + tmp->size;
				if (tmp->next && tmpEndAddress == tmp->next->startAddress) {
					//Coalesce with the enext block
					tmp->size += tmp->next->size + sizeof(BlockDescriptor);
					//Remove the next block from the list
					BlockDescriptor* toRemove = tmp->next;
					if (toRemove->next) {
						toRemove->next->prev = tmp;
					}
				}
				tmp = tmp->next;
			}
			return;
		}
		current = current->next;
	}
}