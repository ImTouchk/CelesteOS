#include "types.hpp"
#include "boot.hpp"
#include "terminal.hpp"

[[gnu::ms_abi]]
extern "C" void KernelMain(BootData* bootData)
{
    PSF1_FONT&  systemFont  = *bootData->pSystemFont;
    ScreenData& screenData  = *bootData->pScreenData;

    BasicTerminal terminal(screenData, systemFont);
    terminal.print("Hello, ", "world", "!\n", (u64)12345, (i64)-12345, (f64)-1.2345);

    return;
}
