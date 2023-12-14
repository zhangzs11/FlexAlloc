# Final Exam

## FixedSizeAllocator Class
Located in `FinalExam/FixedSizeAllocator.h`.

- **Destructor Function (`~FixedSizeAllocator`)**:  
  Emits information in a Debug build only if it's destructed and there are still outstanding allocations.  
  Location: `FinalExam/FixedSizeAllocator.h`, line 14.

- **Allocation Function (`void * alloc()`)**:  
  Provides usable memory blocks of a fixed size.  
  Location: `FinalExam/FixedSizeAllocator.h`, line 33.

- **Freeing Function (`void free(void * i_ptr)`)**:  
  Location: `FinalExam/FixedSizeAllocator.h`, line 68.

- **Validating Function (`bool isValidAllocation(void* i_ptr) const`)**:  
  Validates the returns memory is a valid outstanding allocation from its block pool.  
  Location: `FinalExam/FixedSizeAllocator.h`, line 45.

## BitArray Class
Located in `FinalExam/BitArray.h`.

- **Platform Specific Implementation**:  
  Represents an arbitrary length array of bits using internally an array of a native data type optimal for the platform it's built for (32- or 64-bit). Uses preprocessor directives to distinguish platforms and implement accordingly.  
  Location: `FinalExam/BitArray.h`, lines 6-12.

- **Function for Finding First Clear/Set Bit**:  
  - `bool GetFirstClearBit(size_t& o_bitNumber) const`: Helps find the first set bit in a 32- or 64-bit value. Using compiler intrinsics `_BitScanForward()` and `_BitScanForward64()`.  
    Location: `FinalExam/BitArray.h`, line 58.
  - `bool GetFirstSetBit(size_t& o_bitNumber) const`:  
    Location: `FinalExam/BitArray.h`, line 69.

- **Methods for Querying Bit Status**:  
  - `inline bool IsBitSet(size_t i_bitNumber) const`:  
    Location: `FinalExam/BitArray.h`, line 42.
  - `inline bool IsBitClear(size_t i_bitNumber) const`:  
    Location: `FinalExam/BitArray.h`, line 46.

- **Methods for Setting/Clearing a Given Bit**:  
  - `void SetBit(size_t i_bitNumber)`:  
    Location: `FinalExam/BitArray.h`, line 50.
  - `void ClearBit(size_t i_bitNumber)`:  
    Location: `FinalExam/BitArray.h`, line 54.

- **Methods for Setting/Clearing All Bits**:  
  - `void ClearAll(void)`:  
    Location: `FinalExam/BitArray.h`, line 26.
  - `void SetAll(void)`:  
    Location: `FinalExam/BitArray.h`, line 30.

## General Requirements

- Pass the included unit test.
- Compatible with 32 and 64 bit configurations.
- Exits without leaking memory.


