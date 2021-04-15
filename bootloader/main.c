#include <efi.h>
#include <efilib.h>

#include "bootloader.h"

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
    EFI_INPUT_KEY   Key;
    EFI_STATUS      Status;
    UINTN           MapKey;
    struct BootData KernelData;

    SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

    InitializePrint(&ImageHandle, SystemTable);
    
    FatalError(L"is this working?\r\n");

    KernelData.pSystemFont = LoadFont(
        NULL,
        L"zap-light16.psf",
        &ImageHandle,
        SystemTable
    );

    KernelData.pScreenBuffer = InitializeScreen(
        SystemTable
    );

    KernelData.memoryMap = LoadMemoryInfo(
        &ImageHandle,
        SystemTable,
        &MapKey
    );

    SimplePrint(L"Loading the kernel...\r\n");
    SimplePrint(L"Leaving the UEFI environment...\r\n");

    do { Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key); }
    while(Status == EFI_NOT_READY);

    return Status;
}
