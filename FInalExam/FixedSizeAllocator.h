#pragma once
#include "BitArray.h"

class FixedSizeAllocator {
public:
    FixedSizeAllocator(void* i_pHeapMemory, size_t i_blockSize, size_t i_numBlocks)
        : pHeapMemory(i_pHeapMemory), sizeBlock(i_blockSize), numBlocks(i_numBlocks) {
        // Create and initialize a bit array to track the usage of each block
        pBitArray = BitArray::Create(numBlocks);
        pBitArray->ClearAll(); // Initially, all blocks are free
    }

    ~FixedSizeAllocator() {
        delete pBitArray;
    }
    size_t GetBlockSize() const {
        return sizeBlock;
    }
    void* alloc() {
        size_t freeBitIndex = 0;
        bool found = pBitArray->GetFirstClearBit(freeBitIndex);
        if (!found) {
            return nullptr;
        }

        pBitArray->SetBit(freeBitIndex);

        return static_cast<char*>(pHeapMemory) + (freeBitIndex * sizeBlock);
    }

    bool isValidAllocation(void* i_ptr) const {
        if (i_ptr == nullptr) {
            return false;
        }

        // Check if the pointer is within the range of the memory pool
        char* charPtr = static_cast<char*>(i_ptr);
        char* poolStart = static_cast<char*>(pHeapMemory);
        char* poolEnd = poolStart + numBlocks * sizeBlock;
        if (charPtr < poolStart || charPtr >= poolEnd) {
            return false;
        }

        // Check if the pointer points to the start of a block
        size_t blockIndex = (charPtr - poolStart) / sizeBlock;
        if (poolStart + blockIndex * sizeBlock != charPtr) {
            return false;
        }

        // Check if the block is  allocated
        return pBitArray->IsBitSet(blockIndex);
    }

    void free(void* i_ptr) {
        //if (!isValidAllocation(i_ptr)) {
        //    return;
        //}

        size_t blockIndex = (static_cast<char*>(i_ptr) - static_cast<char*>(pHeapMemory)) / sizeBlock;
        if (blockIndex < numBlocks) {
            pBitArray->ClearBit(blockIndex);
        }
    }

private:
    void* pHeapMemory;   // Pointer to the memory pool
    BitArray* pBitArray; // Bit Array to track the usage of blocks
    size_t sizeBlock;    // Block size
    size_t numBlocks;    // Number of blocks
};
