#include "../utils.hpp"
#include "table_manager.hpp"

namespace Memory {
    void pageDirectoryEntry::set(pageTableFlag flag, bool state)
    {
        u64 bitSelector = (u64)1 << static_cast<u64>(flag);
        value &= ~bitSelector;
        if(state)
            value |= bitSelector;
    }

    bool pageDirectoryEntry::get(pageTableFlag flag)
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
    pageTableManager::pageTableManager(pageTable* pml4addr, pageFrameAllocator* _frameAllocator)
        :  pFrameAllocator(_frameAllocator)
    {
        pPML4 = pml4addr;
    }

    pageTableManager::pageTableManager()
    {
        pPML4           = nullptr;
        pFrameAllocator = nullptr;
    }

    void pageTableManager::map(void* virtualAddress, void* physicalAddress)
    {
        pageMapIndexer     indexer((usize)virtualAddress);
        pageDirectoryEntry pde = pPML4->entries[indexer.pdpi];
        pageTable*         pdp;

        if(!pde.get(pageTableFlag::Present)) {
            pdp = (pageTable*)pFrameAllocator->request();
            Memory::set(pdp, 0x1000, 0x00);

            pde.set((usize)pdp >> 12);
            pde.set(pageTableFlag::Present, true);
            pde.set(pageTableFlag::ReadWrite, true);

            pPML4->entries[indexer.pdpi] = pde;
        } else {
            pdp = (pageTable*)((u64)pde.get() << 12);
        }

        /* ------ */

        pageTable* pd;
        pde = pdp->entries[indexer.pdi];
        if(!pde.get(pageTableFlag::Present)) {
            pd = (pageTable*)pFrameAllocator->request();
            Memory::set(pd, 0x1000, 0x00);

            pde.set((usize)pd >> 12);
            pde.set(pageTableFlag::Present, true);
            pde.set(pageTableFlag::ReadWrite, true);

            pdp->entries[indexer.pdi] = pde;
        } else {
            pd = (pageTable*)((usize)pde.get() << 12);
        }

        /* ------ */

        pageTable* pt;
        pde = pd->entries[indexer.pti];
        if(!pde.get(pageTableFlag::Present)) {
            pt = (pageTable*)pFrameAllocator->request();
            Memory::set(pt, 0x1000, 0x00);

            pde.set((usize)pt >> 12);
            pde.set(pageTableFlag::Present, true);
            pde.set(pageTableFlag::ReadWrite, true);

            pd->entries[indexer.pti] = pde;
        } else {
            pt = (pageTable*)((u64)pde.get() << 12);
        }

        pde = pt->entries[indexer.pi];
        pde.set((usize)physicalAddress >> 12);
        pde.set(pageTableFlag::Present, true);
        pde.set(pageTableFlag::ReadWrite, true);

        pt->entries[indexer.pi] = pde;
    }
}