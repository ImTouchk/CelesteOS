#include <efi.h>
#include <efilib.h>

#include "bootloader.h"

struct PSF1_FONT* LoadFont(
    __IN__ EFI_FILE* Directory,
    __IN__ CHAR16* Path
)
{
    EFI_FILE*           FontFile;
    struct PSF1_HEADER* FontHeader;
    struct PSF1_FONT*   Font;
    UINTN               Size;
    UINTN               BufferSize;
    void*               Buffer;

    FontFile = LoadFile(Directory, Path);
    if(FontFile == NULL) {
        FatalError(L"Could not load the system font file.\r\n");
        return NULL;
    }

    SysTable->BootServices->AllocatePool(
        EfiLoaderData,
        sizeof(struct PSF1_HEADER),
        (void**)&FontHeader
    );

    Size = sizeof(struct PSF1_HEADER);
    FontFile->Read(FontFile, &Size, FontHeader);

    if(FontHeader->magic[0] != PSF1_MAGIC0 ||
       FontHeader->magic[1] != PSF1_MAGIC1) {
        FatalError(L"Invalid system font version.\r\n");
        return NULL;
    }

    BufferSize = FontHeader->charSize * 256;
    if(FontHeader->mode == 1) {
        BufferSize = FontHeader->charSize * 512;
    }

    FontFile->SetPosition(FontFile, Size);
    SysTable->BootServices->AllocatePool(
        EfiLoaderData,
        BufferSize,
        (void**)&Buffer
    );
    FontFile->Read(FontFile, &BufferSize, Buffer);

    SysTable->BootServices->AllocatePool(
        EfiLoaderData,
        sizeof(struct PSF1_FONT),
        (void**)&Font
    );

    Font->pHeader      = FontHeader;
    Font->pGlyphBuffer = Buffer;

    SimplePrint(L"System font loaded.\r\n");
    return Font;
}