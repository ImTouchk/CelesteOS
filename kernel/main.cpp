#include "types.hpp"
#include "boot.hpp"
#include "terminal.hpp"
#include "memory/paging/frame_allocator.hpp"

[[gnu::ms_abi]]
extern "C" void KernelMain(Boot::data* bootData)
{
    Boot::systemFont& systemFont = *bootData->pSystemFont;
    Boot::screenData& screenData = *bootData->pScreenData;
    Boot::memoryData& memoryData =  bootData->memoryData;

    BasicTerminal terminal(screenData, systemFont);
    terminal.print("Hello, world!\n");

    Memory::pageFrameAllocator frameAllocator(memoryData);
    terminal.print("Total memory: ", frameAllocator.totalMemory() / 1024 / 1024, "MB\n");
    terminal.print("Reserved memory: ", frameAllocator.reservedMemory() / 1024 / 1024, "MB\n");

    while(true) {}

    return;
}
