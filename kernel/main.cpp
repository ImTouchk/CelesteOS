#include "types.hpp"
#include "boot.hpp"
#include "terminal.hpp"

[[gnu::ms_abi]]
extern "C" void KernelMain(BootData* bootData)
{
    ScreenData& screenData  = *bootData->pScreenData;
    u32*        frontBuffer = static_cast<u32*>(screenData.pFrontBuffer);

    for(int i = 0; i < 600; i++)
        frontBuffer[i] = 0x00FFFFFF;

    return;
}
