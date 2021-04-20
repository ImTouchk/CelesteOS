#ifndef MEMORY_GDT_H
#define MEMORY_GDT_H

#include "../../types.hpp"

namespace Memory {
    struct gdtDescriptor {
        u16 size;
        usize offset;
    } __attribute__((packed));

    struct gdtEntry {
        u16 limit0;
        u16 base0;
        byte base1;
        byte accessByte;
        byte limit1Flags;
        byte base2;
    } __attribute__((packed));

    struct gdt {
        gdtEntry null;
        gdtEntry kernelCode;
        gdtEntry kernelData;
        gdtEntry userNull;
        gdtEntry userCode;
        gdtEntry userData;
    } __attribute__((packed))
    __attribute__((aligned(0x1000)));

    extern gdt descriptorTable;
}

extern "C" void load_gdt(Memory::gdtDescriptor* descriptor);

#endif // MEMORY_GDT_H
