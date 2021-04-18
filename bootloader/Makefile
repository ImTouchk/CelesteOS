CC       := clang
CFLAGS   := -target x86_64-pc-win32-coff -fno-stack-protector -fshort-wchar -flto -ffreestanding -mno-red-zone -Wall -Wextra
INCLUDES := -Ignu-efi/inc -Ignu-efi/inc/protocol -Ignu-efi/inc/x86_64
LDFLAGS  := -target x86_64-pc-win32-coff -nostdlib -Wl,-entry:efi_main -Wl,-subsystem:efi_application -fuse-ld=lld-link

SRC := $(shell find -name "*.c" -not -path "./gnu-efi/*")
OBJ := $(patsubst ./%.c, .temp/%.o, $(SRC))

$(OBJ): .temp/%.o : %.c
	mkdir -p $(dir $@)
	${CC} ${CFLAGS} ${INCLUDES} -c $(patsubst .temp/%.o, %.c, $@) -o $@

.PHONY: all
all: $(OBJ)
	${CC} ${CFLAGS} ${INCLUDES} -c -o .temp/data.o gnu-efi/lib/data.c
	${CC} ${LDFLAGS} -o ../bin/BOOTX64.efi ${OBJ} .temp/data.o