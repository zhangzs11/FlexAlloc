#include"Allocators.h"

void* __cdecl Mymalloc(size_t i_size) {
    // printf("my malloc %zu\n", i_size);
    // Check if FixedSizeAllocators and HeapManager are being initialized
    if (gIsInitializingFixedAllocatorsAndHeapManager) {
        void* allocatedMemory = ::malloc(i_size);
        return allocatedMemory;
    }

    if (i_size == 0) {
        return nullptr;
    }

    // Attempt to allocate memory using FixedSizeAllocator
    FixedSizeAllocator* allocator = FindFixedSizeAllocator(i_size);
    if (allocator != nullptr) {
        void* allocatedMemory = allocator->alloc();
        if (allocatedMemory != nullptr) {
            return allocatedMemory;
        }
    }

    // Attempt to allocate memory using HeapManagerProxy
    if (gHeapManager) {
        void* heapMemory = HeapManagerProxy::alloc(gHeapManager, i_size);
        if (heapMemory != nullptr) {
            return heapMemory;
        }
    }
    void* allocatedMemory = ::malloc(i_size);
    return allocatedMemory;
}


void __cdecl Myfree(void* i_ptr) {
    if (i_ptr == nullptr) {
        return;
    }
    //printf("my free 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
    for (FixedSizeAllocator* allocator : gFixedSizeAllocators) {
        //std::cout << "1" << std::endl;
        if (allocator->isValidAllocation(i_ptr)) {
            allocator->free(i_ptr);
            return;
        }
    }
    // Attempt to free memory using HeapManagerProxy
    if (gHeapManager) {
        //std::cout << "2" << std::endl;
        if (HeapManagerProxy::IsAllocated(gHeapManager, i_ptr)) {
            HeapManagerProxy::free(gHeapManager, i_ptr);
            return;
        }
    }
    ::free(i_ptr);
}



void* __CRTDECL operator new(size_t i_size) {
    //printf("new %zu\n", i_size);
    void* ptr = Mymalloc(i_size);
    if (ptr == nullptr) {
        throw std::bad_alloc();
    }
    return ptr;
}

void __CRTDECL operator delete(void* i_ptr) noexcept {
    //printf("delete 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
    Myfree(i_ptr);
}

void* __CRTDECL operator new[](size_t i_size) {
    //printf("new [] %zu\n", i_size);
    void* ptr = Mymalloc(i_size);
    if (ptr == nullptr) {
        throw std::bad_alloc();
    }
    return ptr;
}

void __CRTDECL operator delete [](void* i_ptr) noexcept {
    //printf("delete [] 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
    Myfree(i_ptr);
}

