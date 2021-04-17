#ifndef BOOT_H
#define BOOT_H

#include "types.hpp"

struct ScreenData {
    void* pFrontBuffer;
    void* pBackBuffer;
    u64 bufferSize;
    u32 width;
    u32 height;
    u32 pxPerScanline;
};

struct PSF1_FONT {
    struct {
        byte magic[2];
        byte mode;
        byte charSize;
    } *pHeader;

    void* pGlyphBuffer;
};

struct MemoryData {
    void* pMap;
    u64 size;
    u64 descriptorSize;
};

struct BootData {
    PSF1_FONT* pSystemFont;
    ScreenData* pScreenData;
    MemoryData memoryData;
};

#endif // BOOT_H
