DEST = ./
include ../../configure.mk

all:
	${LD} -T kernel.ls --oformat=elf32-i386 *.o -o kernel.elf	

clean:  
	(rm *.o kernel.elf kernel.ls)
