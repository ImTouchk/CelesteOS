#include <efi.h>
#include <efilib.h>

#include "bootloader.h"

struct PSF1_FONT* LoadFont(
    __IN__ EFI_FILE* Directory,
    __IN__ CHAR16* Path,
    __IN__ EFI_HANDLE* ImageHandle,
    __IN__ EFI_SYSTEM_TABLE* SystemTable
)
{
    EFI_FILE*           FontFile;
    struct PSF1_HEADER* FontHeader;
    struct PSF1_FONT*   Font;
    UINTN               Size;
    UINTN               BufferSize;
    void*               Buffer;

    FontFile = LoadFile(Directory, Path, ImageHandle, SystemTable);
    if(FontFile == NULL) {
        return NULL;
    }

    SystemTable->BootServices->AllocatePool(
        EfiLoaderData,
        sizeof(struct PSF1_HEADER),
        (void**)&FontHeader
    );

    Size = sizeof(struct PSF1_HEADER);
    FontFile->Read(FontFile, &Size, FontHeader);

    if(FontHeader->magic[0] != PSF1_MAGIC0 ||
       FontHeader->magic[1] != PSF1_MAGIC1) {
        return NULL;
    }

    BufferSize = FontHeader->charSize * 256;
    if(FontHeader->mode == 1) {
        BufferSize = FontHeader->charSize * 512;
    }

    FontFile->SetPosition(FontFile, Size);
    SystemTable->BootServices->AllocatePool(
        EfiLoaderData,
        BufferSize,
        (void**)&Buffer
    );
    FontFile->Read(FontFile, &BufferSize, Buffer);

    SystemTable->BootServices->AllocatePool(
        EfiLoaderData,
        sizeof(struct PSF1_FONT),
        (void**)&Font
    );

    Font->pHeader      = FontHeader;
    Font->pGlyphBuffer = Buffer;
    return Font;
}