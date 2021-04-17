#include <efi.h>
#include <efilib.h>

#include "bootloader.h"

EFI_HANDLE* ImgHandle;
EFI_SYSTEM_TABLE* SysTable;

VOID InitializeLoader(
    __IN__ EFI_HANDLE* ImageHandle,
    __IN__ EFI_SYSTEM_TABLE* SystemTable
)
{
    ImgHandle = ImageHandle;
    SysTable  = SystemTable;
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
    EFI_INPUT_KEY   Key;
    EFI_STATUS      Status;
    EFI_FILE*       KernelBin;
    UINTN           MapKey;
    struct BootData KernelData;

    SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

    InitializeLoader(&ImageHandle, SystemTable);

    KernelData.pSystemFont   = LoadFont(NULL, L"zap-light16.psf");
    KernelData.pScreenBuffer = InitializeScreen();
    KernelData.memoryMap     = LoadMemoryInfo(&MapKey);

    KernelBin = LoadFile(NULL, L"kernel.elf");
    if(KernelBin == NULL) {
        FatalError(L"Couldn't load the kernel binary.\r\n");
    }
    
    SimplePrint(L"Loaded kernel binary.\n");
    LoadBinary(KernelBin);

    SimplePrint(L"Exitted KernelMain.\r\n");

    do { Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key); }
    while(Status == EFI_NOT_READY);

    return Status;
}
