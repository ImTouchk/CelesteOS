#include "../types.hpp"
#include "../boot.hpp"
#include "memory.hpp"

using size_t = unsigned long;

extern usize __kernelEnd;
extern usize __kernelStart;

namespace Memory {
    runtime::runtime(Boot::data& bootData, BasicTerminal& terminal)
    {
        /* Load global descriptor table */
        Memory::gdtDescriptor descriptor = {
                .size   = sizeof(Memory::gdt) - 1,
                .offset = (usize)&Memory::descriptorTable
        };
        load_gdt(&descriptor);

        /* Set-up paging */
        const usize kernelSize  = (usize)&__kernelEnd - (usize)&__kernelStart;
        const usize kernelPages = kernelSize / 4096 + 1;

        frameAllocator = pageFrameAllocator(bootData.memoryData);
        frameAllocator.lock(&__kernelStart, kernelPages);

        pPML4 = (pageTable*)frameAllocator.request();
        Memory::set(pPML4, 0x1000, 0x00);

        tableManager = pageTableManager(pPML4, &frameAllocator);
        for(usize i = 0; i < frameAllocator.totalMemory(); i += 0x1000) {
            tableManager.map((void*)i, (void*)i);
        }

        const usize bufferBase = (usize)bootData.pScreenData->pFrontBuffer;
        const usize bufferSize = (usize)bootData.pScreenData->bufferSize + 0x1000;
        frameAllocator.lock((void*)bufferBase, bufferSize / 0x10000 + 1);
        for(usize i = bufferBase; i < (bufferBase + bufferSize); i += 4096) {
            tableManager.map((void*)i, (void*)i);
        }

        __asm("mov %0, %%cr3" : : "r" (pPML4));
        /* ^--- switch to the new table manager */
    }
}
