# ---------------------------------------------------------------------------
VERSION = BCB.01
# ---------------------------------------------------------------------------
!ifndef BCB
BCB = $(MAKEDIR)\..
!endif
# ---------------------------------------------------------------------------
PROJECT = GENETIC.exe
OBJFILES = GENETIC.obj Unit2.obj Unit3.obj TGAViz.obj \
   \MyFiles\progs\eo\util\Random.obj
RESFILES = Genetic.res
RESDEPEN = $(RESFILES) Unit2.dfm Unit3.dfm TGAViz.dfm
LIBFILES =
DEFFILE = 
# ---------------------------------------------------------------------------
CFLAG1 = -Od -w -k -r -y -v -vi- -c -a4 -b- -w-par -w-inl -Vx -Ve -x 
CFLAG2 = -Ic:\myfiles\progs\eo\util;$(BCB)\projects;$(BCB)\include;$(BCB)\include\vcl;\MyFiles\progs\eo \
   -H=$(BCB)\lib\vcld.csm 
PFLAGS = -AWinTypes=Windows;WinProcs=Windows;DbiTypes=BDE;DbiProcs=BDE;DbiErrs=BDE \
   -Uc:\myfiles\progs\eo\util;$(BCB)\projects;$(BCB)\lib\obj;$(BCB)\lib \
   -Ic:\myfiles\progs\eo\util;$(BCB)\projects;$(BCB)\include;$(BCB)\include\vcl;\MyFiles\progs\eo \
   -v -$Y -$W -$O- -JPHNV -M  
RFLAGS = -ic:\myfiles\progs\eo\util;$(BCB)\projects;$(BCB)\include;$(BCB)\include\vcl;\MyFiles\progs\eo 
LFLAGS = -Lc:\myfiles\progs\eo\util;$(BCB)\projects;$(BCB)\lib\obj;$(BCB)\lib \
   -aa -Tpe -x -v -V4.0  
IFLAGS = 
LINKER = ilink32
# ---------------------------------------------------------------------------
ALLOBJ = c0w32.obj $(OBJFILES)
ALLRES = $(RESFILES)
ALLLIB = $(LIBFILES) vcl.lib import32.lib cp32mt.lib 
# ---------------------------------------------------------------------------
.autodepend

$(PROJECT): $(OBJFILES) $(RESDEPEN)
    $(BCB)\BIN\$(LINKER) @&&!
    $(LFLAGS) +
    $(ALLOBJ), +
    $(PROJECT),, +
    $(ALLLIB), +
    $(DEFFILE), +
    $(ALLRES) 
!

.pas.hpp:
    $(BCB)\BIN\dcc32 $(PFLAGS) { $** }

.pas.obj:
    $(BCB)\BIN\dcc32 $(PFLAGS) { $** }

.cpp.obj:
    $(BCB)\BIN\bcc32 $(CFLAG1) $(CFLAG2) -o$* $* 

.c.obj:
    $(BCB)\BIN\bcc32 $(CFLAG1) $(CFLAG2) -o$* $**

.rc.res:
    $(BCB)\BIN\brcc32 $(RFLAGS) $<
#-----------------------------------------------------------------------------
