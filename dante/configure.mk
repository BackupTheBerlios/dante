CXX = g++
CC = gcc
ASM = nasm

CFLAGS = -nostdlib -nostdinc --no-builtin -O3
CXXFLAGS = -ffreestanding --no-exceptions -nostdinc++ --no-rtti

${DEST}%.o: %.c
	${CC} -c ${CFLAGS} $< -o $@

${DEST}%.o: %.C
	${CXX} -c ${CFLAGS} ${CXXFLAGS} $< -o $@

${DEST}%.o: %.asm
	${ASM} -o $@ -f elf $<
