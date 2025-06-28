#include <cstddef>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <utility>

template <size_t Bits>
class BitFlag {
    static_assert(Bits > 0, "Bits must be greater than 0");

    using BlockType = uint64_t;
    static constexpr size_t BlockSize = sizeof(BlockType) * 8;
    static constexpr size_t NumBlocks = (Bits + BlockSize - 1) / BlockSize;

    BlockType blocks[NumBlocks]{};

  public:
    void set(size_t pos) {
        if (pos >= Bits) {
            throw std::out_of_range("Position out of range");
        }
        const size_t blockIdx = pos / BlockSize;
        const size_t bitIdx = pos % BlockSize;
        blocks[blockIdx] |= (BlockType{1} << bitIdx);
    }

    void clear(size_t pos) {
        if (pos >= Bits) {
            throw std::out_of_range("Position out of range");
        }
        const size_t blockIdx = pos / BlockSize;
        const size_t bitIdx = pos % BlockSize;
        blocks[blockIdx] &= ~(BlockType{1} << bitIdx);
    }

    [[nodiscard]] bool test(size_t pos) const {
        if (pos >= Bits) {
            throw std::out_of_range("Position out of range");
        }
        const size_t blockIdx = pos / BlockSize;
        const size_t bitIdx = pos % BlockSize;
        return blocks[blockIdx] & (BlockType{1} << bitIdx);
    }

    void reset() noexcept { std::memset(blocks, 0, sizeof(blocks)); }

    [[nodiscard]] static constexpr size_t size() noexcept { return Bits; }

    bool operator==(const BitFlag& other) const noexcept {
        for (size_t i = 0; i < NumBlocks; ++i) {
            if (blocks[i] != other.blocks[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const BitFlag& other) const noexcept {
        return !(*this == other);
    }

    BitFlag& operator&=(const BitFlag& other) noexcept {
        for (size_t i = 0; i < NumBlocks; ++i) {
            blocks[i] &= other.blocks[i];
        }
        return *this;
    }

    BitFlag& operator|=(const BitFlag& other) noexcept {
        for (size_t i = 0; i < NumBlocks; ++i) {
            blocks[i] |= other.blocks[i];
        }
        return *this;
    }

    BitFlag& operator^=(const BitFlag& other) noexcept {
        for (size_t i = 0; i < NumBlocks; ++i) {
            blocks[i] ^= other.blocks[i];
        }
        return *this;
    }

    BitFlag operator~() const noexcept {
        BitFlag result;
        for (size_t i = 0; i < NumBlocks; ++i) {
            result.blocks[i] = ~blocks[i];
        }

        if constexpr (Bits % BlockSize != 0) {
            const size_t lastBlockBits = Bits % BlockSize;
            const BlockType mask = (BlockType{1} << lastBlockBits) - 1;
            result.blocks[NumBlocks - 1] &= mask;
        }
        return result;
    }

    [[nodiscard]] size_t count() const noexcept {
        size_t count = 0;
        for (size_t i = 0; i < NumBlocks; ++i) {
            count += popcount(blocks[i]);
        }
        return count;
    }

  private:
    static size_t popcount(BlockType value) noexcept {
#ifdef __GNUC__
        return __builtin_popcountll(value);
#else

        size_t count = 0;
        while (value != 0) {
            count += value & 1;
            value >>= 1;
        }
        return count;
#endif
    }
};