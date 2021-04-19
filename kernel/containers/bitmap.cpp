#include "../types.hpp"
#include "bitmap.hpp"

namespace Containers {
    bitmap::bitmap(byte* buffer, usize _size)
    {
        pBuffer = buffer;
        size    = _size;

        for(usize i = 0; i < size; i++) {
            pBuffer[i] = 0x00;
        }
    }

    bitmap::bitmap()
    {
        pBuffer = nullptr;
        size    = 0;
    }

    bool bitmap::get(usize index) const
    {
        usize byteIndex  = index / 8;
        byte  bitIndex   = index % 8;
        byte  bitIndexer = 0b10000000 >> bitIndex;
        return pBuffer[byteIndex] & bitIndexer;
    }

    void bitmap::set(usize index, bool value)
    {
        usize byteIndex  = index / 8;
        byte  bitIndex   = index % 8;
        byte  bitIndexer = 0b10000000 >> bitIndex;

        pBuffer[byteIndex] &= ~bitIndexer;
        if(value) pBuffer[byteIndex] |= bitIndexer;
    }
}
