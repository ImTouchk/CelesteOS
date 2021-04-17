#include "types.hpp"
#include "boot.hpp"
#include "terminal.hpp"

extern "C" void KernelMain(BootData* bootData)
{
    u32* buffer = (u32*)bootData->pScreenData->pFrontBuffer;
    buffer[0] = 0x00FF0000;
    return;
}
