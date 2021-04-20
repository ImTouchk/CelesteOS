#ifndef PAGE_TABLE_MANAGER_H
#define PAGE_TABLE_MANAGER_H

#include "../../types.hpp"
#include "map_indexer.hpp"
#include "frame_allocator.hpp"

namespace Memory {
    enum class pageTableFlag {
        Present       = 0,
        ReadWrite     = 1,
        UserSuper     = 2,
        WriteThrough  = 3,
        CacheDisabled = 4,
        Accessed      = 5,
        LargerPages   = 7,
        Custom0       = 9,
        Custom1       = 10,
        Custom2       = 11,
        NX            = 63,
        /* ^-- nx is not always supported */
    };

    struct pageDirectoryEntry {
        void set(pageTableFlag flag, bool state);
        bool get(pageTableFlag flag);

        void set(u64 address);
        u64 get();

        u64 value;
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
