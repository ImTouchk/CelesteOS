global long_mode_start
extern kernel_main

section .text
bits 64
long_mode_start:
    ; Load NULL into data segment registeers
    mov ax, 0x10
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call kernel_main
    hlt