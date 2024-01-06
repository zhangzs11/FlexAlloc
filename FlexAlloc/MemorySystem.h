#pragma once
#include <vector>
#include <iostream>
#include <map>
#include "FixedSizeAllocator.h"
#include "HeapManagerProxy.h"

extern std::vector<FixedSizeAllocator*> gFixedSizeAllocators;
extern HeapManagerProxy::HeapManager* gHeapManager;
extern bool gIsInitializingFixedAllocatorsAndHeapManager;

// InitializeMemorySystem - initialize memory system including HeapManager and some FixedSizeAllocators
bool InitializeMemorySystem(void * i_pHeapMemory, size_t i_sizeHeapMemory, unsigned int i_OptionalNumDescriptors);

// Collect - coalesce free blocks in attempt to create larger blocks
void Collect();

// DestroyMemorySystem - destroy memory systems
void DestroyMemorySystem();

FixedSizeAllocator* FindFixedSizeAllocator(size_t size);