#include "../utils.hpp"
#include "table_manager.hpp"

namespace Memory {
    void pageDirectoryEntry::set(PageTableFlag flag, bool state)
    {
        u64 bitSelector = (u64)1 << static_cast<u64>(flag);
        value &= ~bitSelector;
        if(state)
            value |= bitSelector;
    }

    bool pageDirectoryEntry::get(PageTableFlag flag)
    {
        u64 bitSelector = (u64)1 << static_cast<u64>(flag);
        return (value & bitSelector) > 0;
    }

    void pageDirectoryEntry::set(u64 address)
    {
        address &= 0x000000ffffffffff;
        value   &= 0xfff0000000000fff;
        value   |= (address << 12);
    }

    u64 pageDirectoryEntry::get()
    {
        return (value & 0x000ffffffffff000) >> 12;
    }
}

namespace Memory {
    pageTableManager::pageTableManager(pageTable* pml4addr, pageFrameAllocator& _frameAllocator)
        :  frameAllocator(_frameAllocator)
    {
        pPML4 = pml4addr;
    }

    void pageTableManager::map(void* virtualAddress, void* physicalAddress)
    {
        pageMapIndexer     indexer((usize)virtualAddress);
        pageDirectoryEntry pde = pPML4->entries[indexer.pdpi];
        pageTable*         pdp;

        if(!pde.get(PageTableFlag::Present)) {
            pdp = (pageTable*)frameAllocator.request();
            Memory::set(pdp, 0x1000, 0x00);

            pde.set((usize)pdp >> 12);
            pde.set(PageTableFlag::Present, true);
            pde.set(PageTableFlag::ReadWrite, true);

            pPML4->entries[indexer.pdpi] = pde;
        } else {
            pdp = (pageTable*)((u64)pde.get() << 12);
        }

        /* ------ */

        pageTable* pd;
        pde = pdp->entries[indexer.pdi];
        if(!pde.get(PageTableFlag::Present)) {
            pd = (pageTable*)frameAllocator.request();
            Memory::set(pd, 0x1000, 0x00);

            pde.set((usize)pd >> 12);
            pde.set(PageTableFlag::Present, true);
            pde.set(PageTableFlag::ReadWrite, true);

            pdp->entries[indexer.pdi] = pde;
        } else {
            pd = (pageTable*)((usize)pde.get() << 12);
        }

        /* ------ */

        pageTable* pt;
        pde = pd->entries[indexer.pti];
        if(!pde.get(PageTableFlag::Present)) {
            pt = (pageTable*)frameAllocator.request();
            Memory::set(pt, 0x1000, 0x00);

            pde.set((usize)pt >> 12);
            pde.set(PageTableFlag::Present, true);
            pde.set(PageTableFlag::ReadWrite, true);

            pd->entries[indexer.pti] = pde;
        } else {
            pt = (pageTable*)((u64)pde.get() << 12);
        }

        pde = pt->entries[indexer.pi];
        pde.set((usize)physicalAddress >> 12);
        pde.set(PageTableFlag::Present, true);
        pde.set(PageTableFlag::ReadWrite, true);

        pt->entries[indexer.pi] = pde;
    }
}