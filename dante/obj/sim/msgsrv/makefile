DEST = ./
USE_STDLIB = 1
include ../../../configure.mk

all:
	${CXX} msgsrv.o -o msgsrv -lsocket++
	${CXX} test.o -o test -lsocket++

clean:  
	(rm *.o msgsrv test)
