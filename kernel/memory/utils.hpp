#ifndef MEMORY_UTILS_H
#define MEMORY_UTILS_H

#include "../types.hpp"

extern "C" void memcpy(void* dst, const void* src, usize num);

namespace Memory {
    void set(void* buffer, usize len, usize value);
}

#endif // MEMORY_UTILS_H
