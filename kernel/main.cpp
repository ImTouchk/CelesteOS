#include "types.hpp"
#include "boot.hpp"
#include "terminal.hpp"

extern "C" void KernelMain(void* _bootData)
{
    BootData& bootData     = *((BootData*)_bootData);
    ScreenData& screenData = *bootData.pScreenData;
    PSF1_FONT& systemFont  = *bootData.pSystemFont;

    BasicTerminal terminal(screenData, systemFont);
    terminal.write("Hello, world!");
    return;
}
