CC      := clang++
LD      := ld
CFLAGS  := -target x86_64-pc-none-elf -ffreestanding -mno-red-zone -Wextra -std=c++20 -fno-exceptions -g
LDFLAGS := -T kernel.ld -nostdlib -Bsymbolic -static

SRC := $(shell find . -name '*.cpp')
OBJ := $(patsubst ./%.cpp, .temp/%.o, $(SRC))

$(OBJ): .temp/%.o : %.cpp
	mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c $(patsubst .temp/%.o, %.cpp, $@) -o $@

.PHONY: all clean
all: $(OBJ)
	${LD} ${LDFLAGS} -o ../bin/kernel.elf ${OBJ}
clean:
	rm -r .temp/