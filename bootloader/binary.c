#include <efi.h>
#include <efilib.h>

#include "bootloader.h"
#include "elf.h"

static INTN memcmp(const void* a, const void* b, UINTN size)
{
    const UINT8* a_bytes = a;
    const UINT8* b_bytes = b;
    UINTN i;

    for(i = 0; i < size; i++) {
        if(a_bytes[i] < b_bytes[i]) return -1;
        else if(a_bytes[i] > b_bytes[i]) return 1;
    }

    return 0;
}

VOID LoadBinary(
    EFI_HANDLE* ImageHandle,
    EFI_SYSTEM_TABLE* SystemTable,
    EFI_FILE* File
)
{
    Elf64_Ehdr     Header;
    Elf64_Phdr*    PHeader;
    Elf64_Phdr*    PHeaders;
    EFI_FILE_INFO* FileInfo;
    UINTN          FileInfoSize;
    UINTN          HeaderSize;
    UINTN          PHeadersSize;

    File->GetInfo(File, &gEfiFileInfoGuid, &FileInfoSize, NULL);
    SystemTable->BootServices->AllocatePool(EfiLoaderData, FileInfoSize, (void**)&FileInfo);
    File->GetInfo(File, &gEfiFileInfoGuid, &FileInfoSize, (void**)&FileInfo);

    HeaderSize = sizeof(Header);
    File->Read(File, &HeaderSize, &Header);

    if(
        memcmp(&Header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
        Header.e_ident[EI_CLASS] != ELFCLASS64                 ||
        Header.e_ident[EI_DATA]  != ELFDATA2LSB                ||
        Header.e_type            != ET_EXEC                    ||
        Header.e_machine         != EM_X86_64                  ||
        Header.e_version         != EV_CURRENT
    )
    {
        FatalError(L"Invalid kernel binary.\r\n");
    }

    SimplePrint(L"Kernel binary is valid.\r\n");
    
    File->SetPosition(File, Header.e_phoff);
    PHeadersSize = Header.e_phnum * Header.e_phentsize;
    SystemTable->BootServices->AllocatePool(EfiLoaderData, PHeadersSize, (void**)&PHeaders);
    /*  system crash here ------------^ */

    File->Read(File, &PHeadersSize, PHeaders);



    SimplePrint(L"Binary pheaders read.\r\n");
    
    for(
        Elf64_Phdr* PHeader = PHeaders;
        (UINT8*)PHeader < (UINT8*)PHeaders + PHeadersSize;
        PHeader = (Elf64_Phdr*)((UINT8*)PHeader + Header.e_phentsize)
    )
    {
        switch(PHeader->p_type) {
        case PT_LOAD: {
            UINTN Pages = (PHeader->p_memsz + 0x1000 - 1) / 0x1000;
            Elf64_Addr Segment = PHeader->p_paddr;
            SystemTable->BootServices->AllocatePages(
                AllocateAddress,
                EfiLoaderData,
                Pages,
                &Segment
            );

            UINTN FSize = PHeader->p_filesz;
            File->SetPosition(File, PHeader->p_offset);
            File->Read(File, &FSize, (void*)Segment);
            break;
        }
        }
    }

    SimplePrint(L"Kernel binary loaded. Jumping to `KernelMain`.\r\n");
    void (*KernelMain)() = ((__attribute__((sysv_abi)) void (*)() ) Header.e_entry);
    KernelMain();
}
