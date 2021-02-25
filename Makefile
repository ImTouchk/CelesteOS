LD  := ${HOME}/opt/cross/bin/x86_64-elf-ld
GCC := ${HOME}/opt/cross/bin/x86_64-elf-gcc

CFLAGS      := -ffreestanding -O2 -Wall -Wextra
INC_FOLDERS := src/intf

KERNEL_SRC := $(shell find src/kernel -name *.c)
KERNEL_OBJ := $(patsubst src/kernel/%.c, temp/%_krn.o, $(KERNEL_SRC))

ASM_SRC := $(shell find src/x86_64 -name *.asm)
ASM_OBJ := $(patsubst src/x86_64/%.asm, temp/%_asm.o, $(ASM_SRC))

$(ASM_OBJ): temp/%_asm.o : src/x86_64/%.asm
	mkdir -p $(dir $@)
	nasm -f elf64 $(patsubst temp/%_asm.o, src/x86_64/%.asm, $@) -o $@

$(KERNEL_OBJ): temp/%_krn.o : src/kernel/%.c
	mkdir -p $(dir $@)
	${GCC} -c -I ${INC_FOLDERS} ${CFLAGS} $(patsubst temp/%_krn.o, src/kernel/%.c, $@) -o $@

OBJ_FILES := $(ASM_OBJ)

.PHONY: build-x86_64
build-x86_64: $(KERNEL_OBJ) $(OBJ_FILES)
	if [ -e targets/kernel.iso ]; then rm targets/kernel.iso; fi
	${LD} -n -o temp/kernel.bin -T targets/linker.ld $(KERNEL_OBJ) $(OBJ_FILES)
	cp temp/kernel.bin targets/boot/kernel.bin
	grub-mkrescue /usr/lib/grub/i386-pc -o targets/kernel.iso targets/