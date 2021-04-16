CC      := gcc
CFLAGS  := -ffreestanding -mno-red-zone -Wextra
LDFLAGS := -T kernel.ld -nostdlib -Bsymbolic -static

SRC := $(shell find . -name '*.c')
OBJ := $(patsubst ./%.c, .temp/%.o, $(SRC))

$(OBJ): .temp/%.o : %.c
	mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c $(patsubst .temp/%.o, %.c, $@) -o $@

.PHONY: all
all: $(OBJ)
	${CC} ${LDFLAGS} -o ../bin/kernel.elf ${OBJ}