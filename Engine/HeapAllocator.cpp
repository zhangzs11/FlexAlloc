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
void HeapManager::insertAndCoalesce(BlockDescriptor* block) {
	BlockDescriptor* prev = nullptr;
	BlockDescriptor* current = freeBlocks;

	//find appropriate place to insert
	while (current && current->startAddress < block->startAddress) {
		prev = current;
		current = current->next;
	}
	//Insert he block
	block->next = current;
	block->prev = prev;
	if (prev) {
		prev->next = block;
	}
	else {
		freeBlocks = block; //Update head
	}
	if (current) {
		current->prev = block;
	}

	//Attempt to coalesce with previous and next blocks

	//Try to coalesce with the previous block
	if (prev && static_cast<char*>(prev->startAddress) + prev->size == block->startAddress) {
		prev->size += block->size + sizeof(BlockDescriptor);
		prev->next = block->next;
		if (block->next) {
			block->next->prev = prev;
		}
	}

	//Try to coalesce with the next block
	if (current && static_cast<char*>(block->startAddress) + block->size == current->startAddress) {
		block->size += current->size + sizeof(BlockDescriptor);
		block->next = current->next;
		if (current->next) {
			current->next->prev = block;
		}
	}
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

			insertAndCoalesce(current);
		}
		current = current->next;
	}
}