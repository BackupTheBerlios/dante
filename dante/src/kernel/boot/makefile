DEST = ../../../obj/kernel/

include ../../../configure.mk

CFLAGS += -I../include

OBJECTS = ${DEST}kernel.o
OBJECTS += ${DEST}start.o
OBJECTS += ${DEST}multiBoot.o
OBJECTS += ${DEST}kernel.ls

all: ${OBJECTS}

${DEST}kernel.ls: kernel.ls
	cp $< $@
	
