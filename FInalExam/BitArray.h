#pragma once
#include <cstdint>
#include <cstring>
#include <algorithm>

#ifdef _WIN64
#define BITSCAN _BitScanForward64
typedef uint64_t BitStorageType;
#else
#define BITSCAN _BitScanForward
typedef uint32_t BitStorageType;
#endif


class BitArray
{
public:
	static BitArray* Create(size_t i_numBits, bool i_startClear = true) {
		void* mem = operator new(sizeof(BitArray) + sizeof(BitStorageType) * ((i_numBits + (8 * sizeof(BitStorageType) - 1)) / (8 * sizeof(BitStorageType))));
		return new (mem) BitArray(i_numBits, i_startClear);
	}

	~BitArray() {
	}

	void ClearAll(void) {
		std::memset(m_Bits, 0, m_ArraySize * sizeof(BitStorageType));
	}

	void SetAll(void) {
		std::memset(m_Bits, 0xFF, m_ArraySize * sizeof(BitStorageType));
	}

	bool AreAllBitsClear(void) const {
		return std::all_of(m_Bits, m_Bits + m_ArraySize, [](BitStorageType n) { return n == 0; });
	}

	bool AreAllBitsSet(void) const {
		return std::all_of(m_Bits, m_Bits + m_ArraySize, [](BitStorageType n) { return n == ~BitStorageType(0); });
	}

	inline bool IsBitSet(size_t i_bitNumber) const {
		return (m_Bits[i_bitNumber / (8 * sizeof(BitStorageType))] & (BitStorageType(1) << (i_bitNumber % (8 * sizeof(BitStorageType))))) != 0;
	}

	inline bool IsBitClear(size_t i_bitNumber) const {
		return !IsBitSet(i_bitNumber);
	}

	void SetBit(size_t i_bitNumber) {
		m_Bits[i_bitNumber / (8 * sizeof(BitStorageType))] |= (BitStorageType(1) << (i_bitNumber % (8 * sizeof(BitStorageType))));
	}

	void ClearBit(size_t i_bitNumber) {
		m_Bits[i_bitNumber / (8 * sizeof(BitStorageType))] &= ~(BitStorageType(1) << (i_bitNumber % (8 * sizeof(BitStorageType))));
	}

	bool GetFirstClearBit(size_t& o_bitNumber) const {
		unsigned long index;
		for (size_t i = 0; i < m_ArraySize; ++i) {
			if (m_Bits[i] != ~BitStorageType(0)) {
				BITSCAN(&index, ~m_Bits[i]); // Find the first cleared bit
				o_bitNumber = i * 8 * sizeof(BitStorageType) + index;
				return true;
			}
		}
		return false;
	}
	bool GetFirstSetBit(size_t& o_bitNumber) const {
		unsigned long index;
		for (size_t i = 0; i < m_ArraySize; ++i) {
			if (m_Bits[i] != 0) {
				BITSCAN(&index, m_Bits[i]); // Find the first cleared bit
				o_bitNumber = i * 8 * sizeof(BitStorageType) + index;
				return true;
			}
		}
		return false;
	}
	bool operator[](size_t i_index) const {
		return IsBitSet(i_index);
	}

private:
	BitArray(size_t i_numBits, bool i_startClear) : m_NumBits(i_numBits), m_ArraySize((i_numBits + (8 * sizeof(BitStorageType) - 1)) / (8 * sizeof(BitStorageType))) {
		m_Bits = reinterpret_cast<BitStorageType*>(this + 1);
		if (i_startClear) {
			ClearAll();
		}
		else {
			SetAll();
		}
	}

	size_t m_NumBits;
	size_t m_ArraySize;
	BitStorageType* m_Bits;
};