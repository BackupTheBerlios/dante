CXX = g++
CC = gcc
ASM = nasm

CFLAGS = -O3 -g
CXXFLAGS = 

ifneq (1, ${USE_STDLIB})
CFLAGS += -nostdlib -nostdinc --no-builtin 
CXXFLAGS += -ffreestanding --no-exceptions -nostdinc++ --no-rtti
endif

${DEST}%.o: %.c
	${CC} -c ${CFLAGS} $< -o $@

${DEST}%.o: %.C
	${CXX} -c ${CFLAGS} ${CXXFLAGS} $< -o $@

${DEST}%.o: %.asm
	${ASM} -o $@ -f elf $<
