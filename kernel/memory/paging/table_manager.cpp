#include "table_manager.hpp"

void memset(void* buf, usize len, usize val)
{
    for(usize i = 0; i  < len; i++) {
        *(byte*)((usize)buf + i) = val;
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

        if(!pde.present) {
            pdp = (pageTable*)frameAllocator.request();
            memset(pdp, 0x1000, 0x00);

            pde.address   = (usize)pdp >> 12;
            pde.present   = true;
            pde.readWrite = true;

            pPML4->entries[indexer.pdpi] = pde;
        } else {
            pdp = (pageTable*)((u64)pde.address << 12);
        }

        /* ------ */

        pageTable* pd;
        pde = pdp->entries[indexer.pdi];
        if(!pde.present) {
            pd = (pageTable*)frameAllocator.request();
            memset(pd, 0x1000, 0x00);

            pde.address   = (usize)pd >> 12;
            pde.present   = true;
            pde.readWrite = true;

            pdp->entries[indexer.pdi] = pde;
        } else {
            pd = (pageTable*)((usize)pde.address << 12);
        }

        /* ------ */

        pageTable* pt;
        pde = pd->entries[indexer.pti];
        if(!pde.present) {
            pt = (pageTable*)frameAllocator.request();
            memset(pt, 0x1000, 0x00);

            pde.address   = (usize)pt >> 12;
            pde.present   = true;
            pde.readWrite = true;

            pd->entries[indexer.pti] = pde;
        } else {
            pt = (pageTable*)((u64)pde.address << 12);
        }

        pde = pt->entries[indexer.pi];
        pde.address   = (usize)physicalAddress >> 12;
        pde.readWrite = true;
        pde.present   = true;

        pt->entries[indexer.pi] = pde;
    }
}