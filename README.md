# FlexAlloc

## Introduction

## Comparing FixedSizeAllocator and HeapAllocator

When managing memory in applications, choosing the right allocator is crucial for performance and efficiency. Two common types of allocators are FixedSizeAllocator and HeapAllocator. Each has its own set of advantages and disadvantages, depending on the use case.

### FixedSizeAllocator

FixedSizeAllocator is an allocator that handles fixed-size blocks of memory. It is best suited for situations where the size of allocations is known ahead of time and remains constant.

#### Advantages:

- **Performance**: Offers fast allocation and deallocation since the blocks are all the same size and can be easily managed.
- **Fragmentation**: Less fragmentation compared to variable-size allocators due to the uniform size of allocations.
- **Simplicity**: Easier to implement and manage due to the predictability of the block sizes.

#### Disadvantages:

- **Flexibility**: Not suitable for variable-sized allocations, leading to potential memory waste if the fixed size doesn't closely match the actual needs.
- **Scalability**: As the allocation patterns change or the size of the typical allocation changes, the fixed-size allocator might not adapt as efficiently.

### HeapAllocator

HeapAllocator is a more general-purpose allocator that handles variable-sized blocks of memory, allowing for a wide range of allocation sizes.

#### Advantages:

- **Flexibility**: Can allocate blocks of any size, making it suitable for a wide range of use cases.
- **Adaptability**: Better adapts to varying allocation and deallocation patterns, potentially leading to better overall memory utilization.

#### Disadvantages:

- **Performance**: Generally slower than a fixed-size allocator due to the additional logic needed to manage variable-sized blocks.
- **Fragmentation**: More prone to fragmentation, especially in long-running applications where the allocation and deallocation patterns are unpredictable.
- **Complexity**: Typically more complex to implement and manage, with increased overhead associated with maintaining the heap's state.

### Conclusion

Choosing between a FixedSizeAllocator and a HeapAllocator depends largely on the specific needs and constraints of your application. If your application frequently allocates and deallocates objects of the same size, a FixedSizeAllocator might offer the best performance. On the other hand, if your application requires a variety of allocation sizes, a HeapAllocator is likely more suitable despite the potential for increased complexity and overhead.



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

## Detailed Implementation

### Allocating Memory

#### FixedSizeAllocator Memory Allocation

The `FixedSizeAllocator` is designed to efficiently allocate memory blocks of a fixed size. This approach is beneficial for applications that frequently allocate and deallocate objects of the same size, reducing fragmentation and improving allocation speed. Here's how the memory allocation process works:

#### Allocation Process

1. **Finding Suitable Allocator:**
   - When a request for allocation is made, the system first tries to find a suitable `FixedSizeAllocator` that handles blocks of the desired size.
   - It iterates through a collection of `FixedSizeAllocators` (referred to as `gFixedSizeAllocators`), comparing the required block size (`i_size`) with each allocator's block size.

2. **Selecting the Best Fit:**
   - The code looks for the `FixedSizeAllocator` that provides the closest but larger or equal block size to the requested size.
   - It ensures that the selected allocator offers the smallest block that can fit the requested size, minimizing waste.

3. **Allocation from the Allocator:**
   - Once the appropriate `FixedSizeAllocator` is found, it calls the `alloc()` method on that allocator.
   - If `alloc()` successfully returns a non-null pointer, it means memory has been allocated, and the pointer is returned to the caller.

4. **Handling Allocation Failure:**
   - If no suitable `FixedSizeAllocator` is found, or if the allocator fails to allocate memory (i.e., `alloc()` returns null), the allocation request fails.
   - The system might then try other allocation strategies or report an allocation failure.

#### Benefits of Using FixedSizeAllocator

- **Performance:** Allocating and deallocating fixed-size blocks is typically faster than variable-sized allocations, especially in systems where such operations are frequent.
- **Fragmentation:** Reduces fragmentation by segregating objects of different sizes into different pools.
- **Predictability:** Provides more predictable performance characteristics, as all blocks within a specific allocator are of the same size.

#### Implementation Details

- `FindFixedSizeAllocator(size_t size)`: This function is responsible for finding the most suitable `FixedSizeAllocator` based on the requested size. It iterates through all available allocators and selects the one with the closest block size that is larger than or equal to the requested size.

### HeapManager Memory Allocation

The `HeapManager` class provides two primary methods for memory allocation: one that doesn't consider alignment (`allocate(size_t size)`) and another that does (`allocate(size_t size, size_t alignment)`). These methods facilitate efficient memory management by allowing tailored allocations based on the needs of the application.

#### allocate(size_t size)

This method allocates memory blocks without considering alignment. It's a straightforward approach suitable for scenarios where alignment isn't a concern.

##### How It Works:

1. **Search for a Suitable Block**: It iterates through the `freeBlocks` linked list to find a block large enough to accommodate the requested size plus the size of a new `BlockDescriptor`.

2. **Allocation**: Once a suitable block is found:
   - A new `BlockDescriptor` is created at the start of the free block.
   - The `size` and `actualSize` of the new block are set to the requested size.
   - The new block is inserted into the `outstandingBlocks` list, which tracks allocated blocks.

