# Generated automatically from Makefile.in by configure.
prefix=/home/jmerelo/progs/gprop/eo-140999
CXX=c++
CFLAGS=-g -Wall
RANLIB=ranlib
CXXFLAGS= -I${prefix} ${CFLAGS}


#To make it undertand .cpp, needed for VC++
.SUFFIXES: ${.SUFFIXES} .cpp

.cpp.o: 
	${CXX} -c ${CXXFLAGS} $< -o $@

.cpp: 
	${CXX} ${CXXFLAGS} $< -o $@

OBJS=Parser.o

all: libEOutil.a

libEOutil.a: $(OBJS)
	ar rv $@ $(OBJS)
	test "${RANLIB}" = ":" || ranlib $@

clean:
	-rm $(OBJS) libEOutil.a Makefile
