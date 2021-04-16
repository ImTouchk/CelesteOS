#include "bootloader.h"

static EFI_HANDLE*       IMAGE_HANDLE = NULL;
static EFI_SYSTEM_TABLE* SYSTEM_TABLE = NULL;

VOID InitializePrint(EFI_HANDLE* ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
    IMAGE_HANDLE = ImageHandle;
    SYSTEM_TABLE = SystemTable;
}

VOID SimplePrint(CHAR16* Message)
{
    SYSTEM_TABLE->ConOut->OutputString(
        SYSTEM_TABLE->ConOut,
        Message
    );
}

VOID SimplePrintUInt(UINTN Number)
{
    CHAR16 Buffer[64];
    UINTN  Reversed = 0;
    UINT8  c        = 0;

    if(Number == 0) {
        Buffer[0] = (CHAR16)48;
        Buffer[1] = '\0';
        SimplePrint(Buffer);
        return;
    }

    while(Number != 0) {
        Reversed = (Reversed * 10) + (Number % 10);
        Number  /= 10;
        c++;
    }

    for(UINT8 i = 0; i < c; i++) {
        Buffer[i] = (CHAR16)48 + (Reversed % 10);
        Reversed /= 10;
    }

    Buffer[c] =  '\0';
    SimplePrint(Buffer);
}

VOID FatalError(CHAR16* Message)
{
    SimplePrint(Message);
    SimplePrint(L"Execution stopped.\r\n");
    for(;;) {
        asm("cli");
        asm("hlt");
    }
}
