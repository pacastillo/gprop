# Generated automatically from Makefile.in by configure.
prefix=/home/jmerelo/progs/gprop/eo-140999
CXX=c++
CFLAGS=-g -Wall
AR=@AR@
CXXFLAGS= -I$(prefix) $(CFLAGS)


FILES =	eo/*.h eo/conf* eo/Makefile.in  \
	eo/*.html eo/eodoc.tar.gz\
	eo/ADT/*.h eo/generic/*.h eo/generic/EOselect/*.h \
	eo/util/*.h eo/util/*.cpp  eo/util/*.pl eo/util/Makefile*\
	eo/specific/*.h eo/specific/EOBin/*.h \
	eo/apps/*/*.ds* eo/apps/*/*.cpp eo/apps/*/*.cc eo/apps/*/Makefile*\
	eo/apps/*/*.mak eo/apps/*/*.res eo/apps/*/*.h\

all: utils demos master eoMind

utils: 
	cd util; make -k CXX="$(CXX)" prefix="$(prefix)" CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)"

demos:
	cd apps/examples; make -k CXX="$(CXX)" prefix="$(prefix)" CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)"

master: 
	cd apps/master;  make CXX="$(CXX)" prefix="$(prefix)" CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)"

docunzip:
	gunzip eodoc.tar.gz; tar xvf eodoc.tar

#There are "internal" targets, not to be used by the client
bug-report:
	mail todos@geneura.ugr.es

docs: 
	rm ~/public_html/eo/*; doc++ -d ~/public_html/eo -B foot.html -f EO++.h apps/examples/*.cpp apps/master/GenMM.cpp
	/home/jmerelo/tmp/swish++-1.1b3/index -e html -i /home/jmerelo/index/eo.idx /home/jmerelo/public_html/eo/ -v 3 

doczip:
	cd ~/public_html; tar cvf eodoc.tar doc/*.html doc/*.class doc/*.gif; gzip eodoc.tar; mv eodoc.tar.gz ~/eo

clean:
	-rm config.*
	-cd apps/examples; make clean
	-cd apps/master; make clean
	-cd util; make clean

zip: clean
	cd ..;zip -r  eo`date  "+%d%m%y"`.zip $(FILES)

tgz: clean
	cd ..;tar cvf  eo`date  "+%d%m%y"`.tar $(FILES); \
	gzip eo`date  "+%d%m%y"`.tar

rel:
	cd ..; mv eo`date  "+%d%m%y"`.* /home/ftp/pub/eo

