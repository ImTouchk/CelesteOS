#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <efi.h>

#define __IN__ 
#define __OUT__

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

struct ScreenBuffer {
    void* pFrontBuffer;
    void* pUnused;
    UINT64 bufferSize;
    UINT32 width;
    UINT32 height;
    UINT32 pxPerScanline;
};

struct BootData {
    struct ScreenBuffer* pScreenBuffer;
    void* pSystemFont;
    void* pMemoryMap;
    UINT64 memoryMapSize;
    UINT64 memoryMapDescSize;
};

struct PSF1_HEADER {
    UINT8 magic[2];
    UINT8 mode;
    UINT8 charSize;
};

struct PSF1_FONT {
    struct PSF1_HEADER* pHeader;
    void* pGlyphBuffer;
};

EFI_FILE* LoadFile(
    __IN__ EFI_FILE* Directory,
    __IN__ CHAR16* Path,
    __IN__ EFI_HANDLE* ImageHandle,
    __IN__ EFI_SYSTEM_TABLE* SystemTable
);

struct PSF1_FONT* LoadFont(
    __IN__ EFI_FILE* Directory,
    __IN__ CHAR16* Path,
    __IN__ EFI_HANDLE* ImageHandle,
    __IN__ EFI_SYSTEM_TABLE* SystemTable
);

struct BootData* InitializeScreen(
    __IN__ EFI_HANDLE* ImageHandle,
    __IN__ EFI_SYSTEM_TABLE* SystemTable
);

void InitializePrint(
    __IN__ EFI_HANDLE* ImageHandle,
    __IN__ EFI_SYSTEM_TABLE* SystemTable
);

void SimplePrint(__IN__ CHAR16* Message);
void FatalError(__IN__ CHAR16* Message);

#endif // BOOTLOADER_H
