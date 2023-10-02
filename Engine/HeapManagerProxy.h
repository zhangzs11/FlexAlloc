#pragma once
#include <Windows.h>
#include<iostream>
namespace HeapManagerProxy {

	struct BlockDescriptor {
		void* startAddress;
		void* actualStart;
		size_t size;
		size_t actualSize;
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
		static const size_t s_MinumumToLeave = 64;

		HeapManager(void* pHeapMemory, size_t size, unsigned int numDescriptors);
		~HeapManager();

		BlockDescriptor* GetFreeBlocks() const;
		BlockDescriptor* GetOutstandingBlocks() const;
		void* GetHeapStart() const { return heapStart; }
		size_t GetHeapSize() const { return heapSize; }

		void* allocate(size_t size);
		void* allocate(size_t size, size_t alignment);
		bool deallocate(void* ptr);
		void insertAndCoalesce(BlockDescriptor* block);
	};

	HeapManager* CreateHeapManager(void* pHeapMemory, size_t size, unsigned int numDescriptors);
	void* alloc(HeapManager* pHeapManager, size_t size);
	void* alloc(HeapManager* pHeapManager, size_t size, size_t alignment);
	bool free(HeapManager* pHeapManager, void* ptr);
	size_t GetLargestFreeBlock(HeapManager* pHeapManager);
	void ShowFreeBlocks(HeapManager* pHeapManager);
	void ShowOutstandingAllocations(HeapManager* pHeadManager);
	bool Contains(HeapManager* pHeapManager, void* pPtr);
	bool IsAllocated(HeapManager* pHeadManager, void* pPtr);
	void Collect(HeapManager* pHeapManager);
	void Destroy(HeapManager* pHeadManager);
}
