#include <efi.h>
#include <efilib.h>

#include "bootloader.h"

struct ScreenBuffer* InitializeScreen(
    EFI_HANDLE* ImageHandle,
    EFI_SYSTEM_TABLE* SystemTable,
    EFI_BOOT_SERVICES* BootServices
)
{
    static struct ScreenBuffer            Screen;
    EFI_GRAPHICS_OUTPUT_PROTOCOL*         GOP;
    EFI_STATUS                            Status;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* Info;
    UINTN                                 InfoSize;
    EFI_GUID GopGUID = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

    Status  = BootServices->LocateProtocol(
        &GopGUID,
        NULL,
        (void**)&GOP
    );

    if(EFI_ERROR(Status))
        return NULL;

    Status = GOP->QueryMode(
        GOP, 
        (GOP->Mode == NULL) ? 0 : GOP->Mode->Mode,
        &InfoSize,
        &Info
    );

    if(Status == EFI_NOT_STARTED) {
        Status = GOP->SetMode(GOP, 0);
        if(EFI_ERROR(Status)) {
            FatalError(L"Failed to initialize the screen buffer.\r\n");
            return NULL;
        }
    }

    Screen.pFrontBuffer  = (void*)(GOP->Mode->FrameBufferBase);
    Screen.pUnused       = NULL;
    Screen.bufferSize    = GOP->Mode->FrameBufferSize;
    Screen.width         = GOP->Mode->Info->HorizontalResolution;
    Screen.height        = GOP->Mode->Info->VerticalResolution;
    Screen.pxPerScanline = GOP->Mode->Info->PixelsPerScanLine;

    if(GOP->Mode->Info->PixelFormat != PixelBlueGreenRedReserved8BitPerColor) {
        FatalError(L"Invalid screen pixel format.\r\n");
        return NULL;
    }

    SimplePrint(L"Screen buffer initialized.\r\n");
    return &Screen;
}
