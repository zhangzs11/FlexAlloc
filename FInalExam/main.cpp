#include <Windows.h>

#include "MemorySystem.h"
#include "Allocators.h"

#include <assert.h>
#include <algorithm>
#include <vector>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif // _DEBUG

bool MemorySystem_UnitTest();

int main(int i_arg, char **)
{
	std::cout << "Hello" << std::endl;
	const size_t 		sizeHeap = 1024 * 1024;

	// you may not need this if you don't use a descriptor pool
	const unsigned int 	numDescriptors = 2048;

	// Allocate memory for my test heap.
	void * pHeapMemory = HeapAlloc(GetProcessHeap(), 0, sizeHeap);
	assert(pHeapMemory);

	// Create your HeapManager and FixedSizeAllocators.
	InitializeMemorySystem(pHeapMemory, sizeHeap, numDescriptors);

	//bool success = MemorySystem_UnitTest();
	bool success = MemorySystem_UnitTest();
	assert(success);
	
	if (success) std::cout << "Success!" << std::endl;
	else std::cout << "Fail" << std::endl;

	// Clean up your Memory System (HeapManager and FixedSizeAllocators)
	DestroyMemorySystem();

	HeapFree(GetProcessHeap(), 0, pHeapMemory);

	// in a Debug build make sure we didn't leak any memory.
#if defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

	return 0;
}
bool MemorySystem_UnitTest()
{
	const size_t maxAllocations = 10 * 1024;
	std::vector<void*> AllocatedAddresses;

	long	numAllocs = 0;
	long	numFrees = 0;
	long	numCollects = 0;

	size_t totalAllocated = 0;

	// reserve space in AllocatedAddresses for the maximum number of allocation attempts
	// prevents new returning null when std::vector expands the underlying array
	AllocatedAddresses.reserve(10 * 1024);

	// allocate memory of random sizes up to 1024 bytes from the heap manager
	// until it runs out of memory
	do
	{
		const size_t		maxTestAllocationSize = 1024;

		size_t			sizeAlloc = 1 + (rand() & (maxTestAllocationSize - 1));

		void* pPtr = Mymalloc(sizeAlloc);

		// if allocation failed see if garbage collecting will create a large enough block
		if (pPtr == nullptr)
		{
			Collect();

			pPtr = Mymalloc(sizeAlloc);

			// if not we're done. go on to cleanup phase of test
			if (pPtr == nullptr)
				break;
		}

		AllocatedAddresses.push_back(pPtr);
		numAllocs++;

		totalAllocated += sizeAlloc;

		// randomly free and/or garbage collect during allocation phase
		const unsigned int freeAboutEvery = 0x07;
		const unsigned int garbageCollectAboutEvery = 0x07;

		if (!AllocatedAddresses.empty() && ((rand() % freeAboutEvery) == 0))
		{
			void* pPtrToFree = AllocatedAddresses.back();
			AllocatedAddresses.pop_back();

			Myfree(pPtrToFree);
			numFrees++;
		}
		else if ((rand() % garbageCollectAboutEvery) == 0)
		{
			Collect();

			numCollects++;
		}

	} while (numAllocs < maxAllocations);

	// now free those blocks in a random order
	if (!AllocatedAddresses.empty())
	{
		// randomize the addresses
		std::random_shuffle(AllocatedAddresses.begin(), AllocatedAddresses.end());

		// return them back to the heap manager
		while (!AllocatedAddresses.empty())
		{
			void* pPtrToFree = AllocatedAddresses.back();
			AllocatedAddresses.pop_back();

			delete[] pPtrToFree;
		}

		// do garbage collection
		Collect();
		// our heap should be one single block, all the memory it started with

		// do a large test allocation to see if garbage collection worked
		void* pPtr = Mymalloc(totalAllocated / 2);

		if (pPtr)
		{
			Myfree(pPtr);
		}
		else
		{
			// something failed
			return false;
		}
	}
	else
	{
		return false;
	}

	// this new [] / delete [] pair should run through your allocator
	char* pNewTest = new char[1024];

	delete[] pNewTest;

	// we succeeded
	return true;
}

