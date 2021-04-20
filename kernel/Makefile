CC      := clang++
LD      := ld
CFLAGS  := -target x86_64-pc-none-elf -ffreestanding -fno-exceptions -mno-red-zone -Wextra -std=c++20 -g
LDFLAGS := -T kernel.ld -nostdlib -Bsymbolic -static

CPP_SRC := $(shell find . -name '*.cpp')
CPP_OBJ := $(patsubst ./%.cpp, .temp/%.o, $(CPP_SRC))

ASM_SRC := $(shell find . -name '*.asm')
ASM_OBJ := $(patsubst ./%.asm, .temp/%_asm.o, $(ASM_SRC))

$(ASM_OBJ): .temp/%_asm.o : %.asm
	mkdir -p $(dir $@)
	nasm -f elf64 $(patsubst .temp/%_asm.o, %.asm, $@) -o $@

$(CPP_OBJ): .temp/%.o : %.cpp
	mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c $(patsubst .temp/%.o, %.cpp, $@) -o $@

.PHONY: all clean
all: $(CPP_OBJ) $(ASM_OBJ)
	${LD} ${LDFLAGS} -o ../bin/kernel.elf ${CPP_OBJ} ${ASM_OBJ}
	mcopy -o -i ../bin/celeste.img ../bin/kernel.elf ::
clean:
	rm -r .temp/