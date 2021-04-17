#include <efi.h>

#include "bootloader.h"

struct MemoryMap LoadMemoryInfo(
    UINT64* MemoryMapKey
)
{
    EFI_MEMORY_DESCRIPTOR* Map = NULL;
    EFI_BOOT_SERVICES*     BootServices;
    UINTN                  MapSize;
    UINTN                  MapKey;
    UINTN                  DescriptorSize;
    UINT32                 DescriptorVer;
    struct MemoryMap       MapStruct;

    BootServices = SysTable->BootServices;
    BootServices->GetMemoryMap(
        &MapSize, 
        Map, 
        &MapKey, 
        &DescriptorSize, 
        &DescriptorVer
    );
    BootServices->AllocatePool(EfiLoaderData, MapSize, (void**)&Map);
    BootServices->GetMemoryMap(
        &MapSize, 
        Map, 
        &MapKey, 
        &DescriptorSize, 
        &DescriptorVer
    );

    MapStruct.pData    = Map;
    MapStruct.size     = MapSize;
    MapStruct.descSize = DescriptorSize;
    
    *MemoryMapKey = MapKey;

    SimplePrint(L"Memory info loaded.\r\n");

    return MapStruct;
}
