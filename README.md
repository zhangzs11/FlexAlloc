# FlexAlloc

## Introduction

## Key Components

### HeapManager Class
Located in `FlexAlloc/HeapManagerProxy.h`.

- **Constructor Function (`HeapManager(void* pHeapMemory, size_t size, unsigned int numDescriptors)`)**:  
  Initializes the HeapManager with a given memory block, setting up the initial free block list and preparing the heap for memory allocation tasks. It assigns the heap start, configures the initial block descriptor, and initializes tracking pointers for free and allocated blocks.
  Location: `FlexAlloc/HeapManagerProxy.h`, line 24.

- **Destructor Function (`~HeapManager()`)**:  
  The destructor frees the allocated heap memory to prevent memory leaks and resets all member pointers to nullptr, ensuring the HeapManager is cleanly destructed without leaving dangling references.
  Location: `FlexAlloc/HeapManagerProxy.h`, line 25.

- **Allocation Function (`void* allocate(size_t size)`)**:  
  Provides memory from its internal heap and return NULL if it can not fulfil the request.  
  Location: `FlexAlloc/HeapManagerProxy.h`, line 32.

- **Allocation Function (`void* allocate(size_t size, size_t alignment)`)**:  
  Supports `memory alignment`.
  Provides memory from its internal heap and return NULL if it can not fulfil the request.  
  Location: `FlexAlloc/HeapManagerProxy.h`, line 33.

- **Freeing Function (`bool deallocate(void* ptr)`)**:  
  Location: `FlexAlloc/HeapManagerProxy.h`, line 34.

- **Coalescing Function (`void insertAndCoalesce(BlockDescriptor* block)`)**:  
  Locates the appropriate position to insert a free block based on address order. Once it identifies the correct location, it inserts the free block and then performs coalescing, which combines adjacent free blocks into a single larger block to optimize memory usage and reduce fragmentation.
  Location: `FlexAlloc/HeapManagerProxy.h`, line 35.

### Struct: BlockDescriptor
Location: `FlexAlloc/HeapManagerProxy.h`, line 32.
The `BlockDescriptor` struct is a crucial component in the heap memory management. It keeps track of individual memory blocks' details within the heap. Each block managed by the `HeapManager` is represented using this structure. Below are the details of its members:

- `void* startAddress`: The starting address of the block of memory. It points to where the block's memory begins.

- `void* actualStart`: The actual starting address of the allocated memory block, which may be adjusted for alignment purposes.

- `size_t size`: The size of the memory block in bytes. This represents how much memory is available in the block for allocation.

- `size_t actualSize`: The actual size of the allocated memory block, which may differ from `size` due to alignment and metadata storage.

- `BlockDescriptor* prev`: A pointer to the previous block descriptor in the linked list of blocks. It's `nullptr` if there is no previous block.

- `BlockDescriptor* next`: A pointer to the next block descriptor in the linked list of blocks. It's `nullptr` if there is no next block.

#### Usage

The `BlockDescriptor` is used internally by the `HeapManager` to manage free and allocated blocks of memory. It's not typically interacted with directly by the end-user but is crucial for the heap management operations performed by the `HeapManager`.

#### Example

Here's a conceptual example of how `BlockDescriptor` might be used within a heap manager:

```c++
// Assuming 'heapMemory' is a pointer to the start of some allocated heap memory
BlockDescriptor* initialBlock = new (heapMemory) BlockDescriptor();
initialBlock->startAddress = static_cast<char*>(heapMemory) + sizeof(BlockDescriptor);
initialBlock->size = initialHeapSize - sizeof(BlockDescriptor);
// Set up initial linked list pointers
initialBlock->prev = nullptr;
initialBlock->next = nullptr;
```

### FixedSizeAllocator Class
Located in `FlexAlloc/FixedSizeAllocator.h`.

- **Destructor Function (`~FixedSizeAllocator`)**:  
  Emits information in a Debug build only if it's destructed and there are still outstanding allocations.  
  Location: `FlexAlloc/FixedSizeAllocator.h`, line 14.

- **Allocation Function (`void * alloc()`)**:  
  Provides usable memory blocks of a fixed size.  
  Location: `FlexAlloc/FixedSizeAllocator.h`, line 33.

- **Freeing Function (`void free(void * i_ptr)`)**:  
  Location: `FlexAlloc/FixedSizeAllocator.h`, line 68.

