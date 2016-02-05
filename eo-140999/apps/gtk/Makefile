# Generated automatically from Makefile.in by configure.
# Generated automatically from Makefile.in by configure.
SHELL = /bin/sh

CXX = c++
prefix= /root/oldEO

GTK_CFLAGS = `gtk-config --cflags` -I../.. -I /usr/lib/Gtk--/include
GTK_CONFIG = `gtk-config --libs`
GTK_LIBS = -L ../../util -L/usr/local/lib -L/usr/X11R6/lib -lgtk -lgdk -lglib -lXext -lX11 -lm
LD = ld
LIBTOOLP= libtool
LIBTOOL = $(SHELL) $(LIBTOOLP)


#INCLUDES = 
CXXFLAGS = $(GTK_CFLAGS) -g
LDFLAGS = -L$(prefix)/util $(GTK_LIBS) 
LDADD = /usr/local/lib/libgtkmm.la 
CONFIG_HEADER = ../config.h
CONFIG_CLEAN_FILES = 


DEPENDENCIES = $(LDADD)
DEFS = -DHAVE_CONFIG_H -I.
CPPFLAGS = 
LDFLAGS = -L$(prefix)/util
GTKMM_LIBS = -lgtkmm -lgtk -lgdk -lgdkmm
LIBS = -lEOutil 

CXXCOMPILE = $(CXX) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS)
LTCXXCOMPILE = $(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CXXFLAGS)
CXXLINK = $(LIBTOOL) --mode=link $(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@


PROGRAMS =  ga
OBJECTS = AboutDialog.o TWindowMenu.o gtk--canvas.o TCommonDialog.o  main.o
mitest3_OBJECTS= mitest3.o gtk--canvas.o MiApp1.o


default: ga
.SUFFIXES:
.SUFFIXES: .S .c .cc .lo .o .s .cpp
.c.o:
	$(COMPILE) -c $<
.s.o:
	$(COMPILE) -c $<
.S.o:
	$(COMPILE) -c $<
.c.lo:
	$(LIBTOOL) --mode=compile $(COMPILE) -c $<
.s.lo:
	$(LIBTOOL) --mode=compile $(COMPILE) -c $<
.S.lo:
	$(LIBTOOL) --mode=compile $(COMPILE) -c $<
.cc.o:
	$(CXXCOMPILE) -c $<
.cpp.o:
	$(CXXCOMPILE) -c $<
.cc.lo:
	$(LTCXXCOMPILE) -c $<



ga: $(OBJECTS) Makefile
	@rm -f ga
	$(CXXCOMPILE) $(OBJECTS) $(LIBS) -o ga

mitest3: $(mitest3_OBJECTS) mitest3.cpp Makefile
	@rm -f mitest3
	$(CXXCOMPILE) $(mitest3_OBJECTS) $(GTK_LIBS) $(GTKMM_LIBS) -o mitest3

MiApp1.o: MiApp1.h AxisCanvas.h CanvasLine.h
main.o: TWindow2.h TCommonDialog.h 
AboutDialog.o: AboutDialog.cpp
TWindowMenu.o: TWindowMenu.cpp
gtk--canvas.o: gtk--canvas.cpp
TCommonDialog.o: TCommonDialog.cpp Makefile

.NOEXPORT:
