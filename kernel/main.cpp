#include "types.hpp"
#include "boot.hpp"
#include "terminal.hpp"

[[gnu::ms_abi]]
extern "C" void KernelMain(BootData* bootData)
{
    PSF1_FONT&  systemFont  = *bootData->pSystemFont;
    ScreenData& screenData  = *bootData->pScreenData;

    BasicTerminal terminal(screenData, systemFont);
    terminal.write("Hello, world!\n-%d", -1);

    return;
}
