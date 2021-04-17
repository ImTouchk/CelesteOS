#include <efi.h>
#include <efilib.h>

#include "bootloader.h"

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
    EFI_INPUT_KEY   Key;
    EFI_STATUS      Status;
    EFI_FILE*       KernelBin;
    UINTN           MapKey;
    struct BootData KernelData;

    SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

    InitializePrint(&ImageHandle, SystemTable);

    KernelData.pSystemFont   = LoadFont(&ImageHandle, SystemTable, NULL, L"zap-light16.psf");
    KernelData.pScreenBuffer = InitializeScreen(&ImageHandle, SystemTable);
    KernelData.memoryMap     = LoadMemoryInfo(&ImageHandle, SystemTable, &MapKey);

    KernelBin = LoadFile(&ImageHandle, SystemTable, NULL, L"kernel.elf");
    
    LoadKernel(
        &ImageHandle, 
        SystemTable, 
        SystemTable->BootServices, 
        KernelBin, 
        MapKey, 
        &KernelData
    );

    do { Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key); }
    while(Status == EFI_NOT_READY);

    return Status;
}
