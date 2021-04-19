#include "types.hpp"
#include "boot.hpp"
#include "terminal.hpp"

//#include "memory/paging/frame_allocator.hpp"

[[gnu::ms_abi]]
extern "C" void KernelMain(Boot::data* bootData)
{
    Boot::systemFont& systemFont = *bootData->pSystemFont;
    Boot::screenData& screenData = *bootData->pScreenData;
    Boot::memoryData& memoryData =  bootData->memoryData;

    BasicTerminal terminal(screenData, systemFont);
    terminal.print("Hello, world!\n");
    //Memory::Paging::frameAllocator pageFrameAllocator(memoryData);

    while(true) {}

    return;
}
