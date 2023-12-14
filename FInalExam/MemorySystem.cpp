#include "MemorySystem.h"

std::vector<FixedSizeAllocator*> gFixedSizeAllocators;
HeapManagerProxy::HeapManager* gHeapManager = nullptr;
bool gIsInitializingFixedAllocatorsAndHeapManager = true;

bool InitializeMemorySystem(void* i_pHeapMemory, size_t i_sizeHeapMemory, unsigned int i_OptionalNumDescriptors) {
    gIsInitializingFixedAllocatorsAndHeapManager = true;
    // create HeapManager and FixedSizeAllocators
    char* currentAllocatorMemory = static_cast<char*>(i_pHeapMemory);

    struct AllocatorConfig {
        size_t blockSize;
        size_t numBlocks;
    };

    std::vector<AllocatorConfig> allocatorConfigs = {
        {16, 100},
        {32, 200},
        {96, 400}
    };
    std::cout << "start InitializeMemorySystem" << std::endl;
    for (const auto& config : allocatorConfigs) {
        size_t totalMemoryForAllocator = config.blockSize * config.numBlocks*10;
        std::cout << "Checking memory for FixedSizeAllocator" << std::endl;
        if (currentAllocatorMemory + totalMemoryForAllocator > static_cast<char*>(i_pHeapMemory) + i_sizeHeapMemory) {

            std::cout << "Not enough memory for FixedSizeAllocator" << std::endl;
            return false; 

        }

        FixedSizeAllocator* allocator = new FixedSizeAllocator(currentAllocatorMemory, config.blockSize, config.numBlocks*10);
        gFixedSizeAllocators.push_back(allocator);
        currentAllocatorMemory += totalMemoryForAllocator;
        std::cout << "create fixedAlloctor" << std::endl;
    }

    void* heapManagerMemory = currentAllocatorMemory;
    size_t heapManagerMemorySize = static_cast<char*>(i_pHeapMemory) + i_sizeHeapMemory - currentAllocatorMemory;
    gHeapManager = HeapManagerProxy::CreateHeapManager(heapManagerMemory, heapManagerMemorySize, i_OptionalNumDescriptors);

    gIsInitializingFixedAllocatorsAndHeapManager = false;

    return true;
}

void Collect()
{
	// coalesce free blocks
	// you may or may not need to do this depending on how you've implemented your HeapManager
    HeapManagerProxy::Collect(gHeapManager);
}

void DestroyMemorySystem()
{
	// Destroy your HeapManager and FixedSizeAllocators
    std::cout << "Destroy FixedSizeAllocators" << std::endl;
    for (FixedSizeAllocator* allocator : gFixedSizeAllocators) {
        delete allocator;
    }
    gFixedSizeAllocators.clear();

    // Destroy HeapManager 
    std::cout << "Destroy HeapManager" << std::endl;
    if (gHeapManager != nullptr) {
        HeapManagerProxy::Destroy(gHeapManager);
        gHeapManager = nullptr;
    }
}

FixedSizeAllocator* FindFixedSizeAllocator(size_t size) {
    FixedSizeAllocator* bestFit = nullptr;
    //size_t closestSize = std::numeric_limits<size_t>::max();
    size_t closestSize = SIZE_MAX; 
    for (FixedSizeAllocator* allocator : gFixedSizeAllocators) {
        size_t blockSize = allocator->GetBlockSize();

        if (blockSize >= size && blockSize < closestSize) {
            bestFit = allocator;
            closestSize = blockSize;
        }

    }

    return bestFit;
}
