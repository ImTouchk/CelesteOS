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

struct PSF1_HEADER {
    UINT8 magic[2];
    UINT8 mode;
    UINT8 charSize;
};

struct PSF1_FONT {
    struct PSF1_HEADER* pHeader;
    void* pGlyphBuffer;
};

struct MemoryMap {
    void* pData;
    UINT64 size;
    UINT64 descSize;
};

struct BootData {
    struct ScreenBuffer* pScreenBuffer;
    struct PSF1_FONT* pSystemFont;
    struct MemoryMap memoryMap;
};

EFI_FILE* LoadFile(
    __IN__ EFI_HANDLE* ImageHandle,
    __IN__ EFI_SYSTEM_TABLE* SystemTable,
    __IN__ EFI_FILE* Directory,
    __IN__ CHAR16* Path
);

struct PSF1_FONT* LoadFont(
    __IN__ EFI_HANDLE* ImageHandle,
    __IN__ EFI_SYSTEM_TABLE* SystemTable,
    __IN__ EFI_FILE* Directory,
    __IN__ CHAR16* Path
);

struct MemoryMap LoadMemoryInfo(
    __IN__ EFI_HANDLE* ImageHandle,
    __IN__ EFI_SYSTEM_TABLE* SystemTable,
    __OUT__ UINT64* MemoryMapKey
);

struct ScreenBuffer* InitializeScreen(
    __IN__ EFI_HANDLE* ImageHandle,
    __IN__ EFI_SYSTEM_TABLE* SystemTable,
    __IN__ EFI_BOOT_SERVICES* BootServices
);

VOID LoadKernel(
    __IN__ EFI_HANDLE* ImageHandle,
    __IN__ EFI_SYSTEM_TABLE* SystemTable,
    __IN__ EFI_BOOT_SERVICES* BootServices,
    __IN__ EFI_FILE* File,
    __IN__ UINT64 MapKey,
    __IN__ struct BootData* StartData
);

VOID InitializePrint(
    __IN__ EFI_HANDLE* ImageHandle,
    __IN__ EFI_SYSTEM_TABLE* SystemTable
);

VOID SimplePrintUInt(__IN__ UINTN Number);
VOID SimplePrint(__IN__ CHAR16* Message);
VOID FatalError(__IN__ CHAR16* Message);

#endif // BOOTLOADER_H
