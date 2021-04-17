#include <efi.h>
#include <efilib.h>

#include "bootloader.h"

EFI_FILE* LoadFile(
    EFI_HANDLE* ImageHandle,
    EFI_SYSTEM_TABLE* SystemTable,
    EFI_FILE* Directory,
    CHAR16* Path
)
{
    EFI_FILE*                        File;
    EFI_LOADED_IMAGE_PROTOCOL*       Image;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
    EFI_STATUS                       Status;

    SystemTable->BootServices->HandleProtocol(
        *ImageHandle,
        &gEfiLoadedImageProtocolGuid,
        (void**)&Image
    );

    SystemTable->BootServices->HandleProtocol(
        Image->DeviceHandle,
        &gEfiSimpleFileSystemProtocolGuid,
        (void**)&FileSystem
    );

    if(Directory == NULL) {
        FileSystem->OpenVolume(FileSystem, &Directory);
    }

    Status = Directory->Open(
        Directory, 
        &File, 
        Path, 
        EFI_FILE_MODE_READ, 
        EFI_FILE_READ_ONLY
    );

    if(EFI_ERROR(Status)) {
        return NULL;
    }

    return File;
}
