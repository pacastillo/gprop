# Generated automatically from Makefile.in by configure.
prefix=/home/jmerelo/progs/gprop/eo-140999
CXX=c++
CFLAGS=-g -Wall
CXXFLAGS= -I${prefix} $(CFLAGS)
LDFLAGS= -L${prefix}/util
LIBS=-lEOutil -lm

#To make it undertand .cpp, needed for VC++
.SUFFIXES: ${.SUFFIXES} .cpp

.cpp.o: 
	${CXX} -c ${CXXFLAGS} $< -o $@ 

.cpp: 
	${CXX} ${CXXFLAGS} $< -o $@ ${LDFLAGS} ${LIBS} 

EXEC= pr-sa-1 pr-sa-2 pr-tourn-w-graph-1 pr-tourn-w-graph-2 pr-tourn-w-stat \
	pr-ia-1 pr-str-1 pr-sim-ann  pr-view pr-stat pr-eobin SGA pr-parsing \
	pr-vq-1 pr-bd-1 ES pr-age tsp pr-easyga-func pr-easygen pr-easyga-func-2

all: $(EXEC)

#Dependencies
pr-easygen: pr-easygen.cpp   $(prefix)/util/libEOutil.a

pr-easyga-func-2: pr-easyga-func.cpp   $(prefix)/util/libEOutil.a

pr-easyga-func: pr-easyga-func.cpp   $(prefix)/util/libEOutil.a

pr-parsing: pr-parsing.cpp $(prefix)/util/libEOutil.a

pr-sim-ann: pr-sim-ann.cpp $(prefix)/util/libEOutil.a

pr-sa-1: pr-sa-1.cpp $(prefix)/util/libEOutil.a

pr-sa-2: pr-sa-2.cpp $(prefix)/util/libEOutil.a

pr-tourn-w-graph-1: pr-tourn-w-graph-1.cpp $(prefix)/util/libEOutil.a

pr-tourn-w-graph-2: pr-tourn-w-graph-2.cpp $(prefix)/util/libEOutil.a

pr-tourn-w-stat: pr-tourn-w-stat.cpp $(prefix)/util/libEOutil.a

pr-vq-1: pr-vq-1.cpp $(prefix)/util/libEOutil.a

SGA: SGA.cpp $(prefix)/util/libEOutil.a

pr-ia-1: pr-ia-1.cpp $(prefix)/generic/EOVector.h

pr-bd-1: pr-bd-1.cpp $(prefix)/ADT/EOBidim.h

ES: ES.cpp $(prefix)/generic/EOES.h

#General programs
clean:
	-rm $(EXEC) core Makefile

zip: clean
	cd ..;zip -r  eo`date  "+%d%m%y"`.zip eo 
