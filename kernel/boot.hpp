#ifndef BOOT_H
#define BOOT_H

#include "types.hpp"

namespace Boot {
    struct screenData {
        void* pFrontBuffer;
        void* pBackBuffer;
        u64 bufferSize;
        u32 width;
        u32 height;
        u32 pxPerScanline;
    };

    struct systemFont {
        struct {
            byte magic[2];
            byte mode;
            byte charSize;
        } *pHeader;
        void* pGlyphBuffer;
    };

    struct memoryData {
        void* pMap;
        u64 size;
        u64 descriptorSize;
    };

    struct data {
        systemFont* pSystemFont;
        screenData* pScreenData;
        memoryData  memoryData;
    };
}

#endif // BOOT_H
