DEST = ../../../obj/kernel/

include ../../../configure.mk

CFLAGS += -I../include

OBJECTS = ${DEST}task.o
OBJECTS += ${DEST}timer.o
OBJECTS += ${DEST}interrupt.o
OBJECTS += ${DEST}sched.o

all: ${OBJECTS}
	
