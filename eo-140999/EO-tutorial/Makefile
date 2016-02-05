#Change this to the directory your EO library resides
prefix=/root/oldEO
#Change this to whatever version of c++ you're using
CXX = g++

#You shouldn't need to change this
EXEC = EOtutorial-ex0 EOtutorial-ex1  EOtutorial-ex2  EOtutorial-ex3 EOtutorial-ex4 EOtutorial-ex5
ex0_OBJECTS = SGA-getParams.o
ex2_OBJECTS = ex2-getParams.o
ex4_OBJECTS = ex4-getParams.o
ex5_OBJECTS = IslandGA-getParams.o
OBJECTS= $(ex0_OBJECTS) $(ex2_OBJECTS) $(ex4_OBJECTS)  $(ex5_OBJECTS)
FILES= IslandGA.vars EO-tutorial.html c++.css Makefile df2.gnu  muback1.gif deJongF2.png *.h *.cpp

#Flags and whatnots
CFLAGS=-g -Wall
CXXFLAGS= -I${prefix} $(CFLAGS)
LDFLAGS= -L${prefix}/util
LIBS=-lEOutil -lm
CXXCOMPILE = $(CXX) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS)

#Targets
all: $(EXEC)

.SUFFIXES: .S .c .cc .lo .o .s .cpp

.cpp.o:
	$(CXXCOMPILE) -c $<

EOtutorial-ex0: EOtutorial-ex0.cpp $(ex0_OBJECTS)  Makefile
	$(CXXCOMPILE) $< -o EOtutorial-ex0 $(ex0_OBJECTS) $(LIBS)

EOtutorial-ex1: EOtutorial-ex1.cpp $(ex0_OBJECTS)  Makefile
	$(CXXCOMPILE) $< -o EOtutorial-ex1 $(ex0_OBJECTS) $(LIBS)

EOtutorial-ex2: EOtutorial-ex2.cpp $(ex2_OBJECTS)  Makefile
	$(CXXCOMPILE) $< -o EOtutorial-ex2 $(ex2_OBJECTS) $(LIBS)

EOtutorial-ex3: EOtutorial-ex3.cpp $(ex2_OBJECTS)  Makefile
	$(CXXCOMPILE) $< -o EOtutorial-ex3 $(ex2_OBJECTS) $(LIBS)

EOtutorial-ex4: EOtutorial-ex4.cpp $(ex4_OBJECTS)  Makefile
	$(CXXCOMPILE) $< -o EOtutorial-ex4 $(ex4_OBJECTS) $(LIBS)

EOtutorial-ex5: EOtutorial-ex5.cpp EOIslandGA.h $(ex5_OBJECTS)  Makefile
	$(CXXCOMPILE) $< -o EOtutorial-ex5 $(ex5_OBJECTS) $(LIBS)

#Other targets
clean: 
	rm -f $(OBJECTS) $(EXEC) core *~

zip: clean
	cd ..;zip -r EO-tutorial-`date +"%m%d%y"`.zip EO-tutorial/*