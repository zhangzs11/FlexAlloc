#include "HeapManagerProxy.h"
//为了思路清晰，应该只在collect合并，最好insertAndCoalesce函数删去合并部分
namespace HeapManagerProxy {
	HeapManager::HeapManager(void* pHeapMemory, size_t size, unsigned int numDescriptors) {
		//heapStart = ::HeapAlloc(::GetProcessHeap(), 0, size);
		heapStart = pHeapMemory;
		heapSize = size;
		//freeBlocks = reinterpret_cast<BlockDescriptor*>(heapStart);//revise : freeBlocks = new (heapStart) BlockDescriptor;
		freeBlocks = new (heapStart) BlockDescriptor;
		freeBlocks->startAddress = static_cast<char*>(heapStart) + sizeof(BlockDescriptor);
		freeBlocks->actualStart = freeBlocks->startAddress;
		freeBlocks->size = size - sizeof(BlockDescriptor);
		freeBlocks->actualSize = freeBlocks->size;
		freeBlocks->prev = nullptr;
		freeBlocks->next = nullptr;
		outstandingBlocks = nullptr;
	}
	HeapManager::~HeapManager() {
		if (heapStart) {
			::HeapFree(::GetProcessHeap(), 0, heapStart);
			heapStart = nullptr;
		}
		freeBlocks = nullptr;
		outstandingBlocks = nullptr;
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
			prev->size += block->actualSize + sizeof(BlockDescriptor);
			prev->actualSize += block->actualSize + sizeof(BlockDescriptor);
			prev->next = block->next;
			if (block->next) {
				block->next->prev = prev;
			}
		}