3. **Adjust Free Block**: The original free block is adjusted to reflect the allocated space.

4. **Return Memory**: A pointer to the start of the allocated memory (excluding the `BlockDescriptor`) is returned.

#### allocate(size_t size, size_t alignment)

This method allocates memory considering the specified alignment, ensuring the allocated memory's start address is a multiple of the alignment.

##### How It Works:

1. **Search for a Suitable Block**: Like the previous method, it iterates through `freeBlocks` to find a block that can accommodate the total size needed, considering the alignment adjustments.

2. **Calculate Alignment**: It calculates the adjustment needed to align the block correctly.

3. **Allocation**: Once a suitable block is found:
   - A new `BlockDescriptor` is created at the adjusted address.
   - The `size` and `actualSize` are set considering the alignment adjustment.
   - The new block is inserted into the `outstandingBlocks` list.

4. **Adjust Free Block**: The original free block is adjusted to reflect the allocated space, including the alignment adjustment.

5. **Return Memory**: A pointer to the start of the aligned and allocated memory is returned.

### Coalescing Free Blocks with Collect

The `Collect` function is a critical part of the `HeapManager` class, designed to improve memory usage efficiency by merging adjacent free blocks into larger ones. This process is known as coalescing and helps to reduce fragmentation within the heap.

#### Functionality

The `Collect` function iteratively merges adjacent free blocks in the `HeapManager`. Here's how it works:

1. **Iterate Over Free Blocks**: The function starts by getting the list of free blocks and iteratively checks each one to see if it is adjacent to the next block.

2. **Check Adjacency**: For each pair of blocks, the function calculates the end address of the current block and the start address of the next block. If these addresses are consecutive, it means the blocks are adjacent and can be merged.

3. **Merge Blocks**: When two adjacent blocks are found, the function performs the following:
   - The size of the next block is increased to include the size of the current block and the intervening `BlockDescriptor`.
   - The start address of the next block is moved backward to the start of the current block.
   - The current block is removed from the free list, and pointers are updated accordingly.

4. **Continue Until End**: The function continues this process until all blocks have been checked and merged if possible.

#### Implementation Details

- **Pointer Updates**: Care is taken to properly update the `next` and `prev` pointers of the involved blocks to maintain the integrity of the linked list of free blocks.
- **Efficiency Considerations**: While the function iteratively tries to coalesce blocks, it's designed to run a limited number of times (10 iterations in the provided code) to avoid excessive processing. This limit can be adjusted based on the application's needs and the expected heap usage patterns.


### BitArray Usage in FixedSizeAllocator

The `FixedSizeAllocator` utilizes a `BitArray` to efficiently track the allocation status of each block within the memory pool. This section describes how the `BitArray` is integrated and used for memory management.

#### Role of BitArray

`BitArray` is a data structure used to represent a series of bits (0s or 1s). In the context of the `FixedSizeAllocator`, each bit corresponds to a block in the memory pool, with a bit's value indicating whether the block is free (0) or allocated (1).

#### Initialization

- **Creating BitArray**: When the `FixedSizeAllocator` is instantiated, it creates a `BitArray` with the same number of bits as there are blocks in the memory pool.
- **Setting Initial State**: Initially, all bits are cleared (set to 0) using `pBitArray->ClearAll();`, indicating that all blocks are free.

#### Allocating Memory (`alloc()`)

- **Finding a Free Block**: The allocator uses `pBitArray->GetFirstClearBit(freeBitIndex);` to find the first clear bit, which represents the first available (free) block.
- **Updating BitArray**: Once a free block is found, the corresponding bit in the `BitArray` is set (marked as 1) using `pBitArray->SetBit(freeBitIndex);` to indicate that the block is now allocated.

#### Deallocating Memory (`free(void* i_ptr)`)

- **Validating Pointer**: The allocator first ensures that the pointer provided for deallocation points to a valid allocated block within the memory pool.
- **Calculating Block Index**: It calculates the index of the block based on the pointer's address.
- **Clearing Bit**: To mark the block as free, the corresponding bit in the `BitArray` is cleared (set to 0) using `pBitArray->ClearBit(blockIndex);`.

#### Advantages of Using BitArray

- **Efficiency**: `BitArray` allows the allocator to quickly find free blocks and update allocation status with minimal overhead.
- **Compactness**: It provides a compact representation of the allocation status of all blocks, minimizing memory overhead.
- **Simplicity**: The use of bitwise operations simplifies the process of tracking and updating block statuses.

#### Debugging and Cleanup

- **Debugging**: In debug mode, the allocator can quickly check for any outstanding allocations (i.e., if any bits are still set) upon destruction, aiding in identifying memory leaks.
- **Cleanup**: Upon destruction of the `FixedSizeAllocator`, the `BitArray` is deleted to free the resources it occupies.


## General Requirements

- Pass the included unit test.
- Compatible with 32 and 64 bit configurations.
- Exits without leaking memory


