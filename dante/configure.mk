CXX = g++
CC = gcc
ASM = nasm

CFLAGS = -nostdinc --no-builtin -O3 -g
CXXFLAGS = --no-exceptions -nostdinc++

${DEST}%.o: %.c
	${CC} -c ${CFLAGS} $< -o $@

${DEST}%.o: %.C
	${CXX} -c ${CFLAGS} ${CXXFLAGS} $< -o $@

${DEST}%.o: %.asm
	${ASM} -o $@ -f elf $<