		////Try to coalesce with the next block
		if (current && static_cast<char*>(block->startAddress) + block->size == current->startAddress) {
			block->size += current->actualSize + sizeof(BlockDescriptor);
			block->actualSize += current->actualSize + sizeof(BlockDescriptor);
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
				//BlockDescriptor* newBlock = static_cast<BlockDescriptor*>(current->startAddress);//revise: BlockDescriptor* newBlock = new (current->startAddress) BlockDescriptor();
				BlockDescriptor* newBlock = new (current->startAddress) BlockDescriptor();
				newBlock->startAddress = static_cast<char*>(current->startAddress) + sizeof(BlockDescriptor);
				newBlock->actualStart = newBlock->startAddress;
				newBlock->size = size;
				newBlock->actualSize = newBlock->size;
				newBlock->next = outstandingBlocks;
				newBlock->prev = nullptr;
				if (outstandingBlocks) {
					outstandingBlocks->prev = newBlock;
				}
				outstandingBlocks = newBlock;

				current->startAddress = static_cast<char*>(current->startAddress) + size + sizeof(BlockDescriptor);
				current->actualStart = current->startAddress;
				current->size -= (size + sizeof(BlockDescriptor));
				current->actualSize -= (size + sizeof(BlockDescriptor));
				return newBlock->startAddress;
			}
			current = current->next;
		}
		return nullptr;
	}
	void* HeapManager::allocate(size_t size, size_t alignment) {
		BlockDescriptor* current = freeBlocks;
		while (current) {
			uintptr_t rawAddress = reinterpret_cast<uintptr_t>(current->startAddress) + sizeof(BlockDescriptor);
			size_t misalignment = rawAddress % alignment;
			size_t adjustment = (alignment - misalignment) % alignment;
			uintptr_t adjustedAddress = rawAddress + adjustment;
			size_t totalSize = size + sizeof(BlockDescriptor) + adjustment;

			if (current->size >= totalSize) {
				//BlockDescriptor* newBlock = reinterpret_cast<BlockDescriptor*>(adjustedAddress - sizeof(BlockDescriptor));//revise: BlockDescriptor* newBlock = new (adjustedAddress - sizeof(BlockDescriptor)) BlockDescriptor();
				BlockDescriptor* newBlock = new (reinterpret_cast<void*>(adjustedAddress - sizeof(BlockDescriptor))) BlockDescriptor();
				newBlock->startAddress = reinterpret_cast<void*>(adjustedAddress);
				newBlock->actualStart = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(current->startAddress) + sizeof(BlockDescriptor));
				newBlock->size = size;
				newBlock->actualSize = size + adjustment;
				newBlock->next = outstandingBlocks;
				newBlock->prev = nullptr;
				if (outstandingBlocks) {
					outstandingBlocks->prev = newBlock;
				}
				outstandingBlocks = newBlock;

				current->startAddress = static_cast<char*>(newBlock->startAddress) + size;
				current->actualStart = current->startAddress;
				current->size -= totalSize;
				current->actualSize -= totalSize;
				return newBlock->startAddress;
			}
			current = current->next;
		}
		return nullptr;
	}
	bool HeapManager::deallocate(void* ptr) {
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

				//BlockDescriptor* originalBlock = reinterpret_cast<BlockDescriptor*>(reinterpret_cast<char*>(current->actualStart) - sizeof(BlockDescriptor));//revise: BlockDescriptor temp = *current; 
				//*originalBlock = *current; // revise: BlockDescriptor* originalBlock = new(reinterpret_cast<char*>(current->actualStart) - sizeof(BlockDescriptor)) BlockDescriptor(temp);
				BlockDescriptor temp = *current;
				BlockDescriptor* originalBlock = new(reinterpret_cast<char*>(current->actualStart) - sizeof(BlockDescriptor)) BlockDescriptor(temp);
				originalBlock->startAddress = originalBlock->actualStart;
				originalBlock->size = originalBlock->actualSize;

				insertAndCoalesce(originalBlock);
				return true;
			}
			current = current->next;
		}
		return false;
	}
	BlockDescriptor* HeapManager::GetFreeBlocks() const {
		return freeBlocks;
	}
	BlockDescriptor* HeapManager::GetOutstandingBlocks() const {
		return outstandingBlocks;
	}
	size_t GetLargestFreeBlock(HeapManager* pHeapManager) {
		size_t largestSize = 0;
		BlockDescriptor* current = pHeapManager->GetFreeBlocks();
		while (current) {
			if (current->size > largestSize) {
				largestSize = current->size;
			}
			current = current->next;
		}
		return largestSize;
	}
	void ShowFreeBlocks(HeapManager* pHeapManager) {
		BlockDescriptor* current = pHeapManager->GetFreeBlocks();
		while (current) {
			std::cout << "Free Block - Address: " << reinterpret_cast<uintptr_t>(current->startAddress) << " - Size: " << current->size << " -End Address: " << reinterpret_cast<uintptr_t>(current->startAddress) + current->size << std::endl;
			current = current->next;
		}
	}
	void ShowOutstandingAllocations(HeapManager* pHeadManager) {
		BlockDescriptor* current = pHeadManager->GetOutstandingBlocks();
		if (!current) {
			std::cout << "No outstanding allocations!" << std::endl;
			return;
		}

		while (current)
		{
			std::cout << "Allocated Block - Address: " << reinterpret_cast<uintptr_t>(current->startAddress) << " - Size: " << current->size << " -End Address: " << reinterpret_cast<uintptr_t>(current->startAddress) + current->size << std::endl;
			current = current->next;
		}
		std::cout << std::endl;
	}
	bool Contains(HeapManager* pHeapManager, void* pPtr) {
		uintptr_t heapStartAddress = reinterpret_cast<uintptr_t>(pHeapManager->GetHeapStart());
		uintptr_t heapEndAddress = heapStartAddress + pHeapManager->GetHeapSize();
		uintptr_t targetAddress = reinterpret_cast<uintptr_t>(pPtr);
		return targetAddress >= heapStartAddress && targetAddress < heapEndAddress;
	}
	bool IsAllocated(HeapManager* pHeadManager, void* pPtr) {
		BlockDescriptor* currentBlock = pHeadManager->GetOutstandingBlocks();
		while (currentBlock != nullptr) {
			if (pPtr == currentBlock->startAddress) {
				return true;
			}
			currentBlock = currentBlock->next;
		}
		return false;
	}
	void Collect(HeapManager* pHeapManager) {//合并的是空闲块，应该startAddress和actualStart都一样吧
		BlockDescriptor* current = pHeapManager->GetFreeBlocks();
		while (current && current->next) {
			uintptr_t currentEnd = reinterpret_cast<uintptr_t>(current->startAddress) + current->size;
			uintptr_t nextStart = reinterpret_cast<uintptr_t>(current->next->startAddress);

			if (currentEnd == nextStart || currentEnd + sizeof(BlockDescriptor) == nextStart) {
				//Maybe两种情况分别
				//currentEnd == nextStart : 头部指针需要调换，真正在前面的，链表后面的留
				//currentEnd + sizeof(BlockDescriptor) == nextStart ： 不需要调换
				current->size += current->next->actualSize + sizeof(BlockDescriptor);
				current->actualSize += current->next->actualSize + sizeof(BlockDescriptor);
				BlockDescriptor* toBeDeleted = current->next;
				current->next = toBeDeleted->next;
				if (toBeDeleted->next) {
					toBeDeleted->next->prev = current;
				}
				//std::cout << "1" << std::endl;
			}
			else
			{
				current = current->next;
			}
		}
	}
	void Destroy(HeapManager* pHeadManager) {
		if (pHeadManager) {
			delete pHeadManager;
			pHeadManager = nullptr;
		}
	}
	HeapManager* CreateHeapManager(void* pHeapMemory, size_t size, unsigned int numDescriptors) {
		return new HeapManager(pHeapMemory, size, numDescriptors);
	}
	void* alloc(HeapManager* pHeapManager, size_t size) {
		return pHeapManager->allocate(size);
	}
	void* alloc(HeapManager* pHeapManager, size_t size, size_t alignment) {
		return pHeapManager->allocate(size, alignment);
	}
	bool free(HeapManager* pHeapManager, void* ptr) {
		return pHeapManager->deallocate(ptr);
	}
}
