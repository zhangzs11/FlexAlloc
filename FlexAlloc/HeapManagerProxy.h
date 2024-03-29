#pragma once
#include <Windows.h>
#include<iostream>
namespace HeapManagerProxy {

	struct BlockDescriptor {
		void* startAddress = nullptr;
		void* actualStart = nullptr;
		size_t size = 0;
		size_t actualSize = 0;
		BlockDescriptor* prev = nullptr;
		BlockDescriptor* next = nullptr;
	};

	class HeapManager {
	private:
		void* heapStart;
		size_t heapSize;
	public:
		BlockDescriptor* freeBlocks = nullptr;
		BlockDescriptor* outstandingBlocks = nullptr;
		static const size_t s_MinumumToLeave = 64;

		HeapManager(void* pHeapMemory, size_t size, unsigned int numDescriptors);
		~HeapManager();

		inline BlockDescriptor* GetFreeBlocks() const;
		inline BlockDescriptor* GetOutstandingBlocks() const;
		inline void* GetHeapStart() const { return heapStart; }
		inline size_t GetHeapSize() const { return heapSize; }

		void* allocate(size_t size);
		void* allocate(size_t size, size_t alignment);
		bool deallocate(void* ptr);
		void insertAndCoalesce(BlockDescriptor* block);
	};

	HeapManager* CreateHeapManager(void* pHeapMemory, size_t size, unsigned int numDescriptors);
	void* alloc(HeapManager* pHeapManager, size_t size);
	void* alloc(HeapManager* pHeapManager, size_t size, size_t alignment);
	bool free(HeapManager* pHeapManager, void* ptr);
	size_t GetLargestFreeBlock(const HeapManager* pHeapManager);
	void ShowFreeBlocks(const HeapManager* pHeapManager);
	void ShowOutstandingAllocations(const HeapManager* pHeadManager);
	bool Contains(const HeapManager* pHeapManager, const void* pPtr);
	bool IsAllocated(const HeapManager* pHeadManager, const void* pPtr);
	void Collect(HeapManager* pHeapManager);
	void Destroy(HeapManager* pHeadManager);
}
