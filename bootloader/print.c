#include "bootloader.h"

static EFI_HANDLE*       IMAGE_HANDLE = NULL;
static EFI_SYSTEM_TABLE* SYSTEM_TABLE = NULL;

void InitializePrint(EFI_HANDLE* ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
    IMAGE_HANDLE = ImageHandle;
    SYSTEM_TABLE = SystemTable;
}

void SimplePrint(CHAR16* Message)
{
    SYSTEM_TABLE->ConOut->OutputString(
        SYSTEM_TABLE->ConOut,
        Message
    );
}

void FatalError(CHAR16* Message)
{
    SimplePrint(Message);
    SimplePrint(L"Execution stopped.\r\n");
    for(;;) {
        asm("cli");
        asm("hlt");
    }
}
