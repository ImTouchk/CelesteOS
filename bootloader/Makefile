CC      := clang
CFLAGS  := -target x86_64-pc-win32-coff -fshort-wchar -ffreestanding -mno-red-zone -Wextra -Ignu-efi/inc -Ignu-efi/inc/x86_64 -Ignu-efi/inc/protocol
LDFLAGS := -target x86_64-pc-win32-coff -nostdlib -Wl,-entry:efi_main -Wl,-subsystem:efi_application -fuse-ld=lld-link

SRC := $(shell find -name "*.c" -not -path "./gnu-efi/*")
OBJ := $(patsubst ./%.c, .temp/%.o, $(SRC))

$(OBJ): .temp/%.o : %.c
	mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c $(patsubst .temp/%.o, %.c, $@) -o $@

.PHONY: all
all: $(OBJ)
	${CC} ${CFLAGS} -c -o .temp/data.o gnu-efi/lib/data.c
	${CC} ${LDFLAGS} -o ../bin/BOOTX64.efi ${OBJ} .temp/data.o