# Generated automatically from Makefile.in by configure.
prefix=/home/jmerelo/progs/gprop/eo-140999
CXX=c++
CFLAGS=-g -Wall
CXXFLAGS= -I$(prefix) $(CFLAGS)
LDFLAGS= -L${prefix}/util
LIBS=-lEOutil

OBJS= mmMatchString.o rules.o mmOutput.o
BIN = GenMM-newFitness CheckAlgo RandMM eomind eomind2

#Adding .cc to the list of suffixes
.SUFFIXES: ${.SUFFIXES} .cpp .o

#implicit rules for c++
.cpp.o:
	${CXX} -c ${CXXFLAGS}  $< -o $@

.cpp:
	${CXX} ${CXXFLAGS} $< -o $@

all:	${BIN}

GenMM-newFitness: $(OBJS) GenMM-newFitness.cpp
	$(CXX) $(CXXFLAGS) $(OBJS) $@.cpp -g -o $@ ${LDFLAGS} ${LIBS} 

CheckAlgo: $(OBJS) CheckAlgo.cpp mmRandomAlgo.h mmGenAlgo.h
	$(CXX) $(CXXFLAGS) $(OBJS) $@.cpp -g -o $@ ${LDFLAGS} ${LIBS} 

RandMM: $(OBJS) RandMM.cpp mmRandomAlgo.h
	$(CXX) $(CXXFLAGS) $(OBJS) $@.cpp -g -o $@ ${LDFLAGS} ${LIBS} 

#This is the "old" mm
GenMM: $(OBJS) GenMM.cpp
	$(CXX) $(CXXFLAGS) $(OBJS) $@.cpp -g -o $@ ${LDFLAGS} ${LIBS} 

eomind: $(OBJS) eomind.cpp
	$(CXX) $(CXXFLAGS) -I. $(OBJS) $@.cpp -g -o $@ ${LDFLAGS} ${LIBS} -L/usr/X11R6/lib -lMesaGL -lMesaGLU -lglut -lX11 -lXext -lXmu -lXi

eomind2: $(OBJS) eomind2.cpp
	$(CXX) $(CXXFLAGS) `gtkmm-config --cflags` -I. $(OBJS) $@.cpp -g -o $@ ${LDFLAGS} ${LIBS} -L/usr/X11R6/lib -L/usr/local/lib -lMesaGL -lMesaGLU -lglut -lX11 -lXext -lXmu -lXi -lgtkgl -lgtkglmm `gtkmm-config --libs`

clean:
	rm -f core ${OBJS} ${BIN} *~

rules.o:	rules.h rules.cpp mmMatchString.o 
mmMatchString.o:	mmMatchString.cpp mmMatchString.h
mmOutput.o:	mmOutput.cpp mmOutput.h
