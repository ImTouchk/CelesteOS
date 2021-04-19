#include "types.hpp"
#include "boot.hpp"
#include "terminal.hpp"
#include "memory/memory.hpp"

extern usize __kernelEnd;
extern usize __kernelStart;

[[gnu::ms_abi]]
extern "C" void KernelMain(Boot::data* bootData)
{
    Boot::systemFont& systemFont = *bootData->pSystemFont;
    Boot::screenData& screenData = *bootData->pScreenData;
    Boot::memoryData& memoryData =  bootData->memoryData;

    BasicTerminal terminal(screenData, systemFont);
    terminal.set_color(0x00000000);
    terminal.clear();

    terminal.set_color(0x00FFFFFF);
    terminal.print("Hello, world!\n");

    usize __kernelSize  = (usize)&__kernelEnd - (usize)&__kernelStart;
    usize __kernelPages = __kernelSize / 4096 + 1;

    Memory::pageFrameAllocator frameAllocator(bootData->memoryData);
    frameAllocator.lock(&__kernelStart, __kernelPages);

    Memory::pageTable* pml4 = (Memory::pageTable*)frameAllocator.request();
    Memory::set(pml4, 0x1000, 0x00);

    Memory::pageTableManager tableManager(pml4, frameAllocator);

    for(usize t = 0; t < frameAllocator.totalMemory(); t += 0x1000) {
        tableManager.map((void*)t, (void*)t);
    }

    usize scrBufferBase = (usize)bootData->pScreenData->pFrontBuffer;
    usize scrBufferSize = (usize)bootData->pScreenData->bufferSize + 0x1000;
    frameAllocator.lock((void*)scrBufferBase, scrBufferSize / 0x10000 + 1);

    for(usize t = scrBufferBase; t < (scrBufferBase + scrBufferSize); t += 4096) {
        tableManager.map((void*)t, (void*)t);
    }

    __asm("mov %0, %%cr3" : : "r" (pml4));
    /* ^--- switch to the new table manager */

    terminal.print("Total memory: ", frameAllocator.totalMemory() / 1024 / 1024, "MB\n");
    terminal.print("Reserved memory: ", frameAllocator.reservedMemory() / 1024 / 1024, "MB\n");

    while(true) {}

    return;
}
