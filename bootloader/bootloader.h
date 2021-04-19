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
    struct PSF1_FONT* pSystemFont;
    struct ScreenBuffer* pScreenBuffer;
    struct MemoryMap memoryMap;
};

extern EFI_FILE* LoadFile(
    __IN__ EFI_HANDLE* ImageHandle,
    __IN__ EFI_SYSTEM_TABLE* SystemTable,
    __IN__ EFI_FILE* Directory,
    __IN__ CHAR16* Path
);

extern struct PSF1_FONT* LoadFont(
    __IN__ EFI_HANDLE* ImageHandle,
    __IN__ EFI_SYSTEM_TABLE* SystemTable,
    __IN__ EFI_FILE* Directory,
    __IN__ CHAR16* Path
);

extern struct MemoryMap LoadMemoryInfo(
    __IN__ EFI_SYSTEM_TABLE* SystemTable,
    __OUT__ UINT64* MemoryMapKey
);

extern struct ScreenBuffer* InitializeScreen(
    __IN__ EFI_BOOT_SERVICES* BootServices
);

extern VOID LoadKernel(
    __IN__ EFI_HANDLE ImageHandle,
    __IN__ EFI_SYSTEM_TABLE* SystemTable,
    __IN__ EFI_BOOT_SERVICES* BootServices,
    __IN__ EFI_FILE* File,
    __IN__ UINT64 MapKey,
    __IN__ struct BootData* StartData
);

extern VOID InitializePrint(
    __IN__ EFI_HANDLE* ImageHandle,
    __IN__ EFI_SYSTEM_TABLE* SystemTable
);

extern VOID SimplePrintUInt(__IN__ UINTN Number);
extern VOID SimplePrint(__IN__ CHAR16* Message);
extern VOID FatalError(__IN__ CHAR16* Message);

#endif // BOOTLOADER_H
