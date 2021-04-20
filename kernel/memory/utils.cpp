#include "utils.hpp"

extern "C" void memcpy(void* dst, const void* src, usize num)
{
    byte* dst_as_bytes = (byte*)dst;
    byte* src_as_bytes = (byte*)src;

    for(usize i = 0; i < num; i++) {
        dst_as_bytes[i] = src_as_bytes[i];
    }
}

namespace Memory {
    void set(void* buffer, usize len, usize value)
    {
        for(usize i = 0; i < len; i++) {
            *(byte*)((usize)buffer + i) = value;
        }
    }
}