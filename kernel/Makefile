CC      := clang++
LD      := ld
CFLAGS  := -target x86_64-pc-none-elf -ffreestanding -fno-exceptions -mno-red-zone -Wextra -std=c++20 -g
LDFLAGS := -T kernel.ld -nostdlib -Bsymbolic -static

SRC := $(shell find . -name '*.cpp')
OBJ := $(patsubst ./%.cpp, .temp/%.o, $(SRC))

$(OBJ): .temp/%.o : %.cpp
	mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c $(patsubst .temp/%.o, %.cpp, $@) -o $@

.PHONY: all clean
all: $(OBJ)
	${LD} ${LDFLAGS} -o ../bin/kernel.elf ${OBJ}
	mcopy -o -i ../bin/celeste.img ../bin/kernel.elf ::
clean:
	rm -r .temp/