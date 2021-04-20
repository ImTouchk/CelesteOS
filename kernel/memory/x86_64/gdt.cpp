#include "gdt.hpp"

namespace Memory {
    __attribute__((aligned(0x1000)))
    gdt descriptorTable = {
        .null       = { 0, 0, 0, 0x00, 0x00, 0 },
        .kernelCode = { 0, 0, 0, 0x9a, 0xa0, 0 },
        .kernelData = { 0, 0, 0, 0x92, 0xa0, 0 },
        .userNull   = { 0, 0, 0, 0x00, 0x00, 0 },
        .userCode   = { 0, 0, 0, 0x9a, 0xa0, 0 },
        .userData   = { 0, 0, 0, 0x92, 0xa0, 0 }
    };
}
