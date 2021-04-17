CC      := g++
CFLAGS  := -ffreestanding -mno-red-zone -Wextra
LDFLAGS := -T kernel.ld -nostdlib -Bsymbolic -static

SRC := $(shell find . -name '*.cpp')
OBJ := $(patsubst ./%.cpp, .temp/%.o, $(SRC))

$(OBJ): .temp/%.o : %.cpp
	mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c $(patsubst .temp/%.o, %.cpp, $@) -o $@

.PHONY: all
all: $(OBJ)
	${CC} ${LDFLAGS} -o ../bin/kernel.elf ${OBJ}