- **Validating Function (`bool isValidAllocation(void* i_ptr) const`)**:  
  Validates the returns memory is a valid outstanding allocation from its block pool.  
  Location: `FlexAlloc/FixedSizeAllocator.h`, line 45.

### BitArray Class
Located in `FlexAlloc/BitArray.h`.

- **Platform Specific Implementation**:  
  Represents an arbitrary length array of bits using internally an array of a native data type optimal for the platform it's built for (32- or 64-bit). Uses preprocessor directives to distinguish platforms and implement accordingly.  
  Location: `FlexAlloc/BitArray.h`, lines 6-12.

- **Function for Finding First Clear/Set Bit**:  
  - `bool GetFirstClearBit(size_t& o_bitNumber) const`: Helps find the first set bit in a 32- or 64-bit value. Using compiler intrinsics `_BitScanForward()` and `_BitScanForward64()`.  
    Location: `FlexAlloc/BitArray.h`, line 58.
  - `bool GetFirstSetBit(size_t& o_bitNumber) const`:  
    Location: `FlexAlloc/BitArray.h`, line 69.

- **Methods for Querying Bit Status**:  
  - `inline bool IsBitSet(size_t i_bitNumber) const`:  
    Location: `FlexAlloc/BitArray.h`, line 42.
  - `inline bool IsBitClear(size_t i_bitNumber) const`:  
    Location: `FlexAlloc/BitArray.h`, line 46.

- **Methods for Setting/Clearing a Given Bit**:  
  - `void SetBit(size_t i_bitNumber)`:  
    Location: `FlexAlloc/BitArray.h`, line 50.
  - `void ClearBit(size_t i_bitNumber)`:  
    Location: `FlexAlloc/BitArray.h`, line 54.

- **Methods for Setting/Clearing All Bits**:  
  - `void ClearAll(void)`:  
    Location: `FlexAlloc/BitArray.h`, line 26.
  - `void SetAll(void)`:  
    Location: `FlexAlloc/BitArray.h`, line 30.

## Additional Functions

- **InitializeMemorySystem Function (`bool InitializeMemorySystem(void * i_pHeapMemory, size_t i_sizeHeapMemory, unsigned int i_OptionalNumDescriptors)`)**:  
  Initializes memory system including HeapManager and some FixedSizeAllocators.
  Location: `FlexAlloc/MemorySystem.h`, line 13.

- **Collect Function (`void Collect()`)**:  
  Coalesces free blocks in attempt to create larger blocks.
  Location: `FlexAlloc/MemorySystem.h`, line 16.

- **DestroyMemorySystem Function (`void DestroyMemorySystem()`)**:  
  Location: `FlexAlloc/MemorySystem.h`, line 19.

- **Custom Memory Allocation Functions Function**:  
  Location: `FlexAlloc/Allocators.h`.
Provides custom memory allocation and deallocation functions to give users more control and efficiency in managing memory. These functions are designed to replace the standard allocation and deallocation functions with more robust and performance-tuned alternatives. Below are the custom functions provided:

- `void* __cdecl Mymalloc(size_t i_size)`: Custom malloc function to allocate memory of `i_size` bytes. Similar to `malloc`, it returns a pointer to the beginning of the allocated memory.

- `void __cdecl Myfree(void* i_ptr)`: Custom free function to deallocate memory previously allocated with `Mymalloc`. Pass the pointer returned by `Mymalloc` to free the allocated memory.

- `void* __CRTDECL operator new(size_t i_size)`: Overloaded `new` operator for single object allocation. It uses the custom allocator to allocate `i_size` bytes of memory.

- `void __CRTDECL operator delete(void* i_ptr) noexcept`: Overloaded `delete` operator for single object deallocation. It uses the custom deallocator to free the memory pointed by `i_ptr`.

- `void* __CRTDECL operator new[](size_t i_size)`: Overloaded `new[]` operator for array allocation. It allocates an array of objects, each of size `i_size`, using the custom allocator.

- `void __CRTDECL operator delete[](void* i_ptr) noexcept`: Overloaded `delete[]` operator for array deallocation. It deallocates an array of objects previously allocated with the custom `new[]` operator.

## General Requirements

- Pass the included unit test.
- Compatible with 32 and 64 bit configurations.
- Exits without leaking memory6


