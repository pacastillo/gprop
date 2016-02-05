CXX=g++ 
prefix=..
CXXFLAGS=-I$(prefix)/eo-140999 -I$(prefix)/qp -Wall  #-O4 # -g

###############################################################################

SRC = gprop.cc
BIN = gprop

###############################################################################

all:	${BIN}

clean:	
	rm -f ${BIN} *~ core

gpropSO:	
	g++ -I/home/pedro/progs/eo-140999 -I/home/pedro/progs/qp -Wall -g gprop.cc -o gprop

###############################################################################

.NOEXPORT:
