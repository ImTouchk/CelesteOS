#include "utils.hpp"

namespace Memory {
    void set(void* buffer, usize len, usize value)
    {
        for(usize i = 0; i < len; i++) {
            *(byte*)((usize)buf + i) = val;
        }
    }
}