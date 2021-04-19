#ifndef PAGE_TABLE_MANAGER_H
#define PAGE_TABLE_MANAGER_H

#include "../../types.hpp"
#include "map_indexer.hpp"
#include "frame_allocator.hpp"

namespace Memory {
    struct pageDirectoryEntry {
        bool present        : 1;
        bool readWrite      : 1;
        bool userSuper      : 1;
        bool writeThrough   : 1;
        bool cacheDisabled  : 1;
        bool accessed       : 1;
        bool ignore0        : 1;
        bool largerPages    : 1;
        bool ignore1        : 1;
        bool available      : 3;
        u64 address         : 52;
    };

    struct __attribute__((aligned(0x1000))) pageTable {
        pageDirectoryEntry entries[512];
    };

    struct pageTableManager {
        pageTableManager(pageTable* pml4addr, pageFrameAllocator& _frameAllocator);
        void map(void* virtualAddress, void* physicalAddress);

        pageTable* pPML4;
        pageFrameAllocator& frameAllocator;
    };
}

#endif // PAGE_TABLE_MANAGER_H
