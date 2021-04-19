#include "types.hpp"
#include "boot.hpp"
#include "terminal.hpp"
#include "memory/memory.hpp"

extern usize __kernelStart;
extern usize __kernelEnd;
extern void memset(void* buf, usize len, usize val);

[[gnu::ms_abi]]
extern "C" void KernelMain(Boot::data* bootData)
{
    Boot::systemFont& systemFont = *bootData->pSystemFont;
    Boot::screenData& screenData = *bootData->pScreenData;
    Boot::memoryData& memoryData =  bootData->memoryData;

    BasicTerminal terminal(screenData, systemFont);
    terminal.print("Hello, world!\n");

    usize kernelSize  = (usize)&__kernelEnd - (usize)&__kernelStart;
    usize kernelPages = (usize)kernelSize / 4096 + 1;

    Memory::pageFrameAllocator frameAllocator(memoryData);
    frameAllocator.lock(&__kernelStart, kernelPages);

    Memory::pageTable* pml4 = (Memory::pageTable*)frameAllocator.request();
    memset(pml4, 0x1000, 0x00);

    Memory::pageTableManager tableManager(pml4, frameAllocator);

    for(usize t = 0; t < frameAllocator.totalMemory(); t += 0x1000) {
        tableManager.map((void*)t, (void*)t);
    }

    usize bufferBase = (usize)screenData.pFrontBuffer;
    usize bufferSize = (usize)screenData.bufferSize + 0x1000;

    for(usize t = bufferBase; t < (bufferBase + bufferSize); t += 4096) {
        tableManager.map((void*)t, (void*)t);
    }

    __asm("mov %0, %%cr3" : : "r" (pml4));

    terminal.print("Total memory: ", frameAllocator.totalMemory() / 1024 / 1024, "MB\n");
    terminal.print("Reserved memory: ", frameAllocator.reservedMemory() / 1024 / 1024, "MB\n");

    while(true) {}

    return;
}
