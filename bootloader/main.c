#include <efi.h>
#include <efilib.h>

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
    EFI_INPUT_KEY Key;
    EFI_STATUS    Status;
    UINTN         MapKey;

    SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello, world!\r\n");

    do { Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key); }
    while(Status == EFI_NOT_READY);

    return Status;
}
